#pragma once

#include "stdfunc.h"
#include "hu.h"
#include <utility>



int matchByHu
(
std::pair<unsigned char*, unsigned char*> imgPair,
std::pair<unsigned char*, unsigned char*> imgMarkPair,
std::pair<int, int> pointNum,
pMatchCouple* couple,
int window,
int width,
int height
);


void
matchHu(std::pair<huFeat*, huFeat*>feats, std::pair<int, int> pointNum, pMatchCouple* couple);