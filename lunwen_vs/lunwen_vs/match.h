#ifndef _MATCH_H
#define _MATCH_H

#include "stdtypes.h"
#include "stdfuncs.h"
#include "zernike.h"



typedef struct _POINT_FEAT {
    Complex feat;
    Int16   x;
    Int16   y;
}Feat, *pFeat;

typedef struct _MATCH_PAIR {
    Int32 L_i;
    Int16 L_i_x;
    Int16 L_i_y;
    Int32 R_j;
    Int16 R_j_x;
    Int16 R_j_y;
}MatchPair, *pMatchPair;


Uint32 getFeat(Uint8* img, Feat* zFeat, 
			   Int16 width, Int16 height, 
			   Int32 tarPointNum, Int8 window, Int8 orderMax);

void normalFeat(Feat* zFeat, Int32 actPointNum, Int16 featNum);

double calcModeDist (Feat* pointL, Feat* pointR, Int16 featNum);

void addNewPair (MatchPair* pairs, Int32 L_i, Int32 R_j, Feat *pointL, Feat *pointR);

float match (Feat* pointsL, Uint32 pointLNum, Feat* pointsR, Uint32 pointRNum, Int16 featNum, MatchPair *pairs);


#endif