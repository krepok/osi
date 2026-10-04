#pragma once

#ifdef MYLIB_EXPORTS
    #define MYLIB_API __declspec(dllexport)
#else
    #define MYLIB_API __declspec(dllimport)
#endif

// extern "C" отключает декорирование имён, иначе GetProcAddress не найдёт функцию
extern "C" MYLIB_API double findAv(const int* vec, int n);

// тип указателя на функцию, используется в main.cpp
typedef double (*FindMinMaxFunc)(const int*, int);