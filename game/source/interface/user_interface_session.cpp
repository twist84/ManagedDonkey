#include "interface/user_interface_session.hpp"

#include "cseries/cseries_console.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"

HOOK_DECLARE(0x00A82AD0, user_interface_squad_get_countdown_delaying_player);
HOOK_DECLARE(0x00A82AE0, user_interface_squad_get_countdown_timer);

//.text:00A7F8B0 ; bool __cdecl user_interface_interactive_session_request_campaign_quit(e_network_session_request_campaign_quit_reason campaign_quit_reason)
//.text:00A7F920 ; bool __cdecl user_interface_squad_set_closed_by_user(bool closed_by_user)

bool __cdecl user_interface_squad_set_session_advertisement(e_gui_network_session_advertisement_mode advertisement_mode)
{
    return INVOKE(0x00A7F950, user_interface_squad_set_session_advertisement, advertisement_mode);
}

//.text:00A7FEC0 ; bool __cdecl user_interface_reset_networking_to_pregame()
//.text:00A7FEF0 ; void __cdecl user_interface_sanitize_game_setup_preferences_for_remote_squad_join()
//.text:00A7FFB0 ; void __cdecl user_interface_set_desired_multiplayer_mode(e_desired_multiplayer_mode desired_multiplayer_mode)
//.text:00A80720 ; bool __cdecl user_interface_available_squad_is_local_box(long available_squad_index)
//.text:00A80770 ; bool __cdecl user_interface_create_new_squad(e_network_session_class squad_session_class)
//.text:00A807F0 ; bool __cdecl user_interface_game_ending_writing_stats()
//.text:00A80870 ; e_campaign_difficulty_level __cdecl user_interface_game_settings_get_campaign_difficulty()
//.text:00A80880 ; short __cdecl user_interface_squad_get_campaign_insertion_point()
//.text:00A80890 ; e_campaign_metagame_scoring __cdecl user_interface_squad_get_campaign_metagame_scoring()
//.text:00A808B0 ; c_game_variant const* __cdecl user_interface_game_settings_get_game_variant()
//.text:00A80970 ; e_session_game_mode __cdecl user_interface_get_session_game_mode()

e_session_game_start_status __cdecl user_interface_get_session_game_start_status(e_session_game_start_error* error, dword* player_error_mask)
{
    return INVOKE(0x00A809F0, user_interface_get_session_game_start_status, error, player_error_mask);
}

//.text:00A80AA0 ; bool __cdecl user_interface_get_session_precaching_progress(word* map_load_progress, e_session_game_start_status* game_start_status)
//.text:00A80B90 ; e_controller_index __cdecl user_interface_group_get_controller_index(long player_index)
//.text:00A80BF0 ; long __cdecl user_interface_group_get_guest_index(long player_index)
//.text:00A80C70 ; long __cdecl user_interface_group_get_machine_index(long player_index)
//.text:00A80CD0 ; long __cdecl user_interface_group_get_player_at_local_controller(e_controller_index controller_index)
//.text:00A80D50 ; long __cdecl user_interface_group_get_player_count()
//.text:00A80D80 ; s_player_configuration const* __cdecl user_interface_group_get_player_data(long player_index)
//.text:00A80F10 ; long __cdecl user_interface_group_get_player_join_sequence_number(long player_index)
//.text:00A80F70 ; qword __cdecl user_interface_group_get_player_party_nonce(long player_index)
//.text:00A81120 ; bool __cdecl user_interface_group_is_local_player(long player_index)
//.text:00A81190 ; bool __cdecl user_interface_group_is_player_valid(long player_index)
//.text:00A811F0 ; bool __cdecl user_interface_interactive_session_is_group()
//.text:00A81230 ; bool __cdecl user_interface_interactive_session_is_squad()
//.text:00A81270 ; void __cdecl user_interface_leave_game()
//.text:00A81280 ; void __cdecl user_interface_leave_squad(bool disconnect)
//.text:00A812A0 ; long __cdecl user_interface_local_player_count()
//.text:00A81370 ; bool __cdecl user_interface_remote_squad_contains_local_peer(s_network_session_status_data const* status_data)
//.text:00A81440 ; bool __cdecl user_interface_remote_squad_is_joinable(s_network_session_status_data const* status_data)
//.text:00A81510 ; bool __cdecl user_interface_squad_can_new_user_join(e_output_user_index user_index)

e_controller_index user_interface_session_get_controller_index(long player_index)
{
    return INVOKE(0x00A81620, user_interface_session_get_controller_index, player_index);
}

//.text:00A817B0 ; bool __cdecl user_interface_session_get_map(e_campaign_id* campaign_id, e_map_id* map_id)
//.text:00A817F0 ; long __cdecl user_interface_session_get_player_at_local_controller(e_controller_index controller_index)

s_player_configuration* __cdecl user_interface_session_get_player_data(long player_index)
{
    return INVOKE(0x00A818D0, user_interface_session_get_player_data, player_index);
}

//.text:00A81950 ; s_player_identifier const* __cdecl user_interface_squad_get_player_identifier(long player_index)
//.text:00A81BC0 ; qword __cdecl user_interface_get_player_party_nonce(long player_index)

bool __cdecl user_interface_session_is_local_player(long player_index)
{
    return INVOKE(0x00A81DF0, user_interface_session_is_local_player, player_index);
}

bool __cdecl user_interface_squad_is_player_valid(long player_index)
{
    return INVOKE(0x00A81E80, user_interface_squad_is_player_valid, player_index);
}

//.text:00A81F10 ; bool __cdecl user_interface_session_is_verifying_strings()
//.text:00A81F80 ; void __cdecl user_interface_session_memory_dispose()
//.text:00A81FA0 ; void __cdecl user_interface_session_memory_initialize(e_map_memory_configuration memory_configuration)
//.text:00A81FB0 ; bool __cdecl user_interface_session_parameters_are_valid_for_ui_game_mode(c_network_session_parameters const* session_parameters)

long __cdecl user_interface_squad_get_player_index(s_player_identifier const* player_identifier)
{
    return INVOKE(0x00A82040, user_interface_squad_get_player_index, player_identifier);
}

//.text:00A820E0 ; qword __cdecl user_interface_session_player_get_player_xuid(s_player_identifier const* player_identifier)
//.text:00A82150 ; bool __cdecl user_interface_session_set_name(wchar_t const* machine_name, wchar_t const* session_name)
//.text:00A821D0 ; void __cdecl user_interface_session_string_verify_update()
//.text:00A827D0 ; void __cdecl user_interface_session_update()
//.text:00A827E0 ; bool __cdecl user_interface_squad_boot_player(long player_index)
//.text:00A82860 ; bool __cdecl user_interface_squad_can_set_game_settings()

bool __cdecl user_interface_squad_delegate_leadership(long player_index)
{
    return INVOKE(0x00A82900, user_interface_squad_delegate_leadership, player_index);
}

//.text:00A82960 ; bool __cdecl user_interface_squad_end_game()
//.text:00A82980 ; bool __cdecl user_interface_squad_exists()
//.text:00A829D0 ; bool __cdecl user_interface_squad_exists_or_is_leaving()
//.text:00A82A10 ; bool __cdecl user_interface_squad_fully_established()
//.text:00A82A70 ; e_controller_index __cdecl user_interface_squad_get_controller_index(long player_index)

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

//.text:00A82AF0 ; bool __cdecl user_interface_squad_get_game_setup_changing()
//.text:00A82B90 ; long __cdecl user_interface_squad_get_host_player_index()
//.text:00A82BA0 ; e_squad_join_permission __cdecl user_interface_squad_get_join_permission()
//.text:00A82BE0 ; long __cdecl user_interface_squad_get_leader_player_index()
//.text:00A82C90 ; long __cdecl user_interface_squad_get_local_player_count()
//.text:00A82D20 ; long __cdecl user_interface_squad_get_machine_count()
//.text:00A82D50 ; long __cdecl user_interface_squad_get_machine_index(long player_index)
//.text:00A82E10 ; bool __cdecl user_interface_squad_get_player_at_local_controller(e_controller_index controller_index)
//.text:00A82E90 ; long __cdecl user_interface_squad_get_player_count()
//.text:00A82EC0 ; s_player_configuration const* __cdecl user_interface_squad_get_player_data(long player_index)
//.text:00A82F20 ; s_player_identifier const* __cdecl user_interface_session_get_player_identifier(long player_index)
//.text:00A82F90 ; long __cdecl user_interface_session_get_player_index(s_player_identifier const* player_identifier)
//.text:00A83020 ; long __cdecl user_interface_squad_get_player_join_sequence_number(long player_index)
//.text:00A83110 ; e_network_session_class __cdecl user_interface_squad_get_session_class()
//.text:00A83160 ; wchar_t const* __cdecl user_interface_squad_get_session_name()
//.text:00A83170 ; e_network_game_start_mode __cdecl user_interface_squad_get_start_mode()

e_gui_game_mode __cdecl user_interface_squad_get_ui_game_mode()
{
    //return INVOKE(0x00A83180, user_interface_squad_get_ui_game_mode);

    return network_life_cycle_squad_session_get_ui_game_mode();
}

//.text:00A83240 ; bool __cdecl user_interface_squad_in_matchmaking()
//.text:00A83250 ; bool __cdecl user_interface_squad_in_or_after_countdown()
//.text:00A83300 ; bool __cdecl user_interface_squad_is_booting_allowed()
//.text:00A83350 ; enum e_network_join_refuse_reason __cdecl user_interface_squad_is_joinable()
//.text:00A833A0 ; bool __cdecl user_interface_squad_is_local_player(long player_index)
//.text:00A83410 ; bool __cdecl user_interface_squad_is_offline()
//.text:00A834B0 ; bool __cdecl user_interface_squad_is_player_valid(long player_index)
//.text:00A83510 ; bool __cdecl user_interface_squad_local_peer_is_host()
//.text:00A83520 ; bool __cdecl user_interface_squad_local_peer_is_leader()
//.text:00A835F0 ; bool __cdecl user_interface_squad_set_active_skulls_primary(dword active_skulls_primary)
//.text:00A83600 ; bool __cdecl user_interface_squad_set_active_skulls_secondary(dword active_skulls_secondary)
//.text:00A83610 ; bool __cdecl user_interface_squad_set_campaign_difficulty(e_campaign_difficulty_level campaign_difficulty)
//.text:00A83620 ; bool __cdecl user_interface_squad_set_campaign_insertion_point(short campaign_insertion_point)
//.text:00A83630 ; bool __cdecl user_interface_squad_set_map(e_campaign_id campaign_id, e_map_id map_id)
//.text:00A83680 ; bool __cdecl user_interface_squad_set_campaign_metagame_scoring(e_campaign_metagame_scoring campaign_metagame_scoring)
//.text:00A836F0 ; bool __cdecl user_interface_squad_set_film(struct s_saved_film_description const* film)
//.text:00A83720 ; bool __cdecl user_interface_squad_set_film_internal(s_saved_film_description const* film, bool live)
//.text:00A83870 ; bool __cdecl user_interface_squad_set_game_setup_changing(bool game_setup_changing)

bool __cdecl user_interface_squad_set_game_variant(c_game_variant const* game_variant)
{
    return INVOKE(0x00A838B0, user_interface_squad_set_game_variant, game_variant);
}

//.text:00A838F0 ; bool __cdecl user_interface_squad_set_game_variant_internal(c_game_variant const* game_variant, bool needs_verification)
//.text:00A83AA0 ; bool __cdecl user_interface_squad_set_maximum_player_count(long maximum_player_count)

bool __cdecl user_interface_squad_set_map_variant(c_map_variant const* map_variant)
{
    return INVOKE(0x00A83AB0, user_interface_squad_set_map_variant, map_variant);
}

//.text:00A83AF0 ; bool __cdecl user_interface_squad_set_multiplayer_map_internal(c_map_variant const* map_variant, bool needs_verification)
//.text:00A83C90 ; bool __cdecl user_interface_squad_set_saved_film_game_options(long playback_length_in_ticks, int playback_start_ticks, game_options *options)
//.text:00A83CC0 ; bool __cdecl user_interface_squad_set_simulation_protocol(e_network_game_simulation_protocol simulation_protocol)
//.text:00A83CD0 ; char __cdecl user_interface_squad_set_start_mode(e_network_game_start_mode start_mode)
//.text:00A83CE0 ; bool __cdecl user_interface_squad_set_ui_game_mode(e_gui_game_mode ui_game_mode)
//.text:00A83CF0 ; bool __cdecl user_interface_squad_start_countdown_timer(e_controller_index controller_index, long countdown_timer, long accelerate_countdown_timer)
//.text:00A83D70 ; bool __cdecl user_interface_squad_stop_countdown_timer(e_controller_index controller_index, long countdown_timer, bool halo2)
//.text:00A83DE0 ; bool __cdecl user_interface_target_squad_exists()

