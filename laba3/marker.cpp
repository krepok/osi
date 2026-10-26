#include "marker.h"

std::vector<int> arr;
std::vector<HANDLE> handles;
std::vector<HANDLE> hStop;
std::vector<HANDLE> hTerm;
int numOfThreads{};

CRITICAL_SECTION cs;

HANDLE hStart;

void resetAll()
{
    for (auto& h : hStop)
        ResetEvent(h);
    for (auto& h : hTerm)
        ResetEvent(h);
    numOfThreads = 0;
}

int readMarkersCount()
{
    int markers{};
    std::cout << "Number of markers: ";

    while (!(std::cin >> markers) || markers <= 0 || markers > MAXIMUM_WAIT_OBJECTS)
    {
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        std::cout << "invalid number of markers, enter again: ";
    }

    return markers;
}

int readMarkerId(int markers, const std::vector<bool>& alive)
{
    int marker{};
    std::cout << "Id of marker to terminate: ";

    while (!(std::cin >> marker) || marker <= 0 || marker > markers || alive[marker - 1] == false)
    {
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        std::cout << "invalid id, enter again: ";
    }

    return marker;
}

void printArray()
{
    for (const auto& a : arr)
        std::cout << a << ' ';
    std::cout << '\n';
}

DWORD WINAPI marker(LPVOID info)
{
    int id = (int)info;

    WaitForSingleObject(hStart, INFINITE);

    srand(id);
    int counter{};
    while (true)
    {
        int random = rand() % arr.size();

        EnterCriticalSection(&cs);
        if (arr[random] == 0)
        {
            Sleep(5);
            arr[random] = id;
            Sleep(5);
            ++counter;
            LeaveCriticalSection(&cs);            
            continue;
        }

        std::cout << "Marker " << id << " marked " << counter << " elements, stopped at " << random << '\n';
        
        SetEvent(hStop[numOfThreads]);
        ++numOfThreads;
        LeaveCriticalSection(&cs);

        auto r = WaitForMultipleObjects(hTerm.size(), hTerm.data(), false, INFINITE);
        if (r == WAIT_OBJECT_0 + id - 1)
        {
            EnterCriticalSection(&cs);
            for (auto& m : arr)
                m = (m == id) ? 0 : m;

            SetEvent(hStop[numOfThreads]);
            ++numOfThreads;
            LeaveCriticalSection(&cs);
            ExitThread(0);
        }

        EnterCriticalSection(&cs);

        SetEvent(hStop[numOfThreads]);
        ++numOfThreads;

        LeaveCriticalSection(&cs);

        WaitForSingleObject(hStart, INFINITE);
    }

}