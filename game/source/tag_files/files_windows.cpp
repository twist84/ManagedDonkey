#include "files_windows.hpp"

bool __cdecl file_delete(s_file_reference* file_reference)
{
    return INVOKE(0x00529170, file_delete, file_reference);
}

bool __cdecl file_exists(s_file_reference const* file_reference)
{
    return INVOKE(0x005295F0, file_exists, file_reference);
}