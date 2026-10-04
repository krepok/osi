#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include "employee.h"

int main(int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::string binFileName = argv[1];
    std::string reportFileName = argv[2];
    double rate = std::atof(argv[3]);

    std::ifstream in(binFileName, std::ios::binary);
    if (!in.is_open())
    {
        std::cout << "не удалось открыть файл \"" << binFileName << "\".\n";
        return 1;
    }

    int count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    std::vector<Employee> employees;
    for (int i = 0; i < count; ++i)
    {
        Employee e{};
        int size{};

        in.read(reinterpret_cast<char*>(&e.num), sizeof(int));
        in.read(reinterpret_cast<char*>(&size), sizeof(int));
        e.name.resize(size);
        in.read(reinterpret_cast<char*>(e.name.data()), sizeof(char) * e.name.size());
        in.read(reinterpret_cast<char*>(&e.hours), sizeof(double));

        employees.push_back(e);
    }

    in.close();

    if (employees.empty())
    {
        std::cout << "файл \"" << binFileName << "\" пуст или повреждён.\n";
        return 1;
    }

    std::sort(employees.begin(), employees.end(),
        [](const Employee& a, const Employee& b) { return a.num < b.num; });

    std::ofstream out(reportFileName);
    if (!out.is_open())
    {
        std::cout << "не удалось создать файл \"" << reportFileName << "\".\n";
        return 1;
    }

    out << "Отчет по файлу \"" << binFileName << "\"\n\n";
    out << std::left << std::setw(12) << "Id" << std::setw(12) << "Name" << std::setw(10) << "Hours" << std::setw(12) << "Salary" << "\n";

    out << std::fixed << std::setprecision(2);
    for (const auto& emp : employees)
    {
        double salary = emp.hours * rate;

        out << std::left << std::setw(12) << emp.num << std::setw(12) << emp.name << std::setw(10) << emp.hours << std::setw(12) << salary << "\n";
    }

    out.close();
    return 0;
}