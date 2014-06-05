#include "pointLocate.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

void
opecvHarris
(
const cv::Mat& leftImg,
const cv::Mat& rightImg,
cv::Mat& leftImgMark,
cv::Mat& rightImgMark,
double harrisThresh,
double binaryThresh
)
{
	// Left Image
	cv::cornerHarris(leftImg, leftImgMark, 3, 3, harrisThresh);

	cv::threshold(leftImgMark, leftImgMark, binaryThresh, 255, cv::THRESH_BINARY);
	leftImgMark.convertTo(leftImgMark, CV_8UC1);

	// Right Image
	cv::cornerHarris(rightImg, rightImgMark, 3, 3, harrisThresh);

	cv::threshold(rightImgMark, rightImgMark, binaryThresh, 255, cv::THRESH_BINARY);
	rightImgMark.convertTo(rightImgMark, CV_8UC1);
}


void
sift
(
unsigned char* img,
unsigned char* imgMark,
int width,
int height
)
{

}