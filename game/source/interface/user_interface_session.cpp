#include "interface/user_interface_session.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"

HOOK_DECLARE(0x00A82AD0, user_interface_squad_get_countdown_delaying_player);
HOOK_DECLARE(0x00A82AE0, user_interface_squad_get_countdown_timer);

bool __cdecl user_interface_squad_set_session_advertisement(e_gui_network_session_advertisement_mode advertisement_mode)
{
    return INVOKE(0x00A7F950, user_interface_squad_set_session_advertisement, advertisement_mode);
}

e_session_game_start_status __cdecl user_interface_get_session_game_start_status(e_session_game_start_error* error, dword* player_error_mask)
{
    return INVOKE(0x00A809F0, user_interface_get_session_game_start_status, error, player_error_mask);
}

e_controller_index user_interface_session_get_controller_index(long session_player_index)
{
    return INVOKE(0x00A81620, user_interface_session_get_controller_index, session_player_index);
}

s_player_configuration* __cdecl user_interface_session_get_player_data(long session_player_index)
{
    return INVOKE(0x00A818D0, user_interface_session_get_player_data, session_player_index);
}

bool __cdecl user_interface_session_is_local_player(long session_player_index)
{
    return INVOKE(0x00A81DF0, user_interface_session_is_local_player, session_player_index);
}

bool __cdecl user_interface_squad_is_player_valid(long session_player_index)
{
    return INVOKE(0x00A81E80, user_interface_squad_is_player_valid, session_player_index);
}

long __cdecl user_interface_squad_get_player_index(s_player_identifier const* player_identifier)
{
    return INVOKE(0x00A82040, user_interface_squad_get_player_index, player_identifier);
}

long __cdecl user_interface_squad_get_countdown_delaying_player()
{
    //return INVOKE(0x00A82AD0, user_interface_squad_get_countdown_delaying_player);

    //return network_squad_session_get_countdown_delayed_culprit();

    return -1;
}

long __cdecl user_interface_squad_get_countdown_timer()
{
    //return INVOKE(0x00A82AE0, user_interface_squad_get_countdown_timer);

    if (user_interface_get_session_game_start_status(0, 0) == _session_game_start_status_countdown)
        return network_squad_session_get_countdown_timer();

    return -1;
}

e_gui_game_mode __cdecl user_interface_squad_get_ui_game_mode()
{
    //return INVOKE(0x00A83180, user_interface_squad_get_ui_game_mode);

    return network_life_cycle_squad_session_get_ui_game_mode();
}

bool __cdecl user_interface_squad_set_map_variant(c_map_variant const* map_variant)
{
    return INVOKE(0x00A83AB0, user_interface_squad_set_map_variant, map_variant);
}

bool __cdecl user_interface_squad_set_game_variant(c_game_variant const* game_variant)
{
    return INVOKE(0x00A838B0, user_interface_squad_set_game_variant, game_variant);
}

