#include "Memory.h"

bool attachProc(char* procName, HANDLE& hProc, DWORD& procID)
{
    PROCESSENTRY32 procEntry32;
    procEntry32.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcSnap == NULL || hProcSnap == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to get processes snapshot" << std::endl;
        return false;
    }

    while (Process32Next(hProcSnap, &procEntry32))
    {
        /*std::cout << procEntry32.szExeFile << std::endl;*/
        if (!strcmp(procEntry32.szExeFile, procName))
        {
            std::cout << "Process found: " << procEntry32.szExeFile << " with procID of " << procEntry32.th32ProcessID << std::endl;
            hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
            procID = procEntry32.th32ProcessID;

            if (hProc == NULL)
            {
                std::cout << "Failed getting handle to process." << std::endl;
            }

            CloseHandle(hProcSnap);
            return true;
        }
    }

    std::cout << "Unable to find process name in process list" << std::endl;
    CloseHandle(hProcSnap);
    return false;
}

DWORD getModule(LPSTR moduleName, DWORD procID, HANDLE& hModuleSnap, MODULEENTRY32& modEntry32)
{
    //Taking snapshot of modules in process
    hModuleSnap = INVALID_HANDLE_VALUE;
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procID);

    if (hModuleSnap == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to take a snapshot of modules." << std::endl;
        CloseHandle(hModuleSnap);
        return 0;
    }

    //Defining structure size
    modEntry32.dwSize = sizeof(MODULEENTRY32);

    //Getting information of first module in snapshot
    if (Module32First(hModuleSnap, &modEntry32))
    {
        //If the module we're looking for matches the module in snapshot
        if (!strcmp(moduleName, modEntry32.szModule))
        {
            //Outputting module name and base address
            std::cout << "Found module " << modEntry32.szModule << " with base address " << std::hex << (DWORD)modEntry32.modBaseAddr << std::endl;

            //Close handle to process snapshot then return base address
            CloseHandle(hModuleSnap);
            return (DWORD)modEntry32.modBaseAddr;
        }
    }

    //While there is a next module in the snapshot
    while (Module32Next(hModuleSnap, &modEntry32))
    {
        //If the module we're looking for matches the module in snapshot
        if (!strcmp(moduleName, modEntry32.szModule))
        {
            //Outputting module name and base address
            std::cout << "Found module " << modEntry32.szModule << " with base address 0x" << std::hex << std::uppercase << (DWORD)modEntry32.modBaseAddr << std::endl;

            //Close handle to process snapshot then return base address
            CloseHandle(hModuleSnap);
            return (DWORD)modEntry32.modBaseAddr;
        }
    }
    std::cout << "Couldn't find " << moduleName << " module in module snapshot of the process." << std::endl;
    CloseHandle(hModuleSnap);
    return false;
}

DWORD getAddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
    uintptr_t address = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        ReadProcessMemory(hProc, (LPVOID)address, &address, sizeof(address), 0);
        address += offsets[i];
    }
    return address;
}

