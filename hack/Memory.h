#pragma once
#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
using namespace std;

bool attachProc(char* procName, HANDLE& hProc, DWORD& procID);

DWORD getModule(LPSTR moduleName, DWORD procID, HANDLE& hModuleSnap, MODULEENTRY32& modEntry32);

DWORD getAddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);

template <class DataType>
void writeprocmem(HANDLE hProc, DataType value, DWORD address);

template <class DataType>
DataType readprocmem(HANDLE hProc, DWORD address);

template<class DataType>
inline void writeprocmem(HANDLE hProc, DataType value, DWORD address)
{
    std::cout << "Writing " << value << " to " << address << std::endl;
    WriteProcessMemory(hProc, (LPVOID)address, &value, sizeof(DataType), 0);
}

template<class DataType>
inline DataType readprocmem(HANDLE hProc, DWORD address)
{
    DataType returnBuffer;

    std::cout << "Reading from " << address << std::endl;
    ReadProcessMemory(hProc, (LPVOID)address, &returnBuffer, sizeof(DataType), 0);

    return returnBuffer;
}


