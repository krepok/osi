#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "file.h"

constexpr const char* ENV_NAME = "Ivanov";

int main()
{
    std::string fileName;
    int recordCount;
    int senderCount;

    std::cout << "1 - enter file name from console\n2 - get file name from environment variable\n> ";
    int choice;
    if (!(std::cin >> choice))
        return 1;

    if (choice == 1)
    {
        std::cout << "Enter binary file name: ";
        if (!(std::cin >> fileName))
            return 1;
    }
    else if (choice == 2)
    {
        char* value = nullptr;
        size_t size = 0;
        if (_dupenv_s(&value, &size, ENV_NAME) != 0 || value == nullptr)
        {
            std::cout << "Environment variable not found\n";
            return 1;
        }
        fileName = value;
        free(value);

        std::cout << "File name from environment: " << fileName << "\n";
    }
    else
        return 1;

    std::cout << "Enter number of records: ";
    if (!(std::cin >> recordCount) || recordCount <= 0)
        return 1;

    createHeaderFile(fileName, recordCount);

    std::cout << "Enter number of Sender processes: ";
    if (!(std::cin >> senderCount) || senderCount <= 0)
        return 1;

    HANDLE hMutex = CreateMutexA(nullptr, FALSE, MUTEX_NAME);
    HANDLE hEmpty = CreateSemaphoreA(nullptr, recordCount, recordCount, EMPTY_NAME);
    HANDLE hFull = CreateSemaphoreA(nullptr, 0, recordCount, FULL_NAME);
    HANDLE hReady = CreateSemaphoreA(nullptr, 0, senderCount, READY_NAME);
    if (!hMutex || !hEmpty || !hFull || !hReady)
    {
        std::cout << "Cannot create sync objects\n";
        return 1;
    }

    std::vector<HANDLE> processes;
    for (int i = 0; i < senderCount; ++i) {
        std::string cmd = "Sender.exe \"" + fileName + "\"";

        std::vector<char> cmdBuf(cmd.begin(), cmd.end());
        cmdBuf.push_back('\0');

        STARTUPINFOA si{};
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi{};

        if (!CreateProcessA(nullptr, cmdBuf.data(), nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi))
        {
            std::cout << "Cannot start Sender\n";
            return 1;
        }

        CloseHandle(pi.hThread);
        processes.push_back(pi.hProcess);
    }

    std::cout << "Waiting for all Senders\n";
    for (int i = 0; i < senderCount; ++i)
    {
        WaitForSingleObject(hReady, INFINITE);
    }
    std::cout << "All Senders are ready\n";

    while (true)
    {
        std::cout << "\n1 - read message\n0 - exit\n> ";
        int cmd;

        if (!(std::cin >> cmd))
            return 1;
        if (cmd == 0)
            break;
        if (cmd != 1)
            return 1;

        std::cout << "Waiting for message\n";

        bool gotMessage = false;
        while (true)
        {
            if (WaitForSingleObject(hFull, 100) == WAIT_OBJECT_0)
            {
                gotMessage = true;
                break;
            }

            bool anyAlive = false;
            for (HANDLE p : processes)
            {
                if (WaitForSingleObject(p, 0) == WAIT_TIMEOUT)
                {
                    anyAlive = true;
                    break;
                }
            }

            if (!anyAlive) 
            {
                if (WaitForSingleObject(hFull, 0) == WAIT_OBJECT_0)
                    gotMessage = true;
                break;
            }
        }
        if (!gotMessage)
        {
            std::cout << "All Senders are closed, no more messages\n";
            break;
        }

        WaitForSingleObject(hMutex, INFINITE);
        char msg[MAX_MSG_LEN + 1] = {0};
        popMessage(fileName, msg);
        ReleaseMutex(hMutex);

        ReleaseSemaphore(hEmpty, 1, nullptr);
        std::cout << "Received: " << msg << "\n";
    }

    for (HANDLE p : processes) 
        CloseHandle(p);
    CloseHandle(hReady);
    CloseHandle(hFull);
    CloseHandle(hEmpty);
    CloseHandle(hMutex);
    return 0;
}
