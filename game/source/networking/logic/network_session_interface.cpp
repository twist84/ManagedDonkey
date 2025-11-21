#include "networking/logic/network_session_interface.hpp"

#include "memory/module.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/network_time.hpp"
#include "networking/session/network_session.hpp"
#include "networking/session/network_session_parameter_type_collection.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "simulation/simulation.hpp"

REFERENCE_DECLARE(0x019A0328, s_network_session_interface_globals, session_interface_globals);

int32 __cdecl network_squad_session_get_countdown_timer()
{
	int32 countdown_timer = NONE;
	c_network_session* session = NULL;
	if (network_life_cycle_in_squad_session(&session) && session->established())
	{
		c_network_session_parameters* session_parameters = session->get_session_parameters();
		countdown_timer = session_parameters->m_parameters.countdown_timer.get_countdown_timer();
	}
	return countdown_timer;
}

bool __cdecl network_life_cycle_in_matchmaking()
{
	return INVOKE(0x00434EF0, network_life_cycle_in_matchmaking);
}

//network_squad_session_get_active_skulls_primary
//network_squad_session_get_active_skulls_secondary

e_campaign_difficulty_level __cdecl network_life_cycle_session_get_campaign_difficulty()
{
	return INVOKE(0x00435020, network_life_cycle_session_get_campaign_difficulty);
}

int16 __cdecl network_squad_session_get_campaign_insertion_point()
{
	return INVOKE(0x00435080, network_squad_session_get_campaign_insertion_point);
}

//e_campaign_metagame_scoring __cdecl network_squad_session_get_campaign_metagame_scoring()
int16 __cdecl network_squad_session_get_campaign_metagame_scoring()
{
	return INVOKE(0x004350F0, network_squad_session_get_campaign_metagame_scoring);
}

//sub_435160

const s_saved_film_description* __cdecl network_squad_session_get_film()
{
	return INVOKE(0x00435250, network_squad_session_get_film);
}

const c_game_variant* __cdecl network_life_cycle_session_get_game_variant()
{
	return INVOKE(0x004352B0, network_life_cycle_session_get_game_variant);
}

bool __cdecl network_life_cycle_session_get_map(e_campaign_id* campaign_id, e_map_id* map_id)
{
	return INVOKE(0x004354D0, network_life_cycle_session_get_map, campaign_id, map_id);
}

const c_map_variant* __cdecl network_squad_session_get_map_variant()
{
	return INVOKE(0x00435540, network_squad_session_get_map_variant);
}

//.text:004355A0 ; bool __cdecl network_life_cycle_session_is_player_in_session(const s_player_identifier*)

e_gui_game_mode __cdecl network_life_cycle_squad_session_get_ui_game_mode()
{
	return INVOKE(0x00435640, network_life_cycle_squad_session_get_ui_game_mode);
}

//.text:004356A0 ; void __cdecl network_session_calculate_peer_connectivity(const c_network_session*, s_network_session_peer_connectivity*)

void __cdecl network_session_check_properties(c_network_session* session)
{
	INVOKE(0x00435890, network_session_check_properties, session);
}

//.text:00435DE0 ; e_map_id __cdecl network_session_get_last_network_map_loaded()

bool __cdecl network_session_get_membership(
	const c_network_session* session,
	int32* update_number,
	int32* local_peer_index,
	int32* host_peer_index,
	int32* leader_peer_index,
	int32* peer_count,
	uns32* peer_valid_flags,
	const s_network_session_peer** peers,
	int32* player_count,
	uns32* player_valid_flags,
	const s_network_session_player** players)
{
	return INVOKE(0x00435DF0, network_session_get_membership,
		session,
		update_number,
		local_peer_index,
		host_peer_index,
		leader_peer_index,
		peer_count,
		peer_valid_flags,
		peers,
		player_count,
		player_valid_flags,
		players);
}

void __cdecl network_session_inteface_update_host_framerate_quality()
{
	INVOKE(0x00435F60, network_session_inteface_update_host_framerate_quality);
}

//void __cdecl network_session_interface_add_local_user(int32 user_index, const s_player_identifier* player_identifier)
void __cdecl network_session_interface_add_local_user(int32 user_index, const s_player_identifier* player_identifier)
{
	INVOKE(0x00436000, network_session_interface_add_local_user, user_index, player_identifier);
}

//.text:004360A0 ; real64 __cdecl network_session_interface_calculate_map_progress(e_scenario_type scenario_type, int16 campaign_insertion_point_index, const char* scenario_path)

void __cdecl network_session_interface_clear_peer_status_flags()
{
	INVOKE(0x00436200, network_session_interface_clear_peer_status_flags);

	//session_interface_globals.peer_status_flags.clear();
}

void __cdecl network_session_interface_dispose()
{
	INVOKE(0x00436210, network_life_cycle_squad_session_get_ui_game_mode);
}

//.text:00436230 ; int32 __cdecl network_session_interface_find_unused_local_user()
//.text:00436250 ; void __cdecl network_session_interface_get_game_network_quality(s_network_game_quality* game_quality)

bool __cdecl network_session_interface_get_is_user_signed_in(int32 user_index)
{
	return INVOKE(0x00436390, network_session_interface_get_is_user_signed_in, user_index);
}

bool __cdecl network_session_interface_get_is_zombie_local_user(int32 user_index)
{
	return INVOKE(0x004363C0, network_session_interface_get_is_zombie_local_user, user_index);
}

bool __cdecl network_session_interface_get_live_connection_info(s_transport_qos_result* qos_result, e_online_nat_type* nat_type, int32* estimated_bandwidth_bps, int32* max_machine_count)
{
	return INVOKE(0x004363E0, network_session_interface_get_live_connection_info, qos_result, nat_type, estimated_bandwidth_bps, max_machine_count);
}

//.text:00436450 ; e_network_rough_quality __cdecl network_session_interface_get_local_framerate_quality()
//.text:004364A0 ; int32 __cdecl network_session_interface_get_local_user_count()

bool __cdecl network_session_interface_get_local_user_identifier(int32 user_index, s_player_identifier* player_identifier, bool allow_users_in_erroneous_states)
{
	return INVOKE(0x004364D0, network_session_interface_get_local_user_identifier, user_index, player_identifier, allow_users_in_erroneous_states);
}

bool __cdecl network_session_interface_get_local_user_properties(int32 user_index, e_controller_index* controller_index, s_player_configuration* player_data, uns32* player_voice_settings)
{
	return INVOKE(0x00436520, network_session_interface_get_local_user_properties, user_index, controller_index, player_data, player_voice_settings);

	//bool success = false;
	//if (session_interface_globals.users[user_index].user_state == _network_session_interface_user_state_exists)
	//{
	//	if (controller_index)
	//	{
	//		*controller_index = session_interface_globals.users[user_index].controller_index;
	//	}
	//	if (player_data)
	//	{
	//		*player_data = session_interface_globals.users[user_index].player_data;
	//	}
	//	if (player_voice_settings)
	//	{
	//		*player_voice_settings = session_interface_globals.users[user_index].player_voice_settings;
	//	}
	//	success = true;
	//}
	//return success;
}

//e_network_interface_user_state __cdecl network_session_interface_get_local_user_state(int32 user_index)
int32 __cdecl network_session_interface_get_local_user_state(int32 user_index)
{
	return INVOKE(0x00436580, network_session_interface_get_local_user_state, user_index);
}

uns64 __cdecl network_session_interface_get_local_user_xuid(int32 user_index)
{
	return INVOKE(0x004365A0, network_session_interface_get_local_user_xuid, user_index);
}

//.text:004365C0 ; int32 __cdecl network_session_interface_get_team_index(int32)

void __cdecl network_session_interface_handle_message(e_session_network_message message)
{
	INVOKE(0x004365D0, network_session_interface_handle_message, message);
}

bool __cdecl network_session_interface_initialize(c_network_session_manager* session_manager)
{
	return INVOKE(0x00436710, network_session_interface_initialize, session_manager);
}

//.text:00436790 ; bool __cdecl network_session_interface_invalidate_map_progress()

bool __cdecl network_session_interface_local_user_exists(int32 user_index)
{
	return INVOKE(0x004367B0, network_session_interface_local_user_exists, user_index);

	//return session_interface_globals.users[output_user_index].state == 1;
}

void __cdecl network_session_interface_notify_set_local_specific_film(const s_saved_film_description* saved_film_description)
{
	INVOKE(0x004367D0, network_session_interface_notify_set_local_specific_film, saved_film_description);

	//session_interface_globals.local_specific_film = *saved_film_description;
	//session_interface_globals.local_specific_film_time = network_time_get();
}

void __cdecl network_session_interface_remove_local_user(int32 user_index)
{
	INVOKE(0x00436810, network_session_interface_remove_local_user, user_index);

	//ASSERT(VALID_INDEX(user_index, k_number_of_users));
	//
	//s_network_session_interface_user* user = &session_interface_globals.users[user_index];
	//event(_event_message, "networking:logic:session: local user %d removed, player identifier=%s", user_index, player_identifier_get_string(&user.identifier));
	//csmemset(user, 0, sizeof(s_network_session_interface_user));
	//user.team_index = NONE;
}

void __cdecl network_session_interface_reset(int32 session_index)
{
	INVOKE(0x00436860, network_session_interface_reset, session_index);

	//ASSERT(session_index >= 0 && session_index < k_network_maximum_sessions);
	//
	//session_interface_globals.session_peer_properties_update_times[session_index] = 0;
	//for (int32 user_index = 0; user_index < k_number_of_users; user_index++)
	//{
	//	s_network_session_interface_user& user = session_interface_globals.users[user_index];
	//	user.user_update_timestamp[session_index] = 0;
	//	user.user_remove_timestamp[session_index] = 0;
	//}
}

void __cdecl network_session_interface_set_desired_team_index(int32 user_index, int32 desired_team_index)
{
	INVOKE(0x004368D0, network_session_interface_set_desired_team_index, user_index, desired_team_index);
}

void __cdecl network_session_interface_set_is_zombie_local_user(int32 user_index, bool is_zombie_account)
{
	INVOKE(0x00436920, network_session_interface_set_is_zombie_local_user, user_index, is_zombie_account);
}

void __cdecl network_session_interface_set_live_service_qos(const s_transport_qos_result* qos_result)
{
	INVOKE(0x00436960, network_session_interface_set_live_service_qos, qos_result);
}

void __cdecl network_session_interface_set_local_name(const wchar_t* machine_name, const wchar_t* session_name)
{
	INVOKE(0x00436A30, network_session_interface_set_local_name, machine_name, session_name);
}

void __cdecl network_session_interface_set_local_user_override_hopper_directory(int32 user_index, const char* override_directory)
{
	INVOKE(0x00436A60, network_session_interface_set_local_user_override_hopper_directory, user_index, override_directory);
}

void __cdecl network_session_interface_set_local_user_properties(int32 user_index, e_controller_index controller_index, const s_player_configuration* player_data, uns32 player_voice_settings)
{
	INVOKE(0x00436AB0, network_session_interface_set_local_user_properties, user_index, controller_index, player_data, player_voice_settings);
}

void __cdecl network_session_interface_set_local_user_state(int32 user_index, e_network_interface_user_state user_state)
{
	INVOKE(0x00436B30, network_session_interface_set_local_user_state, user_index, user_state);
}

void __cdecl network_session_interface_set_local_user_xuid(int32 user_index, uns64 xuid)
{
	INVOKE(0x00436B70, network_session_interface_set_local_user_xuid, user_index, xuid);
}

void __cdecl network_session_interface_set_peer_status_flag(e_network_session_peer_properties_status_flags flag, bool value)
{
	INVOKE(0x00436BC0, network_session_interface_set_peer_status_flag, flag, value);

	//session_interface_globals.peer_status_flags.set(flag, value);
}

void __cdecl network_session_interface_set_ready_hopper_identifier(uns16 hopper_identifier, e_session_game_start_error game_start_error)
{
	INVOKE(0x00436BF0, network_session_interface_set_ready_hopper_identifier, hopper_identifier, game_start_error);
}

bool __cdecl network_session_interface_test_peer_status_flag(e_network_session_peer_properties_status_flags flag)
{
	return INVOKE(0x00436C10, network_session_interface_test_peer_status_flag, flag);

	//return session_interface_globals.peer_status_flags.test(flag);
}

void __cdecl network_session_interface_update()
{
	INVOKE(0x00436C30, network_session_interface_update);
}

void __cdecl network_session_interface_update_local_state()
{
	INVOKE(0x00436CF0, network_session_interface_update_local_state);
}

void __cdecl network_session_interface_update_peer_status_flags()
{
	INVOKE(0x00437000, network_session_interface_update_peer_status_flags);

	//network_session_interface_set_peer_status_flag(_network_session_peer_properties_status_match_acknowledge_sync_bit, simulation_aborted());
}

void __cdecl network_session_interface_update_session(c_network_session* session)
{
	INVOKE(0x00437020, network_session_interface_update_session, session);
}

bool __cdecl network_session_interface_was_guide_opened_during_a_multiplayer_session()
{
	return INVOKE(0x00437290, network_session_interface_was_guide_opened_during_a_multiplayer_session);

	//return session_interface_globals.was_guide_opened_during_a_multiplayer_session;
}

void __cdecl network_session_set_player_failure_reason(int32 user_index, e_network_join_refuse_reason refuse_reason)
{
	INVOKE(0x004372C0, network_session_set_player_failure_reason, user_index, refuse_reason);
}

//.text:00437390 ; void __cdecl network_session_update_local_specific_parameters(c_network_session* session)
//.text:004375D0 ; void __cdeck network_session_update_peer_properties(c_network_session* session, const s_network_session_peer* peer)
//.text:00437860 ; void __cdeck network_session_update_team_indices(c_network_session* session, bool is_team_game, bool was_team_game, bool are_observers_allowed, bool are_sve_enabled, int32 team_count)

void __cdecl network_session_update_user_properties(c_network_session* session, int32 user_index)
{
	INVOKE(0x00437B30, network_session_update_user_properties, session, user_index);
}

void __cdecl network_session_update_user_removal(c_network_session* session)
{
	INVOKE(0x00437CE0, network_session_update_user_removal, session);
}

bool __cdecl network_squad_session_boot_player(int32 player_index, e_network_session_boot_reason boot_reason)
{
	return INVOKE(0x00437D60, network_squad_session_boot_player, player_index, boot_reason);
}

bool __cdecl network_squad_session_build_status(s_network_squad_status_data* game_status)
{
	return INVOKE(0x00437EA0, network_squad_session_build_status, game_status);
}

bool __cdecl network_squad_session_can_set_game_settings()
{
	//return INVOKE(0x00438590, network_squad_session_can_set_game_settings);

	c_network_session* in_squad_session = NULL;
	return network_life_cycle_in_squad_session(&in_squad_session)
		&& in_squad_session->established()
		&& in_squad_session->is_leader()
		&& in_squad_session->session_mode() == _network_session_mode_idle;
}

bool __cdecl network_squad_session_controls_coop_game_options(bool* out_is_leader)
{
	return INVOKE(0x004385F0, network_squad_session_controls_coop_game_options, out_is_leader);
}

//.text:00438640 ; bool __cdecl network_squad_session_delegate_leadership(int32 player_index)
//.text:00438780 ; bool __cdecl network_squad_session_end_game()
//.text:00438840 ; int32 __cdecl network_squad_session_get_best_host_estimated_maximum_player_count()
//.text:00438890 ; void __cdecl network_squad_session_get_campaign_active_primary_skulls(int32*)
//.text:00438910 ; void __cdecl network_squad_session_get_campaign_active_secondary_skulls(int32*)
//.text:00438990 ; e_campaign_difficulty_level __cdecl network_squad_session_get_campaign_difficulty()
//.text:00438A10 ; int16 __cdecl network_squad_session_get_campaign_insertion_point()
//.text:00438A90 ; int16 __cdecl network_squad_session_get_campaign_metagame_scoring()
//.text:00438B10 ; bool __cdecl network_interactive_session_get_campaign_quit()
//.text:00438B80 ; bool __cdecl network_squad_session_get_closed()
//.text:00438C00 ; bool __cdecl network_squad_session_get_closed_by_user()
//.text:00438C70 ; e_network_session_privacy __cdecl network_squad_session_get_game_privacy_mode()
//.text:00438CA0 ; uns16 __cdecl network_squad_session_get_hopper()
//.text:00438D10 ; bool __cdecl network_squad_session_get_hopper_statistics(s_matchmaking_single_hopper_statistics*)
//.text:00438D80 ; int32 __cdecl network_squad_session_get_hopper_xlast_index()
//.text:00438DE0 ; int32 __cdecl network_squad_session_get_host_player_index()
//.text:00438E70 ; void __cdecl network_squad_session_get_map(e_campaign_id* campaign_id, e_map_id* map_id, char* scenario_path)
//.text:00438F10 ; e_matchmaking_search_preference __cdecl network_squad_session_get_matchmaking_search_preference()
//.text:00438F70 ; int32 __cdecl network_squad_session_get_matchmaking_tip()
//.text:00438FF0 ; int32 __cdecl network_squad_session_get_maximum_player_count()

bool __cdecl network_squad_session_get_membership(int32* update_number, int32* local_peer_index, int32* host_peer_index, int32* leader_peer_index, int32* peer_count, uns32* peer_valid_flags, const s_network_session_peer** peers, int32* player_count, uns32* player_valid_flags, const s_network_session_player** players)
{
	return INVOKE(0x00439020, network_squad_session_get_membership, update_number, local_peer_index, host_peer_index, leader_peer_index, peer_count, peer_valid_flags, peers, player_count, player_valid_flags, players);
}

//.text:00439080 ; int32 __cdecl network_squad_session_get_percent_loaded()

e_network_session_class __cdecl network_squad_session_get_session_class()
{
	return INVOKE(0x004390D0, network_squad_session_get_session_class);
}

//.text:00439110 ; const wchar_t* __cdecl network_squad_session_get_session_name()
//.text:00439160 ; e_network_game_privacy __cdecl network_squad_session_get_privacy_mode()

e_network_game_start_mode __cdecl network_squad_session_get_start_mode()
{
	return INVOKE(0x004391C0, network_squad_session_get_start_mode);
}

bool __cdecl network_squad_session_set_campaign_difficulty(e_campaign_difficulty_level campaign_difficulty)
{
	return INVOKE(0x00439510, network_squad_session_set_campaign_difficulty, campaign_difficulty);
}

bool __cdecl network_squad_session_set_campaign_insertion_point(int16 insertion_point)
{
	return INVOKE(0x00439580, network_squad_session_set_campaign_insertion_point, insertion_point);
}

bool __cdecl network_squad_session_set_closed_by_user(bool closed)
{
	return INVOKE(0x00439660, network_squad_session_set_closed_by_user, closed);
}

bool __cdecl network_squad_session_set_closed_status(e_network_session_closed_status closed_status)
{
	return INVOKE(0x004396B0, network_squad_session_set_closed_status, closed_status);
}

bool __cdecl network_squad_session_set_coop_game_options(e_campaign_id campaign_id, e_map_id map_id, const char* scenario_path)
{
	return INVOKE(0x004396F0, network_squad_session_set_coop_game_options, campaign_id, map_id, scenario_path);
}

bool __cdecl network_squad_session_set_film(const s_saved_film_description* description)
{
	return INVOKE(0x00439800, network_squad_session_set_film, description);
}

bool __cdecl network_squad_session_set_game_variant(const c_game_variant* variant)
{
	return INVOKE(0x00439860, network_squad_session_set_game_variant, variant);
}

bool network_squad_session_set_map(e_campaign_id campaign_id, e_map_id map_id, const char* scenario_path)
{
	return INVOKE(0x00439A00, network_squad_session_set_map, campaign_id, map_id, scenario_path);
}

bool __cdecl network_squad_session_set_map_variant(const c_map_variant* map_variant)
{
	return INVOKE(0x00439AB0, network_squad_session_set_map_variant, map_variant);
}

bool __cdecl network_squad_session_set_maximum_player_count(int32 maximum_player_count)
{
	return INVOKE(0x00439BA0, network_squad_session_set_maximum_player_count, maximum_player_count);
}

//.text:00439C60 ; bool __cdecl (enum e_network_session_privacy)
//bool __cdecl network_squad_session_set_session_privacy_mode(e_network_session_privacy privacy_mode)
bool __cdecl network_squad_session_set_session_privacy_mode(int32 privacy_mode)
{
	return INVOKE(0x00439C60, network_squad_session_set_session_privacy_mode, privacy_mode);
}

bool __cdecl network_squad_session_set_simulation_protocol(e_network_game_simulation_protocol simulation_protocol)
{
	return INVOKE(0x00439CB0, network_squad_session_set_simulation_protocol, simulation_protocol);
}

bool __cdecl network_squad_session_set_start_mode(e_network_game_start_mode start_mode)
{
	return INVOKE(0x00439D00, network_squad_session_set_start_mode, start_mode);
}

bool __cdecl network_squad_session_set_ui_game_mode(e_gui_game_mode game_mode)
{
	return INVOKE(0x00439D50, network_squad_session_set_ui_game_mode, game_mode);
}

bool __cdecl network_squad_session_set_ui_state(const s_network_ui_state* ui_state)
{
	return INVOKE(0x00439DA0, network_squad_session_set_ui_state, ui_state);
}

bool __cdecl network_squad_session_whack_to_offline_session_class()
{
	return INVOKE(0x00439DF0, network_squad_session_whack_to_offline_session_class);
}

bool __cdecl network_session_interface_wants_main_menu_to_load()
{
	return INVOKE(0x00454B90, network_session_interface_wants_main_menu_to_load);
}

bool __cdecl network_squad_session_local_peer_is_leader()
{
	return INVOKE(0x00455320, network_squad_session_local_peer_is_leader);
}

bool __cdecl network_squad_session_set_session_mode(e_network_session_mode session_mode)
{
	bool success = false;
	//if (network_squad_session_can_set_game_settings())
	if (network_squad_session_local_peer_is_leader())
	{
		c_network_session* session = NULL;
		if (network_life_cycle_in_squad_session(&session))
		{
			ASSERT(session);

			c_network_session_parameters* session_parameters = session->get_session_parameters();
			success = session_parameters->m_parameters.session_mode.request_change(session_mode);
			ASSERT(success);
		}
	}

	return success;
}

