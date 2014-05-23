#ifndef _HARRIS_H
#define _HARRIS_H

#include "stdtypes.h"

void gradImage (Uint8 *img, Int32 *img_Ix, Int32 *img_Iy, Int32* img_Ixy, 
				Int16 width, Int16 height);

void guassin (Int32* inPtr, Int32* outPtr, 
			  Int16 width, Int16 height);

Uint32 harris (Int32 *img_Ix, Int32 *img_Iy, Int32* img_Ixy, Uint8 *img_Mark, 
			   Int16 width, Int16 height, 
			   Uint32 tarPointNum);


#endif