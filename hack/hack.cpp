#include "Memory.h"


HANDLE hProc = NULL;
DWORD procID;

HANDLE hModuleSnap = NULL;
MODULEENTRY32 modEntry32;

DWORD baseAddress = NULL;

std::vector<unsigned int> ammoOffsets = { 0x150 };
std::vector<unsigned int> hpOffsets = { 0xF8 };

int main()
{
    const char* process_name = "ac_client.exe";
    if (attachProc((char*)process_name, hProc, procID))
    {
        baseAddress = getModule((LPSTR)"ac_client.exe", procID, hModuleSnap, modEntry32);
        uintptr_t dynamicPtrBaseAddr = baseAddress + 0x0010F4F4;
        std::cout << "dynamicPtrBaseAddr: " << dynamicPtrBaseAddr << std::endl;
        DWORD ammoAddr = getAddress(hProc, dynamicPtrBaseAddr, ammoOffsets);
        DWORD hpAddr = getAddress(hProc, dynamicPtrBaseAddr, hpOffsets);
        std::cout << "ammoAddr: " << ammoAddr << std::endl;

        while (true)
        {
            int hp = readprocmem<int>(hProc, hpAddr);
            std::cout << "HP: " << hp << std::endl;
            writeprocmem(hProc, 1000, hpAddr);

            int ammo = readprocmem<int>(hProc, ammoAddr);
            std::cout << "Ammo: " << ammo << std::endl;
            writeprocmem(hProc, 10, ammoAddr);


            
        }
    }

}
