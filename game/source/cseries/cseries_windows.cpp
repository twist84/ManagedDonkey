#include "cseries/cseries_windows.hpp"

#include "cseries/cseries.hpp"

#include <windows.h>

void display_debug_string(const char* format, ...)
{
    va_list list;
    va_start(list, format);

    if (IsDebuggerPresent())
    {
        c_static_string<4096> output = {};
        output.append_vprint(format, list);
        output.append("\n");

        OutputDebugStringA(output.get_string());
    }

    va_end(list);
}
