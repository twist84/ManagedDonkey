#include "cseries/console.hpp"

#include "cseries/cseries.hpp"
#include "interface/terminal.hpp"
#include "multithreading/threads.hpp"
#include "text/unicode.hpp"

#include <windows.h>

FILE* c_console::m_file;

bool console_dump_to_debug_display = false;

void c_console::initialize(char const* window_title)
{
#ifdef _DEBUG
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
#ifdef _DEBUG
    fclose(m_file);

    FreeConsole();
    PostMessageW(GetConsoleWindow(), WM_CLOSE, 0, 0);
#endif // _DEBUG
}

void c_console::write(char const* format, ...)
{
    c_static_string<4096> s;

    va_list list;
    va_start(list, format);
    s.vprint(format, list);
    va_end(list);

    char const* str = s.get_string();

#ifdef _DEBUG
    printf(str);
#else
    OutputDebugStringA(str);
#endif // _DEBUG

}

void c_console::write_line(char const* format, ...)
{
    c_static_string<4096> s;

    va_list list;
    va_start(list, format);
    s.vprint(format, list);
    s.append("\n");
    va_end(list);

    char const* str = s.get_string();

#ifdef _DEBUG
    printf(str);
#else
    OutputDebugStringA(str);
#endif // _DEBUG

}

void c_console::write(wchar_t const* format, ...)
{
    c_static_wchar_string<4096> s;

    va_list list;
    va_start(list, format);

    s.vprint(format, list);

    va_end(list);

    wchar_t const* str = s.get_string();

#ifdef _DEBUG
    wprintf(str);
#else
    OutputDebugStringW(str);
#endif // _DEBUG

}

void c_console::write_line(wchar_t const* format, ...)
{
    c_static_wchar_string<4096> s;

    va_list list;
    va_start(list, format);
    s.vprint(format, list);
    s.append(L"\n");
    va_end(list);

    wchar_t const* str = s.get_string();

#ifdef _DEBUG
    wprintf(str);
#else
    OutputDebugStringW(str);
#endif // _DEBUG

}

void c_console::write(char const* format, va_list list)
{
    c_static_string<4096> s;

    s.vprint(format, list);

    char const* str = s.get_string();

#ifdef _DEBUG
    printf(str);
#else
    OutputDebugStringA(str);
#endif // _DEBUG

}

void c_console::write_line(char const* format, va_list list)
{
    c_static_string<4096> s;

    s.vprint(format, list);
    s.append("\n");

    char const* str = s.get_string();

#ifdef _DEBUG
    printf(str);
#else
    OutputDebugStringA(str);
#endif // _DEBUG

}

void c_console::write(wchar_t const* format, va_list list)
{
    c_static_wchar_string<4096> s;

    s.vprint(format, list);

    wchar_t const* str = s.get_string();

#ifdef _DEBUG
    wprintf(str);
#else
    OutputDebugStringW(str);
#endif // _DEBUG

}

void c_console::write_line(wchar_t const* format, va_list list)
{
    c_static_wchar_string<4096> s;

    s.vprint(format, list);
    s.append(L"\n");

    wchar_t const* str = s.get_string();

#ifdef _DEBUG
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

void console_printf(char const* format, ...)
{
    va_list list;
    va_start(list, format);

    c_console::write_line(format, list);
    if (is_main_thread())
    {
        char buffer[256]{};
        cvsnzprintf(buffer, 255, format, list);
        terminal_printf(nullptr, "%s", buffer);

        if (console_dump_to_debug_display)
            display_debug_string(buffer);
    }

    va_end(list);
}

