#include "imageIO.h"
#include <iostream>


std::pair<cv::Mat, cv::Mat>
displayInit
(
std::string leftImgStr,
std::string rightImgStr,
std::string mergeImgStr
)
{
	cv::Mat leftImg = cv::imread(leftImgStr, cv::IMREAD_GRAYSCALE);
	cv::Mat rightImg = cv::imread(rightImgStr, cv::IMREAD_GRAYSCALE);

	std::pair<cv::Mat, cv::Mat> imgPair = std::make_pair(leftImg, rightImg);

	cv::Size size(leftImg.cols + rightImg.cols, MAX(leftImg.rows, rightImg.rows));
	cv::Mat mergeImg(size, CV_8UC1);
	cv::Mat leftMergeImg = mergeImg(cv::Rect(0, 0, leftImg.cols, leftImg.rows));
	cv::Mat rightMergeImg = mergeImg(cv::Rect(leftImg.cols, 0, rightImg.cols, rightImg.rows));
	leftImg.copyTo(leftMergeImg);
	rightImg.copyTo(rightMergeImg);

	cv::imwrite(mergeImgStr, mergeImg);

	cv::namedWindow("Before processing");
	cv::imshow("Before processing", mergeImg);
	cv::waitKey(0);

	return imgPair;
}


void
drawCross
(
uchar* img,
uchar* imgMark,
uchar len,
short width,
short height
)
{
	int i, j, k;

	short x, y;

	int startPoint = width * len + len;
	int endPoint = width * (height - len) - len;
	int pointNum = 0;

	imgMark += startPoint;
	for (k = startPoint; k < endPoint; ++k) {
		if (*imgMark++ == 0)
			continue;

		pointNum++;

		x = k % width;
		y = k / width;

		for (i = x - len; i < x + len; ++i) {
			*(img + y * width + i) = 255;
		}

		for (j = y - len; j < y + len; ++j) {
			*(img + j * width + x) = 255;
		}
	}

	//std::cout << pointNum << " interest points has been detected" << std::endl;
}


void
displayResult
(
std::pair<cv::Mat, cv::Mat> imgPair,
std::pair<unsigned char*, unsigned char*> imgMarkPair,
pMatchCouple couple,
int actPNumL,
std::string leftResultStr,
std::string rightResultStr,
std::string mergeResultStr
)
{
	cv::Mat leftResult = imgPair.first;
	cv::Mat rightResult = imgPair.second;

	drawCross(leftResult.data, imgMarkPair.first, 20, leftResult.cols, leftResult.rows);
	drawCross(rightResult.data, imgMarkPair.second, 20, rightResult.cols, rightResult.rows);

	cv::Size size(leftResult.cols + rightResult.cols, MAX(leftResult.rows, rightResult.rows));
	cv::Mat mergeResult(size, CV_8UC3);
	cv::Mat leftMergeResult = mergeResult(cv::Rect(0, 0, leftResult.cols, leftResult.rows));
	cv::Mat rightMergeResult = mergeResult(cv::Rect(leftResult.cols, 0, rightResult.cols, rightResult.rows));
	cv::cvtColor(leftResult, leftMergeResult, CV_GRAY2BGR);
	cv::cvtColor(rightResult, rightMergeResult, CV_GRAY2BGR);

	for (int i = 0; i < actPNumL; i += 3) {
		cv::line(mergeResult, cv::Point(couple[i].Lx, couple[i].Ly),
			cv::Point(couple[i].Rx + leftMergeResult.cols, couple[i].Ry), cv::Scalar(255, 0, 0));
	}
	for (int i = 1; i < actPNumL; i += 3) {
		cv::line(mergeResult, cv::Point(couple[i].Lx, couple[i].Ly),
			cv::Point(couple[i].Rx + leftMergeResult.cols, couple[i].Ry), cv::Scalar(0, 255, 0));
	}
	for (int i = 2; i < actPNumL; i += 3) {
		cv::line(mergeResult, cv::Point(couple[i].Lx, couple[i].Ly),
			cv::Point(couple[i].Rx + leftMergeResult.cols, couple[i].Ry), cv::Scalar(0, 0, 255));
	}

	// Save results
	cv::imwrite(leftResultStr, leftResult);
	cv::imwrite(rightResultStr, rightResult);
	cv::imwrite(mergeResultStr, mergeResult);

	cv::namedWindow("After processing");
	cv::imshow("After processing", mergeResult);
	cv::waitKey(0);
}