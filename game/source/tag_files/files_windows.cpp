#include "files_windows.hpp"

#include "cseries/console.hpp"

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