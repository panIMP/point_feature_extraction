#include "pointMatch.h"
#include "stdfunc.h"
#include <math.h>


void 
normalZernikeFeat(Feat* zFeat, int actPointNum, int featNum) {
	int i;
	int fNum;

	double maxMode = 0.0;
	double mode;

	Feat* zFeatPtr = zFeat;

	for (fNum = 0; fNum < featNum; ++fNum) {
		if (fNum == 86) {
			fNum++;
			fNum--;
		}

		maxMode = 1.0;

		zFeatPtr = zFeat + fNum;
		for (i = 0; i < actPointNum; ++i) {
			mode = zFeatPtr->feat.Mode;

			if (mode > maxMode) {
				maxMode = mode;
			}

			zFeatPtr += featNum;
		}

		printf_simplify("******Feat : %4d, maxMode: %lf*****\n", fNum, maxMode);

		zFeatPtr = zFeat + fNum;
		for (i = 0; i < actPointNum; ++i) {
			zFeatPtr->feat.Zr /= maxMode;
			zFeatPtr->feat.Zi /= maxMode;

			printf_simplify("Point: %4d, Feat: %4d, Zr: %lf, Zi: %lf\n",
				i, fNum, zFeatPtr->feat.Zr, zFeatPtr->feat.Zi);

			zFeatPtr += featNum;
		}
	}
}


int
describeZernike(unsigned char* img, unsigned char* imgMark, Feat* zFeat, int width, int height, int window, int orderMax)
{
	int	i = 0;
	int startPoint = width * window / 2 + window / 2;
	int endPoint = width * (height - window / 2) - window / 2;
	int pNumCounter = 0;
	int actPointNum = 0;

	int orderN = 0;
	int orderM = 0;
	int featNum = calcFeatNum(orderMax);


	for (i = startPoint; i < endPoint; ++i) {
		int fNum = 0;
		int x, y;

		if (imgMark[i] == 0){
			continue;
		}

		x = i % width;
		y = i / width;
		printf_simplify("\n====================================================\n");
		printf_simplify("point = %d, x = %d, y = %d\n", pNumCounter, x, y);

		// get feat for each point
		for (orderN = 0; orderN <= orderMax; ++orderN)
		{
			// ! requirements : m <= n && (n-m) % 2 = 0
			for (orderM = 0; orderM <= orderN; orderM++) {
				if ((orderN - orderM) % 2 != 0) {
					continue;
				}

				Complex z = Zernike(orderN, orderM, window, &img[i], width);
				zFeat[pNumCounter * featNum + fNum].feat = z;
				zFeat[pNumCounter * featNum + fNum].x = x;
				zFeat[pNumCounter * featNum + fNum].y = y;
				fNum++;

				printf_simplify("fNum = %4d, n = %4d, m = %4d; shi:%lf, xu:%lf, mode:%lf\n", fNum, orderN, orderM, z.Zr, z.Zi, z.Mode);
			}
		}

		// calculate real point feature num
		pNumCounter++;
	}
	actPointNum = pNumCounter;
	printf_detailed("actPointNum = %d\n\n", actPointNum);

	/*------------------- Normalization ---------------------*/
	normalZernikeFeat(zFeat, actPointNum, featNum);

	return actPointNum;
}



double 
matchZernike(Feat* pointsL, int matchNum, Feat* pointsR, int pointRNum, int featNum, MatchCouple* pairs) {
	int LeftNum, rightNum;
	int matchRightNum;

	double zModeDist = 0.0;
	double minZModeDist = 1000.0;

	Feat* pointsLHead = pointsL;
	Feat* pointsRHead = pointsR;

	for (LeftNum = 0; LeftNum < matchNum; ++LeftNum) {
		for (rightNum = 0; rightNum < pointRNum; ++rightNum) {
			zModeDist = calcZernikeModeDist(pointsL, pointsR, featNum);

			if (zModeDist < minZModeDist) {
				minZModeDist = zModeDist;
				matchRightNum = rightNum;
			}
			printf_simplify("L: %4d, R: %4d, zModeDist: %lf\n", LeftNum, rightNum, zModeDist);
			pointsR += featNum;
		}

		addZernikeNewPair(pairs++, LeftNum, matchRightNum, &pointsLHead[featNum * LeftNum], &pointsRHead[featNum * matchRightNum]);

		pointsR -= pointRNum * featNum;
		pointsL += featNum;
		minZModeDist = 1000.0;
		matchRightNum = 0;
	}

	return 1.0;
}


double 
calcZernikeModeDist(Feat* pointL, Feat* pointR, int featNum) {
	int i;

	double zModeDist = 0.0;
	//    double modeL, modeR;
	double modeDist;
	double modeL_zi, modeL_zr, modeR_zi, modeR_zr;

	for (i = 0; i < featNum; ++i) {
		//        modeL = pointL[i].feat.Mode;
		//        modeR = pointR[i].feat.Mode;

		//        modeDist = modeL - modeR;
		//        zModeDist += modeDist * modeDist;

		modeL_zi = pointL[i].feat.Zi;
		modeL_zr = pointL[i].feat.Zr;
		modeR_zi = pointR[i].feat.Zi;
		modeR_zr = pointR[i].feat.Zr;

		modeDist = modeL_zi - modeR_zi;
		zModeDist += modeDist * modeDist;
		modeDist = modeL_zr - modeR_zr;
		zModeDist += modeDist * modeDist;
	}

	zModeDist = sqrt(zModeDist);

	return zModeDist;
}


void 
addZernikeNewPair(MatchCouple* pairs, int L_i, int R_j, Feat* pointL, Feat* pointR) 
{
	static int num = 0;

	pairs->Lx= pointL->x;
	pairs->Ly = pointL->y;
	pairs->Rx = pointR->x;
	pairs->Ry = pointR->y;

	printf_simplify("Pair %4d X,Y: (%4d, %4d), (%4d, %4d)\n",num++, pairs->Lx, pairs->Ly, pairs->Rx, pairs->Ry);
}


// Get zernike feat
void
getZernikeFeat
(
unsigned char* leftImg,
unsigned char* rightImg,
int orderMax,
int window,
int width,
int height,
int leftPointNum,
int rightPointNum,
unsigned char* leftImgMark,
unsigned char* rightImgMark,
pMatchCouple couple,
int* pMatchNum
)
{
	int featNum = calcFeatNum(orderMax);

	Feat* zFeatL = (Feat*)calloc_check(leftPointNum * featNum, sizeof(Feat));
	int actLeftPointNum = describeZernike(leftImg, leftImgMark, zFeatL, width, height, window, orderMax);
	Feat* zFeatR = (Feat*)calloc_check(rightPointNum * featNum, sizeof(Feat));
	int actRightPointNum = describeZernike(rightImg, rightImgMark, zFeatR, width, height, window, orderMax);

	*pMatchNum = MIN(actLeftPointNum, actRightPointNum); 
	matchZernike(zFeatL, *pMatchNum, zFeatR, actRightPointNum, featNum, couple);
}