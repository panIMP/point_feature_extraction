#include "stdfuncs.h"
#include "pointLocate.h"
#include "zernike.h"
#include "pointMatch.h"
#include "stdtypes.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>


void 
displayInit(const cv::Mat& imgL, const cv::Mat& imgR) 
{
	cv::Size size(imgL.cols + imgR.cols, MAX(imgL.rows, imgR.rows));
	cv::Mat imgLR(size, CV_8UC1);
	cv::Mat imgLRL = imgLR(cv::Rect(0, 0, imgL.cols, imgL.rows));
	cv::Mat imgLRR = imgLR(cv::Rect(imgL.cols, 0, imgR.cols, imgR.rows));
	imgL.copyTo(imgLRL);
	imgR.copyTo(imgLRR);

	cv::imwrite("G:/Pics/Results/LR.jpg", imgLR);

	cv::namedWindow("Before processing");
	cv::imshow("Before processing", imgLR);
	cv::waitKey(0);
}


void 
displayResult(const cv::Mat& imgL, const cv::Mat& imgR, MatchPair* pairs, Int32 actPNumL)
{
	cv::Size size_Mark(imgL.cols + imgR.cols, MAX(imgL.rows, imgR.rows));
	cv::Mat imgLR_Mark(size_Mark, CV_8UC3);
	cv::Mat imgLRL_Mark = imgLR_Mark(cv::Rect(0, 0, imgL.cols, imgL.rows));
	cv::Mat imgLRR_Mark = imgLR_Mark(cv::Rect(imgL.cols, 0, imgR.cols, imgR.rows));
	cv::cvtColor(imgL, imgLRL_Mark, CV_GRAY2BGR);
	cv::cvtColor(imgR, imgLRR_Mark, CV_GRAY2BGR);

	/*for (Int32 i = 0; i < actPNumL; i += 3) {
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
	}*/

	// Save results
	cv::imwrite("G:/Pics/Results/LR_Mark.jpg", imgLR_Mark);

	cv::namedWindow("After processing");
	cv::imshow("After processing", imgLR_Mark);
	cv::waitKey(0);
}


int 
main () {
	// Variables definition and Memory allocation
	cv::Mat imgL = cv::imread ("G:/Pics/Images/d.jpg", cv::IMREAD_GRAYSCALE);
	cv::Mat imgR = cv::imread ("G:/Pics/Images/d.jpg", cv::IMREAD_GRAYSCALE);

	Int16 tarPNumL = 200;
	Int16 tarPNumR = 200;
	Int16 actPNumL, actPNumR;

	Int8 orderMax = 4;
	Int16 featNum = calcFeatNum (orderMax);

	Int8 window = 15;

	Feat* zFeatL = (Feat*) calloc_check (featNum * tarPNumL, sizeof(Feat));
	Feat* zFeatR = (Feat*) calloc_check (featNum * tarPNumR, sizeof(Feat));


	// Display init images
	displayInit(imgL, imgR);


	// Get point features
	actPNumL = getFeat (imgL, "L_Mark.jpg", zFeatL, imgL.cols, imgL.rows, tarPNumL, window, orderMax);
	actPNumR = getFeat (imgR, "R_Mark.jpg", zFeatR, imgR.cols, imgR.rows, tarPNumR, window, orderMax);


	// Match point features
	MatchPair* pairs = (MatchPair*) calloc_check (actPNumL, sizeof(MatchPair));
	match (zFeatL, actPNumL, zFeatR, actPNumR, featNum, pairs);


	// Display match results
	displayResult(imgL, imgR, pairs, actPNumL);


	return 0;
}