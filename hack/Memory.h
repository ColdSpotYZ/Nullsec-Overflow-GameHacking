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
void writeprocmem(DataType value, DWORD address);

template <class DataType>
DataType readprocmem(DWORD address);