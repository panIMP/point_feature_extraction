#ifndef _ZERNIKE_H
#define _ZERNIKE_H

#include "stdtypes.h"

typedef struct _COMPLEX
{
	double Zr;
	double Zi;
	double Mode;
}Complex , *pComplex;

Int32 Factorial(Int32);

double RadialPolynomial(double, double,double, Int32, Int32);

double RadialPolynomial(double, Int32, Int32);

int ZernikeMoments(Int32, Int32, Int32, Uint8*, Complex*);

Complex Zernike(Int32, Int32, Int32, Uint8*, int width);

Int16 calcFeatNum (Uint8 order);


#endif