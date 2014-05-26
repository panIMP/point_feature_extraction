#include "stdfuncs.h"
#include "harris.h"
#include "zernike.h"
#include "match.h"
#include "stdtypes.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>


int main () {
	printf("%d", sizeof(float));

	// Variables definition and Memory allocation
	cv::Mat imgL = cv::imread ("G:/Pics/Images/view1_gray.jpg", cv::IMREAD_GRAYSCALE);
	cv::Mat imgR = cv::imread ("G:/Pics/Images/view5_gray.jpg", cv::IMREAD_GRAYSCALE);


	Int16 tarPNumL = 100;
	Int16 tarPNumR = 100;
	Int16 actPNumL, actPNumR;

	Int8 orderMax = 4;
	Int16 featNum = calcFeatNum (orderMax);

	Int8 window = 15;

	Feat* zFeatL = (Feat*) calloc_check (featNum * tarPNumL, sizeof(Feat));
	Feat* zFeatR = (Feat*) calloc_check (featNum * tarPNumR, sizeof(Feat));


	// Display init images
	cv::Size size(imgL.cols + imgR.cols, MAX (imgL.rows, imgR.rows));
	cv::Mat imgLR(size, CV_8UC1);
	cv::Mat imgLRL = imgLR (cv::Rect (0, 0, imgL.cols, imgL.rows));
	cv::Mat imgLRR = imgLR (cv::Rect (imgL.cols, 0, imgR.cols, imgR.rows));
	imgL.copyTo (imgLRL);
	imgR.copyTo (imgLRR);

	cv::namedWindow ("Before processing");
	cv::imshow ("Before processing", imgLR);
	cv::waitKey(0);


	// Get point features
	actPNumL = getFeat (imgL.data, zFeatL, imgL.cols, imgL.rows, tarPNumL, window, orderMax);
	actPNumR = getFeat (imgR.data, zFeatR, imgR.cols, imgR.rows, tarPNumR, window, orderMax);


	// Match point features
	MatchPair* pairs = (MatchPair*) calloc_check (actPNumL, sizeof(MatchPair));
	match (zFeatL, actPNumL, zFeatR, actPNumR, featNum, pairs);


	// Display match results
	cv::Size size_Mark(imgL.cols + imgR.cols, MAX (imgL.rows, imgR.rows));
	cv::Mat imgLR_Mark(size, CV_8UC3);
	cv::Mat imgLRL_Mark = imgLR_Mark (cv::Rect (0, 0, imgL.cols, imgL.rows));
	cv::Mat imgLRR_Mark = imgLR_Mark (cv::Rect (imgL.cols, 0, imgR.cols, imgR.rows));
	cv::cvtColor (imgL, imgLRL_Mark, CV_GRAY2BGR);
	cv::cvtColor (imgR, imgLRR_Mark, CV_GRAY2BGR);

	for (Int32 i = 0; i < actPNumL; i += 3) {
		cv::line(imgLR_Mark, cv::Point(pairs[i].L_i_x, pairs[i].L_i_y), 
			cv::Point(pairs[i].R_j_x + imgL.cols, pairs[i].R_j_y), cv::Scalar(255, 0, 0));
	}
	for (Int32 i = 1; i < actPNumL; i += 3) {
		cv::line(imgLR_Mark, cv::Point(pairs[i].L_i_x, pairs[i].L_i_y), 
			cv::Point(pairs[i].R_j_x + imgL.cols, pairs[i].R_j_y), cv::Scalar(0, 255, 0));
	}
	for (Int32 i = 2; i < actPNumL; i += 3) {
		cv::line(imgLR_Mark, cv::Point(pairs[i].L_i_x, pairs[i].L_i_y), 
			cv::Point(pairs[i].R_j_x + imgL.cols, pairs[i].R_j_y), cv::Scalar(0, 0, 255));
	}

	cv::namedWindow ("After processing");
	cv::imshow ("After processing", imgLR_Mark);
	cv::waitKey(0);


	// Save results
	cv::imwrite ("G:/Pics/Results/L_master.jpg", imgL);
	cv::imwrite ("G:/Pics/Results/R_master.jpg", imgR);
	cv::imwrite ("G:/Pics/Results/LR_master.jpg", imgLR);
	cv::imwrite ("G:/Pics/Results/LRMark_master.jpg", imgLR_Mark);


	return 0;
}