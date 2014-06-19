#pragma once

// Hu matrix piont feature -- treat point neighbourhood pixels as an image
typedef struct HU_MOMENT {
	double m1;
	double m2;
	double m3;
	double m4;
	double m5;
	double m6;
	double m7;
}huMoment, *pHuMoment;

// Hu point feature -- point coordinates included
typedef struct HU_FEAT {
	huMoment moment;
	int x;
	int y;
}huFeat, *pHuFeat;


/*Create sevaral integral image of input image*/
void
createIntSImg
(
unsigned char* img,
double*	imgInt,
int width,
int height
);


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
);