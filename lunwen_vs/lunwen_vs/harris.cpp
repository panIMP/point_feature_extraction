#include "harris.h"
#include "stdfuncs.h"


void gradImage(Uint8 *img, Int32 *img_Ix, Int32 *img_Iy, Int32 *img_Ixy, Int16 width, Int16 height)
{
	Int16 i, j;
	Int16 iEnd = width - 1;
	Int16 jEnd = height - 1;

	Uint8 p00, p01, p02;
	Uint8 p10, p11, p12;
	Uint8 p20, p21, p22;

	Int32 Ix, Iy, Ixy;

	Int32 upLineOut = 1;
	Int32 midLineOut = width + 1;
	Int32 downLineOut = 2 * width + 1;

	for (j = 1; j < jEnd; ++j, upLineOut += width, midLineOut += width, downLineOut += width)
	{
		Int32 upLineIn = upLineOut;
		Int32 midLineIn = midLineOut;
		Int32 downLineIn = downLineOut;

		for (i = 1; i < iEnd; ++i, ++upLineIn, ++midLineIn, ++downLineIn)
		{
			// Fetch 3X3 neighbour pixels
			p00 = img[upLineIn - 1];		p01 = img[upLineIn];		p02 = img[upLineIn + 1];
			p10 = img[midLineIn - 1];		p11 = img[midLineIn];		p12 = img[midLineIn + 1];
			p20 = img[downLineIn - 1];		p21 = img[downLineIn];		p22 = img[downLineIn + 1];

			// Calculate the gradient
			Ix = -p00 + p02
				- p10 + p12
				- p20 + p22;

			Iy = -p00 - p01 - p11

				+ p20 + p21 + p22;

			Ixy = Ix * Iy;

			// Store it.
			img_Ix[midLineIn] = Ix;
			img_Iy[midLineIn] = Iy;
			img_Ixy[midLineIn] = Ixy;
		}
	}
}


void
guassin(Int32* inPtr, F32* outPtr, Int16 width, Int16 height)
{
	Int16 i, j;
	Int16 iEnd = width - 1;
	Int16 jEnd = height - 1;

	Int32 p00, p01, p02;
	Int32 p10, p11, p12;
	Int32 p20, p21, p22;

	F32  sum;

	Int32 upLineOut = 1;
	Int32 midLineOut = width + 1;
	Int32 downLineOut = 2 * width + 1;

	for (j = 1; j < jEnd; ++j, upLineOut += width, midLineOut += width, downLineOut += width)
	{
		Int32 upLineIn = upLineOut;
		Int32 midLineIn = midLineOut;
		Int32 downLineIn = downLineOut;

		for (i = 1; i < iEnd; ++i, ++upLineIn, ++midLineIn, ++downLineIn)
		{
			// Fetch 3X3 neighbour pixels
			p00 = inPtr[upLineIn - 1];			p01 = inPtr[upLineIn];			p02 = inPtr[upLineIn + 1];
			p10 = inPtr[midLineIn - 1];			p11 = inPtr[midLineIn];			p12 = inPtr[midLineIn + 1];
			p20 = inPtr[downLineIn - 1];		p21 = inPtr[downLineIn];		p22 = inPtr[downLineIn + 1];

			// Calculate the 8-neighbour pixel value sum, include itself
			sum =		p00 + 2 * p01 +		p02 +
					2 * p10 + 4 * p11 + 2 * p12 +
						p20 + 2 * p21 +		p22;

			// Store it.
			outPtr[midLineIn] = sum / 16.0f;
		}
	}
}


Uint32
harris(F32 *img_Ix, F32 *img_Iy, F32 *img_Ixy, Uint8 *img_Mark, Int16 width, Int16 height, Int32 tarPointNum)
{
	// Memory allcation
	/*Int32 i;
	Int32 fullSize = width * height;
	Int32 startPoint = width + 1;
	Int32 endPoint = width * (height - 1) - 1;

	Uint32* histBins_cim = (Uint32*)calloc_check(3 * 255 * 3 * 255 / 2, sizeof(Uint32));
	Int32* inPtr_cim = (Int32*)calloc_check(fullSize, sizeof(Int32));
	Int64 cim, cimMax, cimThresh;
	Uint32 cimNumSum;

	Uint32 curPointNum = 0;

	F32 Ix00, Ix01, Ix02;
	F32 Ix10, Ix11, Ix12;
	F32 Ix20, Ix21, Ix22;

	F32 Iy00, Iy01, Iy02;
	F32 Iy10, Iy11, Iy12;
	F32 Iy20, Iy21, Iy22;

	F32 Ixy;


	// Grad mode value and tan value
	F32 I;
	F32 tan;


	// Calculate the hist bin of cim and cim max value
	cimMax = 0;
	for (i = startPoint; i < endPoint; ++i) {
	// fetch neighbour 3X3 pixels
	Ix00 = img_Ix[i - width - 1]; 		Ix01 = img_Ix[i - width]; 	Ix02 = img_Ix[i - width + 1];
	Ix10 = img_Ix[i - 1];				Ix11 = img_Ix[i];			Ix12 = img_Ix[i + 1];
	Ix20 = img_Ix[i + width - 1];		Ix21 = img_Ix[i + width]; 	Ix22 = img_Ix[i + width + 1];

	Iy00 = img_Iy[i - width - 1]; 		Iy01 = img_Iy[i - width]; 	Iy02 = img_Iy[i - width + 1];
	Iy10 = img_Iy[i - 1];				Iy11 = img_Iy[i];			Iy12 = img_Iy[i + 1];
	Iy20 = img_Iy[i + width - 1];		Iy21 = img_Iy[i + width]; 	Iy22 = img_Iy[i + width + 1];

	I = Ix11 * Ix11 + Iy11 * Iy11;

	// non-maximum suppression -- mark extreme points
	tan = Iy11  / (Ix11 + 0.00001f);

	if (tan > 1.0 && tan <= 2.0) {
	if ((I > (Ix02*Ix02 + Iy02*Iy02)) && (I > (Ix20*Ix20 + Iy20*Iy20))) {
	img_Mark[i] = 255;
	}
	}
	else if (tan > 2.0 || tan <= -2.0) {
	if ((I > (Ix01*Ix01 + Iy01*Iy01)) && (I > (Ix21*Ix21 + Iy21*Iy21))) {
	img_Mark[i] = 255;
	}
	}
	else if (tan > -2.0 && tan <= -1.0) {
	if ((I > (Ix00*Ix00 + Iy00*Iy00)) && (I > (Ix22*Ix22 + Iy22*Iy22))) {
	img_Mark[i] = 255;
	}
	}
	else if (tan > -1.0 && tan <= 1.0) {
	if ((I > (Ix10*Ix10 + Iy10*Iy10)) && (I > (Ix12*Ix12 + Iy12*Iy12))) {
	img_Mark[i] = 255;
	}
	}

	// cim and cim-hist calculation
	Ixy = img_Ixy[i];
	cim = (Int64)((Ix11 * Ix11 * Iy11 * Iy11 - Ixy * Ixy) / (I + 0.00001));

	if (cim < 0) {
	continue;
	}

	if (cim > cimMax) {
	cimMax = cim;
	}

	histBins_cim[cim] ++;
	inPtr_cim[i] = (Int32)cim;
	}


	// Get extreme points by number thresh -- the top 2000 biggest cim value points
	cimThresh = 0;
	cimNumSum = 0;
	for (i = (Int32)cimMax; i >= 0; --i) {
	cimNumSum += histBins_cim[i];

	// extremePointThresh extreme points are needed
	if (cimNumSum >= tarPointNum) {
	cimThresh = i;
	break;
	}
	}


	// Thresh suppression -- exclude un-extreme points
	curPointNum = 0;
	for (i = startPoint; i < endPoint; ++i) {
	if (img_Mark[i] == 0) {
	continue;
	}

	if (inPtr_cim[i] < cimThresh) {
	img_Mark[i] = 0;
	continue;
	}

	printf_simplify("Extreme point %d: x: %4d, y: %4d\n", curPointNum, i % width, i / width);

	curPointNum++;
	}

	printf_detailed("Set point num target: %d, get point: %d\n", tarPointNum, curPointNum);


	// Free memory
	free(histBins_cim);
	free(inPtr_cim);


	return curPointNum;*/

	// Memory allcation
	Int16 i, j;
	Int16 iEnd = width - 1;
	Int16 jEnd = height - 1;
	Int32 fullSize = width * height;

	Int32 cim, cimMax, cimThresh;

	Int32 cimNumSum;
	Int32* hist_cim = (Int32*)calloc_check(3 * 255 * 3 * 255 / 2, sizeof(Int32));	
	Int32* img_cim = (Int32*)calloc_check(fullSize, sizeof(Int32));

	Int32 curPointNum = 0;

	F32 Ix00, Ix01, Ix02;
	F32 Ix10, Ix11, Ix12;
	F32 Ix20, Ix21, Ix22;

	F32 Iy00, Iy01, Iy02;
	F32 Iy10, Iy11, Iy12;
	F32 Iy20, Iy21, Iy22;

	F32 Ixy;


	// Grad mode value and tan value
	F32 I;
	F32 tan;


	// Calculate the hist bin of cim and cim max value
	cimMax = 0;
	Int32 upLineOut = 1;
	Int32 midLineOut = width + 1;
	Int32 downLineOut = 2 * width + 1;

	for (j = 1; j < jEnd; ++j, upLineOut += width, midLineOut += width, downLineOut += width)
	{
		Int32 upLineIn = upLineOut;
		Int32 midLineIn = midLineOut;
		Int32 downLineIn = downLineOut;

		for (i = 1; i < iEnd; ++i, ++upLineIn, ++midLineIn, ++downLineIn)
		{
			// fetch neighbour 3X3 pixels
			Ix00 = img_Ix[upLineIn - 1]; 		Ix01 = img_Ix[upLineIn]; 		Ix02 = img_Ix[upLineIn + 1];
			Ix10 = img_Ix[midLineIn - 1];		Ix11 = img_Ix[midLineIn];		Ix12 = img_Ix[midLineIn + 1];
			Ix20 = img_Ix[downLineIn - 1];		Ix21 = img_Ix[downLineIn]; 		Ix22 = img_Ix[downLineIn + 1];

			Iy00 = img_Iy[upLineIn - 1]; 		Iy01 = img_Iy[upLineIn]; 		Iy02 = img_Iy[upLineIn + 1];
			Iy10 = img_Iy[midLineIn - 1];		Iy11 = img_Iy[midLineIn];		Iy12 = img_Iy[midLineIn + 1];
			Iy20 = img_Iy[downLineIn - 1];		Iy21 = img_Iy[downLineIn]; 		Iy22 = img_Iy[downLineIn + 1];

			I = Ix11 * Ix11 + Iy11 * Iy11;

			// non-maximum suppression -- mark extreme points
			tan = Iy11 / (Ix11 + 0.000001f);

			if (tan > 1.0 && tan <= 2.0)
			{
				if ((I > (Ix02*Ix02 + Iy02*Iy02)) && (I > (Ix20*Ix20 + Iy20*Iy20)))
				{
					img_Mark[midLineIn] = 255;
				}
			}
			else if (tan > 2.0 || tan <= -2.0)
			{
				if ((I > (Ix01*Ix01 + Iy01*Iy01)) && (I > (Ix21*Ix21 + Iy21*Iy21)))
				{
					img_Mark[midLineIn] = 255;
				}
			}
			else if (tan > -2.0 && tan <= -1.0)
			{
				if ((I > (Ix00*Ix00 + Iy00*Iy00)) && (I > (Ix22*Ix22 + Iy22*Iy22)))
				{
					img_Mark[midLineIn] = 255;
				}
			}
			else if (tan > -1.0 && tan <= 1.0)
			{
				if ((I > (Ix10*Ix10 + Iy10*Iy10)) && (I > (Ix12*Ix12 + Iy12*Iy12)))
				{
					img_Mark[midLineIn] = 255;
				}
			}

			// cim and cim-hist calculation
			Ixy = img_Ixy[midLineIn];
			cim = (Ix11 * Ix11 * Iy11 * Iy11 - Ixy * Ixy) / (I + 0.00001f);

			if (cim < 0) {
				continue;
			}

			if (cim > cimMax) {
				cimMax = cim;
			}

			hist_cim[cim] ++;
			img_cim[midLineIn] = cim;
		}
	}

	// Get extreme points by number thresh -- the top 2000 biggest cim value points
	cimThresh = 0;
	cimNumSum = 0;
	for (i = cimMax; i >= 0; --i) {
		cimNumSum += hist_cim[i];

		// extremePointThresh extreme points are needed
		if (cimNumSum >= tarPointNum) {
			cimThresh = i;
			break;
		}
	}


	// Thresh suppression -- exclude un-extreme points
	curPointNum = 0;
	midLineOut = width + 1;
	for (j = 1; j < jEnd; ++j, midLineOut += width)
	{
		Int32 midLineIn = midLineOut;

		for (i = 1; i < iEnd; ++i, ++midLineIn)
		{
			if (img_Mark[midLineIn] == 0)
			{
				continue;
			}

			if (img_cim[midLineIn] < cimThresh)
			{
				img_Mark[midLineIn] = 0;
				continue;
			}

			printf_simplify("Extreme point %d: x: %4d, y: %4d\n", curPointNum, i, j);

			curPointNum++;
		}
	}

	printf_detailed("Set point num target: %d, get point: %d\n", tarPointNum, curPointNum);


	// Free memory
	free(hist_cim);
	free(img_cim);


	return curPointNum;
}
