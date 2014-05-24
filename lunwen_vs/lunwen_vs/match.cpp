#include "match.h"
#include <math.h>
#include "harris.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>


void drawCross (Uint8* img, Uint8* img_Mark, Uint8 len, Int16 width, Int16 height) {
	Int32 i,j,k;

	Int16 x,y;

	Int32 startPoint      = width * len + len;
	Int32 endPoint        = width * (height - len) - len;

	img_Mark += startPoint;
	for (k = startPoint; k < endPoint; ++k) {
		if (*img_Mark++ == 0)
			continue;

		x = k % width;
		y = k / width;

		for (i = x - len; i < x + len; ++i) {
			*(img + y * width + i) = 255;
		}

		for (j = y - len; j < y + len; ++j) {
			*(img + j * width + x) = 255;
		}
	}
}


Uint32 getFeat(Uint8* img, Feat* zFeat,
			   Int16 width, Int16 height, 
			   Int32 tarPointNum, Int8 window, Int8 orderMax) {
	/*------------------ Memory allcation --------------------*/
	Int32       i               = 0;

	Int8        orderN          = 0;
	Int8        orderM          = 0;
	Int16       featNum         = calcFeatNum (orderMax);
	Uint32      curPointNum     = 0;
	Int32       actPointNum     = 0;
	Int32       pNumCounter     = 0;

	Int32       fullSize        = width * height;
	Int32       startPoint      = width * window/2 + window/2;
	Int32       endPoint        = width * (height - window/2) - window/2;

	Int32*      img_Ix          = (Int32*) calloc_check (fullSize, sizeof(Int32));
	Int32*      img_Iy          = (Int32*) calloc_check (fullSize, sizeof(Int32));
	Int32*      img_Ixy         = (Int32*) calloc_check (fullSize, sizeof(Int32));

	F32*      img_Ix_out		= (F32*) calloc_check (fullSize, sizeof(F32));
	F32*      img_Iy_out		= (F32*) calloc_check (fullSize, sizeof(F32));
	F32*      img_Ixy_out		= (F32*) calloc_check (fullSize, sizeof(F32));

	Uint8*      img_Mark        = (Uint8*) calloc_check (fullSize, sizeof(Uint8));


	/*--------------------- Processing -----------------------*/
	// step 1 -- generate the gradient image
	gradImage (img, img_Ix, img_Iy, img_Ixy, width, height);
	cv::Mat img_Ix_Mat = cv::Mat(height, width, CV_32SC1, img_Ix);
	cv::imwrite("G:/Pics/Results/view1_Ix_2.jpg", img_Ix_Mat);


	// step 2 -- guassin blur the gradient image
	guassin (img_Ix,  img_Ix_out,  width, height);
	guassin (img_Iy,  img_Iy_out,  width, height);
	guassin (img_Ixy, img_Ixy_out, width, height);


	// step 3 -- using harris to find the extreme points
	curPointNum = harris (img_Ix_out, img_Iy_out, img_Ixy_out, img_Mark, width, height, tarPointNum);


	// step 4 -- using SURF/Hu/Zernike descriptors to descript the extreme points
	for (i = startPoint; i < endPoint ; ++i) {
		Int16 fNum  = 0;
		Int16 x,y;

		if (img_Mark[i] == 0){
			continue;
		}

		x = i % width;
		y = i / width;
		printf_simplify ("\n====================================================\n");
		printf_simplify ("point = %d, x = %d, y = %d\n", pNumCounter, x, y);

		// get feat for each point
		for (orderN = 0; orderN <= orderMax; ++orderN) {

			// ! requirements : m <= n && (n-m) % 2 = 0
			for (orderM = 0; orderM <= orderN; orderM++) {
				Complex z;

				if ((orderN-orderM)%2 != 0) {
					continue;
				}

				z = Zernike (orderN, orderM, window, &img[i], width);
				zFeat[pNumCounter * featNum + fNum].feat = z;
				zFeat[pNumCounter * featNum + fNum].x = x;
				zFeat[pNumCounter * featNum + fNum].y = y;
				fNum ++;

				printf_simplify ("fNum = %4d, n = %4d, m = %4d; shi:%lf, xu:%lf, mode:%lf\n",
								 fNum, orderN, orderM, z.Zr, z.Zi, z.Mode);
			}
		}

		// calculate real point feature num
		pNumCounter      ++;
	}
	actPointNum = pNumCounter;
	printf_detailed ("actPointNum = %d\n\n", actPointNum);


	/*------------------- Normalization ---------------------*/
	normalFeat(zFeat, actPointNum, featNum);


	/*------------------- Image Copy back ---------------------*/
	//memcpy (img, img_Mark, fullSize);
	drawCross (img, img_Mark, 5, width, height);


	/*--------------------- Free Memory -----------------------*/
	free (img_Ix);
	free (img_Iy);
	free (img_Ixy);
	free (img_Ix_out);
	free (img_Iy_out);
	free (img_Ixy_out);
	free (img_Mark);


	/*--------------------- Return Feature -----------------------*/
	return actPointNum;
}


void normalFeat(Feat* zFeat, Int32 actPointNum, Int16 featNum) {
	Int32 i;
	Int16 fNum;

	double maxMode = 0.0;
	double mode;

	Feat* zFeatPtr = zFeat;

	for (fNum = 0; fNum < featNum; ++fNum) {
		if (fNum == 86) {
			fNum ++;
			fNum --;
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

		printf_simplify ("******Feat : %4d, maxMode: %lf*****\n", fNum, maxMode);

		zFeatPtr = zFeat + fNum;
		for (i = 0; i < actPointNum; ++i) {
			zFeatPtr->feat.Zr /= maxMode;
			zFeatPtr->feat.Zi /= maxMode;

			printf_simplify ("Point: %4d, Feat: %4d, Zr: %lf, Zi: %lf\n",
							 i, fNum, zFeatPtr->feat.Zr, zFeatPtr->feat.Zi);

			zFeatPtr += featNum;
		}
	}
}


float match (Feat* pointsL, Uint32 pointLNum, Feat* pointsR, Uint32 pointRNum, Int16 featNum, MatchPair* pairs) {
	Uint32 i,j;
	Int32 matchJ;

	double zModeDist = 0.0;
	double minZModeDist = 1000.0;

	Feat* pointsLHead = pointsL;
	Feat* pointsRHead = pointsR;

	for (i = 0; i < pointLNum; ++i) {
		for (j = 0; j < pointRNum; ++j) {
			zModeDist = calcModeDist (pointsL, pointsR, featNum);

			if (zModeDist < minZModeDist) {
				minZModeDist = zModeDist;
				matchJ = j;
			}
			printf_simplify("L: %4d, R: %4d, zModeDist: %lf\n", i, j, zModeDist);
			pointsR += featNum;
		}

		addNewPair (pairs++, i, matchJ, &pointsLHead[featNum * i], &pointsRHead[featNum * matchJ]);

		pointsR -= pointRNum * featNum;
		pointsL += featNum;
		minZModeDist = 1000000.0;
		matchJ = 0;
	}

	return 1.0;
}


double calcModeDist (Feat* pointL, Feat* pointR, Int16 featNum) {
	Int16 i;

	double zModeDist = 0.0;
//    double modeL, modeR;
	double modeDist;
	double modeL_zi, modeL_zr, modeR_zi, modeR_zr;

	for (i = 0; i < featNum; ++i) {
//        modeL = pointL[i].feat.Mode;
//        modeR = pointR[i].feat.Mode;

//        modeDist = modeL - modeR;
//        zModeDist += modeDist * modeDist;
		if (i == 80) {
			i ++;
			i --;
		}

		modeL_zi = pointL[i].feat.Zi;
		modeL_zr = pointL[i].feat.Zr;
		modeR_zi = pointR[i].feat.Zi;
		modeR_zr = pointR[i].feat.Zr;

		modeDist = modeL_zi - modeR_zi;
		zModeDist += modeDist * modeDist;
		modeDist = modeL_zr - modeR_zr;
		zModeDist += modeDist * modeDist;
	}

	zModeDist = sqrt (zModeDist);

	return zModeDist;
}


void addNewPair (MatchPair* pairs, Int32 L_i, Int32 R_j, Feat* pointL, Feat* pointR) {
	static Int32 num = 0;

	pairs->L_i = L_i;
	pairs->L_i_x = pointL->x;
	pairs->L_i_y = pointL->y;
	pairs->R_j = R_j;
	pairs->R_j_x = pointR->x;
	pairs->R_j_y = pointR->y;

	printf_simplify ("Pair %4d : (%4d, %4d), X,Y: (%4d, %4d), (%4d, %4d)\n",
					 num++, L_i, R_j, pairs->L_i_x, pairs->L_i_y, pairs->R_j_x, pairs->R_j_y);
}
