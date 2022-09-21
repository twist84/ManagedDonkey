#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "cseries/console.hpp"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
    {
        SetProcessDPIAware();
        DisableThreadLibraryCalls(hModule);

        c_console::initialize("ManagedDonkey");
        break;
    }
    case DLL_PROCESS_DETACH:
    {
        c_console::dispose();

        FreeLibrary(hModule);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

// needed for `DetourCreateProcessWithDllA`
extern "C" __declspec(dllexport) void null_export(void)
{
}
