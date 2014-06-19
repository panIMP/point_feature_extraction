#include "huMatch.h"



void
matchHu(std::pair<huFeat*, huFeat*>feats, std::pair<int, int> pointNum, pMatchCouple* couple)
{
	huFeat* featsL = feats.first;
	huFeat* featsR = feats.second;
	int pointNumL = pointNum.first;
	int pointNumR = pointNum.second;


	
}



int matchByHu
(
std::pair<unsigned char*, unsigned char*> imgPair,
std::pair<unsigned char*, unsigned char*> imgMarkPair,
std::pair<int, int> pointNum,
pMatchCouple* couple,
int window,
int width,
int height
)
{
	unsigned char* leftImg = imgPair.first;
	unsigned char* rightImg = imgPair.second;
	unsigned char* leftImgMark = imgMarkPair.first;
	unsigned char* rightImgMark = imgMarkPair.second;
	int leftPointNum = pointNum.first;
	int rightPointNum = pointNum.second;

	// get feature
	huFeat* featsL = (huFeat*)calloc_check(leftPointNum, sizeof(huFeat));
	int actLeftPointNum = getHuMoments(leftImg, leftImgMark, featsL, window, width, height);
	huFeat* featsR = (huFeat*)calloc_check(rightPointNum, sizeof(huFeat));
	int actRightPointNum = getHuMoments(rightImg, rightImgMark, featsR, window, width, height);
	std::pair<huFeat*, huFeat*> feats = std::make_pair(featsL, featsR);
	std::pair<int, int> actPointNum = std::make_pair(actLeftPointNum, actRightPointNum);

	// match
	int matchNum = MIN(actLeftPointNum, actRightPointNum);
	matchHu(feats, actPointNum, couple);
	

	return matchNum;
}
