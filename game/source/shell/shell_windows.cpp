#include "cseries/cseries.hpp"

#include "cache/physical_memory_map.hpp"
#include "main/levels.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
#include "shell/shell.hpp"

#include <Windows.h>

struct s_windows_params
{
	HINSTANCE instance;
	HWND created_window_handle;
	HWND window_handle;
	int cmd_show;
	WNDPROC window_proc;
	CHAR class_name[64];
	CHAR window_name[64];
	LPSTR cmd_line;
};
static_assert(sizeof(s_windows_params));

REFERENCE_DECLARE(0x0199C010, s_windows_params, g_windows_params);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return INVOKE(0x0042E6A0, WndProc, hWnd, uMsg, wParam, lParam);
}

char const* sub_5013A0()
{
	return INVOKE(0x005013A0, sub_5013A0);

	//static c_static_string<256> name;
	//name.print("HaloOnline %s_%d", "cert_ms23_release_106708", levels_get_checksum());
	//return name.get_string();
}

void SetBinDllDirectory()
{
	char dll_directory[1024]{};
	csstrnzcat(dll_directory, ".\\bin", 1024);
	SetDllDirectoryA(dll_directory);

	//generate_event(_event_level_message, "system: dll directory={ %s }", dll_directory);
	printf("system: dll directory={ %s }\n", dll_directory);
}

void RegisterHotKey_Snapshot()
{
	if (shell_application_type() == _shell_application_type_client)
		RegisterHotKey(g_windows_params.created_window_handle, 0x2C /* 0x2C same as VK_SNAPSHOT */, 0, VK_SNAPSHOT);
}

void UnregisterHotKey_Snapshot()
{
	if (shell_application_type() == _shell_application_type_client)
		UnregisterHotKey(g_windows_params.created_window_handle, VK_SNAPSHOT);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetLastError(NO_ERROR);
	SetProcessDPIAware();

	g_windows_params.cmd_line = lpCmdLine;
	g_windows_params.instance = hInstance;
	g_windows_params.cmd_show = nCmdShow;
	g_windows_params.window_proc = WndProc;
	g_windows_params.window_handle = NULL;

	c_static_string<64> name;
	name.print("HaloOnline %s", sub_5013A0());
	csstrnzcpy(g_windows_params.class_name, name.get_string(), sizeof(g_windows_params.class_name));
	csstrnzcpy(g_windows_params.window_name, name.get_string(), sizeof(g_windows_params.window_name));

	physical_memory_initialize();
	physical_memory_stage_push(_memory_stage_game_initialize);

	SetBinDllDirectory(); // PC

	if (shell_initialize(false))
	{
		RegisterHotKey_Snapshot(); // Halo Online

		main_loop();

		UnregisterHotKey_Snapshot(); // Halo Online
	}
	shell_dispose();

	physical_memory_dispose();

	return 0;
}
HOOK_DECLARE(0x0042EB10, WinMain);

