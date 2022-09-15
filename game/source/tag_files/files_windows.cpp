#include "files_windows.hpp"

bool __cdecl file_delete(s_file_reference* file_reference)
{
    return INVOKE(0x00529170, file_delete, file_reference);
}
