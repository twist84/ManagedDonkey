#include "cseries/console.hpp"

#include <windows.h>

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