#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "file.h"

int main(int argc, char* argv[]) {
    if (argc < 2)
        return 1;
    std::string fileName = argv[1];

    HANDLE hMutex = OpenMutexA(SYNCHRONIZE, FALSE, MUTEX_NAME);
    HANDLE hEmpty = OpenSemaphoreA(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, EMPTY_NAME);
    HANDLE hFull = OpenSemaphoreA(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, FULL_NAME);
    HANDLE hReady = OpenSemaphoreA(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, READY_NAME);
    if (!hMutex || !hEmpty || !hFull || !hReady)
    {
        std::cout << "Cannot open sync objects\n";
        return 1;
    }

    ReleaseSemaphore(hReady, 1, nullptr);

    while (true)
    {
        std::cout << "\n1 - send message\n0 - exit\n> ";
        int cmd;
        if (!(std::cin >> cmd))
            return 1;
        if (cmd == 0)
            break;
        if (cmd != 1)
            return 1;

        std::cin.ignore();

        std::string text;
        std::cout << "Enter message (less than " << MAX_MSG_LEN << " characters): ";
        std::getline(std::cin, text);
        while (text.empty() || text.size() >= MAX_MSG_LEN)
        {
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cin.clear();
            std::cout << "Please, try again: ";
            getline(std::cin, text);
        }

        char buf[MAX_MSG_LEN] = {0};
        std::memcpy(buf, text.c_str(), text.size());

        std::cout << "Waiting for free slot\n";
        WaitForSingleObject(hEmpty, INFINITE);

        WaitForSingleObject(hMutex, INFINITE);
        pushMessage(fileName, buf);
        ReleaseMutex(hMutex);

        ReleaseSemaphore(hFull, 1, nullptr);
        std::cout << "Message sent\n";
    }

    CloseHandle(hReady);
    CloseHandle(hFull);
    CloseHandle(hEmpty);
    CloseHandle(hMutex);
    return 0;
}
