#pragma once

#include "cseries/cseries.hpp"

#include <windows.h>

struct window_data
{
	HINSTANCE hInstance;
	HWND hWnd;
	HWND hWndPresentTarget;
	int nShowCmd;
	WNDPROC lpfnWndProc;
	CHAR className[64];
	CHAR windowTitle[64];
	LPSTR lpCmdLine;

	static bool editorWindowCreate;
	static HWND hWndEditor;
	static WNDPROC lpfnWndProcEditor;
	static CHAR classNameEditor[64];
	static CHAR windowTitleEditor[64];
};
static_assert(sizeof(window_data));

extern window_data& window_globals;

extern LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern BOOL WINAPI control_handler_routine(DWORD CtrlType);
extern char* __cdecl shell_get_command_line();
extern void __cdecl shell_idle();
extern void __cdecl shell_platform_dispose();
extern bool __cdecl shell_platform_initialize();
extern void __cdecl shell_platform_verify();
extern void __cdecl shell_halt_if_necessary();
extern void __cdecl shell_screen_pause(bool pause);
extern void __cdecl sub_42EA80();
extern LONG WINAPI unhandled_exception_handler(_EXCEPTION_POINTERS* ExceptionInfo);
extern int WINAPI _WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
extern const char* __cdecl sub_5013A0();
extern bool __cdecl input_process_key(UINT uMsg, WPARAM wParam, LPARAM lParam);
extern void __cdecl sub_511F20();
extern void __cdecl WndProc_HandleMouse(UINT Msg, WPARAM wParam, LPARAM lParam);
extern void __cdecl WndProc_HandleActivate(UINT uMsg, WPARAM wParam);
extern void __cdecl WndProc_HandleRawMouse(UINT uMsg, WPARAM wParam, LPARAM lParam);
extern bool __cdecl shell_get_system_identifier(char* system_identifier, int32 system_identifier_len);

#define IDD_CHOOSE_RASTERIZER_DIALOG 101
#define ID_CHOOSE_RASTERIZER_BUTTON_OK IDOK
#define ID_CHOOSE_RASTERIZER_BUTTON_CANCEL IDCANCEL
#define ID_CHOOSE_RASTERIZER_LISTBOX_RENDERER 1001
#define ID_CHOOSE_RASTERIZER_LISTBOX_SELECTED_DEVICE_CHARACTERISTICS 1002
#define ID_CHOOSE_RASTERIZER_LISTBOX_SELECTED_DEVICE_DISPLAY_MODES 1003
#define ID_CHOOSE_RASTERIZER_LISTBOX_DEVICE 1004
#define ID_CHOOSE_RASTERIZER_BUTTON_RUN_WINDOWED 1005
#define ID_CHOOSE_RASTERIZER_BUTTON_DO_NOT_SHOW_THIS_AGAIN 1007
#define ID_CHOOSE_RASTERIZER_BUTTON_DISABLE_MULTITEXTURING_D3D_ONLY 1008
#define ID_CHOOSE_RASTERIZER_BUTTON_TEXTURE_QUALITY 1013
#define ID_CHOOSE_RASTERIZER_BUTTON_HIGH_RES_32BIT 1014
#define ID_CHOOSE_RASTERIZER_BUTTON_LOW_RES_32BIT 1015
#define ID_CHOOSE_RASTERIZER_BUTTON_HIGH_RES_16BIT 1016
#define ID_CHOOSE_RASTERIZER_BUTTON_LOW_RES_16BIT 1017

extern INT_PTR CALLBACK ChooseRasterizerDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

