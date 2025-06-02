#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>

struct employee {
    int num;
    char name[10];
    double hours;
};

int ReporterMain(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: Reporter <binary_file_name> <report_file_name> <pay_rate>" << std::endl;
        return 1;
    }

    const char* binFileName = argv[1];
    const char* reportFileName = argv[2];
    double payRate = std::atof(argv[3]);

    std::ifstream in(binFileName, std::ios::binary);
    if (!in) {
        std::cout << "Error opening binary file." << std::endl;
        return 1;
    }

    std::vector<employee> employees;
    employee emp;
    while (in.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        employees.push_back(emp);
    }
    in.close();

    std::sort(employees.begin(), employees.end(), [](const employee& a, const employee& b) {
        return a.num < b.num;
        });

    std::ofstream out(reportFileName);
    if (!out) {
        std::cout << "Error opening report file." << std::endl;
        return 1;
    }

    out << "Report for file \"" << binFileName << "\"" << std::endl;
    out << "Employee ID, Employee Name, Hours, Salary" << std::endl;
    for (const auto& emp : employees) {
        double salary = emp.hours * payRate;
        out << emp.num << ", " << emp.name << ", " << emp.hours << ", " << salary << std::endl;
    }
    out.close();

    return 0;
}

#ifndef UNIT_TESTS
int main(int argc, char* argv[]) {
    return ReporterMain(argc, argv);
}
#endif
