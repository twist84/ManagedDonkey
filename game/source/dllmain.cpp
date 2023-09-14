#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

#include "game/cheats.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/gui_custom_bitmap_widget.hpp"
#include "networking/tools/remote_command.hpp"

void process_attach(HMODULE hModule)
{
	set_donkey_module(hModule);
	DisableThreadLibraryCalls(hModule);
	SetProcessDPIAware();

	c_console::initialize("ManagedDonkey");

	apply_all_patches(false);
	apply_all_hooks(false);

	patch_cheats();
	patch_debug_menu();
	patch_gui_custom_bitmap_widget();
	patch_remote_command();
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

extern "C" __declspec(dllexport) void null_export(void) // detours helper export
{
}

