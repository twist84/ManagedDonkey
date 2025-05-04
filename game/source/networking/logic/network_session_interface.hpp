#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_security.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"
#include "networking/transport/transport_qos.hpp"
#include "text/unicode.hpp"

struct c_game_variant;
struct c_map_variant;
struct s_saved_film_description;
struct s_network_ui_state;
struct s_network_squad_status_data;
struct s_network_session_peer;
struct s_network_session_player;

struct s_network_session_interface_user
{
	int32 user_state;
	s_player_identifier player_identifier;
	e_controller_index controller_index;
	s_player_configuration player_data;
	int32 player_update_number;
	char override_hopper_directory[64];

	union
	{
		uns32 player_voice_settings;
		struct
		{
			uns16 player_mute_mask;
			uns16 player_voice_flags;
		};
	};

	int32 session_to_change_teams_on;
	int32 desired_team_index;
	uns32 user_update_timestamp[3];
	uns32 user_remove_timestamp[3];
};
static_assert(sizeof(s_network_session_interface_user) == 0x1698);
static_assert(0x0000 == OFFSETOF(s_network_session_interface_user, user_state));
static_assert(0x0004 == OFFSETOF(s_network_session_interface_user, player_identifier));
static_assert(0x000C == OFFSETOF(s_network_session_interface_user, controller_index));
static_assert(0x0010 == OFFSETOF(s_network_session_interface_user, player_data));
static_assert(0x1630 == OFFSETOF(s_network_session_interface_user, player_update_number));
static_assert(0x1634 == OFFSETOF(s_network_session_interface_user, override_hopper_directory));
static_assert(0x1674 == OFFSETOF(s_network_session_interface_user, player_voice_settings));
static_assert(0x1674 == OFFSETOF(s_network_session_interface_user, player_mute_mask));
static_assert(0x1676 == OFFSETOF(s_network_session_interface_user, player_voice_flags));
static_assert(0x1678 == OFFSETOF(s_network_session_interface_user, session_to_change_teams_on));
static_assert(0x167C == OFFSETOF(s_network_session_interface_user, desired_team_index));
static_assert(0x1680 == OFFSETOF(s_network_session_interface_user, user_update_timestamp));
static_assert(0x168C == OFFSETOF(s_network_session_interface_user, user_remove_timestamp));

struct s_saved_film_description
{
	int32 category;
	e_campaign_id campaign_id;
	e_map_id map_id;
	int16 campaign_insertion_point;
	bool campaign_survival_enabled;
	int32 difficulty;
	wchar_t film_path[256];
	wchar_t film_name[128];
	e_controller_index controller_index;
	int32 length_seconds;
};
static_assert(sizeof(s_saved_film_description) == 0x31C);
static_assert(0x000 == OFFSETOF(s_saved_film_description, category));
static_assert(0x004 == OFFSETOF(s_saved_film_description, campaign_id));
static_assert(0x008 == OFFSETOF(s_saved_film_description, map_id));
static_assert(0x00C == OFFSETOF(s_saved_film_description, campaign_insertion_point));
static_assert(0x00E == OFFSETOF(s_saved_film_description, campaign_survival_enabled));
static_assert(0x010 == OFFSETOF(s_saved_film_description, difficulty));
static_assert(0x014 == OFFSETOF(s_saved_film_description, film_path));
static_assert(0x214 == OFFSETOF(s_saved_film_description, film_name));
static_assert(0x314 == OFFSETOF(s_saved_film_description, controller_index));
static_assert(0x318 == OFFSETOF(s_saved_film_description, length_seconds));

#pragma pack(push, 1)
struct s_network_session_interface_globals
{
	bool initialized;
	uns8 : 8;
	c_static_wchar_string<16> machine_name;
	c_static_wchar_string<32> session_name;
	bool has_live_connection_info;
	uns8 : 8;
	s_transport_qos_result qos_result;
	int32 bandwidth_bps;
	int32 max_machine_count;
	c_flags<e_network_session_peer_properties_status_flags, uns32, k_network_session_peer_properties_status_flags> peer_status_flags;
	int16 ready_hopper_identifier;
	uns8 : 8;
	uns8 : 8;
	int32 game_start_error;
	bool was_guide_opened_during_a_multiplayer_session;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;
	e_map_id map_id;
	int32 current_map;
	int32 current_map_progress_percentage;
	int16 hopper_identifier;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;
	c_static_array<s_network_session_interface_user, k_number_of_users> users;
	uns64 game_instance;
	int32 scenario_type;
	c_static_string<128> scenario_path;
	int16 campaign_insertion_point;
	int16 map_status;
	real32 map_progress;
	s_saved_film_description local_specific_film;
	uns32 local_specific_film_time;
	c_static_array<int32, 3> session_connection_identifiers;
	c_static_array<int32, 3> session_peer_properties_update_times;
	c_static_array<int32, 3> session_membership_update_number;
	c_static_array<bool, 3> session_variant_has_teams;
	c_static_array<bool, 3> session_variant_has_sve_teams;
	c_static_array<bool, 3> session_variant_observers_allowed;
	byte __pad5EF1[0x3];
	c_static_array<int32, 3> session_variant_session_maximum_team_counts;
	c_network_session_manager* session_manager;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;
};
static_assert(sizeof(s_network_session_interface_globals) == 0x5F08);
#pragma pack(pop)

extern s_network_session_interface_globals& session_interface_globals;

extern int32 __cdecl network_squad_session_get_countdown_timer();
extern bool __cdecl network_life_cycle_in_matchmaking();
//network_squad_session_get_active_skulls_primary
//network_squad_session_get_active_skulls_secondary
extern e_campaign_difficulty_level __cdecl network_life_cycle_session_get_campaign_difficulty();
extern int16 __cdecl network_squad_session_get_campaign_insertion_point();
extern int16 __cdecl network_squad_session_get_campaign_metagame_scoring();
//sub_435160
extern s_saved_film_description const* __cdecl network_squad_session_get_film();
extern c_game_variant const* __cdecl network_life_cycle_session_get_game_variant();
extern bool __cdecl network_life_cycle_session_get_map(e_campaign_id* campaign_id, e_map_id* map_id);
extern c_map_variant const* __cdecl network_squad_session_get_map_variant();
//sub_4355A0
extern e_gui_game_mode __cdecl network_life_cycle_squad_session_get_ui_game_mode();
//sub_4356A0
extern void __cdecl network_session_check_properties(c_network_session* session);
//network_session_get_membership
extern void __cdecl network_session_inteface_update_host_framerate_quality();
extern void __cdecl network_session_interface_add_local_user(int32 user_index, s_player_identifier const* player_identifier);
//network_session_interface_calculate_map_progress
extern void __cdecl network_session_interface_clear_peer_status_flags();
extern void __cdecl network_session_interface_dispose();
//sub_436230
//simulation_debug_globals_get_game_network_quality
extern bool __cdecl network_session_interface_get_is_user_signed_in(int32 user_index);
extern bool __cdecl network_session_interface_get_is_zombie_local_user(int32 user_index);
extern bool __cdecl network_session_interface_get_live_connection_info(s_transport_qos_result* qos_result, e_online_nat_type* nat_type, int32* bandwidth_bps, int32* max_machine_count);
//network_session_interface_get_local_framerate_quality
//.text:004364A0 ; int32 __cdecl network_session_interface_get_local_user_count(void)
extern bool __cdecl network_session_interface_get_local_user_identifier(int32 user_index, s_player_identifier* player_identifier, bool allow_users_in_erroneous_states);
extern bool __cdecl network_session_interface_get_local_user_properties(int32 user_index, e_controller_index* controller_index, s_player_configuration* player_data, uns32* player_voice_settings);
extern int32 __cdecl network_session_interface_get_local_user_state(int32 user_index);
extern uns64 __cdecl network_session_interface_get_local_user_xuid(int32 user_index);
extern void __cdecl network_session_interface_handle_message(e_session_network_message message);
extern bool __cdecl network_session_interface_initialize(c_network_session_manager* session_manager);
extern bool __cdecl network_session_interface_local_user_exists(int32 user_index);
extern void __cdecl network_session_interface_notify_set_local_specific_film(s_saved_film_description const* film);
extern void __cdecl network_session_interface_remove_local_user(int32 user_index);
extern void __cdecl network_session_interface_reset(int32 session_index);
extern void __cdecl network_session_interface_set_desired_multiplayer_team(int32 user_index, int32 team_index);
extern void __cdecl network_session_interface_set_is_zombie_local_user(int32 user_index, bool is_zombie_local_user);
extern void __cdecl network_session_interface_set_live_service_qos(s_transport_qos_result const* qos_result);
extern void __cdecl network_session_interface_set_local_name(wchar_t const* machine_name, wchar_t const* session_name);
extern void __cdecl network_session_interface_set_local_user_override_hopper_directory(int32 user_index, char const* override_hopper_directory);
extern void __cdecl network_session_interface_set_local_user_properties(int32 user_index, e_controller_index controller_index, s_player_configuration const* player_data, uns32 player_voice_settings);
extern void __cdecl network_session_interface_set_local_user_state(int32 user_index, int32 state);
extern void __cdecl network_session_interface_set_local_user_xuid(int32 user_index, uns64 xuid);
extern void __cdecl network_session_interface_set_peer_status_flag(e_network_session_peer_properties_status_flags peer_status_flag, bool enabled);
extern void __cdecl network_session_interface_set_ready_hopper_identifier(uns16 hopper_identifier, e_session_game_start_error error);
extern bool __cdecl network_session_interface_test_peer_status_flag(e_network_session_peer_properties_status_flags peer_status_flag);
extern void __cdecl network_session_interface_update();
extern void __cdecl network_session_interface_update_local_state();
extern void __cdecl network_session_interface_update_session(c_network_session* session);
extern bool __cdecl network_session_interface_was_guide_opened_during_a_multiplayer_session();
extern void __cdecl network_session_set_player_failure_reason(int32 user_index, e_network_join_refuse_reason reason);
//network_session_update_local_specific_parameters
//network_session_update_local_peer_properties
//network_session_update_team_indices
extern void __cdecl network_session_update_user_properties(c_network_session* session, int32 user_index);
extern void __cdecl network_session_update_user_removal(c_network_session* session);
extern bool __cdecl network_squad_session_boot_player(int32 player_index, e_network_session_boot_reason reason);
extern bool __cdecl network_squad_session_build_status(s_network_squad_status_data* game_status);
extern bool __cdecl network_squad_session_can_set_game_settings();
extern bool __cdecl network_squad_session_controls_coop_game_options(bool* is_leader);
extern bool __cdecl network_squad_session_get_membership(int32* update_number, int32* local_peer_index, int32* host_peer_index, int32* leader_peer_index, int32* peer_count, uns32* peer_valid_flags, s_network_session_peer const** peers, int32* player_count, uns32* player_valid_flags, s_network_session_player const** players);
extern e_network_game_start_mode __cdecl network_squad_session_get_start_mode();
extern bool __cdecl network_squad_session_set_campaign_difficulty(e_campaign_difficulty_level campaign_difficulty);
extern bool __cdecl network_squad_session_set_campaign_insertion_point(int16 campaign_insertion_point);
extern bool __cdecl network_squad_session_set_closed_by_user(bool closed_by_user);
extern bool __cdecl network_squad_session_set_closed_status(int32 closed_status);
extern bool __cdecl network_squad_session_set_coop_game_options(e_campaign_id campaign_id, e_map_id map_id, char const* scenario_path);
extern bool __cdecl network_squad_session_set_film(s_saved_film_description const* film);
extern bool __cdecl network_squad_session_set_game_variant(c_game_variant const* game_variant);
extern bool __cdecl network_squad_session_set_map(e_campaign_id campaign_id, e_map_id map_id, char const* scenario_path);
extern bool __cdecl network_squad_session_set_map_variant(c_map_variant const* map_variant);
extern bool __cdecl network_squad_session_set_maximum_player_count(int32 maximum_player_count);
extern bool __cdecl network_squad_session_set_privacy_mode(int32 privacy_mode);
extern bool __cdecl network_squad_session_set_simulation_protocol(e_network_game_simulation_protocol simulation_protocol);
extern bool __cdecl network_squad_session_set_start_mode(e_network_game_start_mode start_mode);
extern bool __cdecl network_squad_session_set_ui_game_mode(e_gui_game_mode ui_game_mode);
extern bool __cdecl network_squad_session_set_ui_state(s_network_ui_state const* ui_state);
extern bool __cdecl network_squad_session_whack_to_offline_session_class();
extern bool __cdecl network_squad_session_local_peer_is_leader();
extern bool __cdecl network_session_interface_wants_main_menu_to_load();
extern bool __cdecl network_life_cycle_game_is_in_progress();
extern bool __cdecl network_squad_session_set_session_mode(e_network_session_mode session_mode);

