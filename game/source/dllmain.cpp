#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "cseries/console.hpp"
#include "memory/module.hpp"

void process_attach(HMODULE hModule)
{
	DisableThreadLibraryCalls(hModule);
	SetProcessDPIAware();

	c_console::initialize("ManagedDonkey");

	apply_all_patches(false);
	apply_all_hooks(false);
}

void process_detach(HMODULE hModule)
{
	apply_all_hooks(true);
	apply_all_patches(true);

	c_console::dispose();

	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		process_attach(hModule);
		break;
	case DLL_PROCESS_DETACH:
		process_detach(hModule);
		break;
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
