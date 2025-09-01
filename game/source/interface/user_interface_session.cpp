#include "interface/user_interface_session.hpp"

#include "main/console.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/online/online.hpp"
#include "saved_games/saved_film_manager.hpp"

HOOK_DECLARE(0x00A82AD0, user_interface_squad_get_countdown_delaying_player);
HOOK_DECLARE(0x00A82AE0, user_interface_squad_get_countdown_timer);
HOOK_DECLARE_CALL(0x00B0A120, user_interface_squad_delegate_leadership);

//.text:00A7F8B0 ; bool __cdecl user_interface_interactive_session_request_campaign_quit(e_network_session_request_campaign_quit_reason campaign_quit_reason)
//.text:00A7F920 ; bool __cdecl user_interface_squad_set_closed_by_user(bool closed_by_user)

bool __cdecl user_interface_squad_set_session_advertisement(e_gui_network_session_advertisement_mode advertisement_mode)
{
	return INVOKE(0x00A7F950, user_interface_squad_set_session_advertisement, advertisement_mode);
}

//.text:00A7FEC0 ; bool __cdecl user_interface_reset_networking_to_pregame()
//.text:00A7FEF0 ; void __cdecl user_interface_sanitize_game_setup_preferences_for_remote_squad_join()
//.text:00A7FFB0 ; void __cdecl user_interface_set_desired_multiplayer_mode(e_desired_multiplayer_mode desired_multiplayer_mode)
//.text:00A80720 ; bool __cdecl user_interface_available_squad_is_local_box(int32 available_squad_index)
//.text:00A80770 ; bool __cdecl user_interface_create_new_squad(e_network_session_class squad_session_class)
//.text:00A807F0 ; bool __cdecl user_interface_game_ending_writing_stats()

//.text:00A80810 ; int32 __cdecl user_interface_game_settings_get_campaign_active_primary_skulls()
//.text:00A80840 ; int32 __cdecl user_interface_game_settings_get_campaign_active_secondary_skulls()

e_campaign_difficulty_level __cdecl user_interface_game_settings_get_campaign_difficulty()
{
	return INVOKE(0x00A80870, user_interface_game_settings_get_campaign_difficulty);
}

int16 __cdecl user_interface_game_settings_get_campaign_insertion_point()
{
	return INVOKE(0x00A80880, user_interface_game_settings_get_campaign_insertion_point);
}

e_campaign_metagame_scoring __cdecl user_interface_game_settings_get_campaign_metagame_scoring()
{
	return INVOKE(0x00A80890, user_interface_game_settings_get_campaign_metagame_scoring);
}

const s_saved_film_description* __cdecl user_interface_game_settings_get_film()
{
	return INVOKE(0x00A808A0, user_interface_game_settings_get_film);
}

const c_game_variant* __cdecl user_interface_game_settings_get_game_variant()
{
	return INVOKE(0x00A808B0, user_interface_game_settings_get_game_variant);
}

//.text:00A808C0 ; 

e_session_game_mode __cdecl user_interface_get_session_game_mode()
{
	return INVOKE(0x00A80970, user_interface_get_session_game_mode);
}

e_session_game_start_status __cdecl user_interface_get_session_game_start_status(e_session_game_start_error* error, uns32* player_error_mask)
{
	return INVOKE(0x00A809F0, user_interface_get_session_game_start_status, error, player_error_mask);
}

//.text:00A80AA0 ; bool __cdecl user_interface_get_session_precaching_progress(uns16* map_load_progress, e_session_game_start_status* game_start_status)
//.text:00A80B90 ; e_controller_index __cdecl user_interface_group_get_controller_index(int32 player_index)
//.text:00A80BF0 ; int32 __cdecl user_interface_group_get_guest_index(int32 player_index)
//.text:00A80C70 ; int32 __cdecl user_interface_group_get_machine_index(int32 player_index)
//.text:00A80CD0 ; int32 __cdecl user_interface_group_get_player_at_local_controller(e_controller_index controller_index)
//.text:00A80D50 ; int32 __cdecl user_interface_group_get_player_count()
//.text:00A80D80 ; const s_player_configuration* __cdecl user_interface_group_get_player_data(int32 player_index)
//.text:00A80F10 ; int32 __cdecl user_interface_group_get_player_join_sequence_number(int32 player_index)
//.text:00A80F70 ; uns64 __cdecl user_interface_group_get_player_party_nonce(int32 player_index)
//.text:00A81120 ; bool __cdecl user_interface_group_is_local_player(int32 player_index)
//.text:00A81190 ; bool __cdecl user_interface_group_is_player_valid(int32 player_index)
//.text:00A811F0 ; bool __cdecl user_interface_interactive_session_is_group()

bool __cdecl user_interface_interactive_session_is_squad()
{
	return INVOKE(0x00A81230, user_interface_interactive_session_is_squad);
}

//.text:00A81270 ; void __cdecl user_interface_leave_game()

void __cdecl user_interface_leave_sessions(e_user_interface_session_leave_type leave_type, e_user_interface_session_leave_reason leave_reason)
{
	INVOKE(0x00A81280, user_interface_leave_sessions, leave_type, leave_reason);
}

int32 __cdecl user_interface_local_player_count()
{
	return INVOKE(0x00A812A0, user_interface_local_player_count);
}

//.text:00A81370 ; bool __cdecl user_interface_remote_squad_contains_local_peer(const s_network_squad_status_data* status_data)
//.text:00A81440 ; bool __cdecl user_interface_remote_squad_is_joinable(const s_network_squad_status_data* status_data)
//.text:00A81510 ; bool __cdecl user_interface_squad_can_new_user_join(int32 user_index)

e_controller_index user_interface_session_get_controller_index(int32 player_index)
{
	return INVOKE(0x00A81620, user_interface_session_get_controller_index, player_index);
}

bool __cdecl user_interface_session_get_map(e_campaign_id* campaign_id, e_map_id* map_id)
{
	return INVOKE(0x00A817B0, user_interface_session_get_map, campaign_id, map_id);
}

int32 __cdecl user_interface_session_get_player_at_local_controller(e_controller_index controller_index)
{
	return INVOKE(0x00A817F0, user_interface_session_get_player_at_local_controller, controller_index);
}

s_player_configuration* __cdecl user_interface_session_get_player_data(int32 session_player_index)
{
	return INVOKE(0x00A818D0, user_interface_session_get_player_data, session_player_index);
}

const s_player_identifier* __cdecl user_interface_squad_get_player_identifier(int32 session_player_index)
{
	return INVOKE(0x00A81950, user_interface_squad_get_player_identifier, session_player_index);
}

//.text:00A81AC0 ; int32 __cdecl user_interface_session_get_player_index_from_xuid(uns64, const c_network_session*)
//.text:00A81BC0 ; uns64 __cdecl user_interface_get_player_party_nonce(int32)
//.text:00A81C50 ; uns64 __cdecl user_interface_session_get_player_xuid(int32)
//.text:00A81CE0 ; uns64 __cdecl user_interface_session_get_usable_player_xuid(int32)
//.text:00A81D90 ; void __cdecl user_interface_session_initialize()
//.text:00A81DA0 ; bool __cdecl user_interface_session_is_joining_allowed()

bool __cdecl user_interface_session_is_local_player(int32 player_index)
{
	return INVOKE(0x00A81DF0, user_interface_session_is_local_player, player_index);
}

bool __cdecl user_interface_squad_is_player_valid(int32 player_index)
{
	return INVOKE(0x00A81E80, user_interface_squad_is_player_valid, player_index);
}

bool __cdecl user_interface_session_is_verifying_strings()
{
	return INVOKE(0x00A81F10, user_interface_session_is_verifying_strings);
}

//.text:00A81F80 ; void __cdecl user_interface_session_memory_dispose()
//.text:00A81FA0 ; void __cdecl user_interface_session_memory_initialize(e_map_memory_configuration)
//.text:00A81FB0 ; bool __cdecl user_interface_session_parameters_are_valid_for_ui_game_mode(const c_network_session_parameters*)

int32 __cdecl user_interface_squad_get_player_index(const s_player_identifier* identifier)
{
	return INVOKE(0x00A82040, user_interface_squad_get_player_index, identifier);
}

uns64 __cdecl user_interface_session_player_get_player_xuid(const s_player_identifier* identifier)
{
	return INVOKE(0x00A82080, user_interface_session_player_get_player_xuid, identifier);
}

//.text:00A820E0 ; uns64 __cdecl user_interface_session_player_get_usable_player_xuid(const s_player_identifier*)
//.text:00A82140 ; bool __cdecl user_interface_session_player_in_current_session(const s_player_identifier*)
//.text:00A82150 ; bool __cdecl user_interface_session_set_name(const wchar_t*, const wchar_t*)
//.text:00A821D0 ; void __cdecl user_interface_session_string_verify_update()
//.text:00A827D0 ; void __cdecl user_interface_session_update()
//.text:00A827E0 ; bool __cdecl user_interface_squad_boot_player(int32)
//.text:00A82860 ; bool __cdecl user_interface_squad_can_set_game_settings()
//.text:00A82870 ; void __cdecl user_interface_squad_clear_game_settings()

bool __cdecl user_interface_squad_delegate_leadership(int32 player_index)
{
	//return INVOKE(0x00A82900, user_interface_squad_delegate_leadership, player_index);

	console_printf_color(global_real_argb_red, "unimplemented: user_interface_squad_delegate_leadership");

	// skip doing any sort leadership delegating, it will just make no one the leader
	// thus making it impossible to do anything with the current lobby
	// $TODO fix any underlying issues causing the above behaviour
	return false;
}

//.text:00A82960 ; bool __cdecl user_interface_squad_end_game()

bool __cdecl user_interface_squad_exists()
{
	return INVOKE(0x00A82980, user_interface_squad_exists);
}

//.text:00A829D0 ; bool __cdecl user_interface_squad_exists_or_is_leaving()
//.text:00A82A10 ; bool __cdecl user_interface_squad_fully_established()
//.text:00A82A60 ; int32 __cdecl user_interface_squad_get_best_host_estimated_maximum_player_count()
//.text:00A82A70 ; e_controller_index __cdecl user_interface_squad_get_controller_index(int32)

int32 __cdecl user_interface_squad_get_countdown_delaying_player()
{
	//return INVOKE(0x00A82AD0, user_interface_squad_get_countdown_delaying_player);

	//return network_squad_session_get_countdown_delayed_culprit();

	return NONE;
}

int32 __cdecl user_interface_squad_get_countdown_timer()
{
	//return INVOKE(0x00A82AE0, user_interface_squad_get_countdown_timer);

	if (user_interface_get_session_game_start_status(NULL, NULL) == _session_game_start_status_countdown)
	{
		return network_squad_session_get_countdown_timer();
	}

	return NONE;
}

//.text:00A82AF0 ; bool __cdecl user_interface_squad_get_game_setup_changing()
//.text:00A82B10 ; int32 __cdecl user_interface_squad_get_guest_index(int32)
//.text:00A82B90 ; int32 __cdecl user_interface_squad_get_host_player_index()
//.text:00A82BA0 ; e_squad_join_permission __cdecl user_interface_squad_get_join_permission()
//.text:00A82BE0 ; int32 __cdecl user_interface_squad_get_leader_player_index()

int32 __cdecl user_interface_squad_get_local_player_count()
{
	return INVOKE(0x00A82C90, user_interface_squad_get_local_player_count);
}

int32 __cdecl user_interface_squad_get_machine_count()
{
	return INVOKE(0x00A82D20, user_interface_squad_get_machine_count);
}

//.text:00A82D50 ; int32 __cdecl user_interface_squad_get_machine_index(int32)
//.text:00A82DB0 ; int32 __cdecl user_interface_squad_get_maximum_player_count()
//.text:00A82DC0 ; int32 __cdecl user_interface_squad_get_open_player_slot_count()
//.text:00A82E00 ; int32 __cdecl user_interface_squad_get_percent_loaded()
//.text:00A82E10 ; bool __cdecl user_interface_squad_get_player_at_local_controller(e_controller_index, s_user_interface_session_player_index*)

int32 __cdecl user_interface_squad_get_player_count()
{
	return INVOKE(0x00A82E90, user_interface_squad_get_player_count);
}

const s_player_configuration* __cdecl user_interface_squad_get_player_data(int32 session_player_index)
{
	return INVOKE(0x00A82EC0, user_interface_squad_get_player_data, session_player_index);
}

const s_player_identifier* __cdecl user_interface_session_get_player_identifier(int32 session_player_index)
{
	return INVOKE(0x00A82F20, user_interface_session_get_player_identifier, session_player_index);
}

//.text:00A82F90 ; int32 __cdecl user_interface_squad_get_player_index(const s_player_identifier*)
//.text:00A82FD0 ; int32 __cdecl user_interface_squad_get_player_index_from_xuid(uns64)
//.text:00A83020 ; int32 __cdecl user_interface_squad_get_player_join_sequence_number(int32 player_index)

e_network_session_class __cdecl user_interface_squad_get_session_class()
{
	return INVOKE(0x00A83110, user_interface_squad_get_session_class);
}

//.text:00A83120 ; bool __cdecl user_interface_squad_get_session_id(s_transport_secure_identifier* session_id)
//.text:00A83160 ; const wchar_t* __cdecl user_interface_squad_get_session_name()
//.text:00A83170 ; e_network_game_start_mode __cdecl user_interface_squad_get_start_mode()

e_gui_game_mode __cdecl user_interface_squad_get_ui_game_mode()
{
	//return INVOKE(0x00A83180, user_interface_squad_get_ui_game_mode);

	return network_life_cycle_squad_session_get_ui_game_mode();
}

bool __cdecl user_interface_squad_in_matchmaking()
{
	return INVOKE(0x00A83240, user_interface_squad_in_matchmaking);
}

bool __cdecl user_interface_squad_in_or_after_countdown()
{
	return INVOKE(0x00A83250, user_interface_squad_in_or_after_countdown);
}

//.text:00A83300 ; bool __cdecl user_interface_squad_is_booting_allowed()
//.text:00A83350 ; e_network_join_refuse_reason __cdecl user_interface_squad_is_joinable()
//.text:00A833A0 ; bool __cdecl user_interface_squad_is_local_player(int32 player_index)
//.text:00A83410 ; bool __cdecl user_interface_squad_is_offline()
//.text:00A834B0 ; bool __cdecl user_interface_squad_is_player_valid(int32 player_index)

bool __cdecl user_interface_squad_local_peer_is_host()
{
	return INVOKE(0x00A83510, user_interface_squad_local_peer_is_host);
}

bool __cdecl user_interface_squad_local_peer_is_leader()
{
	return INVOKE(0x00A83520, user_interface_squad_local_peer_is_leader);
}

bool __cdecl user_interface_squad_set_campaign_active_primary_skulls(uns32 active_skulls_primary)
{
	return INVOKE(0x00A835F0, user_interface_squad_set_campaign_active_primary_skulls, active_skulls_primary);
}

bool __cdecl user_interface_squad_set_campaign_active_secondary_skulls(uns32 active_skulls_secondary)
{
	return INVOKE(0x00A83600, user_interface_squad_set_campaign_active_secondary_skulls, active_skulls_secondary);
}

bool __cdecl user_interface_squad_set_campaign_difficulty(e_campaign_difficulty_level campaign_difficulty)
{
	return INVOKE(0x00A83610, user_interface_squad_set_campaign_difficulty, campaign_difficulty);
}

bool __cdecl user_interface_squad_set_campaign_insertion_point(int16 campaign_insertion_point)
{
	return INVOKE(0x00A83620, user_interface_squad_set_campaign_insertion_point, campaign_insertion_point);
}

bool __cdecl user_interface_squad_set_campaign_level(e_campaign_id campaign_id, e_map_id map_id)
{
	return INVOKE(0x00A83630, user_interface_squad_set_campaign_level, campaign_id, map_id);
}

bool __cdecl user_interface_squad_set_campaign_metagame_scoring(e_campaign_metagame_scoring campaign_metagame_scoring)
{
	return INVOKE(0x00A83680, user_interface_squad_set_campaign_metagame_scoring, campaign_metagame_scoring);
}

bool __cdecl user_interface_squad_set_campaign_progress(e_campaign_id campaign_id, e_map_id map_id, int16 campaign_inserion_point, s_campaign_armaments* player_armaments, s_campaign_game_progression* game_progression, s_hub_progression* hub_progression)
{
	return INVOKE(0x00A83690, user_interface_squad_set_campaign_progress, campaign_id, map_id, campaign_inserion_point, player_armaments, game_progression, hub_progression);
}

bool __cdecl user_interface_squad_set_film(const s_saved_film_description* description)
{
	//return INVOKE(0x00A836F0, user_interface_squad_set_film, description);

	return user_interface_squad_set_film_internal(description, network_squad_session_get_session_class() == _network_session_class_xbox_live);
}

bool __cdecl user_interface_squad_set_film_internal(const s_saved_film_description* description, bool needs_verification)
{
	//return INVOKE(0x00A83720, user_interface_squad_set_film_internal, description, needs_verification);

	ASSERT(description);

	bool is_valid = true;

	if (description->category == _saved_film_category_none)
	{
		// nothing to verify
	}
	else if (description->category == _saved_film_category_invalid)
	{
		is_valid = false;
	}
	else
	{
		e_controller_index controller_index = description->controller_index;
		c_static_string<256> film_path;
		film_path.print("%ls", description->film_path);

		is_valid = (controller_index == k_no_controller) || saved_film_manager_film_valid(controller_index, film_path.get_string());
	}

	if (needs_verification && online_is_connected_to_live())
	{
		// $IMPLEMENT
	}

	if (!is_valid)
	{
		s_saved_film_description blank_description{};
		csmemset(&blank_description, 0, sizeof(s_saved_film_description));
		blank_description.difficulty = _campaign_difficulty_level_normal;
		blank_description.category = is_valid ? _saved_film_category_none : _saved_film_category_invalid;
		blank_description.controller_index = k_no_controller;

		return network_squad_session_set_film(&blank_description);
	}

	if (!network_squad_session_set_film(description))
	{
		return false;
	}

	network_session_interface_notify_set_local_specific_film(description);
	return true;
}

//.text:00A83870 ; bool __cdecl user_interface_squad_set_game_setup_changing(bool game_setup_changing)

bool __cdecl user_interface_squad_set_game_variant(const c_game_variant* game_variant)
{
	return INVOKE(0x00A838B0, user_interface_squad_set_game_variant, game_variant);
}

//.text:00A838F0 ; bool __cdecl user_interface_squad_set_game_variant_internal(const c_game_variant* game_variant, bool needs_verification)
//.text:00A83AA0 ; bool __cdecl user_interface_squad_set_maximum_player_count(int32 maximum_player_count)

bool __cdecl user_interface_squad_set_multiplayer_map(const c_map_variant* map_variant)
{
	return INVOKE(0x00A83AB0, user_interface_squad_set_multiplayer_map, map_variant);
}

bool __cdecl user_interface_squad_set_multiplayer_map_internal(const c_map_variant* map_variant, bool needs_verification)
{
	return INVOKE(0x00A83AF0, user_interface_squad_set_multiplayer_map_internal, map_variant, needs_verification);
}

bool __cdecl user_interface_squad_set_saved_film_game_options(int32 length_in_ticks, int32 start_tick, const game_options* options)
{
	return INVOKE(0x00A83C90, user_interface_squad_set_saved_film_game_options, length_in_ticks, start_tick, options);
}

//.text:00A83CC0 ; bool __cdecl user_interface_squad_set_simulation_protocol(e_network_game_simulation_protocol simulation_protocol)
//.text:00A83CD0 ; char __cdecl user_interface_squad_set_start_mode(e_network_game_start_mode start_mode)
//.text:00A83CE0 ; bool __cdecl user_interface_squad_set_ui_game_mode(e_gui_game_mode ui_game_mode)

bool __cdecl user_interface_squad_start_countdown_timer(e_controller_index controller_index, int32 countdown_timer, int32 accelerate_countdown_timer)
{
	return INVOKE(0x00A83CF0, user_interface_squad_start_countdown_timer, controller_index, countdown_timer, accelerate_countdown_timer);
}

bool __cdecl user_interface_squad_stop_countdown_timer(e_controller_index controller_index, int32 countdown_timer, bool halo2)
{
	return INVOKE(0x00A83D70, user_interface_squad_stop_countdown_timer, controller_index, countdown_timer, halo2);
}

//.text:00A83DE0 ; bool __cdecl user_interface_target_squad_exists()

