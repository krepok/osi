#include "marker.h"

int main()
{
    int n;
    std::cout << "Array size: ";
    if (!(std::cin >> n) || n <= 0)
        return 0;

    arr.assign(n, 0);

    int markers = readMarkersCount();

    InitializeCriticalSection(&cs);
    hStart = CreateEvent(nullptr, true, false, nullptr);

    hStop.resize(markers);
    handles.resize(markers);
    hTerm.resize(markers);

    for (int i = 0; i < markers; i++)
    {
        handles[i] = CreateThread(nullptr, 0, marker, (void*)(i + 1), 0, NULL);
        hStop[i] = CreateEvent(nullptr, true, false, nullptr);
        hTerm[i] = CreateEvent(nullptr, true, false, nullptr);
    }

    std::vector<bool> alive(markers, true);

    for (int i = 0; i < markers; i++)
    {
        SetEvent(hStart);

        WaitForMultipleObjects(hStop.size(), hStop.data(), true, INFINITE);

        ResetEvent(hStart);
        resetAll();

        printArray();
        
        int marker = readMarkerId(markers, alive);

        alive[marker - 1] = false;

        SetEvent(hTerm[marker - 1]);

        WaitForMultipleObjects(hStop.size(), hStop.data(), true, INFINITE);

        WaitForSingleObject(handles[marker - 1], INFINITE);

        printArray();
        
        CloseHandle(handles[marker - 1]);
        CloseHandle(hStop.back());
        hStop.pop_back();

        resetAll();
    }

    return 0;
}
