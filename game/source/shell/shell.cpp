#include "shell/shell.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

void __cdecl shell_halt_with_message(char const* message)
{
    INVOKE(0x0042E4B0, shell_halt_with_message, message);
}

bool __cdecl shell_get_system_identifier(char* system_identifier, long system_identifier_len)
{
    return INVOKE(0x0051CE40, shell_get_system_identifier, system_identifier, system_identifier_len);
}

