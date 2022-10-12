#include "shell/shell.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

bool __cdecl shell_get_system_identifier(char* system_identifier, long system_identifier_len)
{
    FUNCTION_BEGIN(false);

    return INVOKE(0x0051CE40, shell_get_system_identifier, system_identifier, system_identifier_len);
}
