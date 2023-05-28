#include "xbox/xbox.hpp"

#include "cseries/cseries.hpp"
#include "main/main.hpp"
#include "resource.h"

#include <windows.h>

REFERENCE_DECLARE(0x0199C014, HWND, g_game_window_handle);

bool __cdecl get_clipboard_as_text(char* buf, long len)
{
	if (!IsClipboardFormatAvailable(1) || !OpenClipboard(g_game_window_handle))
		return false;

	bool result = false;

	HANDLE clipboard_data = GetClipboardData(1u);
	if (clipboard_data)
	{
		const char* clipboard_text = (const char*)GlobalLock(clipboard_data);
		if (clipboard_text)
		{
			result = true;
			csstrnzcpy(buf, clipboard_text, len);
			GlobalUnlock(clipboard_data);
		}
	}

	CloseClipboard();

	return result;
}

struct XShowKeyboardUI_struct
{
	long& controller_index;
	unsigned long& character_flags;
	wchar_t const*& default_text;
	wchar_t const*& title_text;
	wchar_t const*& description_text;
	wchar_t*& result_text;
	unsigned long& maximum_character_count;
	void*& platform_handle;

	void dialog_initialize_handler(HWND dialog_handle)
	{
		SetWindowText(dialog_handle, title_text);
		SetWindowText(GetDlgItem(dialog_handle, ID_TEXT_BOX_TEXT), default_text);
		SetFocus(GetDlgItem(dialog_handle, ID_TEXT_BOX_TEXT));
	}

	void dialog_command_handler(HWND dialog_handle, long command)
	{
		switch (command)
		{
		case IDOK:
			SendMessage(GetDlgItem(dialog_handle, ID_TEXT_BOX_TEXT), WM_GETTEXT, (WPARAM)maximum_character_count, (LPARAM)result_text);
			EndDialog(dialog_handle, IDOK);
			break;
		case IDCANCEL:
			EndDialog(dialog_handle, IDCANCEL);
			break;
		}
	}
};

INT_PTR CALLBACK XShowKeyboardUI_proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static XShowKeyboardUI_struct* params = nullptr;

#define safe_params(_params) if ((_params)) (_params)
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		params = reinterpret_cast<XShowKeyboardUI_struct*>(lParam);
		safe_params(params)->dialog_initialize_handler(hDlg);

		return true;
	}
	case WM_COMMAND:
		safe_params(params)->dialog_command_handler(hDlg, LOWORD(wParam));
		break;
	default:
		return false;
	}
#undef safe_params

	return true;
}

long XShowKeyboardUI(
	long controller_index,
	unsigned long character_flags,
	wchar_t const* default_text,
	wchar_t const* title_text,
	wchar_t const* description_text,
	wchar_t* result_text,
	unsigned long maximum_character_count,
	void* platform_handle
)
{
	XShowKeyboardUI_struct params
	{
		controller_index,
		character_flags,
		default_text,
		title_text,
		description_text,
		result_text,
		maximum_character_count,
		platform_handle
	};
	DialogBoxParam((HINSTANCE)platform_handle, MAKEINTRESOURCE(IDD_TEXT_BOX_DIALOG), g_game_window_handle, &XShowKeyboardUI_proc, (LPARAM)&params);

	return 0;
}

