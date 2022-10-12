#include "networking/network_utilities.hpp"

#include "memory/module.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"

HOOK_DECLARE(0x0042ED50, network_get_machine_name);

bool __cdecl network_get_machine_name(wchar_t* machine_name, long machine_name_len)
{
    char system_identifier[264];

    if (!shell_get_system_identifier(system_identifier, 256) || !system_identifier[0])
        return false;

    ascii_string_to_wchar_string(system_identifier, machine_name, machine_name_len, nullptr);
    return true;
}
