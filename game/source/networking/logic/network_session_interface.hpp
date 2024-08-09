#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_security.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"
#include "networking/transport/transport_qos.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"

#pragma pack(push, 1)
struct s_network_session_interface_user
{
	long state;
	s_player_identifier identifier;
	c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> controller_index;
	s_player_configuration player_data;
	long player_update_number;
	c_static_string<64> override_hopper_directory;
	long player_voice_settings;
	long session_index;
	long team_index;
	long __times1680[3];
	long __times168C[3];
};
static_assert(sizeof(s_network_session_interface_user) == 0x1698);

struct s_saved_film_description
{
	long film_category;
	long campaign_id;
	long map_id;
	short campaign_insertion_point;
	bool campaign_survival_enabled;
	byte : 8;
	long difficulty;
	c_static_wchar_string<256> film_path;
	c_static_wchar_string<128> film_name;
	c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> controller_index;
	long length_seconds;
};
static_assert(sizeof(s_saved_film_description) == 0x31C);

struct s_network_session_interface_globals
{
	bool initialized;
	byte : 8;
	c_static_wchar_string<16> machine_name;
	c_static_wchar_string<32> session_name;
	bool has_live_connection_info;
	byte : 8;
	s_transport_qos_result qos_result;
	long bandwidth_bps;
	long max_machine_count;
	c_flags<e_network_session_peer_properties_status_flags, dword, k_network_session_peer_properties_status_flags> peer_status_flags;
	short ready_hopper_identifier;
	byte : 8;
	byte : 8;
	long game_start_error;
	bool was_guide_opened_during_a_multiplayer_session;
	byte : 8;
	byte : 8;
	byte : 8;
	long map_id;
	long current_map;
	long current_map_progress_percentage;
	short hopper_identifier;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	c_static_array<s_network_session_interface_user, k_number_of_output_users> users;
	qword game_instance;
	long scenario_type;
	c_static_string<128> scenario_path;
	short campaign_insertion_point;
	short map_status;
	real map_progress;
	s_saved_film_description local_specific_film;
	dword local_specific_film_time;
	c_static_array<long, 3> session_connection_identifiers;
	c_static_array<long, 3> session_peer_properties_update_times;
	c_static_array<long, 3> session_membership_update_number;
	c_static_array<bool, 3> session_variant_has_teams;
	c_static_array<bool, 3> session_variant_has_sve_teams;
	c_static_array<bool, 3> session_variant_observers_allowed;
	byte __pad5EF1[3];
	c_static_array<long, 3> session_variant_session_maximum_team_counts;
	c_network_session_manager* session_manager;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_network_session_interface_globals) == 0x5F08);
#pragma pack(pop)

extern s_network_session_interface_globals& session_interface_globals;

enum e_campaign_difficulty_level;
enum e_network_game_simulation_protocol;
enum e_network_game_start_mode;
enum e_gui_game_mode;
enum e_online_nat_type;
struct s_saved_film_description;
struct c_game_variant;
struct c_map_variant;
struct s_network_ui_state;
struct s_network_session_status_data;


extern long __cdecl network_squad_session_get_countdown_timer();
extern bool __cdecl network_life_cycle_in_matchmaking();
//network_squad_session_get_active_skulls_primary
//network_squad_session_get_active_skulls_secondary
extern e_campaign_difficulty_level __cdecl network_life_cycle_session_get_campaign_difficulty();
extern short __cdecl network_squad_session_get_campaign_insertion_point();
extern short __cdecl network_squad_session_get_campaign_metagame_scoring();
//sub_435160
extern s_saved_film_description const* __cdecl network_squad_session_get_film();
extern c_game_variant const* __cdecl network_life_cycle_session_get_game_variant();
extern bool __cdecl network_life_cycle_session_get_map(long* campaign_id, long* map_id);
extern c_map_variant const* __cdecl network_squad_session_get_map_variant();
//sub_4355A0
extern e_gui_game_mode __cdecl network_life_cycle_squad_session_get_ui_game_mode();
//sub_4356A0
extern void __cdecl network_session_check_properties(c_network_session* session);
//network_session_get_membership
extern void __cdecl network_session_interface_update_join_a_friend();
extern void __cdecl network_session_interface_add_local_user(long user_index, s_player_identifier const* player_identifier);
//network_session_interface_calculate_map_progress
extern void __cdecl network_session_interface_clear_peer_status_flags();
extern void __cdecl network_session_interface_dispose();
//sub_436230
//simulation_debug_globals_get_game_network_quality
extern bool __cdecl network_session_interface_get_is_user_signed_in(long user_index);
extern bool __cdecl network_session_interface_get_is_zombie_local_user(long user_index);
extern bool __cdecl network_session_interface_get_live_connection_info(s_transport_qos_result* qos_result, e_online_nat_type* nat_type, long* bandwidth_bps, long* max_machine_count);
//get_local_framerate_quality
//sub_4364A0
extern bool __cdecl network_session_interface_get_local_user_identifier(long user_index, s_player_identifier* player_identifier, bool a3);
extern bool __cdecl network_session_interface_get_local_user_properties(long user_index, e_controller_index* controller_index, s_player_configuration* player_data, dword* player_voice_settings);
extern long __cdecl network_session_interface_get_local_user_state(long user_index);
extern qword __cdecl network_session_interface_get_local_user_xuid(long user_index);
extern void __cdecl network_session_interface_handle_message(long session_network_message);
extern bool __cdecl network_session_interface_initialize(c_network_session_manager* session_manager);
extern bool __cdecl network_session_interface_local_user_exists(e_output_user_index output_user_index);
extern void __cdecl network_session_interface_notify_set_local_specific_film(s_saved_film_description const* film);
extern void __cdecl network_session_interface_remove_local_user(long user_index);
extern void __cdecl network_session_interface_reset(long session_index);
extern void __cdecl network_session_interface_set_desired_multiplayer_team(long user_index, long team_index);
extern void __cdecl network_session_interface_set_is_zombie_local_user(long user_index, bool is_zombie_local_user);
extern void __cdecl network_session_interface_set_live_service_qos(s_transport_qos_result const* qos_result);
extern void __cdecl network_session_interface_set_local_name(wchar_t const* machine_name, wchar_t const* session_name);
extern void __cdecl network_session_interface_set_local_user_override_hopper_directory(long user_index, char const* override_hopper_directory);
extern void __cdecl network_session_interface_set_local_user_properties(long user_index, e_controller_index controller_index, s_player_configuration const* player_data, dword player_voice_settings);
extern void __cdecl network_session_interface_set_local_user_state(long user_index, long state);
extern void __cdecl network_session_interface_set_local_user_xuid(long user_index, qword xuid);
extern void __cdecl network_session_interface_set_peer_status_flag(e_network_session_peer_properties_status_flags peer_status_flag, bool enabled);
extern void __cdecl network_session_interface_set_ready_hopper_identifier(word hopper_identifier, e_session_game_start_error error);
extern bool __cdecl network_session_interface_test_peer_status_flag(e_network_session_peer_properties_status_flags peer_status_flag);
extern void __cdecl network_session_interface_update();
extern void __cdecl network_session_interface_update_local_state();
extern void __cdecl network_session_interface_update_session(c_network_session* session);
extern bool __cdecl network_session_interface_was_guide_opened_during_a_multiplayer_session();
extern void __cdecl network_session_set_player_failure_reason(long user_index, e_network_join_refuse_reason reason);
//network_session_update_local_specific_parameters
//network_session_update_local_peer_properties
//network_session_update_team_indices
extern void __cdecl network_session_update_user_properties(c_network_session* session, e_output_user_index output_user_index);
extern void __cdecl network_session_update_user_removal(c_network_session* session);
extern bool __cdecl network_squad_session_boot_player(long player_index, e_network_session_boot_reason reason);
extern bool __cdecl network_squad_session_build_status(s_network_session_status_data* game_status);
extern bool __cdecl network_squad_session_can_set_game_settings();
extern bool __cdecl network_squad_session_controls_coop_game_options(bool* is_leader);
extern bool __cdecl network_squad_session_set_campaign_difficulty(e_campaign_difficulty_level campaign_difficulty);
extern bool __cdecl network_squad_session_set_campaign_insertion_point(short campaign_insertion_point);
extern bool __cdecl network_squad_session_set_closed_by_user(bool closed_by_user);
extern bool __cdecl network_squad_session_set_closed_status(long closed_status);
extern bool __cdecl network_squad_session_set_coop_game_options(long campaign_id, long map_id, char const* scenario_path);
extern bool __cdecl network_squad_session_set_film(s_saved_film_description const* film);
extern bool __cdecl network_squad_session_set_game_variant(c_game_variant const* game_variant);
extern bool __cdecl network_squad_session_set_map(long campaign_id, long map_id, char const* scenario_path);
extern bool __cdecl network_squad_session_set_map_variant(c_map_variant const* map_variant);
extern bool __cdecl network_squad_session_set_maximum_player_count(long maximum_player_count);
extern bool __cdecl network_squad_session_set_privacy_mode(long privacy_mode);
extern bool __cdecl network_squad_session_set_simulation_protocol(e_network_game_simulation_protocol simulation_protocol);
extern bool __cdecl network_squad_session_set_start_mode(e_network_game_start_mode start_mode);
extern bool __cdecl network_squad_session_set_ui_game_mode(e_gui_game_mode ui_game_mode);
extern bool __cdecl network_squad_session_set_ui_state(s_network_ui_state const* ui_state);
extern bool __cdecl network_squad_session_whack_to_offline_session_class();
extern bool __cdecl network_squad_session_local_peer_is_leader();
extern bool __cdecl network_session_interface_wants_main_menu_to_load();
extern bool __cdecl network_life_cycle_game_is_in_progress();
extern bool __cdecl network_squad_session_set_session_mode(e_network_session_mode session_mode);

