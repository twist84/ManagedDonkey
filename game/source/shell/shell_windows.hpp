#pragma once

#include "cseries/cseries.hpp"

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

extern s_windows_params& g_windows_params;

extern LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern BOOL WINAPI HandlerRoutine(DWORD CtrlType);
extern char* __cdecl shell_get_command_line();
extern void __cdecl shell_idle();
extern void __cdecl shell_platform_dispose();
extern bool __cdecl shell_platform_initialize();
extern void __cdecl shell_platform_verify();
extern void __cdecl shell_screen_pause(bool pause);
extern void __cdecl sub_42EA80();
extern LONG WINAPI TopLevelExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo);
extern int WINAPI _WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
extern char const* __cdecl sub_5013A0();
extern bool __cdecl WndProc_HandleKeys(UINT uMsg, WPARAM wParam, LPARAM lParam);
extern void __cdecl WndProc_HandleMouse(UINT Msg, WPARAM wParam, LPARAM lParam);
extern void __cdecl WndProc_HandleActivate(UINT uMsg, WPARAM wParam);
extern void __cdecl WndProc_HandleRawMouse(UINT uMsg, WPARAM wParam, LPARAM lParam);
extern bool __cdecl shell_get_system_identifier(char* system_identifier, long system_identifier_len);

extern INT_PTR CALLBACK ChooseRasterizerDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

