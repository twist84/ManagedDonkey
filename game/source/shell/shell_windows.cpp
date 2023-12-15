#include "cseries/cseries.hpp"

#include "cache/physical_memory_map.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"

#include <Windows.h>

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//	SetLastError(NO_ERROR);
//	SetProcessDPIAware();
//
//	g_lpCmdLine = lpCmdLine;
//	g_hInstance = hInstance;
//	g_nShowCmd = nShowCmd;
//	g_lpfnWndProc = WndProc;
//	g_hWnd = NULL;
//
//	c_static_string<64> name;
//	//name.print("HaloOnline %s", sub_5013A0());
//	name.print("HaloOnline %s_%d", "cert_ms23_release_106708", g_level_globals.checksum);
//	csstrnzcpy(g_lpszClassName, name.get_string(), sizeof(g_lpszClassName));
//	csstrnzcpy(g_lpszWindowName, name.get_string(), sizeof(g_lpszClassName));
//
//	physical_memory_initialize();
//	physical_memory_stage_push(_memory_stage_game_initialize);
//
//	SetBinDllDirectory(); // PC
//
//	if (shell_initialize(false))
//	{
//		RegisterHotKey_Snapshot(); // Halo Online
//
//		main_loop();
//		shell_dispose();
//
//		UnregisterHotKey_Snapshot(); // Halo Online
//	}
//
//	physical_memory_stage_pop(_memory_stage_game_initialize);
//	physical_memory_dispose();
//
//	return 0;
//}
//HOOK_DECLARE(0x0042EB10, WinMain);

