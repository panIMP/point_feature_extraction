#include "imageIO.h"
#include "pointLocate.h"
#include "zernikeMatch.h"
#include "stdfunc.h"
#include <iostream>



int
main(int argc, char* argv[])
{
	int* hessinTemplate = getHessinTemplate();

	// Image Load and Display Stage
	std::string srcFolder = "E:/Pics/Images/";
	std::string dstFolder = "E:/Pics/Results/";
	std::pair<cv::Mat, cv::Mat> imgMatPair = displayInit(srcFolder + "d.jpg", srcFolder + "d.jpg", dstFolder + "match.jpg");
	cv::Mat leftImgMat = imgMatPair.first;
	cv::Mat rightImgMat = imgMatPair.second;
	unsigned char* leftImg = leftImgMat.data;
	unsigned char* rightImg = rightImgMat.data;
	std::pair<unsigned char*, unsigned char*> imgPair = std::make_pair(leftImg, rightImg);
	
	// image size information
	int width = leftImgMat.cols;
	int height = rightImgMat.rows;
	int fullSize = width * height;

	// mark image (mark the interest points)
	unsigned char* leftImgMark = (unsigned char*)calloc_check(fullSize, sizeof(unsigned char));
	unsigned char* rightImgMark = (unsigned char*)calloc_check(fullSize, sizeof(unsigned char));
	std::pair<unsigned char*, unsigned char*> imgMarkPair = std::make_pair(leftImgMark, rightImgMark);

	// Image Interest Points Extracting Stage
	int leftPointNum = surf(leftImg, leftImgMark, width, height, 1, 3, hessinTemplate);
	int rightPointNum = surf(rightImg, rightImgMark, width, height, 1, 3, hessinTemplate);
	std::pair<int, int> pointNumPair = std::make_pair(leftPointNum, rightPointNum);

	// Get feat and match feat
	// Method 1: use zernike descriptors
	pMatchCouple couple = NULL;
	int matchNum = 0;
	matchNum = matchByZernike(imgPair, imgMarkPair, pointNumPair, 4, 15, width, height, &couple);

	// Result Display Stage
	displayResult(imgMatPair, imgMarkPair, couple, matchNum, dstFolder + "left.jpg", dstFolder + "right.jpg", dstFolder + "matchResult.jpg");


	// Free unused memory
	if (couple != NULL)
		free(couple);
	free(leftImgMark);
	free(rightImgMark);

	return 0;
}

