#include "cseries/cseries_console.hpp"

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

#include <windows.h>

#if defined(_DEBUG) || defined(DEDICATED_SERVER)
#define CONSOLE_ENABLED
#endif // _DEBUG, DEDICATED_SERVER

FILE* c_console::m_file = NULL;
bool c_console::m_initialized = false;

void c_console::initialize(char const* window_title)
{
	m_initialized = true;

#if defined(CONSOLE_ENABLED)
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	SetConsoleTitleA(window_title);

	freopen_s(&m_file, "CONIN$", "r", stdin);
	freopen_s(&m_file, "CONOUT$", "w", stderr);
	freopen_s(&m_file, "CONOUT$", "w", stdout);
#endif // _DEBUG
}

void c_console::dispose()
{
#if defined(CONSOLE_ENABLED)
	m_initialized = false;

	if (m_file)
		fclose(m_file);

	FreeConsole();
	PostMessageW(GetConsoleWindow(), WM_CLOSE, 0, 0);
#endif // _DEBUG
}

void c_console::write(char const* format, ...)
{
	if (!m_initialized)
		return;

	c_static_string<4096> str;

	va_list list;
	va_start(list, format);
	str.vprint(format, list);
	va_end(list);

#if defined(CONSOLE_ENABLED)
	printf(str);
#else
	OutputDebugStringA(str);
#endif // _DEBUG

}

void c_console::write_line(char const* format, ...)
{
	if (!m_initialized)
		return;

	c_static_string<4096> str;

	va_list list;
	va_start(list, format);
	str.vprint(format, list);
	str.append("\n");
	va_end(list);

#if defined(CONSOLE_ENABLED)
	printf(str);
#else
	OutputDebugStringA(str);
#endif // _DEBUG

}

void c_console::write(wchar_t const* format, ...)
{
	if (!m_initialized)
		return;

	c_static_wchar_string<4096> str;

	va_list list;
	va_start(list, format);

	str.vprint(format, list);

	va_end(list);

#if defined(CONSOLE_ENABLED)
	wprintf(str);
#else
	OutputDebugStringW(str);
#endif // _DEBUG

}

void c_console::write_line(wchar_t const* format, ...)
{
	if (!m_initialized)
		return;

	c_static_wchar_string<4096> str;

	va_list list;
	va_start(list, format);
	str.vprint(format, list);
	str.append(L"\n");
	va_end(list);

#if defined(CONSOLE_ENABLED)
	wprintf(str);
#else
	OutputDebugStringW(str);
#endif // _DEBUG

}

void c_console::write(char const* format, va_list list)
{
	if (!m_initialized)
		return;

	c_static_string<4096> str;

	str.vprint(format, list);

#if defined(CONSOLE_ENABLED)
	printf(str);
#else
	OutputDebugStringA(str);
#endif // _DEBUG

}

void c_console::write_line(char const* format, va_list list)
{
	if (!m_initialized)
		return;

	c_static_string<4096> str;

	str.vprint(format, list);
	str.append("\n");

#if defined(CONSOLE_ENABLED)
	printf(str);
#else
	OutputDebugStringA(str);
#endif // _DEBUG

}

void c_console::write(wchar_t const* format, va_list list)
{
	if (!m_initialized)
		return;

	c_static_wchar_string<4096> str;

	str.vprint(format, list);

#if defined(CONSOLE_ENABLED)
	wprintf(str);
#else
	OutputDebugStringW(str);
#endif // _DEBUG

}

void c_console::write_line(wchar_t const* format, va_list list)
{
	if (!m_initialized)
		return;

	c_static_wchar_string<4096> str;

	str.vprint(format, list);
	str.append(L"\n");

#if defined(CONSOLE_ENABLED)
	wprintf(str);
#else
	OutputDebugStringW(str);
#endif // _DEBUG

}

void get_error_message(unsigned long message_id, char(&message_buffer)[2048])
{
	ASSERT(message_buffer);

	FormatMessageA(
		FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		message_id,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		message_buffer,
		2048,
		NULL
	);
}

