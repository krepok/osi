#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
#include "employee.h"

int main(int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << argv[0];

    std::string fileName = argv[1];
    int count = std::atoi(argv[2]);

    std::ofstream out(fileName, std::ios::binary);
    if (!out.is_open())
    {
        std::cout << "не удалось создать файл \"" << fileName << "\".\n";
        return 1;
    }

    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (int i = 0; i < count; ++i)
    {
        Employee e{};

        std::cout << "\nСотрудник " << (i + 1) << '\n';

        std::cout << "Номер сотрудника: ";
        std::cin >> e.num;

        std::cout << "Имя сотрудника: ";
        std::cin >> e.name;

        std::cout << "Отработано часов: ";
        while (!(std::cin >> e.hours))
        {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "Введите число: ";
        }

        int size = e.name.size();
        out.write(reinterpret_cast<const char*>(&e.num), sizeof(int));
        out.write(reinterpret_cast<const char*>(&size), sizeof(int));
        out.write(reinterpret_cast<const char*>(e.name.data()), sizeof(char) * e.name.size());
        out.write(reinterpret_cast<const char*>(&e.hours), sizeof(double));
    }

    out.close();
    return 0;
}