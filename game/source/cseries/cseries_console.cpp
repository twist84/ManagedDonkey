#include "cseries/cseries_console.hpp"

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

#include <windows.h>

FILE* c_console::m_file = nullptr;
bool c_console::m_initialized = false;

void c_console::initialize(const char* window_title)
{
	if (!m_initialized)
	{
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		SetConsoleTitleA(window_title);

		freopen_s(&m_file, "CONIN$", "r", stdin);
		freopen_s(&m_file, "CONOUT$", "w", stderr);
		freopen_s(&m_file, "CONOUT$", "w", stdout);

#if !defined(_DEBUG)
		toggle_window_visibility();
#endif

		m_initialized = true;
	}
}

void c_console::dispose()
{
	if (m_initialized)
	{
		m_initialized = false;

		if (m_file)
		{
			fclose(m_file);
		}

		FreeConsole();
		PostMessageW(GetConsoleWindow(), WM_CLOSE, 0, 0);
	}
}

void c_console::toggle_window_visibility()
{
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, IsWindowVisible(hwnd) ? SW_HIDE : SW_SHOW);

	c_console::enable_ansi();
}

void c_console::clear()
{
	HANDLE output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (output_handle != INVALID_HANDLE_VALUE)
	{
		CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
		GetConsoleScreenBufferInfo(output_handle, &console_screen_buffer_info);

		DWORD console_size = console_screen_buffer_info.dwSize.X * console_screen_buffer_info.dwSize.Y;
		COORD top_left = { 0, 0 };
		DWORD chars_written = 0;
		FillConsoleOutputCharacter(output_handle, ' ', console_size, top_left, &chars_written);

		FillConsoleOutputAttribute(output_handle, console_screen_buffer_info.wAttributes, console_size, top_left, &chars_written);
		SetConsoleCursorPosition(output_handle, top_left);
	}
}

bool c_console::console_allocated()
{
	return GetConsoleWindow() != nullptr;
}

void c_console::enable_ansi()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(h, &mode);
	SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void c_console::write(const char* format, ...)
{
	va_list list;
	va_start(list, format);
	write_va(format, list);
	va_end(list);
}

void c_console::write_line(const char* format, ...)
{
	va_list list;
	va_start(list, format);
	write_line_va(format, list);
	va_end(list);
}

void c_console::write(const wchar_t* format, ...)
{
	va_list list;
	va_start(list, format);
	write_va(format, list);
	va_end(list);
}

void c_console::write_line(const wchar_t* format, ...)
{
	va_list list;
	va_start(list, format);
	write_line_va(format, list);
	va_end(list);
}

void c_console::write_va(const char* format, va_list list)
{
	if (m_initialized)
	{
		c_static_string<4096> str;

		str.print_va(format, list);

		if (console_allocated())
		{
			printf(str.get_string());
		}
		else
		{
			OutputDebugStringA(str.get_string());
		}
	}
}

void c_console::write_line_va(const char* format, va_list list)
{
	if (m_initialized)
	{
		c_static_string<4096> str;

		str.print_va(format, list);
		str.append("\n");

		if (console_allocated())
		{
			printf(str.get_string());
		}
		else
		{
			OutputDebugStringA(str.get_string());
		}
	}
}

void c_console::write_va(const wchar_t* format, va_list list)
{
	if (m_initialized)
	{
		c_static_wchar_string<4096> str;

		str.print_va(format, list);

		if (console_allocated())
		{
			wprintf(str.get_string());
		}
		else
		{
			OutputDebugStringW(str.get_string());
		}
	}
}

void c_console::write_line_va(const wchar_t* format, va_list list)
{
	if (m_initialized)
	{
		c_static_wchar_string<4096> str;

		str.print_va(format, list);
		str.append(L"\n");

		if (console_allocated())
		{
			wprintf(str.get_string());
		}
		else
		{
			OutputDebugStringW(str.get_string());
		}
	}
}

void get_error_message(uns32 message_id, char(&message_buffer)[2048])
{
	ASSERT(message_buffer);

	FormatMessageA(
		FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		message_id,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		message_buffer,
		2048,
		nullptr
	);
}

