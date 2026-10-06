#pragma once

#ifdef MYLIB_EXPORTS
    #define MYLIB_API __declspec(dllexport)
#else
    #define MYLIB_API __declspec(dllimport)
#endif

extern "C" MYLIB_API double findAv(const int* vec, int n);
typedef double (*FindMinMaxFunc)(const int*, int);