#include "tag_files/files_windows.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"

#include <windows.h>

HOOK_DECLARE(0x005294F0, file_error);
HOOK_DECLARE(0x00529170, file_delete);
HOOK_DECLARE(0x005295F0, file_exists);
HOOK_DECLARE(0x0052A220, file_open);

void suppress_file_errors(bool suppress)
{
    FUNCTION_BEGIN(true);

    if (get_tls())
        get_tls()->g_file_errors_suppressed = suppress;
}

bool file_errors_suppressed()
{
    FUNCTION_BEGIN(true);

    if (get_tls())
        return get_tls()->g_file_errors_suppressed;

    return true;
}

//void file_error(char const*, s_file_reference const*, s_file_reference const*, bool)
void __cdecl file_error(char const* file_function, s_file_reference* file0, s_file_reference* file1, bool suppress_error)
{
    FUNCTION_BEGIN(true);

    file_reference_info* info0 = file_reference_get_info(file0);
    file_reference_info* info1 = nullptr;
    if (file1)
        info1 = file_reference_get_info(file1);

    DWORD error = GetLastError();
    if (!file_errors_suppressed() && !suppress_error)
    {
        char system_text[1024]{};
        if (info1)
            csnzprintf(system_text, sizeof(system_text), "%s('%s', '%s')", file_function, info0->path, info1->path);
        else
            csnzprintf(system_text, sizeof(system_text), "%s('%s')", file_function, info0->path);

        char error_text[2048]{};
        FormatMessageA(
            FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            0,
            error,
            0,
            error_text,
            sizeof(error_text),
            NULL
        );

        c_console::write_line("system: %s error 0x%08x '%s'", system_text, error, error_text);
    }
    SetLastError(ERROR_SUCCESS);

    //HOOK_INVOKE(, file_error, file_function, file0, file1, suppress_error);
}

bool __cdecl file_delete(s_file_reference* file_reference)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, file_delete, file_reference);
    return result;
}

bool __cdecl file_exists(s_file_reference const* file_reference)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, file_exists, file_reference);
    return result;
}

bool __cdecl file_open(s_file_reference* file_reference, dword open_flags, dword* error)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, file_open, file_reference, open_flags, error);
    return result;
}