#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include "employee.h"

void printBinaryFile(const std::string& fileName)
{
    std::ifstream in(fileName, std::ios::binary);
    if (!in.is_open())
    {
        std::cout << "не удалось открыть файл \"" << fileName << "\".\n";
        return;
    }

    int count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < count; ++i)
    {
        Employee e{};
        int size{};

        in.read(reinterpret_cast<char*>(&e.num), sizeof(int));
        in.read(reinterpret_cast<char*>(&size), sizeof(int));
        e.name.resize(size);
        in.read(reinterpret_cast<char*>(e.name.data()), sizeof(char) * e.name.size());
        in.read(reinterpret_cast<char*>(&e.hours), sizeof(double));

        std::cout << e.num << " " << e.name << " " << e.hours << "\n";
    }
}

void printTextFile(const std::string& fileName)
{
    std::ifstream in(fileName);
    if (!in.is_open())
    {
        std::cout << "не удалось открыть файл \"" << fileName << "\".\n";
        return;
    }

    std::string line;
    while (std::getline(in, line))
        std::cout << line << "\n";
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);


    std::string binFileName;
    int count{};

    std::cout << "Введите имя бинарного файла: ";
    std::cin >> binFileName;

    std::cout << "Введите количество записей: ";
    std::cin >> count;

    std::string creatorCmd = "Creator.exe \"" + binFileName + "\" " + std::to_string(count);

    runProcess(creatorCmd);

    printBinaryFile(binFileName);

    std::string reportFileName;
    double rate{};

    std::cout << "\nВведите имя файла отчёта: ";
    std::cin >> reportFileName;

    std::cout << "Введите оплату за час работы: ";
    std::cin >> rate;

    std::string reporterCmd = "Reporter.exe \"" + binFileName + "\" \"" + reportFileName + "\" " + std::to_string(rate);
    runProcess(reporterCmd);

    printTextFile(reportFileName);
    return 0;
}