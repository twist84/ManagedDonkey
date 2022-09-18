#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
    {
        SetProcessDPIAware();
        DisableThreadLibraryCalls(hModule);
        break;
    }
    case DLL_PROCESS_DETACH:
    {
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
