#include "hu.h"
#include "stdfunc.h"
#include <math.h>



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
	// the size of every int image is (width + 1, height + 1) to deal with the subtraction while at edge
	int widthInt = width + 1;
	int heightInt = height + 1;
	int fullSizeInt = widthInt * heightInt;

	// the first effective pixel position of every int image
	double* imgIntX0Y0Ptr = imgInt + widthInt + 1;
	double* imgIntX1Y0Ptr = imgIntX0Y0Ptr + fullSizeInt;
	double* imgIntX0Y1Ptr = imgIntX1Y0Ptr + fullSizeInt;
	double* imgIntX2Y0Ptr = imgIntX0Y1Ptr + fullSizeInt;
	double* imgIntX0Y2Ptr = imgIntX2Y0Ptr + fullSizeInt;
	double* imgIntX1Y1Ptr = imgIntX0Y2Ptr + fullSizeInt;
	double* imgIntX3Y0Ptr = imgIntX1Y1Ptr + fullSizeInt;
	double* imgIntX2Y1Ptr = imgIntX3Y0Ptr + fullSizeInt;
	double* imgIntX1Y2Ptr = imgIntX2Y1Ptr + fullSizeInt;
	double* imgIntX0Y3Ptr = imgIntX1Y2Ptr + fullSizeInt;

	// only imgIntX0Y0 is not zero, others are zero since x == 0 && y == 0
	*imgIntX0Y0Ptr = *img;

	// move further every pointer
	img++;

	imgIntX0Y0Ptr++;
	imgIntX1Y0Ptr++;
	imgIntX0Y1Ptr++;
	imgIntX2Y0Ptr++;
	imgIntX0Y2Ptr++;
	imgIntX1Y1Ptr++;
	imgIntX3Y0Ptr++;
	imgIntX2Y1Ptr++;
	imgIntX1Y2Ptr++;
	imgIntX0Y3Ptr++;

	// calculate the integral images of the first row
	for (int x = 1; x < width; ++x, ++imgIntX0Y0Ptr, ++img)
	{
		// y == 0 in the first row of img (not imgInt!)
		*imgIntX0Y0Ptr = *img + *(imgIntX0Y0Ptr - 1);
		*imgIntX1Y0Ptr = *img + *(imgIntX1Y0Ptr - 1) * x;
		*imgIntX0Y1Ptr = *img + *(imgIntX0Y1Ptr - 1) * 0;
		*imgIntX2Y0Ptr = *img + *(imgIntX2Y0Ptr - 1) * x * x;
		*imgIntX0Y2Ptr = *img + *(imgIntX0Y2Ptr - 1) * 0 * 0;
		*imgIntX1Y1Ptr = *img + *(imgIntX1Y1Ptr - 1) * x * 0;
		*imgIntX3Y0Ptr = *img + *(imgIntX3Y0Ptr - 1) * x * x * x;
		*imgIntX2Y1Ptr = *img + *(imgIntX2Y1Ptr - 1) * x * x * 0;
		*imgIntX1Y2Ptr = *img + *(imgIntX1Y2Ptr - 1) * x * 0 * 0;
		*imgIntX0Y3Ptr = *img + *(imgIntX0Y3Ptr - 1) * 0 * 0 * 0;
	}

	for (int y = 1; y < height; ++y)
	{
		int sumX0Y0 = 0;
		int sumX1Y0 = 0;
		int sumX0Y1 = 0;
		int sumX2Y0 = 0;
		int sumX0Y2 = 0;
		int sumX1Y1 = 0;
		int sumX3Y0 = 0;
		int sumX2Y1 = 0;
		int sumX1Y2 = 0;
		int sumX0Y3 = 0;

		imgIntX0Y0Ptr = imgInt + (y + 1) * (width + 1) + 1;
		imgIntX1Y0Ptr = imgIntX0Y0Ptr + fullSizeInt;
		imgIntX0Y1Ptr = imgIntX1Y0Ptr + fullSizeInt;
		imgIntX2Y0Ptr = imgIntX0Y1Ptr + fullSizeInt;
		imgIntX0Y2Ptr = imgIntX2Y0Ptr + fullSizeInt;
		imgIntX1Y1Ptr = imgIntX0Y2Ptr + fullSizeInt;
		imgIntX3Y0Ptr = imgIntX1Y1Ptr + fullSizeInt;
		imgIntX2Y1Ptr = imgIntX3Y0Ptr + fullSizeInt;
		imgIntX1Y2Ptr = imgIntX2Y1Ptr + fullSizeInt;
		imgIntX0Y3Ptr = imgIntX1Y2Ptr + fullSizeInt;

		for (int x = 0; x < width; ++x)
		{
			sumX0Y0 += *img;
			*imgIntX0Y0Ptr = *(imgIntX0Y0Ptr - width - 1) + sumX0Y0;

			sumX1Y0 += *img * x;
			*imgIntX1Y0Ptr = *(imgIntX1Y0Ptr - width - 1) + sumX1Y0;

			sumX0Y1 += *img * y;
			*imgIntX0Y1Ptr = *(imgIntX0Y1Ptr - width - 1) + sumX0Y1;

			sumX2Y0 += *img * x * x;
			*imgIntX2Y0Ptr = *(imgIntX2Y0Ptr - width - 1) + sumX2Y0;

			sumX0Y2 += *img * y * y;
			*imgIntX0Y2Ptr = *(imgIntX0Y2Ptr - width - 1) + sumX0Y2;

			sumX1Y1 += *img * x * y;
			*imgIntX1Y1Ptr = *(imgIntX1Y1Ptr - width - 1) + sumX1Y1;

			sumX3Y0 += *img * x * x * x;
			*imgIntX3Y0Ptr = *(imgIntX3Y0Ptr - width - 1) + sumX3Y0;

			sumX2Y1 += *img * x * x * y;
			*imgIntX2Y1Ptr = *(imgIntX2Y1Ptr - width - 1) + sumX2Y1;

			sumX1Y2 += *img * x * y * y;
			*imgIntX1Y2Ptr = *(imgIntX1Y2Ptr - width - 1) + sumX1Y2;

			sumX0Y3 += *img * y * y * y;
			*imgIntX0Y3Ptr = *(imgIntX0Y3Ptr - width - 1) + sumX0Y3;


			// increment pointers
			img++;

			imgIntX0Y0Ptr++;
			imgIntX1Y0Ptr++;
			imgIntX0Y1Ptr++;
			imgIntX2Y0Ptr++;
			imgIntX0Y2Ptr++;
			imgIntX1Y1Ptr++;
			imgIntX3Y0Ptr++;
			imgIntX2Y1Ptr++;
			imgIntX1Y2Ptr++;
			imgIntX0Y3Ptr++;
		}
	}
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
	// 10 integral image: x0y0, x1y0, x0y1, x2y0, x0y2, x1y1, x3y0, x2y1, x1y2, x0y3 and calculate them
	double* imgInt = (double*)calloc_check((width + 1) * (height + 1) * 10, sizeof(double));
	createIntSImg(img, imgInt, width, height);

	// image size information
	int widthInt = width + 1;
	int heightInt = height + 1;
	int fullSizeInt = widthInt * heightInt;

	// the first effective pixel position of every int image
	double* imgIntX0Y0Ptr = imgInt;
	double* imgIntX1Y0Ptr = imgIntX0Y0Ptr + fullSizeInt;
	double* imgIntX0Y1Ptr = imgIntX1Y0Ptr + fullSizeInt;
	double* imgIntX2Y0Ptr = imgIntX0Y1Ptr + fullSizeInt;
	double* imgIntX0Y2Ptr = imgIntX2Y0Ptr + fullSizeInt;
	double* imgIntX1Y1Ptr = imgIntX0Y2Ptr + fullSizeInt;
	double* imgIntX3Y0Ptr = imgIntX1Y1Ptr + fullSizeInt;
	double* imgIntX2Y1Ptr = imgIntX3Y0Ptr + fullSizeInt;
	double* imgIntX1Y2Ptr = imgIntX2Y1Ptr + fullSizeInt;
	double* imgIntX0Y3Ptr = imgIntX1Y2Ptr + fullSizeInt;

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
			int pLT = (y - dist) * widthInt + x - dist + 1;
			int pRT = (y - dist) * widthInt + x + dist + 1;
			int pLB = (y + dist) * widthInt + x - dist + 1;
			int pRB = (y + dist) * widthInt + x + dist + 1;

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

			double xEven = m10 / (m00 + 0.000001);
			double yEven = m01 / (m00 + 0.000001);

			// calculate ¦Ì00, ¦Ì01, ¦Ì10, ...
			double u00 = m00;
			double u10 = m10 - xEven * m00;
			double u01 = m01 - yEven * m00;
			double u20 = m20 - 2 * xEven * m10 + xEven * xEven * m00;
			double u11 = m11 - xEven * m01 - yEven * m10 + xEven * yEven * m00;
			double u02 = m02 - 2 * yEven * m01 + yEven * yEven * m00;
			double u30 = m30 - 3 * xEven * m20 + 3 * xEven * xEven * m10 - xEven * xEven * xEven;
			double u21 = m21 - 2 * xEven * m11 + xEven * xEven * m01 - yEven * m20 + 2 * xEven * yEven * m10 - xEven * xEven * yEven;
			double u12 = m12 - 2 * yEven * m11 + yEven * yEven * m10 - xEven * m02 + 2 * yEven * yEven * m01 - yEven * yEven * xEven;
			double u03 = m03 - 3 * yEven * m02 + 3 * yEven * yEven * m01 - yEven * yEven * yEven;

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
			double M1 = n20 + n02;
			double M2 = pow(n20 - n02, 2) + 4 * n11 * n11;
			double M3 = pow(n30 - 3 * n12, 2) + pow(3 * n21 - n03, 2);
			double M4 = pow(n30 + n12, 2) + pow(n21 + n03, 2);
			double M5 = (n30 - 3 * n12)*(n30 + n12)*(pow(n30 + n12, 2) - 3 * pow(n21 + n03, 2)) +
						(3 * n21 - n03)*(n21 + n03)*(3 * pow(n12 + n30, 2) - pow(n21 + n03, 2));
			double M6 = (n20 - n02)*(pow(n30 + n12, 2) - pow(n21 + n03, 2)) + 4 * n11 * (n30 + n12) * (n21 + n03);
			double M7 = (3 * n21 - n03) * (n30 + n12) * (pow(n30 + n12, 2) - 3 * pow(n21 + n03, 2)) -
						(n30 - 3 * n12) * (n21 + n03) * (3 * pow(n30 + n12, 2) - pow(n21 + n03, 2));

			// store the feature 
			huMoment moment = { M1, M2, M3, M4, M5, M6, M7 };
			huFeat feat = { moment, x, y };
			feats[actPointNum] = feat;

			actPointNum++;
		}
	}


	return actPointNum;
}