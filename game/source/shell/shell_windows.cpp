#include "cseries/cseries.hpp"

#include "cache/physical_memory_map.hpp"
#include "input/input.hpp"
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

// forward declare these here to avoid putting `#include <Windows.h>` in the header
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void __cdecl UnregisterHotKey_Snapshot();
void __cdecl RegisterHotKey_Snapshot();
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
char const* __cdecl sub_5013A0();
bool __cdecl WndProc_HandleKeys(UINT uMsg, WPARAM wParam, LPARAM lParam);
void __cdecl WndProc_HandleMouse(UINT Msg, WPARAM wParam, LPARAM lParam);
void __cdecl WndProc_HandleActivate(UINT uMsg, WPARAM wParam);
void __cdecl WndProc_HandleRawMouse(UINT uMsg, WPARAM wParam, LPARAM lParam);
void __cdecl SetBinDllDirectory();

//HOOK_DECLARE(0x0042E6A0, WndProc);
//HOOK_DECLARE(0x0042E850, UnregisterHotKey_Snapshot);
//HOOK_DECLARE(0x0042E870, RegisterHotKey_Snapshot);
HOOK_DECLARE(0x0042EB10, WinMain);
//HOOK_DECLARE(0x005013A0, sub_5013A0);
//HOOK_DECLARE(0x00511F40, WndProc_HandleKeys);
//HOOK_DECLARE(0x00512160, WndProc_HandleMouse);
//HOOK_DECLARE(0x00512320, WndProc_HandleActivate);
//HOOK_DECLARE(0x00512350, WndProc_HandleRawMouse);
//HOOK_DECLARE(0x0051CF30, SetBinDllDirectory);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//return INVOKE(0x0042E6A0, WndProc, hWnd, uMsg, wParam, lParam);

	if (uMsg <= WM_SYSCHAR)
	{
		if (uMsg < WM_SYSKEYDOWN)
		{
			if (uMsg <= WM_MOUSEACTIVATE)
			{
				switch (uMsg)
				{
				case WM_MOUSEACTIVATE:
					return 2;
				case WM_DESTROY:
					PostQuitMessage(0);
					return 0;
				case WM_ACTIVATE:
					WndProc_HandleActivate(WM_ACTIVATE, wParam);
					return 0;
				case WM_ACTIVATEAPP:
					if (input_get_mouse_state(_input_type_ui))
					{
						int v4 = strcmp(shell_get_target(), "blam");

						if (v4)
							v4 = v4 < 0 ? -1 : 1;

						if (v4)
						{
							shell_application_pause(wParam == 0);
							return 0;
						}
					}

					return 0;
				}

				return DefWindowProcA(hWnd, uMsg, wParam, lParam);
			}

			if (uMsg == WM_INPUT)
			{
				WndProc_HandleRawMouse(WM_INPUT, wParam, lParam);

				if (!LOBYTE(wParam))
					return DefWindowProcA(hWnd, WM_INPUT, wParam, lParam);

				return 0;
			}

			if (uMsg <= WM_INPUT || uMsg > WM_CHAR)
				return DefWindowProcA(hWnd, uMsg, wParam, lParam);
		}

		WndProc_HandleKeys(uMsg, wParam, lParam);
		return 0;
	}

	if (uMsg > WM_MBUTTONUP)
	{
		if (uMsg < WM_MOUSEWHEEL)
			return DefWindowProcA(hWnd, uMsg, wParam, lParam);

		if (uMsg > WM_XBUTTONUP)
		{
			if (uMsg != WM_HOTKEY)
				return DefWindowProcA(hWnd, uMsg, wParam, lParam);

			WndProc_HandleKeys(uMsg, wParam, lParam);
			return 0;
		}
	}
	else if (uMsg < WM_MBUTTONDOWN)
	{
		switch (uMsg)
		{
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			break;
		default:
			return DefWindowProcA(hWnd, uMsg, wParam, lParam);
		}
	}

	WndProc_HandleMouse(uMsg, wParam, lParam);
	return 0;
}

void __cdecl UnregisterHotKey_Snapshot()
{
	//INVOKE(0x0042E850, UnregisterHotKey_Snapshot);

	if (shell_application_type() == _shell_application_type_client)
		UnregisterHotKey(g_windows_params.created_window_handle, VK_SNAPSHOT);
}

void __cdecl RegisterHotKey_Snapshot()
{
	//INVOKE(0x0042E870, RegisterHotKey_Snapshot);

	if (shell_application_type() == _shell_application_type_client)
		RegisterHotKey(g_windows_params.created_window_handle, 0x2C /* 0x2C same as VK_SNAPSHOT */, 0, VK_SNAPSHOT);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//return INVOKE(0x0042EB10, WinMain, hInstance, hPrevInstance, lpCmdLine, nCmdShow);

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

	s_key_state key{};
	byte const* key_table = nullptr;

	key.key_type = k_key_type_none;
	key.vk_code = NONE;
	key.key_code = k_key_code_none;
	key.was_key_down = false;

	if (uMsg != WM_KEYDOWN && uMsg != WM_KEYUP && uMsg != WM_SYSKEYDOWN && uMsg != WM_SYSKEYUP && uMsg != WM_HOTKEY)
	{
		if (uMsg == WM_CHAR || uMsg == WM_SYSCHAR)
		{
			key.vk_code = wParam;
			key_table = key_to_ascii_table;
			key.key_code = ascii_to_key_table[wParam];
		}
	}
	else if (virtual_to_key_table[wParam] != NONE)
	{
		key.vk_code = NONE;
		key_table = key_to_virtual_table;
		key.key_code = virtual_to_key_table[wParam];
	}

	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		key.key_type = _key_type_down;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		key.key_type = _key_type_up;
		break;
	case WM_CHAR:
	case WM_SYSCHAR:
		key.key_type = _key_type_char;
		break;
	}

	if (key.key_type == _key_type_down || key.key_type == _key_type_char)
		key.was_key_down = TEST_MASK(lParam, 0x40000000);

	if (key.key_code != k_key_code_none)
		return false;

	if (uMsg == WM_HOTKEY)
	{
		key.modifier.set(_key_modifier_flag_shift_key_bit, (lParam & MOD_SHIFT) == MOD_SHIFT);
		key.modifier.set(_key_modifier_flag_control_key_bit, (lParam & MOD_CONTROL) == MOD_CONTROL);
		key.modifier.set(_key_modifier_flag_alt_key_bit, (lParam & MOD_ALT) == MOD_ALT);
	}
	else
	{
		key.modifier.set(_key_modifier_flag_shift_key_bit, GetKeyState(VK_SHIFT) & 0x8000);
		key.modifier.set(_key_modifier_flag_control_key_bit, GetKeyState(VK_CONTROL) & 0x8000);
		key.modifier.set(_key_modifier_flag_alt_key_bit, GetKeyState(VK_MENU) & 0x8000);
	}

	if (input_globals.buffered_key_read_count < input_globals.buffered_keys.get_count())
		input_globals.buffered_keys[input_globals.buffered_key_read_count++] = key;

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
		input_globals.raw_mouse_state.raw_flags.set(_mouse_button_1, true);
		mouse.mouse_type = _mouse_type_down;
		mouse.mouse_button = _mouse_button_1;
		break;
	case WM_LBUTTONUP:
		input_globals.raw_mouse_state.raw_flags.set(_mouse_button_1, false);
		mouse.mouse_type = _mouse_type_up;
		mouse.mouse_button = _mouse_button_1;
		break;

	case WM_RBUTTONDOWN:
		input_globals.raw_mouse_state.raw_flags.set(_mouse_button_2, true);
		mouse.mouse_type = _mouse_type_down;
		mouse.mouse_button = _mouse_button_2;
		break;
	case WM_RBUTTONUP:
		input_globals.raw_mouse_state.raw_flags.set(_mouse_button_2, false);
		mouse.mouse_type = _mouse_type_up;
		mouse.mouse_button = _mouse_button_2;
		break;

	case WM_MBUTTONDOWN:
		input_globals.raw_mouse_state.raw_flags.set(_mouse_button_3, true);
		mouse.mouse_type = _mouse_type_down;
		mouse.mouse_button = _mouse_button_3;
		break;
	case WM_MBUTTONUP:
		input_globals.raw_mouse_state.raw_flags.set(_mouse_button_3, false);
		mouse.mouse_type = _mouse_type_up;
		mouse.mouse_button = _mouse_button_3;
		break;

	case WM_MOUSEWHEEL:
		input_globals.raw_mouse_state.wheel_delta += HIWORD(wParam);
		mouse.mouse_type = _mouse_type_wheel;
		mouse.wheel_delta = HIWORD(wParam);
		break;

	case WM_XBUTTONDOWN:
		if (HIWORD(wParam) == 1)
		{
			input_globals.raw_mouse_state.raw_flags.set(_mouse_button_4, true);
			mouse.mouse_type = _mouse_type_down;
			mouse.mouse_button = _mouse_button_4;
		}
		else if (HIWORD(wParam) == 2)
		{
			input_globals.raw_mouse_state.raw_flags.set(_mouse_button_5, true);
			mouse.mouse_type = _mouse_type_down;
			mouse.mouse_button = _mouse_button_5;
		}
		break;
	case WM_XBUTTONUP:
		if (HIWORD(wParam) == 1)
		{
			input_globals.raw_mouse_state.raw_flags.set(_mouse_button_4, false);
			mouse.mouse_type = _mouse_type_up;
			mouse.mouse_button = _mouse_button_4;
		}
		else if (HIWORD(wParam) == 2)
		{
			input_globals.raw_mouse_state.raw_flags.set(_mouse_button_5, false);
			mouse.mouse_type = _mouse_type_up;
			mouse.mouse_button = _mouse_button_5;
		}
		break;
	default:
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

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_1_UP))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_1, false);

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_2_DOWN))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_2, true);

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_2_UP))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_2, false);

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_3_DOWN))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_3, true);

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_3_UP))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_3, false);

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_4_DOWN))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_4, true);

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_4_UP))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_4, false);

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_5_DOWN))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_5, true);

			if (TEST_MASK(raw_buf.data.mouse.usButtonFlags, RI_MOUSE_BUTTON_5_UP))
				input_globals.raw_mouse_state.raw_flags.set(_mouse_button_5, false);
		}
	}
}

void __cdecl SetBinDllDirectory()
{
	//INVOKE(0x0051CF30, SetBinDllDirectory);

	char dll_directory[1024]{};
	csstrnzcat(dll_directory, ".\\bin", 1024);
	SetDllDirectoryA(dll_directory);

	//generate_event(_event_level_message, "system: dll directory={ %s }", dll_directory);
	printf("system: dll directory={ %s }\n", dll_directory);
}

