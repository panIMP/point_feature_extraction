#include "imageIO.h"
#include "pointLocate.h"
#include "pointMatch.h"
#include "stdfunc.h"
#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\calib3d\calib3d.hpp>




int
main(int argc, char* argv[])
{
	int* hessinTemplate = getHessinTemplate();

	// Image Load and Display Stage
	std::string srcFolder = "E:/Pics/Images/";
	std::string dstFolder = "E:/Pics/Results/";

	std::pair<cv::Mat, cv::Mat> imgPair = displayInit(srcFolder + "spot1.bmp", srcFolder + "spot2.bmp", dstFolder + "merge.jpg");

	cv::Mat leftImg = imgPair.first;
	cv::Mat rightImg = imgPair.second;
	
	int width = leftImg.cols;
	int height = rightImg.rows;
	int fullSize = width * height;
	
	cv::Mat leftImgMark = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));
	cv::Mat rightImgMark = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));

	int orderMax = 4;
	int featNum = calcFeatNum(orderMax);

	int window = 15;

	std::cout << leftImgMark.channels();

	// Image Interest Points Extracting Stage

	int leftPointNum = surf(leftImg, leftImgMark, width, height, 2, 4, hessinTemplate);

	int rightPointNum = surf(rightImg, rightImgMark, width, height, 2, 4, hessinTemplate);


	std::pair<unsigned char*, unsigned char*> imgMarkPair = std::make_pair(leftImgMark.data, rightImgMark.data);
	
	/*
	Feat* zFeatL = (Feat*)calloc_check(leftPointNum * tarPNumL, sizeof(Feat));
	Feat* zFeatR = (Feat*)calloc_check(rightPointNum * tarPNumR, sizeof(Feat));
	describe(leftImg.data, leftImgMark.data, zFeatL, width, height, window, orderMax);
	describe(rightImg.data, rightImgMark.data, zFeatR, width, height, window, orderMax);*/
	
	pMatchCouple couple = (pMatchCouple)calloc_check(leftPointNum, sizeof(MatchCouple));
	//match(zFeatL, leftPointNum, zFeatR, rightPointNum, featNum, couple);

	// Result Display Stage
	displayResult(imgPair, imgMarkPair, couple, leftPointNum, dstFolder + "left.jpg", dstFolder + "right.jpg", dstFolder + "mergeResult.jpg");



	return 0;
}