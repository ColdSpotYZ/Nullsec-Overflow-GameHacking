#include "Memory.h"


HANDLE hProc = NULL;
DWORD procID;

HANDLE hModuleSnap = NULL;
MODULEENTRY32 modEntry32;

DWORD baseAddress = NULL;

std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
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
            if (GetAsyncKeyState(VK_NUMPAD1))
            {
                // Set ammo to 10
                int ammo = readprocmem<int>(ammoAddr);
                std::cout << "Ammo: " << ammo << std::endl;
                writeprocmem(10, ammoAddr);

                Sleep(1);
            }
            if (GetAsyncKeyState(VK_NUMPAD2))
            {
                int hp = readprocmem<int>(hpAddr);
                std::cout << "HP: " << hp << std::endl;
                writeprocmem(100, hpAddr);

                Sleep(1);
            }
            
        }
    }

}
