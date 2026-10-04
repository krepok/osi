#include <windows.h>
#include <iostream>
#include <vector>

int n{};
int cMin{};
int cMax{};
int cAv{};

DWORD WINAPI minMax(LPVOID data)
{
    int* temp = (int*)data;

    int min = 0;
    int max = 0;

    for (int i = 0; i < n; i++)
    {
        if (temp[i] > temp[max])
        {
            max = i;
            Sleep(7);
        }
        if (temp[i] < temp[min])
        {
            min = i;
            Sleep(7);
        }
    }

    cMin = min;
    cMax = max;

    std::cout << "Min: " << temp[min] << "\nMax: " << temp[max] << '\n';
    return 0;
}

DWORD WINAPI average(LPVOID data)
{
    int* temp = (int*)data;

    HMODULE hLib = LoadLibrary("lib.dll");
    double(*func)(const int*, int) = (double (*)(const int*, int))GetProcAddress(hLib, "findAv");

    double av = func(temp, n);
    FreeLibrary(hLib);
    
    cAv = av;
    std::cout << "Average: " << av << '\n';
    return 0;
}

int main()
{
    std::cout << "Enter size of vector: ";
    std::cin >> n;
    std::vector<int> vec(n);

    std::cout << "Enter elements: ";
    for (int i = 0; i < n; i++)
    {
        std::cin >> vec[i];
    }
    
    DWORD iDMinMax;
    DWORD iDAverage;
    HANDLE hMinMax = CreateThread(nullptr, 0, minMax, (void*)(vec.data()), 0, &iDMinMax);
    HANDLE hAverage = CreateThread(nullptr, 0, average, (void*)(vec.data()), 0, &iDAverage);

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);
    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    vec[cMin] = cAv;
    vec[cMax] = cAv;

    std::cout << "New vector:\n";
    for (const auto& a : vec)
    {
        std::cout << a << ' ';
    }
    std::cout << '\n';
    return 0;
}