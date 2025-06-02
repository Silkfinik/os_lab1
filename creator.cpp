#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

struct employee {
    int num;
    char name[10];
    double hours;
};

int CreatorMain(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: Creator <binary_file_name> <record_count>" << std::endl;
        return 1;
    }

    const char* binFileName = argv[1];
    int recordCount = std::atoi(argv[2]);

    std::ofstream out(binFileName, std::ios::binary);
    if (!out) {
        std::cout << "Error opening file for writing." << std::endl;
        return 1;
    }

    employee emp;
    for (int i = 0; i < recordCount; i++) {
        std::cout << "Enter data for employee " << i + 1 << " (ID, name, hours): ";
        std::cin >> emp.num >> emp.name >> emp.hours;
        out.write(reinterpret_cast<char*>(&emp), sizeof(emp));
    }
    out.close();

    return 0;
}

#ifndef UNIT_TESTS
int main(int argc, char* argv[]) {
    return CreatorMain(argc, argv);
}
#endif
