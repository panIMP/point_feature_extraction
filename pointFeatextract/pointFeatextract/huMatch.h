#pragma once

#include "stdfunc.h"
#include "hu.h"
#include <utility>

/*Calculate the ou - distance between two feat*/
double
calcHuFeatDistance(huFeat* featL, huFeat* featR);


/*Add the matched pair to the couple*/
void
addHuNewPair(MatchCouple* couple, huFeat* featL, huFeat* featR);


/*match the hu-moment feat*/
int
matchHu(std::pair<huFeat*, huFeat*>feats, std::pair<int, int> pointNum, pMatchCouple couples);


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
);
