#include "pointMatch.h"
#include "stdfunc.h"
#include <math.h>


void 
normalFeat(Feat* zFeat, int actPointNum, int featNum) {
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
describe(unsigned char* img, unsigned char* img_Mark, Feat* zFeat, int width, int height, int window, int orderMax)
{
	int       i = 0;
	int       startPoint = width * window / 2 + window / 2;
	int       endPoint = width * (height - window / 2) - window / 2;
	int       pNumCounter = 1;
	int       actPointNum = 0;

	int        orderN = 0;
	int        orderM = 0;
	int       featNum = calcFeatNum(orderMax);


	for (i = startPoint; i < endPoint; ++i) {
		int fNum = 0;
		int x, y;

		if (img_Mark[i] == 0){
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
				Complex z;

				if ((orderN - orderM) % 2 != 0) {
					continue;
				}

				z = Zernike(orderN, orderM, window, &img[i], width);
				zFeat[pNumCounter * featNum + fNum].feat = z;
				zFeat[pNumCounter * featNum + fNum].x = x;
				zFeat[pNumCounter * featNum + fNum].y = y;
				fNum++;

				printf_simplify("fNum = %4d, n = %4d, m = %4d; shi:%lf, xu:%lf, mode:%lf\n",
					fNum, orderN, orderM, z.Zr, z.Zi, z.Mode);
			}
		}

		// calculate real point feature num
		pNumCounter++;
	}
	actPointNum = pNumCounter;
	printf_detailed("actPointNum = %d\n\n", actPointNum);

	/*------------------- Normalization ---------------------*/
	normalFeat(zFeat, actPointNum, featNum);

	return actPointNum;
}



double 
match(Feat* pointsL, int pointLNum, Feat* pointsR, int pointRNum, int featNum, MatchCouple* pairs) {
	int i, j;
	int matchJ;

	double zModeDist = 0.0;
	double minZModeDist = 1000.0;

	Feat* pointsLHead = pointsL;
	Feat* pointsRHead = pointsR;

	for (i = 0; i < pointLNum; ++i) {
		for (j = 0; j < pointRNum; ++j) {
			zModeDist = calcModeDist(pointsL, pointsR, featNum);

			if (zModeDist < minZModeDist) {
				minZModeDist = zModeDist;
				matchJ = j;
			}
			printf_simplify("L: %4d, R: %4d, zModeDist: %lf\n", i, j, zModeDist);
			pointsR += featNum;
		}

		addNewPair(pairs++, i, matchJ, &pointsLHead[featNum * i], &pointsRHead[featNum * matchJ]);

		pointsR -= pointRNum * featNum;
		pointsL += featNum;
		minZModeDist = 1000000.0;
		matchJ = 0;
	}

	return 1.0;
}


double 
calcModeDist(Feat* pointL, Feat* pointR, int featNum) {
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
addNewPair(MatchCouple* pairs, int L_i, int R_j, Feat* pointL, Feat* pointR) 
{
	static int num = 0;

	pairs->Lx= pointL->x;
	pairs->Ly = pointL->y;
	pairs->Rx = pointR->x;
	pairs->Ry = pointR->y;

	printf_simplify("Pair %4d X,Y: (%4d, %4d), (%4d, %4d)\n",num++, pairs->Lx, pairs->Ly, pairs->Rx, pairs->Ry);
}
