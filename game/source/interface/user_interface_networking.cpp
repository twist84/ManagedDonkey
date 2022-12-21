#include "interface/user_interface_networking.hpp"

REFERENCE_DECLARE(0x05253D88, s_user_interface_networking_globals, user_interface_networking_globals);

// sets `pregame_location` to `_ui_game_mode_multiplayer`,
// sets `session_advertisement_mode` to `_gui_network_session_advertisement_mode_system_link`
void __cdecl sub_69D600()
{
	INVOKE(0x0069D600, sub_69D600);
}

bool __cdecl user_interface_join_remote_session(bool join_to_public_slots, long session_class, s_transport_secure_identifier* remote_session_id, s_transport_secure_address* remote_host_address, s_transport_secure_key* key)
{
	return INVOKE(0x00A7E3C0, user_interface_join_remote_session, join_to_public_slots, session_class, remote_session_id, remote_host_address, key);
}

