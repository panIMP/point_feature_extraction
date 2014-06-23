#include "huMatch.h"
#include <math.h>


/*Calculate the ou - distance between two feat*/
double 
calcHuFeatDistance(huFeat* featL, huFeat* featR)
{
	double dist = 0.0;

	dist += pow(featL->moment.m1 - featR->moment.m1, 2);
	dist += pow(featL->moment.m2 - featR->moment.m2, 2);
	/*dist += pow(featL->moment.m3 - featR->moment.m3, 2);
	dist += pow(featL->moment.m4 - featR->moment.m4, 2);
	dist += pow(featL->moment.m5 - featR->moment.m5, 2);
	dist += pow(featL->moment.m6 - featR->moment.m6, 2);
	dist += pow(featL->moment.m7 - featR->moment.m7, 2);*/

	return dist;
}


/*Add the matched pair to the couple*/
void
addHuNewPair(MatchCouple* couple, huFeat* featL, huFeat* featR)
{
	static int num = 1;

	couple->Lx = featL->x;
	couple->Ly = featL->y;
	couple->Rx = featR->x;
	couple->Ry = featR->y;

	printf_simplify("Pair %4d X,Y: (%4d, %4d), (%4d, %4d)\n", num++, couple->Lx, couple->Ly, couple->Rx, couple->Ry);
}


/*match the hu-moment feat*/
int
matchHu(std::pair<huFeat*, huFeat*>feats, std::pair<int, int> pointNum, pMatchCouple couples)
{
	huFeat* featsL = feats.first;
	huFeat* featsR = feats.second;
	int pointNumL = pointNum.first;
	int pointNumR = pointNum.second;

	// set the mininum of the two as matchNum
	int matchNum = 0;

	// set maxDist;
	double minFeatDist = 1E20;

	// set record matched j of the right image
	int matchJ = 0;
	int matchI = 0;

	// iterate over the interest points in the left image
	for (int i = 0; i < pointNumL; ++i)
	{
		// find the point(matchJ) int the right image that is nearest to the i in the left
		for (int j = 0; j < pointNumR; ++j)
		{
			double featDist = calcHuFeatDistance(featsL + i, featsR + j);
			if (featDist < minFeatDist)
			{
				minFeatDist = featDist;
				matchJ = j;
			}
		}

		// reverse find -- find if i is the nearest for matchJ in the left image
		matchI = i;
		for (int newI = 0; newI < pointNumL; ++newI)
		{
			double featDist = calcHuFeatDistance(featsL + newI, featsR + matchJ);

			// minFeatDist is recorded from above
			if (featDist < minFeatDist)
			{
				minFeatDist = featDist;
				matchI = newI;
			}
		}

		// if (i, matchJ) == (matchI, matchJ), then the two points are coupled
		if (matchI == i)
		{
			printf("Left %d is Matched to Right %3d ", matchI, matchJ);
			addHuNewPair(couples++, featsL + matchI, featsR + matchJ);
			matchNum++;
		}

		// reset those compare variables for another point in left image
		minFeatDist = 1E20;
		matchJ = 0;
	}

	printf_simplify("%d pairs are matched", matchNum);

	return matchNum;
}


/*get hu-moment feat and match them*/
int matchByHu
(
std::pair<unsigned char*, unsigned char*> imgPair,
std::pair<unsigned char*, unsigned char*> imgMarkPair,
std::pair<int, int> pointNum,
pMatchCouple* couples,
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
	*couples = (pMatchCouple)calloc_check(MIN(actLeftPointNum, actRightPointNum), sizeof(MatchCouple));
	int matchNum = matchHu(feats, actPointNum, *couples);
	

	return matchNum;
}
