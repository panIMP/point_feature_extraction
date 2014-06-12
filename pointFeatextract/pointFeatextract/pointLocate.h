#pragma once

#include <opencv2/core/core.hpp>


/*get the template to generate hessin pyramids*/
int*
getHessinTemplate(void);


/*OpenCV method to find the interest points by using harris*/
void
opecvHarris
(
const cv::Mat& img,
cv::Mat& imgMark,
double harrisThresh,
double binaryThresh
);


/*Create the integral image of input image*/
void
createIntImg
(
unsigned char* img,
double*	imgInt,
int width,
int height
);


/*Generate the hessin-pyramid*/
void
createHessinPyramid
(
unsigned char* img,
double* imgInt,
double* imgHesPyr,
double* imgLxxPyr,
double* imgLyyPyr,
double* imgLxyPyr,
int octaveNum,
int layerNum,
const int* hesTempl,
unsigned char* imgMark,
int width,
int height
);


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
);


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
);


/*Using surf to find the interest points by appointed surf parameters*/
int
surf
(
cv::Mat& img,
cv::Mat& imgMark,
int width,
int height,
int octaveNum,
int layerNum,
const int* hesTempl
);