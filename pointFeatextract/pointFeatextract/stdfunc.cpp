#include "stdfunc.h"

void *__cdecl malloc_check(size_t _Size) {
	void* p = malloc(_Size);
	if (p == NULL) {
		printf_detailed("Memory allocation failed!");
		exit(-1);
	}

	return p;
}


void *__cdecl calloc_check(size_t _NumOfElements, size_t _SizeOfElements) {
	void* p = calloc(_NumOfElements, _SizeOfElements);
	if (p == NULL) {
		printf_detailed("Memory allocation failed!");
		exit(-1);
	}

	return p;
}

// Solve the 3X3 hessin
void solve3_3(const double* A, const double* B, double* result)
{
	double eps = 0.000001;

	double a11 = A[0];
	double a12 = A[1];
	double a13 = A[2];

	double a21 = A[3];
	double a22 = A[4];
	double a23 = A[5];

	double a31 = A[6];
	double a32 = A[7];
	double a33 = A[8];

	double b1 = B[0];
	double b2 = B[1];
	double b3 = B[2];

	double a11Comp = a21 * a12 - a22 * a11;
	double a12Comp = a21 * a13 - a23 * a11;
	double a21Comp = a31 * a22 - a32 * a21;
	double a22Comp = a31 * a23 - a33 * a21;
	double b1Comp = b1 * a21 - b2 * a11;
	double b2Comp = b2 * a31 - b3 * a21;

	double sigma = (b1Comp * a21Comp - b2Comp * a11Comp) / (a12Comp * a21Comp - a22Comp * a11Comp + eps);
	double y = (b1Comp - a12Comp * sigma) / (a11Comp + eps);
	double x = (b1 - a13 * sigma - a12 * y) / (a11 + eps);

	result[0] = x;
	result[1] = y;
	result[2] = sigma;
}

