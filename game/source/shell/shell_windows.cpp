#include "shell/shell_windows.hpp"

#include "cache/physical_memory_map.hpp"
#include "hs/hs_runtime.hpp"
#include "input/input_windows.hpp"
#include "main/console.hpp"
#include "main/global_preferences.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
#include "multithreading/threads.hpp"
#include "rasterizer/rasterizer.hpp"
#include "shell/shell.hpp"
#include "shell/shell_splash.hpp"

#include <commctrl.h>  // For common controls like status bar
#pragma comment(lib, "Comctl32.lib")

REFERENCE_DECLARE(0x0199C010, s_windows_params, g_windows_params);

HOOK_DECLARE(0x0042E940, shell_idle);
HOOK_DECLARE(0x0042EB10, _WinMain);
HOOK_DECLARE(0x0051CE40, shell_get_system_identifier);

bool fake_system_identifier = false;

bool s_windows_params::editor_window_create = false;
HWND s_windows_params::editor_window_handle = NULL;
WNDPROC s_windows_params::editor_window_proc = DefWindowProcA;
CHAR s_windows_params::editor_class_name[64]{};
CHAR s_windows_params::editor_window_name[64]{};

#define ID_FILE_OPTION_01 57601
#define ID_FILE_OPTION_02 57603
#define ID_FILE_OPTION_03 57604
#define ID_FILE_OPTION_04 32803
#define ID_FILE_OPTION_05 32802
#define ID_FILE_OPTION_06 57665

#define ID_EDIT_OPTION_01 32925
#define ID_EDIT_OPTION_02 32837
#define ID_EDIT_OPTION_03 273
#define ID_EDIT_OPTION_04 32863
#define ID_EDIT_OPTION_05 32864
#define ID_EDIT_OPTION_06 32880
#define ID_EDIT_OPTION_07 32893

#define ID_VIEW_OPTION_01 59392
#define ID_VIEW_OPTION_02 59393
#define ID_VIEW_OPTION_03 32794
#define ID_VIEW_OPTION_04 32795
#define ID_VIEW_OPTION_05 32796
#define ID_VIEW_OPTION_06 32811
#define ID_VIEW_OPTION_07 32975
#define ID_VIEW_OPTION_08 32883
#define ID_VIEW_OPTION_09 32920

#define ID_SCENARIOS_OPTION_01 32870
#define ID_SCENARIOS_OPTION_02 32871
#define ID_SCENARIOS_OPTION_03 32916
#define ID_SCENARIOS_OPTION_04 32917
#define ID_SCENARIOS_OPTION_05 32839
#define ID_SCENARIOS_OPTION_06 264
#define ID_SCENARIOS_OPTION_07 266
#define ID_SCENARIOS_OPTION_08 268
#define ID_SCENARIOS_OPTION_09 269
#define ID_SCENARIOS_OPTION_10 270
#define ID_SCENARIOS_OPTION_11 32888

#define ID_ABOUT_OPTION_1 57664

#define MF_UNIMPLEMENTED MF_GRAYED

#define UPDATE_MENU_OPTION_CHECKBOX(MENU_HANDLE) \
{ \
	MENUITEMINFO menu_item_info = { sizeof(MENUITEMINFO) }; \
	menu_item_info.fMask = MIIM_STATE; \
	GetMenuItemInfo((MENU_HANDLE), LOWORD(wParam), FALSE, &menu_item_info); \
	SET_MASK(menu_item_info.fState, MFS_CHECKED, !TEST_MASK(menu_item_info.fState, MFS_CHECKED)); \
	SetMenuItemInfo((MENU_HANDLE), LOWORD(wParam), FALSE, &menu_item_info); \
	DrawMenuBar(GetParent((HWND)(MENU_HANDLE))); \
}

LRESULT CALLBACK EditorWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HMENU menu_handle = NULL;
	static HMENU file_menu_handle = NULL;
	static HMENU edit_menu_handle = NULL;
	static HMENU view_menu_handle = NULL;
	static HMENU scenarios_menu_handle = NULL;
	static HMENU help_menu_handle = NULL;

	static HINSTANCE status_bar_instance = NULL;
	static HWND status_bar_handle = NULL;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		if (menu_handle = CreateMenu())
		{
			if (file_menu_handle = CreateMenu())
			{
				AppendMenu(file_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_FILE_OPTION_01, TEXT("&Open Scenario\tCtrl+O"));
				AppendMenu(file_menu_handle, MF_SEPARATOR, 0, NULL);
				AppendMenu(file_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_FILE_OPTION_02, TEXT("&Save Scenario\tCtrl+S"));
				AppendMenu(file_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_FILE_OPTION_03, TEXT("Save Scenario &As..."));
				AppendMenu(file_menu_handle, MF_SEPARATOR, 0, NULL);
				AppendMenu(file_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_FILE_OPTION_04, TEXT("&Compile scripts\tCtrl+Shift+C"));
				AppendMenu(file_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_FILE_OPTION_05, TEXT("&Export script names\tCtrl+Shift+E"));
				AppendMenu(file_menu_handle, MF_SEPARATOR, 0, NULL);
				AppendMenu(file_menu_handle, MF_STRING, ID_FILE_OPTION_06, TEXT("E&xit"));
				AppendMenu(menu_handle, MF_POPUP, (UINT_PTR)file_menu_handle, TEXT("&File"));
			}

			if (edit_menu_handle = CreateMenu())
			{
				AppendMenu(edit_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_EDIT_OPTION_01, TEXT("&Switch Zone Set...\tCtrl+B"));
				AppendMenu(edit_menu_handle, MF_STRING | MF_UNCHECKED | MF_UNIMPLEMENTED, ID_EDIT_OPTION_02, TEXT("E&xpert mode...\tCtrl+Alt+Shift+X"));
				AppendMenu(edit_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_EDIT_OPTION_03, TEXT("Reset object &z...\tCtrl+Alt+Z"));
				AppendMenu(edit_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_EDIT_OPTION_04, TEXT("Copy object transform\tCtrl+K"));
				AppendMenu(edit_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_EDIT_OPTION_05, TEXT("Apply object transform\tCtrl+L"));
				AppendMenu(edit_menu_handle, MF_STRING | MF_UNCHECKED | MF_UNIMPLEMENTED, ID_EDIT_OPTION_06, TEXT("&Hexidecimal mode..."));
				AppendMenu(edit_menu_handle, MF_STRING, ID_EDIT_OPTION_07, TEXT("&Clear output window"));
				AppendMenu(menu_handle, MF_POPUP, (UINT_PTR)edit_menu_handle, TEXT("&Edit"));
			}

			if (view_menu_handle = CreateMenu())
			{
				AppendMenu(view_menu_handle, MF_STRING | MF_UNCHECKED | MF_UNIMPLEMENTED, ID_VIEW_OPTION_01, TEXT("Toolbar"));
				AppendMenu(view_menu_handle, MF_STRING | MF_CHECKED, ID_VIEW_OPTION_02, TEXT("&Status Bar"));
				AppendMenu(view_menu_handle, MF_SEPARATOR, 0, NULL);
				AppendMenu(view_menu_handle, MF_STRING | MF_CHECKED, ID_VIEW_OPTION_03, TEXT("&Game window"));
				AppendMenu(view_menu_handle, MF_STRING | MF_UNCHECKED | MF_UNIMPLEMENTED, ID_VIEW_OPTION_04, TEXT("&Properties palette"));
				AppendMenu(view_menu_handle, MF_STRING | MF_UNCHECKED | MF_UNIMPLEMENTED, ID_VIEW_OPTION_05, TEXT("&Hierarchy view"));
				AppendMenu(view_menu_handle, MF_STRING | MF_UNCHECKED | MF_UNIMPLEMENTED, ID_VIEW_OPTION_06, TEXT("&Tool window"));
				AppendMenu(view_menu_handle, MF_STRING | MF_CHECKED, ID_VIEW_OPTION_07, TEXT("&Output Window"));
				AppendMenu(view_menu_handle, MF_SEPARATOR, 0, NULL);
				AppendMenu(view_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_VIEW_OPTION_08, TEXT("Reset window prefs"));
				AppendMenu(view_menu_handle, MF_STRING | MF_UNCHECKED | MF_UNIMPLEMENTED, ID_VIEW_OPTION_09, TEXT("&Lock aspect ratio"));
				AppendMenu(menu_handle, MF_POPUP, (UINT_PTR)view_menu_handle, TEXT("View"));
			}

			if (scenarios_menu_handle = CreateMenu())
			{
				AppendMenu(scenarios_menu_handle, MF_STRING, ID_SCENARIOS_OPTION_01, TEXT("Run game scripts\tAlt+G"));
				AppendMenu(scenarios_menu_handle, MF_STRING, ID_SCENARIOS_OPTION_02, TEXT("Map reset\tAlt+R"));
				AppendMenu(scenarios_menu_handle, MF_SEPARATOR, 0, NULL);
				AppendMenu(scenarios_menu_handle, MF_STRING | MF_DISABLED | MF_UNIMPLEMENTED, ID_SCENARIOS_OPTION_03, TEXT("Place Squad\tCtrl+P"));
				AppendMenu(scenarios_menu_handle, MF_STRING | MF_DISABLED | MF_UNIMPLEMENTED, ID_SCENARIOS_OPTION_04, TEXT("Erase Squad\tCtrl+Shift+P"));
				AppendMenu(scenarios_menu_handle, MF_SEPARATOR, 0, NULL);
				AppendMenu(scenarios_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_SCENARIOS_OPTION_05, TEXT("Generate all pathfinding data"));
				AppendMenu(scenarios_menu_handle, MF_SEPARATOR, 0, NULL);
				AppendMenu(scenarios_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_SCENARIOS_OPTION_06, TEXT("S&plit Mission resources\tCtrl+Shift+P"));
				AppendMenu(scenarios_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_SCENARIOS_OPTION_07, TEXT("Split Mission &scripts"));
				AppendMenu(scenarios_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_SCENARIOS_OPTION_08, TEXT("&Add Mission script"));
				AppendMenu(scenarios_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_SCENARIOS_OPTION_09, TEXT("Split Mission AI"));
				AppendMenu(scenarios_menu_handle, MF_STRING | MF_DISABLED | MF_UNIMPLEMENTED, ID_SCENARIOS_OPTION_10, TEXT("Set Active Mission AI"));
				AppendMenu(scenarios_menu_handle, MF_SEPARATOR, 0, NULL);
				AppendMenu(scenarios_menu_handle, MF_STRING | MF_UNIMPLEMENTED, ID_SCENARIOS_OPTION_11, TEXT("&Import comments\tCtrl+Shift+L"));
				AppendMenu(menu_handle, MF_POPUP, (UINT_PTR)scenarios_menu_handle, TEXT("Scenarios"));
			}

			if (help_menu_handle = CreateMenu())
			{
				AppendMenu(help_menu_handle, MF_STRING, ID_ABOUT_OPTION_1, TEXT("About Donkey..."));
				AppendMenu(menu_handle, MF_POPUP, (UINT_PTR)help_menu_handle, TEXT("Help"));
			}

			SetMenu(hWnd, menu_handle);
		}

		INITCOMMONCONTROLSEX init_common_controls_ex{};
		init_common_controls_ex.dwSize = sizeof(init_common_controls_ex);
		init_common_controls_ex.dwICC = ICC_BAR_CLASSES;
		InitCommonControlsEx(&init_common_controls_ex);

		status_bar_handle = CreateWindowEx(
			0,
			STATUSCLASSNAME,
			NULL,
			WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
			0, 0,
			0, 0,
			hWnd,
			(HMENU)1,
			status_bar_instance,
			NULL);

		SendMessageA(status_bar_handle, SB_SETTEXT, 0, (LPARAM)L"Ready");
		SetWindowPos(status_bar_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
	break;
	case WM_SIZE:
	{
		SendMessageA(status_bar_handle, WM_SIZE, 0, 0);
		SetWindowPos(status_bar_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPTION_06:
		{
			PostQuitMessage(0);
		}
		break;
		case ID_EDIT_OPTION_07:
		{
			c_console::clear();
		}
		break;
		case ID_VIEW_OPTION_02:
		{
			ShowWindow(status_bar_handle, IsWindowVisible(status_bar_handle) ? SW_HIDE : SW_SHOW);
		}
		break;
		case ID_VIEW_OPTION_03:
		{
			ShowWindow(g_windows_params.game_window_handle, IsWindowVisible(g_windows_params.game_window_handle) ? SW_HIDE : SW_SHOW);
		}
		break;
		case ID_VIEW_OPTION_07:
		{
			c_console::toggle_window_visibility();
		}
		break;
		case ID_SCENARIOS_OPTION_01:
		{
			g_toggle_game_scripts = true;
		}
		break;
		case ID_SCENARIOS_OPTION_02:
		{
			main_reset_map();
		}
		break;
		case ID_ABOUT_OPTION_1:
		{
			ShellExecute(NULL, TEXT("open"), TEXT("https://github.com/twist84/ManagedDonkey"), NULL, NULL, SW_SHOWNORMAL);
		}
		break;
		}

		switch (LOWORD(wParam))
		{
		case ID_EDIT_OPTION_02:
		case ID_EDIT_OPTION_06:
		{
			UPDATE_MENU_OPTION_CHECKBOX(edit_menu_handle);
		}
		break;
		case ID_VIEW_OPTION_01:
		case ID_VIEW_OPTION_02:
		case ID_VIEW_OPTION_03:
		case ID_VIEW_OPTION_04:
		case ID_VIEW_OPTION_05:
		case ID_VIEW_OPTION_06:
		case ID_VIEW_OPTION_07:
		case ID_VIEW_OPTION_09:
		{
			UPDATE_MENU_OPTION_CHECKBOX(view_menu_handle);
		}
		break;
		}
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	break;
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//return INVOKE(0x0042E6A0, WndProc, hWnd, uMsg, wParam, lParam);

	if (uMsg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	if (uMsg == WM_ACTIVATE)
	{
		WndProc_HandleActivate(WM_ACTIVATE, wParam);
		return 0;
	}

	if (uMsg == WM_SIZE)
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		global_preferences_set_screen_resolution(width, height);
		rasterizer_reset_device();
		return 0;
	}

	if (uMsg == WM_ACTIVATEAPP)
	{
		if (!input_get_mouse_state(_input_type_ui))
			return 0;

		int v4 = strcmp(shell_get_target(), "blam");
		if (v4)
			v4 = v4 < 0 ? -1 : 1;
		if (v4)
		{
			shell_application_pause(wParam == 0);
			return 0;
		}
	}

	if (uMsg == WM_INPUT)
	{
		WndProc_HandleRawMouse(WM_INPUT, wParam, lParam);
		if (!GET_RAWINPUT_CODE_WPARAM(wParam))
			return DefWindowProcA(hWnd, WM_INPUT, wParam, lParam);

		return 0;
	}

	if (IN_RANGE_INCLUSIVE(uMsg, WM_KEYFIRST, WM_KEYLAST))
	{
		input_process_key(uMsg, wParam, lParam);
		return 0;
	}

	if (IN_RANGE_INCLUSIVE(uMsg, WM_MOUSEFIRST, WM_MOUSELAST))
	{
		WndProc_HandleMouse(uMsg, wParam, lParam);
		return 0;
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

BOOL WINAPI control_handler_routine(DWORD CtrlType)
{
	return INVOKE(0x0042E900, control_handler_routine, CtrlType);
}

char* __cdecl shell_get_command_line()
{
	//return INVOKE(0x0042E930, shell_get_command_line);

	return g_windows_params.cmd_line;
}

bool shell_get_command_line_parameter(char* command_line, const char* parameter_name, int32* value, int32 default_value)
{
	if (!command_line)
	{
		return false;
	}

	if (value)
	{
		*value = default_value;
	}

	if (const char* parameter_offset = csstrstr(command_line, parameter_name))
	{
		parameter_offset += strlen_debug(parameter_name) + 1;
		c_static_string<32> parameter = parameter_offset;
		int32 separator = parameter.index_of(" ");
		if (separator != NONE)
		{
			parameter.set_bounded(parameter_offset, separator);
		}

		if (value)
		{
			*value = atol(parameter.get_string());
		}

		return true;
	}

	return false;
}

void __cdecl shell_idle()
{
	//INVOKE(0x0042E940, shell_idle);

	static uns32 quit_timeout = NONE;
	
	MSG message{};
	while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		if (message.message == WM_QUIT)
		{
			main_exit_game();
			quit_timeout = system_milliseconds();
			continue;
		}
		DispatchMessageA(&message);
	}

	if (quit_timeout != NONE && system_milliseconds() > quit_timeout + 2000)
		ExitProcess(0);

	sub_511F20();

	if (!shell_application_is_paused() && !input_get_mouse_state(_input_type_ui))
		Sleep(1);
}

void __cdecl shell_platform_dispose()
{
	//INVOKE(0x0042EA00, shell_platform_dispose);

	SetConsoleCtrlHandler(control_handler_routine, FALSE);
}

bool __cdecl shell_platform_initialize()
{
	//return INVOKE(0x0042EA10, shell_platform_initialize);

	SetConsoleCtrlHandler(control_handler_routine, TRUE);
	SetUnhandledExceptionFilter(unhandled_exception_handler);
	sub_42EA80();

	return true;
}

void __cdecl shell_platform_verify()
{
	INVOKE(0x0042EA60, shell_platform_verify);
}

void __cdecl shell_halt_if_necessary()
{

}

void __cdecl shell_screen_pause(bool pause)
{
	//INVOKE(0x0042EA70, shell_screen_pause, pause);
}

void __cdecl sub_42EA80()
{
	//INVOKE(0x0042EA80, sub_42EA80);

	static CHAR path[4096];
	DWORD path_size = GetEnvironmentVariableA("path", path, sizeof(path));
	if (path_size > 0 && path_size < sizeof(path))
	{
		//event(_event_message, "system: path={ %s }", path);
		printf("system: path={ %s }", path); printf("\n");
	}
	
	CHAR current_directory[MAX_PATH]{};
	if (GetCurrentDirectoryA(MAX_PATH, current_directory))
	{
		//event(_event_message, "system: current directory={ %s }", current_directory);
		printf("system: current directory={ %s }", current_directory); printf("\n");
	}

	ULONG HeapInformation;
	if (HeapQueryInformation(GetProcessHeap(), HeapCompatibilityInformation, &HeapInformation, sizeof(HeapInformation), NULL))
	{
		//event(_event_message, "HeapCompatibilityInformation == %d!", HeapInformation);
		printf("HeapCompatibilityInformation == %d!", HeapInformation); printf("\n");
	}
}

LONG WINAPI unhandled_exception_handler(_EXCEPTION_POINTERS* ExceptionInfo)
{
	return INVOKE(0x0042EAC0, unhandled_exception_handler, ExceptionInfo);

	//if (!is_main_thread())
	//{
	//	cache_exception_information(ExceptionInfo);
	//	PostThreadMessageA(get_main_thread_id(), WM_NULL, 0, 0);
	//
	//	while (true)
	//	{
	//		sleep(1);
	//	}
	//}
	//return generic_exception_filter(ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo);
}

int WINAPI _WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//return INVOKE(0x0042EB10, WinMain, hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	SetLastError(NO_ERROR);
	SetProcessDPIAware();

	g_windows_params.cmd_line = GetCommandLineA(); // lpCmdLine is empty set
	g_windows_params.instance = hInstance;
	g_windows_params.cmd_show = nCmdShow;
	g_windows_params.window_proc = WndProc;
	g_windows_params.window_handle = NULL;

	static bool x_splash_enabled = false;
	if (x_splash_enabled && shell_get_command_line_parameter(g_windows_params.cmd_line, "-nosplash", NULL, 0))
	{
		splash_screen_show(L"donkey_splash.png", 500, 2000, 500);
	}

	c_console::toggle_window_visibility();

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) // if shift is held open the "Choose Rasterizer" dialog
	{
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_CHOOSE_RASTERIZER_DIALOG), NULL, ChooseRasterizerDialogProc);
	}

	if (shell_get_command_line_parameter(g_windows_params.cmd_line, "-haltonstartup", NULL, 0))
	{
		while (!is_debugger_present())
		{
			sleep(1000);
		}
	}

	int32 cache_size_increase = 0;
	if (shell_get_command_line_parameter(g_windows_params.cmd_line, "-cache-memory-increase", &cache_size_increase, cache_size_increase))
	{
		g_physical_memory_cache_size_increase_mb = static_cast<uns32>(cache_size_increase);
	}

	if (shell_get_command_line_parameter(g_windows_params.cmd_line, "-editor", NULL, 0))
	{
		g_windows_params.editor_window_create = true;
		g_windows_params.editor_window_proc = EditorWndProc;

		SetConsoleTitleA("Output window");

		c_static_string<64> editor_name = "Managed Donkey - Editor";
		editor_name.copy_to(g_windows_params.editor_class_name, sizeof(g_windows_params.editor_class_name));
		editor_name.copy_to(g_windows_params.editor_window_name, sizeof(g_windows_params.editor_window_name));

		c_static_string<64> name("Game window");
		name.copy_to(g_windows_params.class_name, sizeof(g_windows_params.class_name));
		name.copy_to(g_windows_params.window_name, sizeof(g_windows_params.window_name));
	}
	else
	{
		c_static_string<64> name = c_static_string<64>().print("HaloOnline %s", sub_5013A0());
		name.copy_to(g_windows_params.class_name, sizeof(g_windows_params.class_name));
		name.copy_to(g_windows_params.window_name, sizeof(g_windows_params.window_name));
	}

	physical_memory_initialize();
	physical_memory_stage_push(_memory_stage_game_initialize);

	system_set_dll_directory();
	system_set_maps_directory();

	static bool shell_initialized = false;
	if (shell_initialize(false))
	{
		main_loop();

		shell_initialized = true;
		shell_dispose();
	}

	if (!shell_initialized)
	{
		shell_dispose();
	}

	physical_memory_dispose();

	return 0;
}

const char* __cdecl sub_5013A0()
{
	return INVOKE(0x005013A0, sub_5013A0);

	//static c_static_string<256> name;
	//name.print("%s_%d", "cert_ms23_release_106708", levels_get_checksum());
	//return name.get_string();
}

bool __cdecl input_process_key(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//return INVOKE(0x00511F40, input_process_key, uMsg, wParam, lParam);

	if (!input_globals.mouse_acquired)
	{
		return false;
	}

	s_key_state key
	{
		.key_type = k_key_type_none,
		.ascii_code = _key_not_a_key,
		.vk_code = uns16(NONE),
		.repeating = false
	};

	int16 key_code = NONE;
	const uns8* key_table = NULL;

	if (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN || uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP)
	{
		ASSERT(wParam >= 0 && wParam < k_number_of_windows_input_virtual_codes);

		key_code = virtual_to_key_table[wParam];
		if (key_code != _key_not_a_key)
		{
			key.key_type = uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN ? _key_type_down : uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP ? _key_type_up : k_key_type_none;
			key.ascii_code = key_code;
			key.vk_code = uns16(NONE);
			key_table = key_to_virtual_table;
		}
	}
	else if (uMsg == WM_CHAR || uMsg == WM_SYSCHAR && wParam < k_number_of_input_ascii_codes)
	{
		key_code = ascii_to_key_table[wParam];
		if (key_code != _key_not_a_key)
		{
			key.key_type = _key_type_char;
			key.ascii_code = key_code;
			key.vk_code = wParam;
			key_table = key_to_ascii_table;
		}
	}

	if (key_code == _key_not_a_key)
	{
		return false;
	}

	SET_BIT(key.modifier_flags, _key_modifier_flag_shift_key_bit, GetKeyState(VK_SHIFT) & 0x8000);
	SET_BIT(key.modifier_flags, _key_modifier_flag_control_key_bit, GetKeyState(VK_CONTROL) & 0x8000);
	SET_BIT(key.modifier_flags, _key_modifier_flag_alt_key_bit, GetKeyState(VK_MENU) & 0x8000);

	if (input_globals.buffered_key_read_count < input_globals.buffered_keys.get_count())
	{
		key.repeating = (HIWORD(lParam) & KF_REPEAT) == KF_REPEAT;
		input_globals.buffered_keys[input_globals.buffered_key_read_count++] = key;
	}

	if (key_table)
	{
		for (int32 key_index = _key_escape; key_index < k_key_code_count; key_index++)
		{
			if (key_table[key_index] == wParam && !input_globals.keys[key_index].latched && !input_globals.keys[key_index].msec)
			{
				input_globals.keys[key_index].frames = 0;
				input_globals.keys[key_index].msec = 1;
			}
		}
	}

	return true;
}

void __cdecl sub_511F20()
{
	INVOKE(0x00511F20, sub_511F20);

	//if (input_globals.raw_input_unknownAB5)
	//{
	//	sub_5114A0();
	//
	//	if (input_globals.raw_input_unknownAB5)
	//		sub_511410();
	//}
}

void __cdecl WndProc_HandleMouse(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	//INVOKE(0x00512160, WndProc_HandleMouse, Msg, wParam, lParam);

	s_mouse_state mouse{};

	mouse.x = LOWORD(lParam);
	mouse.y = HIWORD(lParam);

	switch (Msg)
	{
#define HANDLE_MOUSE_BUTTON(TYPE, BUTTON) \
{ \
	mouse.mouse_type = _mouse_##TYPE; \
	mouse.mouse_button = _mouse_##BUTTON; \
	input_globals.raw_mouse_state.raw_flags.set(mouse.mouse_button, true); \
	break; \
}

	case WM_MOUSEMOVE:
		mouse.mouse_type = _mouse_type_move;
		break;
	case WM_LBUTTONDOWN: HANDLE_MOUSE_BUTTON(type_down, button_left_click);
	case WM_LBUTTONUP:   HANDLE_MOUSE_BUTTON(type_up,   button_left_click);
	case WM_RBUTTONDOWN: HANDLE_MOUSE_BUTTON(type_down, button_right_click);
	case WM_RBUTTONUP:   HANDLE_MOUSE_BUTTON(type_up,   button_right_click);
	case WM_MBUTTONDOWN: HANDLE_MOUSE_BUTTON(type_down, button_middle_click);
	case WM_MBUTTONUP:   HANDLE_MOUSE_BUTTON(type_up,   button_middle_click);
	case WM_MOUSEWHEEL:
		input_globals.raw_mouse_state.wheel_delta += GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.mouse_type = _mouse_type_wheel;
		mouse.wheel_delta = input_globals.raw_mouse_state.wheel_delta;
		break;
	case WM_XBUTTONDOWN:
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) HANDLE_MOUSE_BUTTON(type_down, button_4);
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) HANDLE_MOUSE_BUTTON(type_down, button_5);
		break;
	case WM_XBUTTONUP:
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) HANDLE_MOUSE_BUTTON(type_up, button_4);
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) HANDLE_MOUSE_BUTTON(type_up, button_5);
		break;

#undef HANDLE_MOUSE
	}

	if (input_globals.buffered_mouse_button_read_count < input_globals.buffered_mouse_buttons.get_count())
		input_globals.buffered_mouse_buttons[input_globals.buffered_mouse_button_read_count++] = mouse;
}

void __cdecl WndProc_HandleActivate(UINT uMsg, WPARAM wParam)
{
	INVOKE(0x00512320, WndProc_HandleActivate, uMsg, wParam);

	//if (input_globals.raw_input_mouse_state_update)
	//{
	//	if (TEST_MASK(wParam, 0xF))
	//	{
	//		acquire_mouse();
	//		sub_5129B0();
	//	}
	//	else
	//	{
	//		unacquire_mouse();
	//	}
	//}
}

void __cdecl WndProc_HandleRawMouse(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//INVOKE(0x00512350, WndProc_HandleRawMouse, uMsg, wParam, lParam);

	if (input_globals.raw_input_mouse_state_update)
	{
		RAWINPUT raw_buf = {};
		UINT cb_size = sizeof(RAWINPUT);
		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw_buf, &cb_size, sizeof(RAWINPUTHEADER)) == sizeof(RAWINPUT) && raw_buf.header.dwType == RIM_TYPEMOUSE)
		{
			input_globals.raw_mouse_state.relative_x += raw_buf.data.mouse.lLastX;
			input_globals.raw_mouse_state.relative_y += raw_buf.data.mouse.lLastY;
			input_globals.raw_mouse_state.wheel_delta += TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_WHEEL) ? raw_buf.data.mouse.usButtonData : 0;
			input_globals.raw_mouse_state.hwheel_delta += TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_HWHEEL) ? raw_buf.data.mouse.usButtonData : 0;

#define HANDLE_RAW_MOUSE_BUTTON(RAW_BUTTON, BUTTON) \
{ \
	if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_##RAW_BUTTON##_DOWN)) \
	input_globals.raw_mouse_state.raw_flags.set(_mouse_##BUTTON, true); \
	else if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_##RAW_BUTTON##_UP)) \
	input_globals.raw_mouse_state.raw_flags.set(_mouse_##BUTTON, false); \
}
			HANDLE_RAW_MOUSE_BUTTON(LEFT_BUTTON,   button_left_click);
			HANDLE_RAW_MOUSE_BUTTON(RIGHT_BUTTON,  button_right_click);
			HANDLE_RAW_MOUSE_BUTTON(MIDDLE_BUTTON, button_middle_click);
			HANDLE_RAW_MOUSE_BUTTON(BUTTON_4,      button_4);
			HANDLE_RAW_MOUSE_BUTTON(BUTTON_5,      button_5);

#undef HANDLE_RAW_MOUSE_BUTTON
		}
	}
}

bool __cdecl shell_get_system_identifier(char* system_identifier, int32 system_identifier_len)
{
	if (fake_system_identifier)
		return false;

	bool result = false;
	HOOK_INVOKE(result =, shell_get_system_identifier, system_identifier, system_identifier_len);
	return result;
}

INT_PTR CALLBACK ChooseRasterizerDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		// Initialization code if needed
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hwndDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}

	return FALSE;
}


