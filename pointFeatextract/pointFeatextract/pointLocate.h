#pragma once

#include <opencv2/core/core.hpp>


void
opecvHarris
(
const cv::Mat& leftImg,
const cv::Mat& rightImg,
cv::Mat& leftImgMark,
cv::Mat& rightImgMark,
double harrisThresh,
double binaryThresh
);


void
sift
(
unsigned char* img, 
unsigned char* imgMark, 
int width, 
int height
);