#include "stdfuncs.h"


bool _trace(TCHAR *format, ...) {
	TCHAR buffer[1000];

	va_list argptr;
	va_start(argptr, format);
	wvsprintf(buffer, format, argptr);
	va_end(argptr);
	OutputDebugString(buffer);
	
	return true;
}


void *__cdecl malloc_check(size_t _Size) {
	void* p = malloc (_Size);
	if (p == NULL) {
		printf_detailed("Memory allocation failed!");
		exit(-1);
	}

	return p;
}


void *__cdecl calloc_check(size_t _NumOfElements,size_t _SizeOfElements) {
	void* p = calloc (_NumOfElements, _SizeOfElements);
	if (p == NULL) {
		printf_detailed("Memory allocation failed!");
		exit(-1);
	}

	return p;
}


