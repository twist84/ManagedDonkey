#include "cseries/console.hpp"

#include <windows.h>
#include <assert.h>

FILE* c_console::m_file;

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
#ifdef _DEBUG
    va_list list;
    va_start(list, format);

    vprintf(format, list);

    va_end(list);
#endif // _DEBUG
}

void c_console::write_line(char const* format, ...)
{
#ifdef _DEBUG
    va_list list;
    va_start(list, format);

    vprintf(format, list);
    printf("\n");

    va_end(list);
#endif // _DEBUG
}

void c_console::write(wchar_t const* format, ...)
{
#ifdef _DEBUG
    va_list list;
    va_start(list, format);

    vwprintf(format, list);

    va_end(list);
#endif // _DEBUG
}

void c_console::write_line(wchar_t const* format, ...)
{
#ifdef _DEBUG
    va_list list;
    va_start(list, format);

    vwprintf(format, list);
    wprintf(L"\n");

    va_end(list);
#endif // _DEBUG
}

void c_console::write(char const* format, va_list list)
{
#ifdef _DEBUG
    vprintf(format, list);
#endif // _DEBUG
}
void c_console::write_line(char const* format, va_list list)
{
#ifdef _DEBUG
    vprintf(format, list);
    printf("\n");
#endif // _DEBUG
}

void c_console::write(wchar_t const* format, va_list list)
{
#ifdef _DEBUG
    vwprintf(format, list);
#endif // _DEBUG
}

void c_console::write_line(wchar_t const* format, va_list list)
{
#ifdef _DEBUG
    vwprintf(format, list);
    wprintf(L"\n");
#endif // _DEBUG
}

void get_error_message(unsigned long message_id, char(&message_buffer)[2048])
{
    assert(message_buffer);

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
