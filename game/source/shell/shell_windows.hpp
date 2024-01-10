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
extern void __cdecl UnregisterHotKey_Snapshot();
extern void __cdecl RegisterHotKey_Snapshot();
extern int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
extern char const* __cdecl sub_5013A0();
extern bool __cdecl WndProc_HandleKeys(UINT uMsg, WPARAM wParam, LPARAM lParam);
extern void __cdecl WndProc_HandleMouse(UINT Msg, WPARAM wParam, LPARAM lParam);
extern void __cdecl WndProc_HandleActivate(UINT uMsg, WPARAM wParam);
extern void __cdecl WndProc_HandleRawMouse(UINT uMsg, WPARAM wParam, LPARAM lParam);

