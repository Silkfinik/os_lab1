#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <cstdlib>

struct employee {
    int num;
    char name[10];
    double hours;
};

int main() {
    setlocale(LC_ALL, "English");

    std::string binFileName;
    int recordCount;

    std::cout << "Enter binary file name: ";
    std::cin >> binFileName;
    std::cout << "Enter number of records: ";
    std::cin >> recordCount;

    std::string creatorCmd = "Creator.exe " + binFileName + " " + std::to_string(recordCount);
    std::wstring wCreatorCmd(creatorCmd.begin(), creatorCmd.end());

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessW(NULL,
        const_cast<LPWSTR>(wCreatorCmd.c_str()),
        NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cout << "Error launching Creator." << std::endl;
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::ifstream binFile(binFileName, std::ios::binary);
    if (!binFile) {
        std::cout << "Error opening binary file." << std::endl;
        return 1;
    }

    std::cout << "\nBinary file content:" << std::endl;

    employee emp;

    while (binFile.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        std::cout << "ID: " << emp.num << ", Name: " << emp.name
            << ", Hours: " << emp.hours << std::endl;
    }

    binFile.close();

    std::string reportFileName;
    double payRate;

    std::cout << "\nEnter report file name: ";
    std::cin >> reportFileName;
    std::cout << "Enter hourly rate: ";
    std::cin >> payRate;

    std::string reporterCmd = "Reporter.exe " + binFileName + " " + reportFileName + " " + std::to_string(payRate);
    std::wstring wReporterCmd(reporterCmd.begin(), reporterCmd.end());

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessW(NULL,
        const_cast<LPWSTR>(wReporterCmd.c_str()),
        NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cout << "Error launching Reporter." << std::endl;
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::ifstream reportFile(reportFileName);

    if (!reportFile) {
        std::cout << "Error opening report file." << std::endl;
        return 1;
    }

    std::cout << "\nReport content:" << std::endl;
    std::string line;

    while (std::getline(reportFile, line)) {
        std::cout << line << std::endl;
    }

    reportFile.close();

    return 0;
}