#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/transport/transport_security.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"
#include "networking/transport/transport_qos.hpp"
#include "text/unicode.hpp"

class c_game_variant;
class c_map_variant;
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
	e_saved_film_category category;
	e_campaign_id campaign_id;
	e_map_id map_id;
	int16 campaign_insertion_point;
	bool campaign_survival_enabled;
	e_campaign_difficulty_level difficulty;
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

typedef c_flags<e_network_session_peer_properties_status_flags, uns32, k_network_session_peer_properties_status_flag_count> c_network_session_peer_properties_status_flags;

struct s_network_session_interface_globals
{
	bool initialized;
	wchar_t machine_name[16];
	wchar_t session_name[32];
	bool live_service_qos_valid;
	s_transport_qos_result live_service_qos_result;
	int32 live_estimated_bandwidth_bps;
	int32 live_max_machine_count;
	c_network_session_peer_properties_status_flags flags;
	int16 ready_hopper_identifier;
	e_session_game_start_error game_start_error;
	bool guide_opened_during_multiplayer_session;
	e_map_id current_map_id;
	e_network_session_map_status current_map_status;
	int32 current_map_progress_percentage;
	uns16 current_display_level_hopper_identifier;
	byte pad[4];
	s_network_session_interface_user users[4];
	uns64 current_game_instance;
	int32 persistent_progress_scenario_type;
	char persistent_progress_scenario_path[128];
	int16 persistent_campaign_insertion_point_index;
	int16 persistent_load_status;
	real32 persistent_progress_initial;
	s_saved_film_description local_saved_film_description;
	uns32 local_saved_film_description_timestamp;
	int32 session_connection_identifier[3];
	uns32 peer_update_timestamp[3];
	int32 membership_update_number[3];
	bool variant_is_team_game[3];
	bool session_variant_has_sve_teams[3];
	bool variant_are_observers_allowed[3];
	int32 variant_team_count[3];
	c_network_session_manager* session_manager;

	//char friend_to_join[32];
	//s_transport_secure_identifier session_id_to_join;
	//bool join_with_squad;
	//bool found_friend;
	//uns32 time_of_last_join_attempt;
	//int32 join_attempt_delay_msec;
	//int32 this_join_attempt_delay_msec;
};
static_assert(sizeof(s_network_session_interface_globals) == 0x5F08);
static_assert(0x0000 == OFFSETOF(s_network_session_interface_globals, initialized));
static_assert(0x0002 == OFFSETOF(s_network_session_interface_globals, machine_name));
static_assert(0x0022 == OFFSETOF(s_network_session_interface_globals, session_name));
static_assert(0x0062 == OFFSETOF(s_network_session_interface_globals, live_service_qos_valid));
static_assert(0x0064 == OFFSETOF(s_network_session_interface_globals, live_service_qos_result));
static_assert(0x0084 == OFFSETOF(s_network_session_interface_globals, live_estimated_bandwidth_bps));
static_assert(0x0088 == OFFSETOF(s_network_session_interface_globals, live_max_machine_count));
static_assert(0x008C == OFFSETOF(s_network_session_interface_globals, flags));
static_assert(0x0090 == OFFSETOF(s_network_session_interface_globals, ready_hopper_identifier));
static_assert(0x0094 == OFFSETOF(s_network_session_interface_globals, game_start_error));
static_assert(0x0098 == OFFSETOF(s_network_session_interface_globals, guide_opened_during_multiplayer_session));
static_assert(0x009C == OFFSETOF(s_network_session_interface_globals, current_map_id));
static_assert(0x00A0 == OFFSETOF(s_network_session_interface_globals, current_map_status));
static_assert(0x00A4 == OFFSETOF(s_network_session_interface_globals, current_map_progress_percentage));
static_assert(0x00A8 == OFFSETOF(s_network_session_interface_globals, current_display_level_hopper_identifier));
static_assert(0x00B0 == OFFSETOF(s_network_session_interface_globals, users));
static_assert(0x5B10 == OFFSETOF(s_network_session_interface_globals, current_game_instance));
static_assert(0x5B18 == OFFSETOF(s_network_session_interface_globals, persistent_progress_scenario_type));
static_assert(0x5B1C == OFFSETOF(s_network_session_interface_globals, persistent_progress_scenario_path));
static_assert(0x5B9C == OFFSETOF(s_network_session_interface_globals, persistent_campaign_insertion_point_index));
static_assert(0x5B9E == OFFSETOF(s_network_session_interface_globals, persistent_load_status));
static_assert(0x5BA0 == OFFSETOF(s_network_session_interface_globals, persistent_progress_initial));
static_assert(0x5BA4 == OFFSETOF(s_network_session_interface_globals, local_saved_film_description));
static_assert(0x5EC0 == OFFSETOF(s_network_session_interface_globals, local_saved_film_description_timestamp));
static_assert(0x5EC4 == OFFSETOF(s_network_session_interface_globals, session_connection_identifier));
static_assert(0x5ED0 == OFFSETOF(s_network_session_interface_globals, peer_update_timestamp));
static_assert(0x5EDC == OFFSETOF(s_network_session_interface_globals, membership_update_number));
static_assert(0x5EE8 == OFFSETOF(s_network_session_interface_globals, variant_is_team_game));
static_assert(0x5EEB == OFFSETOF(s_network_session_interface_globals, session_variant_has_sve_teams));
static_assert(0x5EEE == OFFSETOF(s_network_session_interface_globals, variant_are_observers_allowed));
static_assert(0x5EF4 == OFFSETOF(s_network_session_interface_globals, variant_team_count));
static_assert(0x5F00 == OFFSETOF(s_network_session_interface_globals, session_manager));

extern s_network_session_interface_globals& session_interface_globals;

extern int32 __cdecl network_squad_session_get_countdown_timer();
extern bool __cdecl network_life_cycle_in_matchmaking();
extern e_campaign_difficulty_level __cdecl network_life_cycle_session_get_campaign_difficulty();
extern int16 __cdecl network_squad_session_get_campaign_insertion_point();
extern int16 __cdecl network_squad_session_get_campaign_metagame_scoring();
extern const s_saved_film_description* __cdecl network_squad_session_get_film();
extern const c_game_variant* __cdecl network_life_cycle_session_get_game_variant();
extern bool __cdecl network_life_cycle_session_get_map(e_campaign_id* campaign_id, e_map_id* map_id);
extern const c_map_variant* __cdecl network_squad_session_get_map_variant();
extern e_gui_game_mode __cdecl network_life_cycle_squad_session_get_ui_game_mode();
extern void __cdecl network_session_check_properties(c_network_session* session);
extern bool __cdecl network_session_get_membership(const c_network_session* session, int32* update_number, int32* local_peer_index, int32* host_peer_index, int32* leader_peer_index, int32* peer_count, uns32* peer_valid_flags, const s_network_session_peer** peers, int32* player_count, uns32* player_valid_flags, const s_network_session_player** players);
extern void __cdecl network_session_inteface_update_host_framerate_quality();
extern void __cdecl network_session_interface_add_local_user(int32 user_index, const s_player_identifier* player_identifier);
extern void __cdecl network_session_interface_clear_peer_status_flags();
extern void __cdecl network_session_interface_dispose();
extern bool __cdecl network_session_interface_get_is_user_signed_in(int32 user_index);
extern bool __cdecl network_session_interface_get_is_zombie_local_user(int32 user_index);
extern bool __cdecl network_session_interface_get_live_connection_info(s_transport_qos_result* qos_result, e_online_nat_type* nat_type, int32* estimated_bandwidth_bps, int32* max_machine_count);
extern bool __cdecl network_session_interface_get_local_user_identifier(int32 user_index, s_player_identifier* player_identifier, bool allow_users_in_erroneous_states);
extern bool __cdecl network_session_interface_get_local_user_properties(int32 user_index, e_controller_index* controller_index, s_player_configuration* player_data, uns32* player_voice_settings);
extern e_network_interface_user_state __cdecl network_session_interface_get_local_user_state(int32 user_index);
extern uns64 __cdecl network_session_interface_get_local_user_xuid(int32 user_index);
extern int32 __cdecl network_session_interface_get_team_index(int32 user_index);
extern void __cdecl network_session_interface_handle_message(e_session_network_message message);
extern bool __cdecl network_session_interface_initialize(c_network_session_manager* session_manager);
extern bool __cdecl network_session_interface_local_user_exists(int32 user_index);
extern void __cdecl network_session_interface_notify_set_local_specific_film(const s_saved_film_description* saved_film_description);
extern void __cdecl network_session_interface_remove_local_user(int32 user_index);
extern void __cdecl network_session_interface_reset(int32 session_index);
extern void __cdecl network_session_interface_set_desired_team_index(int32 user_index, int32 desired_team_index);
extern void __cdecl network_session_interface_set_is_zombie_local_user(int32 user_index, bool is_zombie_account);
extern void __cdecl network_session_interface_set_live_service_qos(const s_transport_qos_result* qos_result);
extern void __cdecl network_session_interface_set_local_name(const wchar_t* machine_name, const wchar_t* session_name);
extern void __cdecl network_session_interface_set_local_user_override_hopper_directory(int32 user_index, const char* override_directory);
extern void __cdecl network_session_interface_set_local_user_properties(int32 user_index, e_controller_index controller_index, const s_player_configuration* player_data, uns32 player_voice_settings);
extern void __cdecl network_session_interface_set_local_user_state(int32 user_index, e_network_interface_user_state user_state);
extern void __cdecl network_session_interface_set_local_user_xuid(int32 user_index, uns64 xuid);
extern void __cdecl network_session_interface_set_peer_status_flag(e_network_session_peer_properties_status_flags flag, bool value);
extern void __cdecl network_session_interface_set_ready_hopper_identifier(uns16 hopper_identifier, e_session_game_start_error game_start_error);
extern bool __cdecl network_session_interface_test_peer_status_flag(e_network_session_peer_properties_status_flags flag);
extern void __cdecl network_session_interface_update();
extern void __cdecl network_session_interface_update_local_state();
extern void __cdecl network_session_interface_update_session(c_network_session* session);
extern bool __cdecl network_session_interface_was_guide_opened_during_a_multiplayer_session();
extern void __cdecl network_session_set_player_failure_reason(int32 user_index, e_network_join_refuse_reason refuse_reason);
extern void __cdecl network_session_update_user_properties(c_network_session* session, int32 user_index);
extern void __cdecl network_session_update_user_removal(c_network_session* session);
extern bool __cdecl network_squad_session_boot_player(int32 player_index, e_network_session_boot_reason boot_reason);
extern bool __cdecl network_squad_session_build_status(s_network_squad_status_data* game_status);
extern bool __cdecl network_squad_session_can_set_game_settings();
extern bool __cdecl network_squad_session_controls_coop_game_options(bool* out_is_leader);
extern bool __cdecl network_squad_session_get_membership(int32* update_number, int32* local_peer_index, int32* host_peer_index, int32* leader_peer_index, int32* peer_count, uns32* peer_valid_flags, const s_network_session_peer** peers, int32* player_count, uns32* player_valid_flags, const s_network_session_player** players);
extern e_network_session_class __cdecl network_squad_session_get_session_class();
extern e_network_game_start_mode __cdecl network_squad_session_get_start_mode();
extern bool __cdecl network_squad_session_set_campaign_difficulty(e_campaign_difficulty_level campaign_difficulty);
extern bool __cdecl network_squad_session_set_campaign_insertion_point(int16 insertion_point);
extern bool __cdecl network_squad_session_set_closed_by_user(bool closed);
extern bool __cdecl network_squad_session_set_closed_status(e_network_session_closed_status closed_status);
extern bool __cdecl network_squad_session_set_coop_game_options(e_campaign_id campaign_id, e_map_id map_id, const char* scenario_path);
extern bool __cdecl network_squad_session_set_film(const s_saved_film_description* description);
extern bool __cdecl network_squad_session_set_game_variant(const c_game_variant* variant);
extern bool __cdecl network_squad_session_set_map(e_campaign_id campaign_id, e_map_id map_id, const char* scenario_path);
extern bool __cdecl network_squad_session_set_map_variant(const c_map_variant* map_variant);
extern bool __cdecl network_squad_session_set_maximum_player_count(int32 maximum_player_count);
extern bool __cdecl network_squad_session_set_session_privacy_mode(int32 privacy_mode);
extern bool __cdecl network_squad_session_set_simulation_protocol(e_network_game_simulation_protocol simulation_protocol);
extern bool __cdecl network_squad_session_set_start_mode(e_network_game_start_mode start_mode);
extern bool __cdecl network_squad_session_set_ui_game_mode(e_gui_game_mode game_mode);
extern bool __cdecl network_squad_session_set_ui_state(const s_network_ui_state* ui_state);
extern bool __cdecl network_squad_session_whack_to_offline_session_class();
extern bool __cdecl network_squad_session_local_peer_is_leader();
extern bool __cdecl network_session_interface_wants_main_menu_to_load();
extern bool __cdecl network_squad_session_set_session_mode(e_network_session_mode session_mode);

