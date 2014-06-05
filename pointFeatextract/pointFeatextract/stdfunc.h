#pragma once


#include <stdlib.h>
#include <stdio.h>


// Debug control
#define DEBUG   1


// Debug macros
#ifdef DEBUG

#define printf_detailed(format, ...) \
	printf("\n" "File: " __FILE__ "\n" "Line: %05d" "\n" format, __LINE__, ##__VA_ARGS__)

#define printf_simplify(format, ...) printf (format, ##__VA_ARGS__)

#else
#define printf_detail(format, ...)
#define printf_simplify(format, ...)

#endif


// Packaged functions
void *__cdecl malloc_check(size_t _Size);
void *__cdecl calloc_check(size_t _NumOfElements, size_t _SizeOfElements);
