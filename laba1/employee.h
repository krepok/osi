#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <windows.h>

struct Employee
{
    int num;
    std::string name;
    double hours;
};

void runProcess(const std::string& commandLine)
{
    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);

    std::string cmd = commandLine;
    std::vector<char> buf(cmd.begin(), cmd.end());
    buf.push_back('\0');

    CreateProcessA(nullptr, cmd.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}