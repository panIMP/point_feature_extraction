#include "hu.h"
#include "stdfunc.h"
#include <math.h>
#include <string.h>


/*Create sevaral integral image of input image*/
void
createIntSImg
(
unsigned char* img,
double*	imgInt,
int width,
int height
)
{
	// size of the init image and integral image
	int fullSize = width * height;
	int fullSizeInt = (width + 1) * (height + 1);

	double* imgTmp = (double*)calloc_check(fullSize, sizeof(double));

	int seq = 0;
	for (int xOrder = 0; xOrder <= 3; ++xOrder)
	{
		for (int yOrder = 0; yOrder <= 3; ++yOrder)
		{
			// only calculate: x0y0, x0y1, x0y2, x0y3, x1y0, x1y1, x1y2, x2y0, x2y1, x3y0
			if (xOrder + yOrder > 3)
				continue;

			// Step 1. construct a image that are used as img, x*img, y*img, x*y*img,...
			for (int i = 0; i < fullSize; ++i)
			{
				int x = i % width;
				int y = i / width;

				imgTmp[i] = pow((double)x, xOrder) * pow((double)y, yOrder) * (double)img[i];
			}

			// Step 2. construct the corresponding integral image
			createIntImg(imgTmp, imgInt + seq * fullSizeInt, width, height);
			
			// Step 3. point to the next integral image
			seq++;
		}
	}

	free(imgTmp);
}



/*Get the hu matrix by appointed window of the appointed image, imgInt is the integral image of source image*/
int
getHuMoments
(
unsigned char* img,
unsigned char* imgMark,
huFeat* feats,
int window,
int width,
int height
)
{
	// image size information
	int widthInt = width + 1;
	int heightInt = height + 1;
	int fullSizeInt = widthInt * heightInt;

	// 10 integral image: x0y0, x1y0, x0y1, x2y0, x0y2, x1y1, x3y0, x2y1, x1y2, x0y3 and calculate them
	double* imgInt = (double*)calloc_check(fullSizeInt * 10, sizeof(double));
	createIntSImg(img, imgInt, width, height);

	// the first effective pixel position of every int image
	double* imgIntX0Y0Ptr = imgInt;
	double* imgIntX0Y1Ptr = imgIntX0Y0Ptr + fullSizeInt;
	double* imgIntX0Y2Ptr = imgIntX0Y1Ptr + fullSizeInt;
	double* imgIntX0Y3Ptr = imgIntX0Y2Ptr + fullSizeInt;
	double* imgIntX1Y0Ptr = imgIntX0Y3Ptr + fullSizeInt;
	double* imgIntX1Y1Ptr = imgIntX1Y0Ptr + fullSizeInt;
	double* imgIntX1Y2Ptr = imgIntX1Y1Ptr + fullSizeInt;
	double* imgIntX2Y0Ptr = imgIntX1Y2Ptr + fullSizeInt;
	double* imgIntX2Y1Ptr = imgIntX2Y0Ptr + fullSizeInt;
	double* imgIntX3Y0Ptr = imgIntX2Y1Ptr + fullSizeInt;

	// district that can calculate feature
	int dist = window / 2;
	int xStart = dist;
	int xEnd = width - xStart;
	int yStart = dist;
	int yEnd = height - yStart;
	int actPointNum = 0;

	for (int y = yStart; y < yEnd; ++y)
	{
		for (int x = xStart; x < xEnd; ++x)
		{
			int pixNum = y * width + x;

			// only those marked as interest point will be described with hu-matrix
			if (imgMark[pixNum] == 0)
				continue;

			// four corners of the window which (x,y) is the center in the integral image
			int pLT = (y - dist) * widthInt + x - dist;
			int pRT = (y - dist) * widthInt + x + dist + 1;
			int pLB = (y + dist + 1) * widthInt + x - dist;
			int pRB = (y + dist + 1) * widthInt + x + dist + 1;

			// calculate n moments
			double m00 = imgIntX0Y0Ptr[pRB] - imgIntX0Y0Ptr[pLB] - imgIntX0Y0Ptr[pRT] + imgIntX0Y0Ptr[pLT];
			double m10 = imgIntX1Y0Ptr[pRB] - imgIntX1Y0Ptr[pLB] - imgIntX1Y0Ptr[pRT] + imgIntX1Y0Ptr[pLT];
			double m01 = imgIntX0Y1Ptr[pRB] - imgIntX0Y1Ptr[pLB] - imgIntX0Y1Ptr[pRT] + imgIntX0Y1Ptr[pLT];
			double m20 = imgIntX2Y0Ptr[pRB] - imgIntX2Y0Ptr[pLB] - imgIntX2Y0Ptr[pRT] + imgIntX2Y0Ptr[pLT];
			double m11 = imgIntX1Y1Ptr[pRB] - imgIntX1Y1Ptr[pLB] - imgIntX1Y1Ptr[pRT] + imgIntX1Y1Ptr[pLT];
			double m02 = imgIntX0Y2Ptr[pRB] - imgIntX0Y2Ptr[pLB] - imgIntX0Y2Ptr[pRT] + imgIntX0Y2Ptr[pLT];
			double m30 = imgIntX3Y0Ptr[pRB] - imgIntX3Y0Ptr[pLB] - imgIntX3Y0Ptr[pRT] + imgIntX3Y0Ptr[pLT];
			double m21 = imgIntX2Y1Ptr[pRB] - imgIntX2Y1Ptr[pLB] - imgIntX2Y1Ptr[pRT] + imgIntX2Y1Ptr[pLT];
			double m12 = imgIntX1Y2Ptr[pRB] - imgIntX1Y2Ptr[pLB] - imgIntX1Y2Ptr[pRT] + imgIntX1Y2Ptr[pLT];
			double m03 = imgIntX0Y3Ptr[pRB] - imgIntX0Y3Ptr[pLB] - imgIntX0Y3Ptr[pRT] + imgIntX0Y3Ptr[pLT];

			double xEven = m10 / (m00);
			double yEven = m01 / (m00);

			// calculate ¦Ì00, ¦Ì01, ¦Ì10, ...
			double u00 = m00;
			double u20 = m20 - 2 * xEven * m10 + xEven * xEven * m00;
			double u11 = m11 - xEven * m01 - yEven * m10 + xEven * yEven * m00;
			double u02 = m02 - 2 * yEven * m01 + yEven * yEven * m00;
			double u30 = m30 - 3 * xEven * m20 + 3 * xEven * xEven * m10 - xEven * xEven * xEven * m00;
			double u21 = m21 - 2 * xEven * m11 + xEven * xEven * m01 - yEven * m20 + 2 * xEven * yEven * m10 - xEven * xEven * yEven * m00;
			double u12 = m12 - 2 * yEven * m11 + yEven * yEven * m10 - xEven * m02 + 2 * yEven * xEven * m01 - yEven * yEven * xEven * m00;
			double u03 = m03 - 3 * yEven * m02 + 3 * yEven * yEven * m01 - yEven * yEven * yEven * m00;

			// calculate n11, n12, n21, ...
			double u00sqrt = sqrt(u00);
			double u00square = u00 * u00;
			double n20 = u20 / u00square;
			double n02 = u02 / u00square;
			double n11 = u11 / u00square;
			double n30 = u30 / (u00square * u00sqrt);
			double n21 = u21 / (u00square * u00sqrt);
			double n12 = u12 / (u00square * u00sqrt);
			double n03 = u03 / (u00square * u00sqrt);

			// calculte hu-moments: M1, M2, M3, ...
			/*double M1 = n20 + n02;
			double M2 = pow(n20 - n02, 2) + 4 * n11 * n11;
			double M3 = pow(n30 - 3 * n12, 2) + pow(3 * n21 - n03, 2);
			double M4 = pow(n30 + n12, 2) + pow(n21 + n03, 2);
			double M5 = (n30 - 3 * n12)*(n30 + n12)*(pow(n30 + n12, 2) - 3 * pow(n21 + n03, 2)) +
						(3 * n21 - n03)*(n21 + n03)*(3 * pow(n12 + n30, 2) - pow(n21 + n03, 2));
			double M6 = (n20 - n02)*(pow(n30 + n12, 2) - pow(n21 + n03, 2)) + 4 * n11 * (n30 + n12) * (n21 + n03);
			double M7 = (3 * n21 - n03) * (n30 + n12) * (pow(n30 + n12, 2) - 3 * pow(n21 + n03, 2)) -
						(n30 - 3 * n12) * (n21 + n03) * (3 * pow(n30 + n12, 2) - pow(n21 + n03, 2));*/
			double M1 = n20;
			double M2 = n02;
			double M3 = n11;
			double M4 = n30;
			double M5 = n21;
			double M6 = n12;
			double M7 = n03;

			// store the feature 
			huMoment moment = { M1, M2, M3, M4, M5, M6, M7 };
			huFeat feat = { moment, x, y };
			feats[actPointNum] = feat;

			// print it, for debug
			printf_simplify("Interest Point %3d :(%3d, %3d) : (%e, %e, %e, %e, %e, %e, %e)\n", 
				actPointNum, x, y, M1, M2, M3, M4, M5, M6, M7);

			actPointNum++;
		}
	}

	printf_simplify("%d interest points are described\n\n", actPointNum);

	return actPointNum;
}