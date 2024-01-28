#include "xbox/xbox.hpp"

#include "cseries/cseries.hpp"
#include "main/main.hpp"
#include "shell/shell_windows.hpp"
#include "resource.h"

#include <windows.h>

bool __cdecl get_clipboard_as_text(char* buf, long len)
{
	if (!IsClipboardFormatAvailable(CF_TEXT) || !OpenClipboard(g_windows_params.created_window_handle))
		return false;

	bool result = false;

	HANDLE clipboard_data = GetClipboardData(CF_TEXT);
	if (!clipboard_data)
	{
		CloseClipboard();
		return false;
	}

	if (char const* clipboard_text = (char const*)GlobalLock(clipboard_data))
	{
		csstrnzcpy(buf, clipboard_text, len);
		GlobalUnlock(clipboard_data);
	}

	CloseClipboard();
	return true;
}

bool __cdecl set_clipboard_as_text(char* buf, long len)
{
	if (!OpenClipboard(g_windows_params.created_window_handle))
		return false;

	EmptyClipboard();

	HANDLE clipboard_data = GlobalAlloc(GMEM_MOVEABLE, len);
	if (!clipboard_data)
	{
		CloseClipboard();
		return false;
	}

	if (char* clipboard_text = (char*)GlobalLock(clipboard_data))
	{
		csmemcpy(clipboard_text, buf, len);
		clipboard_text[len - 1] = 0;

		GlobalUnlock(clipboard_data);
		SetClipboardData(CF_TEXT, clipboard_data);
	}

	CloseClipboard();
	return true;
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
	DialogBoxParam((HINSTANCE)platform_handle, MAKEINTRESOURCE(IDD_TEXT_BOX_DIALOG), g_windows_params.created_window_handle, &XShowKeyboardUI_proc, (LPARAM)&params);

	return 0;
}

// #TODO: add an icon for notifications, more customization?
void display_notification(wchar_t const* tip, wchar_t const* info, wchar_t const* info_title)
{
	ASSERT(tip != NULL);
	ASSERT(info != NULL);
	ASSERT(info_title != NULL);

	NOTIFYICONDATAW notify_icon_data = { sizeof(NOTIFYICONDATAW) };

	notify_icon_data.hWnd = g_windows_params.created_window_handle;

	// Unique ID for the notification icon
	notify_icon_data.uID = 1;
	notify_icon_data.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;

	// Load an icon
	notify_icon_data.hIcon = LoadIcon(NULL, IDI_INFORMATION);
	wcscpy_s(notify_icon_data.szTip, tip);
	notify_icon_data.uCallbackMessage = WM_USER;


	// Add the notification icon
	Shell_NotifyIcon(NIM_ADD, &notify_icon_data);


	// Show a notification
	notify_icon_data.uFlags = NIF_INFO;
	notify_icon_data.dwInfoFlags = NIIF_INFO;
	wcscpy_s(notify_icon_data.szInfo, info);
	wcscpy_s(notify_icon_data.szInfoTitle, info_title);
	Shell_NotifyIcon(NIM_MODIFY, &notify_icon_data);

	Sleep(25);

	// Remove the notification icon
	Shell_NotifyIcon(NIM_DELETE, &notify_icon_data);
}

