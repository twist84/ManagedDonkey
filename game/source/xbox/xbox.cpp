#include "xbox/xbox.hpp"

#include "cseries/cseries.hpp"
#include "resource.h"

#include <windows.h>

REFERENCE_DECLARE(0x0199C014, HWND, g_game_window_handle);

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

struct XShowConnectUI_struct
{
	wchar_t const* default_ip_text;
	wchar_t const* default_port_text;
	wchar_t const* default_id_text;
	wchar_t const* default_address_text;

	wchar_t* result_ip_text;       // buffer must be 16 characters in size
	wchar_t* result_port_text;     // buffer must be 16 characters in size
	wchar_t* result_id_text;       // buffer must be 128 characters in size
	wchar_t* result_address_text;  // buffer must be 128 characters in size

	bool* dialog_succeeded;

	void dialog_initialize_handler(HWND dialog_handle)
	{
		SetWindowText(dialog_handle, L"Direct Connect");
		SetWindowText(GetDlgItem(dialog_handle, ID_IP_TEXT), default_ip_text);
		SetWindowText(GetDlgItem(dialog_handle, ID_PORT_TEXT), default_port_text);
		SetWindowText(GetDlgItem(dialog_handle, ID_SESSION_ID_TEXT), default_id_text);
		SetWindowText(GetDlgItem(dialog_handle, ID_SESSION_ADDRESS_TEXT), default_address_text);
		SetFocus(GetDlgItem(dialog_handle, ID_IP_TEXT));
	}

	void dialog_command_handler(HWND dialog_handle, long command)
	{
		switch (command)
		{
		case IDOK:
			SendMessage(GetDlgItem(dialog_handle, ID_IP_TEXT), WM_GETTEXT, (WPARAM)16, (LPARAM)result_ip_text);
			SendMessage(GetDlgItem(dialog_handle, ID_PORT_TEXT), WM_GETTEXT, (WPARAM)16, (LPARAM)result_port_text);
			SendMessage(GetDlgItem(dialog_handle, ID_SESSION_ID_TEXT), WM_GETTEXT, (WPARAM)128, (LPARAM)result_id_text);
			SendMessage(GetDlgItem(dialog_handle, ID_SESSION_ADDRESS_TEXT), WM_GETTEXT, (WPARAM)128, (LPARAM)result_address_text);
			EndDialog(dialog_handle, IDOK);
			*dialog_succeeded = true;
			break;
		case IDCANCEL:
			EndDialog(dialog_handle, IDCANCEL);
			*dialog_succeeded = false;
			break;
		}
	}
};

INT_PTR CALLBACK XShowConnectUI_proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static XShowConnectUI_struct* params = nullptr;

#define safe_params(_params) if ((_params)) (_params)
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		params = reinterpret_cast<XShowConnectUI_struct*>(lParam);
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

long XShowConnectUI(
	wchar_t const* default_ip_text,
	wchar_t const* default_port_text,
	wchar_t const* default_id_text,
	wchar_t const* default_address_text,
	wchar_t* result_ip_text,
	wchar_t* result_port_text,
	wchar_t* result_id_text,
	wchar_t* result_address_text,
	void* platform_handle,
	bool* dialog_succeeded
)
{
	static XShowConnectUI_struct params
	{
		default_ip_text,
		default_port_text,
		default_id_text,
		default_address_text,
		result_ip_text,
		result_port_text,
		result_id_text,
		result_address_text,
		dialog_succeeded
	};
	CreateDialogParam((HINSTANCE)platform_handle, MAKEINTRESOURCE(IDD_CONNECT_DIALOG), g_game_window_handle, &XShowConnectUI_proc, (LPARAM)&params);

	return 0;
}

