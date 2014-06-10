#include "imageIO.h"
#include "pointLocate.h"
#include "pointMatch.h"
#include "stdfunc.h"
#include <iostream>


int
main(int argc, char* argv[])
{
	int* hessinTemplate = getHessinTemplate();

	/* Image Load and Display Stage*/
	std::string srcFolder = "E:/Pics/Images/";
	std::string dstFolder = "E:/Pics/Results/";

	std::pair<cv::Mat, cv::Mat> imgPair = displayInit(srcFolder + "view1_gray.jpg", srcFolder + "view2_gray.jpg", dstFolder + "merge.jpg");

	cv::Mat leftImg = imgPair.first;
	cv::Mat rightImg = imgPair.second;
	
	int width = leftImg.cols;
	int height = rightImg.rows;
	int fullSize = width * height;
	
	cv::Mat leftImgMark = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));
	cv::Mat rightImgMark = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));

	pMatchCouple couple = nullptr;


	/* Image Interest Points Extracting Stage*/
	int leftPointNum = surf(leftImg, leftImgMark, width, height, 2, 4, hessinTemplate);
	int rightPointNum = surf(rightImg, rightImgMark, width, height, 2, 4, hessinTemplate);

	std::pair<unsigned char*, unsigned char*> imgMarkPair = std::make_pair(leftImgMark.data, rightImgMark.data);


	/* Result Display Stage*/
	displayResult(imgPair, imgMarkPair, couple, dstFolder + "left.jpg", dstFolder + "right.jpg", dstFolder + "mergeResult.jpg");

	return 0;
}
