#ifndef _STD_FUNCS
#define _STD_FUNCS

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>


#define DEBUG 1

// Debug macros
#ifdef DEBUG
bool _trace(TCHAR *format, ...);

#define printf_detailed(format, ...) \
        printf ("\n" "File: " __FILE__ "\n" "Line: %05d" "\n" format, __LINE__, ##__VA_ARGS__)

#define printf_simplify(format, ...) printf (format, ##__VA_ARGS__)

#else
#define printf_detail(format, ...)
#define printf_simplify(format, ...)

#endif


// Packaged functions
void *__cdecl malloc_check(size_t _Size);
void *__cdecl calloc_check(size_t _NumOfElements,size_t _SizeOfElements);


#endif // !_STD_FUNC
