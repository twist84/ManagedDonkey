#include "networking/logic/network_session_interface.hpp"

#include "networking/logic/network_life_cycle.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/session/network_session.hpp"
#include "networking/session/network_session_parameter_type_collection.hpp"

REFERENCE_DECLARE(0x019A0328, s_network_session_interface_globals, session_interface_globals);

long __cdecl network_squad_session_get_countdown_timer()
{
	c_network_session* active_squad_session = nullptr;
	if (network_life_cycle_in_squad_session(&active_squad_session) && active_squad_session->established())
	{
		c_network_session_parameters* parameters = active_squad_session->get_session_parameters();
		c_network_session_parameter_countdown_timer* countdown_timer =
			static_cast<c_network_session_parameter_countdown_timer*>(parameters->m_parameter_interfaces[_network_session_parameter_type_countdown_timer]);
		return countdown_timer->get_countdown_timer();
	}

	return -1;
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

short __cdecl network_squad_session_get_campaign_insertion_point()
{
	return INVOKE(0x00435080, network_squad_session_get_campaign_insertion_point);
}

//e_campaign_metagame_scoring __cdecl network_squad_session_get_campaign_metagame_scoring()
short __cdecl network_squad_session_get_campaign_metagame_scoring()
{
	return INVOKE(0x004350F0, network_squad_session_get_campaign_metagame_scoring);
}

//sub_435160

s_saved_film_description const* __cdecl network_squad_session_get_film()
{
	return INVOKE(0x00435250, network_squad_session_get_film);
}

c_game_variant const* __cdecl network_life_cycle_session_get_game_variant()
{
	return INVOKE(0x004352B0, network_life_cycle_session_get_game_variant);
}

bool __cdecl network_life_cycle_session_get_map(long* campaign_id, long* map_id)
{
	return INVOKE(0x004354D0, network_life_cycle_session_get_map, campaign_id, map_id);
}

c_map_variant const* __cdecl network_squad_session_get_map_variant()
{
	return INVOKE(0x00435540, network_squad_session_get_map_variant);
}

//sub_4355A0

e_gui_game_mode __cdecl network_life_cycle_squad_session_get_ui_game_mode()
{
	return INVOKE(0x00435640, network_life_cycle_squad_session_get_ui_game_mode);
}

//sub_4356A0

void __cdecl network_session_check_properties(c_network_session* session)
{
	INVOKE(0x00435890, network_session_check_properties, session);
}

//network_session_get_membership

void __cdecl network_session_interface_update_join_a_friend()
{
	INVOKE(0x00435F60, network_session_interface_update_join_a_friend);
}

//void __cdecl network_session_interface_add_local_user(e_output_user_index user_index, s_player_identifier const* player_identifier)
void __cdecl network_session_interface_add_local_user(long user_index, s_player_identifier const* player_identifier)
{
	INVOKE(0x00436000, network_session_interface_add_local_user, user_index, player_identifier);
}

//network_session_interface_calculate_map_progress

void __cdecl network_session_interface_clear_peer_status_flags()
{
	INVOKE(0x00436200, network_session_interface_clear_peer_status_flags);
}

void __cdecl network_session_interface_dispose()
{
	INVOKE(0x00436210, network_life_cycle_squad_session_get_ui_game_mode);
}

//sub_436230
//simulation_debug_globals_get_game_network_quality

bool __cdecl network_session_interface_get_is_user_signed_in(long user_index)
{
	return INVOKE(0x00436390, network_session_interface_get_is_user_signed_in, user_index);
}

//bool __cdecl network_session_interface_get_is_zombie_local_user(e_output_user_index user_index)
bool __cdecl network_session_interface_get_is_zombie_local_user(long user_index)
{
	return INVOKE(0x004363C0, network_session_interface_get_is_zombie_local_user, user_index);
}

bool __cdecl network_session_interface_get_live_connection_info(s_transport_qos_result* qos_result, e_online_nat_type* nat_type, long* bandwidth_bps, long* max_machine_count)
{
	return INVOKE(0x004363E0, network_session_interface_get_live_connection_info, qos_result, nat_type, bandwidth_bps, max_machine_count);
}

//get_local_framerate_quality
//sub_4364A0

bool __cdecl network_session_interface_get_local_user_identifier(long user_index, s_player_identifier* player_identifier, bool a3)
{
	return INVOKE(0x004364D0, network_session_interface_get_local_user_identifier, user_index, player_identifier, a3);
}

bool __cdecl network_session_interface_get_local_user_properties(long user_index, e_controller_index* controller_index, s_player_configuration* player_data, dword* player_voice_settings)
{
	return INVOKE(0x00436520, network_session_interface_get_local_user_properties, user_index, controller_index, player_data, player_voice_settings);
}

//e_network_interface_user_state __cdecl network_session_interface_get_local_user_state(long user_index)
long __cdecl network_session_interface_get_local_user_state(long user_index)
{
	return INVOKE(0x00436580, network_session_interface_get_local_user_state, user_index);
}

qword __cdecl network_session_interface_get_local_user_xuid(long user_index)
{
	return INVOKE(0x004365A0, network_session_interface_get_local_user_xuid, user_index);
}

void __cdecl network_session_interface_handle_message(long session_network_message)
{
	INVOKE(0x004365D0, network_session_interface_handle_message, session_network_message);
}

bool __cdecl network_session_interface_initialize(c_network_session_manager* session_manager)
{
	return INVOKE(0x00436710, network_session_interface_initialize, session_manager);
}

//bool __cdecl network_session_interface_local_user_exists(e_output_user_index user_index)
bool __cdecl network_session_interface_local_user_exists(long user_index)
{
	return INVOKE(0x004367B0, network_session_interface_local_user_exists, user_index);
}

void __cdecl network_session_interface_notify_set_local_specific_film(s_saved_film_description const* film)
{
	INVOKE(0x004367D0, network_session_interface_notify_set_local_specific_film, film);
}

void __cdecl network_session_interface_remove_local_user(long user_index)
{
	INVOKE(0x00436810, network_session_interface_remove_local_user, user_index);
}

void __cdecl network_session_interface_reset(long session_index)
{
	INVOKE(0x00436860, network_session_interface_reset, session_index);
}

//void __cdecl network_session_interface_set_desired_multiplayer_team(e_output_user_index user_index, e_multiplayer_team team_index)
void __cdecl network_session_interface_set_desired_multiplayer_team(long user_index, long team_index)
{
	INVOKE(0x004368D0, network_session_interface_set_desired_multiplayer_team, user_index, team_index);
}

//void __cdecl network_session_interface_set_is_zombie_local_user(e_output_user_index user_index, bool is_zombie_local_user)
void __cdecl network_session_interface_set_is_zombie_local_user(long user_index, bool is_zombie_local_user)
{
	INVOKE(0x00436920, network_session_interface_set_is_zombie_local_user, user_index, is_zombie_local_user);
}

void __cdecl network_session_interface_set_live_service_qos(s_transport_qos_result const* qos_result)
{
	INVOKE(0x00436960, network_session_interface_set_live_service_qos, qos_result);
}

void __cdecl network_session_interface_set_local_name(wchar_t const* machine_name, wchar_t const* session_name)
{
	INVOKE(0x00436A30, network_session_interface_set_local_name, machine_name, session_name);
}

//void __cdecl network_session_interface_set_local_user_override_hopper_directory(e_output_user_index user_index, char const* override_hopper_directory)
void __cdecl network_session_interface_set_local_user_override_hopper_directory(long user_index, char const* override_hopper_directory)
{
	INVOKE(0x00436A60, network_session_interface_set_local_user_override_hopper_directory, user_index, override_hopper_directory);
}

void __cdecl network_session_interface_set_local_user_properties(long user_index, e_controller_index controller_index, s_player_configuration const* player_data, dword player_voice_settings)
{
	INVOKE(0x00436AB0, network_session_interface_set_local_user_properties, user_index, controller_index, player_data, player_voice_settings);
}

//void __cdecl network_session_interface_set_local_user_state(long user_index, e_network_interface_user_state local_user_state)
void __cdecl network_session_interface_set_local_user_state(long user_index, long local_user_state)
{
	INVOKE(0x00436B30, network_session_interface_set_local_user_state, user_index, local_user_state);
}

void __cdecl network_session_interface_set_local_user_xuid(long user_index, qword xuid)
{
	INVOKE(0x00436B70, network_session_interface_set_local_user_xuid, user_index, xuid);
}

//void __cdecl network_session_interface_set_peer_status_flag(e_network_session_peer_properties_status_flags peer_status_flag, bool enabled)
void __cdecl network_session_interface_set_peer_status_flag(long peer_status_flag, bool enabled)
{
	INVOKE(0x00436BC0, network_session_interface_set_peer_status_flag, peer_status_flag, enabled);
}

void __cdecl network_session_interface_set_ready_hopper_identifier(word hopper_identifier, e_session_game_start_error error)
{
	INVOKE(0x00436BF0, network_session_interface_set_ready_hopper_identifier, hopper_identifier, error);
}

//bool __cdecl network_session_interface_test_peer_status_flag(e_network_session_peer_properties_status_flags peer_status_flags)
bool __cdecl network_session_interface_test_peer_status_flag(long peer_status_flag)
{
	return INVOKE(0x00436C10, network_session_interface_test_peer_status_flag, peer_status_flag);
}

void __cdecl network_session_interface_update()
{
	INVOKE(0x00436C30, network_session_interface_update);
}

void __cdecl network_session_interface_update_local_state()
{
	INVOKE(0x00436CF0, network_session_interface_update_local_state);
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

void __cdecl network_session_set_player_failure_reason(long user_index, e_network_join_refuse_reason reason)
{
	INVOKE(0x004372C0, network_session_set_player_failure_reason, user_index, reason);
}

//.text:00437390 ; network_session_update_local_specific_parameters
//.text:004375D0 ; network_session_update_local_peer_properties
//.text:00437860 ; network_session_update_team_indices

void __cdecl network_session_update_user_properties(c_network_session* session, e_output_user_index output_user_index)
{
	INVOKE(0x00437B30, network_session_update_user_properties, session, output_user_index);
}

void __cdecl network_session_update_user_removal(c_network_session* session)
{
	INVOKE(0x00437CE0, network_session_update_user_removal, session);
}

bool __cdecl network_squad_session_boot_player(long player_index, e_network_session_boot_reason reason)
{
	return INVOKE(0x00437D60, network_squad_session_boot_player, player_index, reason);
}

bool __cdecl network_squad_session_build_status(s_network_session_status_data* game_status)
{
	return INVOKE(0x00437EA0, network_squad_session_build_status, game_status);
}

bool __cdecl network_squad_session_can_set_game_settings()
{
	//return INVOKE(0x00438590, network_squad_session_can_set_game_settings);

	c_network_session* in_squad_session = nullptr;
	return network_life_cycle_in_squad_session(&in_squad_session)
		&& in_squad_session->established()
		&& in_squad_session->is_leader()
		&& in_squad_session->session_mode() == _network_session_mode_idle;
}

bool __cdecl network_squad_session_controls_coop_game_options(bool* is_leader)
{
	return INVOKE(0x004385F0, network_squad_session_controls_coop_game_options, is_leader);
}

bool __cdecl network_squad_session_set_campaign_difficulty(e_campaign_difficulty_level campaign_difficulty)
{
	return INVOKE(0x00439510, network_squad_session_set_campaign_difficulty, campaign_difficulty);
}

bool __cdecl network_squad_session_set_campaign_insertion_point(short campaign_insertion_point)
{
	return INVOKE(0x00439580, network_squad_session_set_campaign_insertion_point, campaign_insertion_point);
}

bool __cdecl network_squad_session_set_closed_by_user(bool closed_by_user)
{
	return INVOKE(0x00439660, network_squad_session_set_closed_by_user, closed_by_user);
}

//bool __cdecl network_squad_session_set_closed_status(enum e_network_session_closed_status closed_status)
bool __cdecl network_squad_session_set_closed_status(long closed_status)
{
	return INVOKE(0x004396B0, network_squad_session_set_closed_status, closed_status);
}

//bool __cdecl network_squad_session_set_coop_game_options(e_campaign_id campaign_id, e_map_id map_id, char const* scenario_path)
bool __cdecl network_squad_session_set_coop_game_options(long campaign_id, long map_id, char const* scenario_path)
{
	return INVOKE(0x004396F0, network_squad_session_set_coop_game_options, campaign_id, map_id, scenario_path);
}

bool __cdecl network_squad_session_set_film(s_saved_film_description const* film)
{
	return INVOKE(0x00439800, network_squad_session_set_film, film);
}

bool __cdecl network_squad_session_set_game_variant(c_game_variant const* game_variant)
{
	return INVOKE(0x00439860, network_squad_session_set_game_variant, game_variant);
}

//bool network_squad_session_set_map(e_campaign_id campaign_id, e_map_id map_id, char const* scenario_path)
bool __cdecl network_squad_session_set_map(long campaign_id, long map_id, char const* scenario_path)
{
	return INVOKE(0x00439A00, network_squad_session_set_map, campaign_id, map_id, scenario_path);
}

bool __cdecl network_squad_session_set_map_variant(c_map_variant const* map_variant)
{
	return INVOKE(0x00439AB0, network_squad_session_set_map_variant, map_variant);
}

bool __cdecl network_squad_session_set_maximum_player_count(long maximum_player_count)
{
	return INVOKE(0x00439BA0, network_squad_session_set_maximum_player_count, maximum_player_count);
}

//bool __cdecl network_squad_session_set_privacy_mode(e_network_game_privacy privacy_mode)
bool __cdecl network_squad_session_set_privacy_mode(long privacy_mode)
{
	return INVOKE(0x00439C60, network_squad_session_set_privacy_mode, privacy_mode);
}

bool __cdecl network_squad_session_set_simulation_protocol(e_network_game_simulation_protocol simulation_protocol)
{
	return INVOKE(0x00439CB0, network_squad_session_set_simulation_protocol, simulation_protocol);
}

bool __cdecl network_squad_session_set_start_mode(e_network_game_start_mode start_mode)
{
	return INVOKE(0x00439D00, network_squad_session_set_start_mode, start_mode);
}

bool __cdecl network_squad_session_set_ui_game_mode(e_gui_game_mode ui_game_mode)
{
	return INVOKE(0x00439D50, network_squad_session_set_ui_game_mode, ui_game_mode);
}

bool __cdecl network_squad_session_set_ui_state(s_network_ui_state const* ui_state)
{
	return INVOKE(0x00439DA0, network_squad_session_set_ui_state, ui_state);
}

bool __cdecl network_squad_session_whack_to_offline_session_class()
{
	return INVOKE(0x00439DF0, network_squad_session_whack_to_offline_session_class);
}

bool __cdecl network_squad_session_local_peer_is_leader()
{
	return INVOKE(0x00455320, network_squad_session_local_peer_is_leader);
}

bool __cdecl network_session_interface_wants_main_menu_to_load()
{
	return INVOKE(0x00454B90, network_session_interface_wants_main_menu_to_load);
}

bool __cdecl network_life_cycle_game_is_in_progress()
{
	return INVOKE(0x00454BB0, network_life_cycle_game_is_in_progress);
}

bool __cdecl network_squad_session_set_session_mode(e_network_session_mode session_mode)
{
	bool success = false;
	//if (network_squad_session_can_set_game_settings())
	if (network_squad_session_local_peer_is_leader())
	{
		c_network_session* in_squad_session = nullptr;
		if (network_life_cycle_in_squad_session(&in_squad_session))
		{
			ASSERT(in_squad_session);

			success = in_squad_session->get_session_parameters()->session_mode.request_change(session_mode);
			ASSERT(success);
		}
	}

	return success;
}

