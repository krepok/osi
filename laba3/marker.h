#ifndef MARKER
#define MARKER

#include <iostream>
#include <windows.h>
#include <vector>
#include <random>
#include <limits>

extern std::vector<int> arr;
extern std::vector<HANDLE> handles;
extern std::vector<HANDLE> hStop;
extern std::vector<HANDLE> hTerm;
extern int numOfThreads;

extern CRITICAL_SECTION cs;

extern HANDLE hStart;

void resetAll();
DWORD WINAPI marker(LPVOID info);

int readMarkersCount();
int readMarkerId(int markers, const std::vector<bool>& alive);
void printArray();

#endif