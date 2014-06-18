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

	std::pair<cv::Mat, cv::Mat> imgMatPair = displayInit(srcFolder + "view1_gray.jpg", srcFolder + "view1_gray.jpg", dstFolder + "match.jpg");

	cv::Mat leftImgMat = imgMatPair.first;
	cv::Mat rightImgMat = imgMatPair.second;
	unsigned char* leftImg = leftImgMat.data;
	unsigned char* rightImg = rightImgMat.data;
	
	int width = leftImgMat.cols;
	int height = rightImgMat.rows;
	int fullSize = width * height;

	unsigned char* leftImgMark = (unsigned char*)calloc_check(fullSize, sizeof(unsigned char));
	unsigned char* rightImgMark = (unsigned char*)calloc_check(fullSize, sizeof(unsigned char));

	// Image Interest Points Extracting Stage
	int leftPointNum = surf(leftImg, leftImgMark, width, height, 1, 3, hessinTemplate);
	int rightPointNum = surf(rightImg, rightImgMark, width, height, 1, 3, hessinTemplate);

	// Get feat and match feat
	int matchNum = MIN(leftPointNum, rightPointNum);
	pMatchCouple couple = (pMatchCouple)calloc_check(matchNum, sizeof(MatchCouple));
	// Method 1: use zernike descriptors
	getZernikeFeat(leftImg, rightImg, 4, 15, width, height, leftPointNum, rightPointNum, leftImgMark, rightImgMark, couple, &matchNum);

	// Result Display Stage
	std::pair<unsigned char*, unsigned char*> imgMarkPair = std::make_pair(leftImgMark, rightImgMark);
	displayResult(imgMatPair, imgMarkPair, couple, matchNum, dstFolder + "left.jpg", dstFolder + "right.jpg", dstFolder + "matchResult.jpg");

	return 0;


	/*cv::Mat  image, image1 = cv::imread("E:/Pics/Images/view1_gray.jpg");
	//�Ҷȱ任
	cv::cvtColor(image1, image, CV_BGR2GRAY);
	std::vector<cv::KeyPoint> keypoints;
	cv::SurfFeatureDetector surf(2500);
	surf.detect(image, keypoints);
	cv::drawKeypoints(image, keypoints, image, cv::Scalar::all(255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	cv::namedWindow("surf");
	cv::imshow("surf", image);
	cv::imwrite("E:/Pics/Results/opencvsurf.jpg", image);
	cv::waitKey(0);*/

}

