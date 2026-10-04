#include "lib.h"
#include <windows.h>

extern "C" MYLIB_API double findAv(const int* vec, int n)
{
    double av{};

    for (int i = 0; i < n; i++)
    {
        av += vec[i];
        Sleep(12);
    }
    av = av / n;

    return av;
}