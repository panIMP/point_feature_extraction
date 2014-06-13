#include "pointLocate.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "stdfunc.h"


/*Template to generate hessin pyramids*/
const int g_hessin_template[2 * 4 *3 * 26] = 
{
	/*1st octave*/
	// 9 * 9
	3, 0, 2,  2,  6,  15, 1,  3,  2, 5,   6,  15, -2,  6,  2,  8,  6,  15,  1,  0,  0,  0,  0,   0, 0, 1.2,
	3, 2, 0,  6,  2,  15, 1,  2,  3, 6,   5,  15, -2,  2,  6,  6,  8,  15,  1,  0,  0,  0,  0,   0, 0, 1.2,
	4, 1, 1,  3,  3,   9, 1,  5,  1, 7,   3,   9, -1,  1,  5,  3,  7,   9, -1,  5,  5,  7,  7,   9, 1, 1.2,

	// 15 * 15
	3, 0, 3,  4, 11,  45, 1,  5,  3,  9, 11,  45, -2, 10,  3, 14, 11,  45,  1,  0,  0,  0,  0,   0, 0, 2.0,
	3, 3, 0, 11,  4,  45, 1,  3,  5, 11,  9,  45, -2,  3, 10, 11, 14,  45,  1,  0,  0,  0,  0,   0, 0, 2.0,
	4, 2, 2,  6,  6,  25, 1,  8,  2, 12,  6,  25, -1,  2,  8,  6, 12,  25, -1,  8,  8, 12, 12,  25, 1, 2.0,

	// 21 * 21
	3, 0, 4,  6, 16,  91, 1,  7,  4, 13, 16,  91, -2, 14,  4, 20, 16,  91,  1,  0,  0,  0,  0,   0, 0, 2.8,
	3, 4, 0, 16,  6,  91, 1,  4,  7, 16, 13,  91, -2,  4, 14, 16, 20,  91,  1,  0,  0,  0,  0,   0, 0, 2.8,
	4, 3, 3,  9,  9,  49, 1, 11,  3, 17,  9,  49, -1,  3, 11,  9, 17,  49, -1, 11, 11, 17, 17,  49, 1, 2.8,

	// 27 * 27
	3, 0, 5,  8, 21, 153, 1,  9,  5, 17, 21, 153, -2, 18,  5, 26, 21, 153,  1,  0,  0,  0,  0,   0, 0, 3.6,
	3, 5, 0, 21,  8, 153, 1,  5,  9, 21, 17, 153, -2,  5, 18, 21, 26, 153,  1,  0,  0,  0,  0,   0, 0, 3.6,
	4, 4, 4, 12, 12,  81, 1, 14,  4, 22, 12,  81, -1,  4, 14, 12, 22,  81, -1, 14, 14, 22, 22,  81, 1, 3.6,

	/*2nd octave*/
	// 15 * 15
	3, 0, 3,  4, 11,  45, 1,  5,  3,  9, 11,  45, -2, 10,  3, 14, 11,  45,  1,  0,  0,  0,  0,   0, 0, 2.0,
	3, 3, 0, 11,  4,  45, 1,  3,  5, 11,  9,  45, -2,  3, 10, 11, 14,  45,  1,  0,  0,  0,  0,   0, 0, 2.0,
	4, 2, 2,  6,  6,  25, 1,  8,  2, 12,  6,  25, -1,  2,  8,  6, 12,  25, -1,  8,  8, 12, 12,  25, 1, 2.0,

	// 27 * 27
	3, 0, 5,  8, 21, 153, 1,  9,  5, 17, 21, 153, -2, 18,  5, 26, 21, 153,  1,  0,  0,  0,  0,   0, 0, 3.6,
	3, 5, 0, 21,  8, 153, 1,  5,  9, 21, 17, 153, -2,  5, 18, 21, 26, 153,  1,  0,  0,  0,  0,   0, 0, 3.6,
	4, 4, 4, 12, 12,  81, 1, 14,  4, 22, 12,  81, -1,  4, 14, 12, 22,  81, -1, 14, 14, 22, 22,  81, 1, 3.6,

	// 39 * 39
	3, 0, 7, 12, 31, 325, 1, 13,  7, 25, 31, 325, -2, 26,  7, 38, 31, 325,  1,  0,  0,  0,  0,   0, 0, 5.2,
	3, 7, 0, 31, 12, 325, 1,  7, 13, 31, 25, 325, -2,  7, 26, 31, 28, 325,  1,  0,  0,  0,  0,   0, 0, 5.2,
	4, 6, 6, 18, 18, 169, 1, 20,  6, 32, 18, 169, -1,  6, 20, 18, 32, 169, -1, 20, 20, 32, 32, 169, 1, 5.2,

	// 51 * 51
	3, 0, 9, 16, 41, 561, 1, 17,  9, 33, 41, 561, -2, 34,  9, 50, 41, 561,  1,  0,  0,  0,  0,   0, 0, 6.8,
	3, 9, 0, 41, 16, 561, 1,  9, 17, 41, 33, 561, -2,  9, 34, 41, 50, 561,  1,  0,  0,  0,  0,   0, 0, 6.8,
	4, 8, 8, 24, 24, 289, 1, 26,  8, 42, 24, 289, -1,  8, 26, 24, 42, 289, -1, 26, 26, 42, 42, 289, 1, 6.8,
};


int* getHessinTemplate(void)
{
	return (int*)g_hessin_template;
}


/*OpenCV method to find the interest points by using harris*/ 
void
opecvHarris
(
const cv::Mat& img,
cv::Mat& imgMark,
double harrisThresh,
double binaryThresh
)
{
	cv::cornerHarris(img, imgMark, 3, 3, harrisThresh);
	cv::threshold(imgMark, imgMark, binaryThresh, 255, cv::THRESH_BINARY);
	imgMark.convertTo(imgMark, CV_8UC1);
}


/*Create the integral image of input image*/
void
createIntImg
(
unsigned char* img,
double*	imgInt,
int width,
int height
)
{
	*imgInt = *img;
	imgInt++;
	img++;
	for (int x = 1; x < width; ++x, ++imgInt, ++img)
	{
		*imgInt = *img + *(imgInt-1);
	}

	for (int y = 1; y < height; ++y)
	{
		int sum = 0;
		for (int x = 0; x < width; ++x, ++img, ++imgInt)
		{
			sum += *img;
			*imgInt = *(imgInt - width) + sum;
		}
	}
}


/*Calculate thresh for edge suppression*/
static double calcEdgeSupprThresh(double r)
{
	return ((r + 1.0) * (r + 1.0) / r);
}


/*Generate the hessin-pyramid*/
void
createHessinPyramid
(
unsigned char* img,
double* imgInt,
double* imgHesPyr,
int octaveNum,
int layerNum,
const int* hesTempl,
unsigned char* imgMark,
int width,
int height
)
{
	for (int oct = 0; oct < octaveNum; ++oct)
	{
		for (int lay = 0; lay < layerNum; ++lay)
		{
			// Corner point coordinates of the 3 rects in Lxx image
			int startPos = oct * layerNum * 3 * 26 + lay * 3 * 26;

			int xStart = (hesTempl[startPos + 7] + hesTempl[startPos + 9]) / 2;
			int yStart = (hesTempl[startPos + 8] + hesTempl[startPos + 10]) / 2;
			int xEnd = width - xStart;
			int yEnd = height - yStart;

			int pxx11 = hesTempl[startPos + 2] * width + hesTempl[startPos + 1];
			int pxx12 = hesTempl[startPos + 2] * width + hesTempl[startPos + 3];
			int pxx13 = hesTempl[startPos + 4] * width + hesTempl[startPos + 1];
			int pxx14 = hesTempl[startPos + 4] * width + hesTempl[startPos + 3];
			int sxx1 = hesTempl[startPos + 5];
			int wxx1 = hesTempl[startPos + 6];

			int pxx21 = hesTempl[startPos + 8] * width + hesTempl[startPos + 7];
			int pxx22 = hesTempl[startPos + 8] * width + hesTempl[startPos + 9];
			int pxx23 = hesTempl[startPos + 10] * width + hesTempl[startPos + 7];
			int pxx24 = hesTempl[startPos + 10] * width + hesTempl[startPos + 9];
			int sxx2 = hesTempl[startPos + 11];
			int wxx2 = hesTempl[startPos + 12];

			int pxx31 = hesTempl[startPos + 14] * width + hesTempl[startPos + 13];
			int pxx32 = hesTempl[startPos + 14] * width + hesTempl[startPos + 15];
			int pxx33 = hesTempl[startPos + 16] * width + hesTempl[startPos + 13];
			int pxx34 = hesTempl[startPos + 16] * width + hesTempl[startPos + 15];
			int sxx3 = hesTempl[startPos + 17];
			int wxx3 = hesTempl[startPos + 18];

			// Corner point coordinates of the 3 rects in Lyy image
			startPos += 26;

			int pyy11 = hesTempl[startPos + 2] * width + hesTempl[startPos + 1];
			int pyy12 = hesTempl[startPos + 2] * width + hesTempl[startPos + 3];
			int pyy13 = hesTempl[startPos + 4] * width + hesTempl[startPos + 1];
			int pyy14 = hesTempl[startPos + 4] * width + hesTempl[startPos + 3];
			int syy1 = hesTempl[startPos + 5];
			int wyy1 = hesTempl[startPos + 6];

			int pyy21 = hesTempl[startPos + 8] * width + hesTempl[startPos + 7];
			int pyy22 = hesTempl[startPos + 8] * width + hesTempl[startPos + 9];
			int pyy23 = hesTempl[startPos + 10] * width + hesTempl[startPos + 7];
			int pyy24 = hesTempl[startPos + 10] * width + hesTempl[startPos + 9];
			int syy2 = hesTempl[startPos + 11];
			int wyy2 = hesTempl[startPos + 12];

			int pyy31 = hesTempl[startPos + 14] * width + hesTempl[startPos + 13];
			int pyy32 = hesTempl[startPos + 14] * width + hesTempl[startPos + 15];
			int pyy33 = hesTempl[startPos + 16] * width + hesTempl[startPos + 13];
			int pyy34 = hesTempl[startPos + 16] * width + hesTempl[startPos + 15];
			int syy3 = hesTempl[startPos + 17];
			int wyy3 = hesTempl[startPos + 18];

			// Corner point coordinates of the 4 rects in Lxy(Lyx) image
			startPos += 26;

			int pxy11 = hesTempl[startPos + 2] * width + hesTempl[startPos + 1];
			int pxy12 = hesTempl[startPos + 2] * width + hesTempl[startPos + 3];
			int pxy13 = hesTempl[startPos + 4] * width + hesTempl[startPos + 1];
			int pxy14 = hesTempl[startPos + 4] * width + hesTempl[startPos + 3];
			int sxy1 = hesTempl[startPos + 5];
			int wxy1 = hesTempl[startPos + 6];

			int pxy21 = hesTempl[startPos + 8] * width + hesTempl[startPos + 7];
			int pxy22 = hesTempl[startPos + 8] * width + hesTempl[startPos + 9];
			int pxy23 = hesTempl[startPos + 10] * width + hesTempl[startPos + 7];
			int pxy24 = hesTempl[startPos + 10] * width + hesTempl[startPos + 9];
			int sxy2 = hesTempl[startPos + 11];
			int wxy2 = hesTempl[startPos + 12];

			int pxy31 = hesTempl[startPos + 14] * width + hesTempl[startPos + 13];
			int pxy32 = hesTempl[startPos + 14] * width + hesTempl[startPos + 15];
			int pxy33 = hesTempl[startPos + 16] * width + hesTempl[startPos + 13];
			int pxy34 = hesTempl[startPos + 16] * width + hesTempl[startPos + 15];
			int sxy3 = hesTempl[startPos + 17];
			int wxy3 = hesTempl[startPos + 18];

			int pxy41 = hesTempl[startPos + 20] * width + hesTempl[startPos + 19];
			int pxy42 = hesTempl[startPos + 20] * width + hesTempl[startPos + 21];
			int pxy43 = hesTempl[startPos + 22] * width + hesTempl[startPos + 19];
			int pxy44 = hesTempl[startPos + 22] * width + hesTempl[startPos + 21];
			int sxy4 = hesTempl[startPos + 23];
			int wxy4 = hesTempl[startPos + 24];

			// Start filtering
			int pixelPos = yStart * width + xStart + oct * layerNum * width * height + lay * width * height;
			int pixelPosStart = pixelPos;
			for (int y = yStart; y < yEnd; ++y)
			{
				pixelPos = y * width + xStart + oct * layerNum * width * height + lay * width * height;
				for (int x = xStart; x < xEnd; ++x, ++pixelPos)
				{
					int gap = pixelPos - pixelPosStart;
					double* imgIntTmp = imgInt + gap;

					double lxx =(imgIntTmp[pxx14] - imgIntTmp[pxx13] - imgIntTmp[pxx12] + imgIntTmp[pxx11]) * wxx1 +
								(imgIntTmp[pxx24] - imgIntTmp[pxx23] - imgIntTmp[pxx22] + imgIntTmp[pxx21]) * wxx2 +
								(imgIntTmp[pxx34] - imgIntTmp[pxx33] - imgIntTmp[pxx32] + imgIntTmp[pxx31]) * wxx3;
					double lyy =(imgIntTmp[pyy14] - imgIntTmp[pyy13] - imgIntTmp[pyy12] + imgIntTmp[pyy11]) * wyy1 +
								(imgIntTmp[pyy24] - imgIntTmp[pyy23] - imgIntTmp[pyy22] + imgIntTmp[pyy21]) * wyy2 +
								(imgIntTmp[pyy34] - imgIntTmp[pyy33] - imgIntTmp[pyy32] + imgIntTmp[pyy31]) * wyy3;
					double lxy =(imgIntTmp[pxy14] - imgIntTmp[pxy13] - imgIntTmp[pxy12] + imgIntTmp[pxy11]) * wxy1 +
								(imgIntTmp[pxy24] - imgIntTmp[pxy23] - imgIntTmp[pxy22] + imgIntTmp[pxy21]) * wxy2 +
								(imgIntTmp[pxy34] - imgIntTmp[pxy33] - imgIntTmp[pxy32] + imgIntTmp[pxy31]) * wxy3 +
								(imgIntTmp[pxy44] - imgIntTmp[pxy43] - imgIntTmp[pxy42] + imgIntTmp[pxy41]) * wxy4;

					// normally sxx1 == sxx2 == sxx3 == sxx4, the others are the same.
					imgHesPyr[pixelPos] = (lxx / sxx1) * (lyy / syy1) - (0.9 * lxy / sxy1)*(0.9 * lxy / sxy1);

					double trH = lxx + lyy;
					double detH = lxx * lyy - lxy * lxy;
					static double thresh = calcEdgeSupprThresh(10.0);

					if ((trH * trH) / detH > thresh)
					{
						imgMark[y * width + x] = 1;
					}
				}
			}
		}
	}
	
}


/*Non-maximum suppression*/
void
suppressNonMaximum
(
double* imgHesPyr,
int octaveNum,
int layerNum,
const int* hesTempl,
unsigned char* imgMark,
int width,
int height
)
{
	double* A = (double*)calloc_check(9, sizeof(double));
	double* B = (double*)calloc_check(3, sizeof(double));
	double* result = (double*)calloc_check(3, sizeof(double));
	int imgSize = width * height;

	for (int oct = 0; oct < octaveNum; ++oct)
	{
		for (int lay = 1; lay < layerNum - 1; ++lay)
		{
			double* imgUp = imgHesPyr + oct * layerNum * width * height + (lay - 1) * width * height;
			double* imgMid = imgHesPyr + oct * layerNum * width * height + lay * width * height;
			double* imgDown = imgHesPyr + oct * layerNum * width * height + (lay + 1) * width * height;

			int startPos = oct * layerNum * 3 * 26 + lay * 3 * 26;

			int xStart = (hesTempl[startPos + 7] + hesTempl[startPos + 9]) / 2;
			int yStart = (hesTempl[startPos + 8] + hesTempl[startPos + 10]) / 2;
			int xEnd = width - xStart;
			int yEnd = height - yStart;

			for (int y = yStart; y < yEnd; ++y)
			{
				int firstLine = (y - 1) * width + xStart;
				int secondLine = y * width + xStart;
				int thirdLine = (y + 1) * width + xStart;
				for (int x = xStart; x < xEnd; ++x, ++firstLine, ++secondLine, ++thirdLine)
				{
					double pUp00 = imgUp[firstLine - 1];
					double pUp01 = imgUp[firstLine];
					double pUp02 = imgUp[firstLine + 1];
					double pUp10 = imgUp[secondLine - 1];
					double pUp11 = imgUp[secondLine];
					double pUp12 = imgUp[secondLine + 1];
					double pUp20 = imgUp[thirdLine - 1];
					double pUp21 = imgUp[thirdLine];
					double pUp22 = imgUp[thirdLine + 1];

					double pMid00 = imgMid[firstLine - 1];
					double pMid01 = imgMid[firstLine];
					double pMid02 = imgMid[firstLine + 1];
					double pMid10 = imgMid[secondLine - 1];
					double pMid11 = imgMid[secondLine];
					double pMid12 = imgMid[secondLine + 1];
					double pMid20 = imgMid[thirdLine - 1];
					double pMid21 = imgMid[thirdLine];
					double pMid22 = imgMid[thirdLine + 1];

					double pDown00 = imgDown[firstLine - 1];
					double pDown01 = imgDown[firstLine];
					double pDown02 = imgDown[firstLine + 1];
					double pDown10 = imgDown[secondLine - 1];
					double pDown11 = imgDown[secondLine];
					double pDown12 = imgDown[secondLine + 1];
					double pDown20 = imgDown[thirdLine - 1];
					double pDown21 = imgDown[thirdLine];
					double pDown22 = imgDown[thirdLine + 1];

					if ((pMid11 > pMid00) && (pMid11 > pMid01) && (pMid11 > pMid02) && (pMid11 > pMid10)
						&& (pMid11 > pMid12) && (pMid11 > pMid20) && (pMid11 > pMid21) && (pMid11 > pMid22)
						&& (pMid11 > pUp00) && (pMid11 > pUp01) && (pMid11 > pUp02) && (pMid11 > pUp10)
						&& (pMid11 > pUp11) && (pMid11 > pUp12) && (pMid11 > pUp20) && (pMid11 > pUp21)&& (pMid11 > pUp22) 
						&& (pMid11 > pDown00) && (pMid11 > pDown01) && (pMid11 > pDown02) && (pMid11 > pDown10) && (pMid11 > pDown11) && (pMid11 > pDown12) && (pMid11 > pDown20) && (pMid11 > pDown21) && (pMid11 > pDown22))
					{
						/*A[0] = pMid10 + pMid12 - 2 * pMid11;
						A[1] = (pMid00 + pMid22 - pMid02 - pMid20) / 4;
						A[2] = (pUp10 + pDown12 - pUp12 - pDown10) / 4;
						A[3] = A[1];
						A[4] = pMid01 + pMid21 - 2 * pMid11;
						A[5] = (pUp12 + pDown21 - pUp21 - pDown12) / 4;
						A[6] = A[2];
						A[7] = A[5];
						A[8] = pUp11 + pDown11 - 2 * pMid11;

						B[0] = -(pMid12 - pMid10) / 2;
						B[1] = -(pMid21 - pMid01) / 2;
						B[2] = -(pDown11 - pUp11) / 2;

						solve3_3(A, B, result);

						double realX = result[0];
						double realY = result[1];
						double realSigma = result[2];
						int realPos = (int)(realY * width + realX);
						if (realPos < 0)
							realPos = 0;
						else if (realPos >= imgSize)
							realPos = imgSize - 1;

						imgMark[realPos] = 255;*/

						imgMark[secondLine] = 255;
					}
					else
					{
						imgMark[secondLine] = 0;
					}
				}
			}
		}
	}
}


/*Conduct accurate sub-pixel interest point location & edge suppression & threshold suppression*/
int
locateRefine
(
double* imgHesPyr,
int octaveNum,
int layerNum,
const int* hesTempl,
double fxThresh,
double edgeThresh,
unsigned char* imgMark
)
{
	int interPointN = 0;

	return interPointN;
}


/*Using surf to find the interest points by appointed surf parameters*/
int
surf
(
cv::Mat& imgMat,
cv::Mat& imgMarkMat,
int width,
int height,
int octaveNum,
int layerNum,
const int* hesTempl
)
{
	int fullSize = width * height;

	/*Memory allocation*/
	unsigned char* img = imgMat.data;
	unsigned char* imgMark = imgMarkMat.data;
	unsigned char* imgMarkTmp = (unsigned char*)calloc_check(fullSize, sizeof(unsigned char));
	double* imgInt = (double*)calloc_check(fullSize, sizeof(double));
	double* imgHesPyr = (double*)calloc_check(fullSize * octaveNum * layerNum, sizeof(double));
	double* imgHesPyrTmp = imgHesPyr;
	for (int oct = 0; oct < octaveNum; ++oct)
	{
		for (int lay = 0; lay < layerNum; ++lay)
		{
			for (int i = 0; i < fullSize; ++i)
				*imgHesPyrTmp++ = img[i];
		}
	}

	// Step 1: Generate the integral image
	createIntImg(img, imgInt, width, height);

	// Step 2: Generate the approximate guassin-derivate image
	createHessinPyramid(img, imgInt, imgHesPyr, octaveNum, layerNum, g_hessin_template, imgMarkTmp, width, height);

	static int flag = 0;
	std::ostringstream fileName;
	if (!flag)
	{
		fileName << "E:/Pics/Results/Left";
		flag++;
	}
	else
	{
		fileName << "E:/Pics/Results/Right";
		flag--;
	}

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			cv::Mat imgOne = cv::Mat(height, width, CV_64FC1);
			double* imgHes = imgHesPyr + i * 4 * fullSize + j * fullSize;

			for (int k = 0; k < fullSize; ++k)
			{
				imgOne.at<double>(k) = imgHes[k];
			}

			fileName << "oct" << i << "lay" << j << ".jpg";
			cv::imshow(fileName.str(), imgOne);
			cv::imwrite(fileName.str(), imgOne);
			cv::waitKey(0);
		}

	}


	// Step 3: Conduct non-maximum suppression
	suppressNonMaximum(imgHesPyr, octaveNum, layerNum, g_hessin_template, imgMark, width, height);

	// Step 4: Conduct accurate sub-pixel interest point location,edge & threshold suppression
	int interPointNum = locateRefine(imgHesPyr, octaveNum, layerNum, g_hessin_template, 0.3, 10, imgMark);

	for (int i = 0; i < fullSize; ++i, ++imgMark)
	{
		//*imgMark = 255;
		if (*imgMark = *imgMark * imgMarkTmp[i])
			interPointNum++;
	}


	free(imgInt);
	free(imgHesPyr);
	free(imgMarkTmp);

	return interPointNum;
}