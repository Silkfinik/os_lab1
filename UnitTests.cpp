#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cstring>
#include <string>

struct employee {
    int num;
    char name[10];
    double hours;
};

int CreatorMain(int argc, char* argv[]);
int ReporterMain(int argc, char* argv[]);

TEST(CreatorTests, InvalidArguments) {
    int argc = 1;
    char* argv[1];
    argv[0] = const_cast<char*>("Creator.exe");
    int ret = CreatorMain(argc, argv);
    EXPECT_NE(ret, 0);
}

TEST(CreatorTests, SingleRecord) {
    const char* binaryFileName = "input";
    int argc = 3;
    char* argv[3];
    argv[0] = const_cast<char*>("Creator.exe");
    argv[1] = const_cast<char*>(binaryFileName);
    argv[2] = const_cast<char*>("1");

    std::streambuf* origCin = std::cin.rdbuf();
    std::istringstream simulatedInput("1 John 40\n");
    std::cin.rdbuf(simulatedInput.rdbuf());

    int ret = CreatorMain(argc, argv);
    EXPECT_EQ(ret, 0);
    std::cin.rdbuf(origCin);

    std::ifstream in(binaryFileName, std::ios::binary);
    ASSERT_TRUE(in.is_open());
    employee emp;
    in.read(reinterpret_cast<char*>(&emp), sizeof(emp));
    in.close();

    EXPECT_EQ(emp.num, 1);
    EXPECT_EQ(std::string(emp.name), "John");
    EXPECT_DOUBLE_EQ(emp.hours, 40.0);

    std::remove(binaryFileName);
}

TEST(CreatorTests, MultipleRecords) {
    const char* binaryFileName = "input";
    int argc = 3;
    char* argv[3];
    argv[0] = const_cast<char*>("Creator.exe");
    argv[1] = const_cast<char*>(binaryFileName);
    argv[2] = const_cast<char*>("2");

    std::streambuf* origCin = std::cin.rdbuf();
    std::istringstream simulatedInput("1 Alice 35.5\n2 Bob 40\n");
    std::cin.rdbuf(simulatedInput.rdbuf());

    int ret = CreatorMain(argc, argv);
    EXPECT_EQ(ret, 0);
    std::cin.rdbuf(origCin);

    std::ifstream in(binaryFileName, std::ios::binary);
    ASSERT_TRUE(in.is_open());
    employee emp1, emp2;
    in.read(reinterpret_cast<char*>(&emp1), sizeof(emp1));
    in.read(reinterpret_cast<char*>(&emp2), sizeof(emp2));
    in.close();

    EXPECT_EQ(emp1.num, 1);
    EXPECT_EQ(std::string(emp1.name), "Alice");
    EXPECT_DOUBLE_EQ(emp1.hours, 35.5);

    EXPECT_EQ(emp2.num, 2);
    EXPECT_EQ(std::string(emp2.name), "Bob");
    EXPECT_DOUBLE_EQ(emp2.hours, 40.0);

    std::remove(binaryFileName);
}

TEST(ReporterTests, InvalidArguments) {
    int argc = 2;
    char* argv[2];
    argv[0] = const_cast<char*>("Reporter.exe");
    argv[1] = const_cast<char*>("input");
    int ret = ReporterMain(argc, argv);
    EXPECT_NE(ret, 0);
}

TEST(ReporterTests, MissingInputFile) {
    const char* binaryFileName = "nonexistent.bin";
    const char* reportFileName = "report";
    int argc = 4;
    char* argv[4];
    argv[0] = const_cast<char*>("Reporter.exe");
    argv[1] = const_cast<char*>(binaryFileName);
    argv[2] = const_cast<char*>(reportFileName);
    argv[3] = const_cast<char*>("10");

    int ret = ReporterMain(argc, argv);
    EXPECT_NE(ret, 0);
}

TEST(ReporterTests, EmptyInputFile) {
    const char* binaryFileName = "empty.bin";
    std::ofstream out(binaryFileName, std::ios::binary);
    out.close();

    const char* reportFileName = "report";
    int argc = 4;
    char* argv[4];
    argv[0] = const_cast<char*>("Reporter.exe");
    argv[1] = const_cast<char*>(binaryFileName);
    argv[2] = const_cast<char*>(reportFileName);
    argv[3] = const_cast<char*>("10");

    int ret = ReporterMain(argc, argv);
    EXPECT_EQ(ret, 0);

    std::ifstream report(reportFileName);
    ASSERT_TRUE(report.is_open());
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(report, line))
        lines.push_back(line);
    report.close();

    EXPECT_EQ(lines.size(), 2u);
    EXPECT_EQ(lines[0], "Report for file \"empty.bin\"");
    EXPECT_EQ(lines[1], "Employee ID, Employee Name, Hours, Salary");

    std::remove(binaryFileName);
    std::remove(reportFileName);
}

TEST(ReporterTests, SortsAndCalculatesSalary) {
    const char* binaryFileName = "input";
    std::ofstream out(binaryFileName, std::ios::binary);
    ASSERT_TRUE(out.is_open());

    employee emp1;
    emp1.num = 2;
    std::strncpy(emp1.name, "Bob", sizeof(emp1.name));
    emp1.name[sizeof(emp1.name) - 1] = '\0';
    emp1.hours = 40.0;

    employee emp2;
    emp2.num = 1;
    std::strncpy(emp2.name, "Alice", sizeof(emp2.name));
    emp2.name[sizeof(emp2.name) - 1] = '\0';
    emp2.hours = 35.5;

    out.write(reinterpret_cast<char*>(&emp1), sizeof(employee));
    out.write(reinterpret_cast<char*>(&emp2), sizeof(employee));
    out.close();

    const char* reportFileName = "report";
    int argc = 4;
    char* argv[4];
    argv[0] = const_cast<char*>("Reporter.exe");
    argv[1] = const_cast<char*>(binaryFileName);
    argv[2] = const_cast<char*>(reportFileName);
    argv[3] = const_cast<char*>("10");

    int ret = ReporterMain(argc, argv);
    EXPECT_EQ(ret, 0);

    std::ifstream report(reportFileName);
    ASSERT_TRUE(report.is_open());
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(report, line))
        lines.push_back(line);
    report.close();

    ASSERT_EQ(lines.size(), 4u);
    EXPECT_EQ(lines[0], "Report for file \"input\"");
    EXPECT_EQ(lines[1], "Employee ID, Employee Name, Hours, Salary");
    EXPECT_EQ(lines[2], "1, Alice, 35.5, 355");
    EXPECT_EQ(lines[3], "2, Bob, 40, 400");

    std::remove(binaryFileName);
    std::remove(reportFileName);
}

TEST(ReporterTests, MultipleRecordsSorting) {
    const char* binaryFileName = "input";
    std::ofstream out(binaryFileName, std::ios::binary);
    ASSERT_TRUE(out.is_open());

    employee records[3];
    records[0].num = 3;
    std::strncpy(records[0].name, "Charlie", sizeof(records[0].name));
    records[0].name[sizeof(records[0].name) - 1] = '\0';
    records[0].hours = 30.0;

    records[1].num = 1;
    std::strncpy(records[1].name, "Alice", sizeof(records[1].name));
    records[1].name[sizeof(records[1].name) - 1] = '\0';
    records[1].hours = 35.5;

    records[2].num = 2;
    std::strncpy(records[2].name, "Bob", sizeof(records[2].name));
    records[2].name[sizeof(records[2].name) - 1] = '\0';
    records[2].hours = 40.0;

    for (int i = 0; i < 3; i++) {
        out.write(reinterpret_cast<char*>(&records[i]), sizeof(employee));
    }
    out.close();

    const char* reportFileName = "report";
    int argc = 4;
    char* argv[4];
    argv[0] = const_cast<char*>("Reporter.exe");
    argv[1] = const_cast<char*>(binaryFileName);
    argv[2] = const_cast<char*>(reportFileName);
    argv[3] = const_cast<char*>("10");

    int ret = ReporterMain(argc, argv);
    EXPECT_EQ(ret, 0);

    std::ifstream report(reportFileName);
    ASSERT_TRUE(report.is_open());
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(report, line))
        lines.push_back(line);
    report.close();

    ASSERT_EQ(lines.size(), 5u);
    EXPECT_EQ(lines[0], "Report for file \"input\"");
    EXPECT_EQ(lines[1], "Employee ID, Employee Name, Hours, Salary");
    EXPECT_EQ(lines[2], "1, Alice, 35.5, 355");
    EXPECT_EQ(lines[3], "2, Bob, 40, 400");
    EXPECT_EQ(lines[4], "3, Charlie, 30, 300");

    std::remove(binaryFileName);
    std::remove(reportFileName);
}
