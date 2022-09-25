#include "tag_files/files_windows.hpp"

#include "cseries/console.hpp"
#include "memory/thread_local.hpp"

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

bool __cdecl file_delete(s_file_reference* file_reference)
{
    FUNCTION_BEGIN(true);

    return INVOKE(0x00529170, file_delete, file_reference);
}

bool __cdecl file_exists(s_file_reference const* file_reference)
{
    FUNCTION_BEGIN(true);

    return INVOKE(0x005295F0, file_exists, file_reference);
}