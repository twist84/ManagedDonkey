#include "interface/user_interface_session.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"

HOOK_DECLARE(0x00A82AE0, user_interface_squad_get_countdown_timer);

long __cdecl user_interface_squad_get_countdown_timer()
{
    if (user_interface_get_session_game_start_status(0, 0) == 7)
        return network_squad_session_get_countdown_timer();

    return -1;
}

//e_session_game_start_status user_interface_get_session_game_start_status(e_session_game_start_error*, dword*)
long __cdecl user_interface_get_session_game_start_status(long* error, dword* status)
{
    return INVOKE(0x00A809F0, user_interface_get_session_game_start_status, error, status);
}
