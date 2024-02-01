#include "shell/shell_windows.hpp"

#include "cache/physical_memory_map.hpp"
#include "input/input_windows.hpp"
#include "main/main.hpp"
#include "memory/module.hpp"
#include "shell/shell.hpp"

REFERENCE_DECLARE(0x0199C010, s_windows_params, g_windows_params);

HOOK_DECLARE(0x0042E940, shell_idle);
HOOK_DECLARE(0x0042EB10, _WinMain);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//return INVOKE(0x0042E6A0, WndProc, hWnd, uMsg, wParam, lParam);

	if (uMsg == WM_MOUSEACTIVATE)
		return 2;

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
		WndProc_HandleKeys(uMsg, wParam, lParam);
		return 0;
	}

	if (IN_RANGE_INCLUSIVE(uMsg, WM_MOUSEFIRST, WM_MOUSELAST))
	{
		WndProc_HandleMouse(uMsg, wParam, lParam);
		return 0;
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

BOOL WINAPI HandlerRoutine(DWORD CtrlType)
{
	return INVOKE(0x0042E900, HandlerRoutine, CtrlType);
}

char* __cdecl shell_get_command_line()
{
	//return INVOKE(0x0042E930, shell_get_command_line);

	return g_windows_params.cmd_line;
}

void __cdecl shell_idle()
{
	//INVOKE(0x0042E940, shell_idle);

	static dword quit_timeout = NONE;
	
	MSG message{};
	while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
	{
		if (message.message == WM_QUIT)
		{
			main_exit_game();
			quit_timeout = system_milliseconds();
		}
		else
		{
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
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

	SetConsoleCtrlHandler(HandlerRoutine, FALSE);
}

bool __cdecl shell_platform_initialize()
{
	//return INVOKE(0x0042EA10, shell_platform_initialize);

	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	SetUnhandledExceptionFilter(TopLevelExceptionFilter);
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
		//generate_event(_event_level_message, "system: path={ %s }", path);
		printf("system: path={ %s }", path); printf("\n");
	}
	
	CHAR current_directory[MAX_PATH]{};
	if (GetCurrentDirectoryA(MAX_PATH, current_directory))
	{
		//generate_event(_event_level_message, "system: current directory={ %s }", current_directory);
		printf("system: current directory={ %s }", current_directory); printf("\n");
	}

	ULONG HeapInformation;
	if (HeapQueryInformation(GetProcessHeap(), HeapCompatibilityInformation, &HeapInformation, sizeof(HeapInformation), NULL))
	{
		//generate_event(_event_level_message, "HeapCompatibilityInformation == %d!", HeapInformation);
		printf("HeapCompatibilityInformation == %d!", HeapInformation); printf("\n");
	}
}

LONG WINAPI TopLevelExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
	return INVOKE(0x0042EAC0, TopLevelExceptionFilter, ExceptionInfo);

	//if (!is_main_thread())
	//{
	//	cache_exception_information(ExceptionInfo);
	//	PostThreadMessageA(get_main_thread_id(), WM_NULL, 0, 0);
	//
	//	while (true)
	//		Sleep(1);
	//}
	//return generic_exception_filter(ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo);
}

int WINAPI _WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//return INVOKE(0x0042EB10, WinMain, hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) // if shift is held open the "Choose Rasterizer" dialog
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_CHOOSE_RASTERIZER_DIALOG), NULL, ChooseRasterizerDialogProc);

	lpCmdLine = GetCommandLineA();

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

	system_set_dll_directory();

	static bool shell_initialized = false;
	if (shell_initialize(false))
	{
		main_loop();

		shell_initialized = true;
		shell_dispose();
	}

	if (!shell_initialized)
		shell_dispose();

	physical_memory_dispose();

	return 0;
}

char const* __cdecl sub_5013A0()
{
	return INVOKE(0x005013A0, sub_5013A0);

	//static c_static_string<256> name;
	//name.print("HaloOnline %s_%d", "cert_ms23_release_106708", levels_get_checksum());
	//return name.get_string();
}

bool __cdecl WndProc_HandleKeys(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//return INVOKE(0x00511F40, WndProc_HandleKeys, uMsg, wParam, lParam);

	if (!input_globals.mouse_acquired)
		return false;

	s_key_state key
	{
		.key_type = k_key_type_none,
		.key_code = k_key_code_none,
		.vk_code = word(NONE),
		.was_key_down = false
	};

	short key_code = NONE;
	byte const* key_table = NULL;

	if (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN || uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP)
	{
		ASSERT(wParam >= 0 && wParam < k_number_of_windows_input_virtual_codes);

		key_code = virtual_to_key_table[wParam];
		if (key_code != k_key_code_none)
		{
			key.key_type = uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN ? _key_type_down : uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP ? _key_type_up : k_key_type_none;
			key.key_code = key_code;
			key.vk_code = word(NONE);
			key_table = key_to_virtual_table;
		}
	}
	else if (uMsg == WM_CHAR || uMsg == WM_SYSCHAR && wParam < k_number_of_input_ascii_codes)
	{
		key_code = ascii_to_key_table[wParam];
		if (key_code != k_key_code_none)
		{
			key.key_type = _key_type_char;
			key.key_code = key_code;
			key.vk_code = wParam;
			key_table = key_to_ascii_table;
		}
	}

	if (key_code == k_key_code_none)
		return false;

	key.modifier.set(_key_modifier_flag_shift_key_bit, GetKeyState(VK_SHIFT) & 0x8000);
	key.modifier.set(_key_modifier_flag_control_key_bit, GetKeyState(VK_CONTROL) & 0x8000);
	key.modifier.set(_key_modifier_flag_alt_key_bit, GetKeyState(VK_MENU) & 0x8000);

	if (input_globals.buffered_key_read_count < input_globals.buffered_keys.get_count())
	{
		key.was_key_down = (HIWORD(lParam) & KF_REPEAT) == KF_REPEAT;
		input_globals.buffered_keys[input_globals.buffered_key_read_count++] = key;
	}

	if (key_table)
	{
		for (long key_index = _key_code_escape; key_index < k_key_code_count; key_index++)
		{
			if (key_table[key_index] == wParam && !input_globals.keys[key_index].__unknown3 && !input_globals.keys[key_index].msec_down)
			{
				input_globals.keys[key_index].frames_down = 0;
				input_globals.keys[key_index].msec_down = 1;
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
	case WM_MOUSEMOVE:
		mouse.mouse_type = _mouse_type_move;
		break;
	case WM_LBUTTONDOWN:
		mouse.mouse_type = _mouse_type_down;
		mouse.mouse_button = _mouse_button_1;
		input_globals.raw_mouse_state.raw_flags.set(mouse.mouse_button, true);
		break;
	case WM_LBUTTONUP:
		mouse.mouse_type = _mouse_type_up;
		mouse.mouse_button = _mouse_button_1;
		input_globals.raw_mouse_state.raw_flags.set(mouse.mouse_button, false);
		break;
	case WM_RBUTTONDOWN:
		mouse.mouse_type = _mouse_type_down;
		mouse.mouse_button = _mouse_button_3;
		input_globals.raw_mouse_state.raw_flags.set(mouse.mouse_button, true);
		break;
	case WM_RBUTTONUP:
		mouse.mouse_type = _mouse_type_up;
		mouse.mouse_button = _mouse_button_3;
		input_globals.raw_mouse_state.raw_flags.set(mouse.mouse_button, false);
		break;
	case WM_MBUTTONDOWN:
		mouse.mouse_type = _mouse_type_down;
		mouse.mouse_button = _mouse_button_2;
		input_globals.raw_mouse_state.raw_flags.set(mouse.mouse_button, true);
		break;
	case WM_MBUTTONUP:
		mouse.mouse_type = _mouse_type_up;
		mouse.mouse_button = _mouse_button_2;
		input_globals.raw_mouse_state.raw_flags.set(mouse.mouse_button, false);
		break;
	case WM_MOUSEWHEEL:
		mouse.mouse_type = _mouse_type_wheel;
		mouse.wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam);
		input_globals.raw_mouse_state.wheel_delta += mouse.wheel_delta;
		break;
	case WM_XBUTTONDOWN:
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
		{
			mouse.mouse_type = _mouse_type_down;
			mouse.mouse_button = _mouse_button_4;
			input_globals.raw_mouse_state.raw_flags.set(mouse.mouse_button, true);
		}
		else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
		{
			mouse.mouse_type = _mouse_type_down;
			mouse.mouse_button = _mouse_button_5;
			input_globals.raw_mouse_state.raw_flags.set(mouse.mouse_button, true);
		}
		break;
	case WM_XBUTTONUP:
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
		{
			mouse.mouse_type = _mouse_type_up;
			mouse.mouse_button = _mouse_button_4;
			input_globals.raw_mouse_state.raw_flags.set(mouse.mouse_button, false);
		}
		else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
		{
			mouse.mouse_type = _mouse_type_up;
			mouse.mouse_button = _mouse_button_5;
			input_globals.raw_mouse_state.raw_flags.set(mouse.mouse_button, false);
		}
		break;
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

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_1_DOWN))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_1, true);
			else if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_1_UP))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_1, false);

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_2_DOWN))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_2, true);
			else if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_2_UP))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_2, false);

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_3_DOWN))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_3, true);
			else if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_3_UP))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_3, false);

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_4_DOWN))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_4, true);
			else if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_4_UP))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_4, false);

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_5_DOWN))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_5, true);
			else if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_5_UP))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_5, false);
		}
	}
}

bool __cdecl shell_get_system_identifier(char* system_identifier, long system_identifier_len)
{
	return INVOKE(0x0051CE40, shell_get_system_identifier, system_identifier, system_identifier_len);
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


