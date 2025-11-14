#include "xbox/xbox.hpp"

#include "cseries/cseries.hpp"
#include "main/main.hpp"
#include "shell/shell_windows.hpp"
#include "resource.h"

// ==============================================================================================

// from `https://github.com/pnill/cartographer/blob/f280be10d04975fd48dda115c2eab3a239ffca4d/xlive/xliveless.cpp#L69-L111`

DWORD WINAPI XGetOverlappedExtendedError(PXOVERLAPPED pOverlapped)
{
	if (pOverlapped == NULL)
		return GetLastError();

	if (pOverlapped->InternalLow == ERROR_IO_PENDING)
		return ERROR_IO_INCOMPLETE;

	return pOverlapped->dwExtendedError;
}

DWORD WINAPI XGetOverlappedResult(PXOVERLAPPED pOverlapped, LPDWORD pResult, BOOL bWait)
{
	if (bWait)
	{
		while (pOverlapped->InternalLow == ERROR_IO_INCOMPLETE)
			Sleep(1);
	}

	if (pResult)
		*pResult = pOverlapped->InternalHigh;

	return pOverlapped->InternalLow;
}

// ==============================================================================================

bool __cdecl get_clipboard_as_text(char* buf, int32 len)
{
	if (!IsClipboardFormatAvailable(CF_TEXT) || !OpenClipboard(window_globals.hWnd))
		return false;

	bool result = false;

	HANDLE clipboard_data = GetClipboardData(CF_TEXT);
	if (!clipboard_data)
	{
		CloseClipboard();
		return false;
	}

	if (const char* clipboard_text = (const char*)GlobalLock(clipboard_data))
	{
		csstrnzcpy(buf, clipboard_text, len);
		GlobalUnlock(clipboard_data);
	}

	CloseClipboard();
	return true;
}

bool __cdecl set_clipboard_as_text(char* buf, int32 len)
{
	if (!OpenClipboard(window_globals.hWnd))
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
	int32& controller_index;
	uns32& character_flags;
	const wchar_t*& default_text;
	const wchar_t*& title_text;
	const wchar_t*& description_text;
	wchar_t*& result_text;
	uns32& maximum_character_count;
	void*& platform_handle;

	void dialog_initialize_handler(HWND dialog_handle)
	{
		SetWindowText(dialog_handle, title_text);
		SetWindowText(GetDlgItem(dialog_handle, ID_TEXT_BOX_TEXT), default_text);
		SetFocus(GetDlgItem(dialog_handle, ID_TEXT_BOX_TEXT));
	}

	void dialog_command_handler(HWND dialog_handle, int32 command)
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

int32 XShowKeyboardUI(
	int32 controller_index,
	uns32 character_flags,
	const wchar_t* default_text,
	const wchar_t* title_text,
	const wchar_t* description_text,
	wchar_t* result_text,
	uns32 maximum_character_count,
	void* overlapped
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
		overlapped
	};
	DialogBoxParam((HINSTANCE)overlapped, MAKEINTRESOURCE(IDD_TEXT_BOX_DIALOG), window_globals.hWnd, &XShowKeyboardUI_proc, (LPARAM)&params);

	return 0;
}

// $TODO add an icon for notifications, more customization?
void display_notification(const wchar_t* tip, const wchar_t* info, const wchar_t* info_title)
{
	ASSERT(tip != NULL);
	ASSERT(info != NULL);
	ASSERT(info_title != NULL);

	NOTIFYICONDATAW notify_icon_data = { sizeof(NOTIFYICONDATAW) };

	notify_icon_data.hWnd = window_globals.hWnd;

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

