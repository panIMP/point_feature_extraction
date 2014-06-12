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
describe(unsigned char* img, unsigned char* img_Mark, Feat* zFeat, int width, int height, int window, int orderMax);

double
match(Feat* pointsL, int pointLNum, Feat* pointsR, int pointRNum, int featNum, MatchCouple* pairs);

double
calcModeDist(Feat* pointL, Feat* pointR, int featNum);

void
addNewPair(MatchCouple* pairs, int L_i, int R_j, Feat* pointL, Feat* pointR);

void normalFeat(Feat* zFeat, int actPointNum, int featNum);