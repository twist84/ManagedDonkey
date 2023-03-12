#include "interface/user_interface_networking.hpp"

#include "cseries/console.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_session.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/logic/network_session_interface.hpp"
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

void __cdecl user_interface_networking_set_start_game_when_ready(bool a1, long controller_index)
{
    INVOKE(0x00A7FAC0, user_interface_networking_set_start_game_when_ready, a1, controller_index);
}

void __cdecl user_interface_networking_set_ui_upload_quota(long ui_upload_quota)
{
    INVOKE(0x00A7FB00, user_interface_networking_set_ui_upload_quota, ui_upload_quota);
}

bool __cdecl user_interface_networking_should_allow_team_switching()
{
    return INVOKE(0x00A7FB20, user_interface_networking_should_allow_team_switching);
}

bool __cdecl sub_A7FB70()
{
    return INVOKE(0x00A7FB70, sub_A7FB70);
}

bool __cdecl user_interface_networking_should_show_teams()
{
    return INVOKE(0x00A7FBF0, user_interface_networking_should_show_teams);
}

void __cdecl user_interface_networking_show_signed_out_while_in_live_alert()
{
    INVOKE(0x00A7FCB0, user_interface_networking_show_signed_out_while_in_live_alert);
}

void __cdecl user_interface_networking_start_game_when_ready_update()
{
    INVOKE(0x00A7FCD0, user_interface_networking_start_game_when_ready_update);
}

void __cdecl user_interface_networking_update()
{
    INVOKE(0x00A7FE50, user_interface_networking_update);
}

bool __cdecl user_interface_networking_xbox_guide_was_active_recently()
{
    return INVOKE(0x00A7FE70, user_interface_networking_xbox_guide_was_active_recently);
}

bool __cdecl user_interface_reset_networking_to_pregame()
{
    //return INVOKE(0x00A7FEC0, user_interface_reset_networking_to_pregame);

    // halo online inplementation
    //return network_squad_session_local_peer_is_leader() && network_life_cycle_set_pre_game_state();

    if (!network_squad_session_local_peer_is_leader())
    {
        c_console::write_line("ui:networking:user_interface_reset_networking_to_pregame: we are not the leader of a custom game");
        return false;
    }

    if (!network_life_cycle_set_pre_game_state())
    {
        c_console::write_line("ui:networking:user_interface_reset_networking_to_pregame: failed to return networking to pregame mode");
        return false;
    }

    return true;
}

void __cdecl user_interface_sanitize_game_setup_preferences_for_remote_squad_join()
{
    INVOKE(0x00A7FEF0, user_interface_sanitize_game_setup_preferences_for_remote_squad_join);
}

// game_status->connection_quality
long __cdecl sub_A7FFA0(long player_index)
{
    return INVOKE(0x00A7FFA0, sub_A7FFA0, player_index);
}

void __cdecl user_interface_set_desired_multiplayer_mode(e_desired_multiplayer_mode desired_multiplayer_mode)
{
    INVOKE(0x00A7FFB0, user_interface_set_desired_multiplayer_mode, desired_multiplayer_mode);
}

