#include "interface/user_interface_networking.hpp"

#include "interface/user_interface.hpp"
#include "interface/user_interface_session.hpp"
#include "networking/network_globals.hpp"
#include "shell/shell.hpp"

REFERENCE_DECLARE(0x05253D88, s_user_interface_networking_globals, user_interface_networking_globals);

void __cdecl sub_69D600()
{
    //INVOKE(0x0069D600, sub_69D600);

    user_interface_networking_enter_pregame_location(_ui_game_mode_multiplayer);
    do
    {
        user_interface_update(0.1f);
        network_update();
    } while (user_interface_squad_get_ui_game_mode() != _ui_game_mode_multiplayer);

    user_interface_squad_set_session_advertisement(_gui_network_session_advertisement_mode_system_link);
    do
    {
        user_interface_update(0.1f);
        network_update();
    } while (user_interface_networking_get_session_advertisement() != _gui_network_session_advertisement_mode_system_link);
}

bool __cdecl user_interface_join_remote_session(bool join_to_public_slots, long session_class, s_transport_secure_identifier* remote_session_id, s_transport_secure_address* remote_host_address, s_transport_secure_key* key)
{
	return INVOKE(0x00A7E3C0, user_interface_join_remote_session, join_to_public_slots, session_class, remote_session_id, remote_host_address, key);
}

bool __cdecl user_interface_networking_enter_pregame_location(e_gui_game_mode game_mode)
{
    return INVOKE(0x00A7EE70, user_interface_networking_enter_pregame_location, game_mode);
}

e_gui_network_session_advertisement_mode __cdecl user_interface_networking_get_session_advertisement()
{
    return INVOKE(0x00A7F160, user_interface_networking_get_session_advertisement);
}

