#pragma once

#include "zernike.h"

typedef struct _POINT_FEAT {
	Complex feat;
	int   x;
	int   y;
}Feat, *pFeat;

typedef struct _MATCH_COUPLE {
	short Lx;
	short Ly;
	short Rx;
	short Ry;
}MatchCouple, *pMatchCouple;


int
describeZernike(unsigned char* img, unsigned char* img_Mark, Feat* zFeat, int width, int height, int window, int orderMax);

double
matchZernike(Feat* pointsL, int matchNum, Feat* pointsR, int pointRNum, int featNum, MatchCouple* pairs);

double
calcZernikeModeDist(Feat* pointL, Feat* pointR, int featNum);

void
addZernikeNewPair(MatchCouple* pairs, int L_i, int R_j, Feat* pointL, Feat* pointR);

void normalZernikeFeat(Feat* zFeat, int actPointNum, int featNum);

void
getZernikeFeat
(
unsigned char* leftImg,
unsigned char* rightImg,
int orderMax,
int window,
int width,
int height,
int leftPointNum,
int rightPointNum,
unsigned char* leftImgMark,
unsigned char* rightImgMark,
pMatchCouple couple,
int* pMatchNum
);