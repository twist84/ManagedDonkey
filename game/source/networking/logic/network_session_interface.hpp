#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_security.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"
#include "networking/transport/transport_qos.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"

#pragma pack(push, 1)
struct s_network_session_status_data
{
	struct s_header
	{
		short protocol;
		short platform;
		long executable_type;
		long executable_version;
		long compatible_version;
	};
	static_assert(sizeof(s_header) == 0x10);

	s_header header;
	c_enum<e_network_session_mode, long, k_network_session_mode_count> session_mode;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	c_enum<e_network_session_class, long, k_network_session_class_count> session_class;
	c_enum<e_network_session_type, long, k_network_session_type_count> session_type;
	long privacy_mode;
	c_static_wchar_string<32> session_name;
	c_enum<e_life_cycle_state, short, k_life_cycle_state_count> life_cycle_state;
	s_transport_secure_identifier session_id;
	s_transport_secure_address host_address;
	s_transport_secure_key key;
	short public_slots;
	short private_slots;
	short public_players;
	short private_players;
	c_enum<e_session_game_mode, short, k_session_game_mode_count> game_mode;
	short game_state;
	c_enum<e_network_game_type, short, k_network_game_type_count> network_game_type;
	short connection_quality;
	c_static_string<16> playlist_name;
	byte : 8;
	byte : 8;
	long game_engine_type;
	c_static_wchar_string<16> game_engine_variant_name;
	long campaign_id;
	long campaign_difficulty_level;
	long map_id;
	short insertion_point;
	c_static_wchar_string<16> map_variant_name;
	c_static_wchar_string<16> saved_film_name;
	bool game_has_teams;
	byte : 8;
	short maximum_team_count;
	short score_to_win_round;
	short best_player_score;
	short player_count;
	long __unknown134;
	c_static_array<s_network_session_status_data_player, 16> players;
	long player_encoded_count;
	long game_timer_type;
	long game_timer_seconds;
	dword team_mask;
	c_static_array<long, 8> team_scores;
	s_matchmaking_gather_party_properties gather_party_properties;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_network_session_status_data) == 0x164C0);

struct s_network_session_interface_user
{
	long state;
	s_player_identifier identifier;
	long controller_index;
	s_player_configuration player_data;
	long output_user_index;
	c_static_string<64> override_hopper_directory;
	long player_voice_settings;
	byte __data78[0x20];
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
	c_enum<e_controller_index, long, k_number_of_controllers> controller_index;
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
	dword peer_status_flags;
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
	c_static_array<s_network_session_interface_user, 4> users;
	qword game_instance;
	long scenario_type;
	c_static_string<128> scenario_path;
	short campaign_insertion_point;
	short map_status;
	real map_progress;
	s_saved_film_description local_specific_film;
	dword local_specific_film_time;
	c_static_array<long, 8> session_connection_identifiers;
	byte __data5EE4[0x1C];
	c_network_session_manager* session_manager;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_network_session_interface_globals) == 0x5F08);
#pragma pack(pop)

extern s_network_session_interface_globals& session_interface_globals;

enum e_gui_game_mode;
enum e_campaign_difficulty_level;
enum e_network_game_simulation_protocol;
enum e_network_game_start_mode;
enum e_gui_game_mode;
struct s_saved_film_description;
struct c_game_variant;
struct c_map_variant;
struct s_network_ui_state;

extern long __cdecl network_squad_session_get_countdown_timer();
extern e_gui_game_mode __cdecl network_life_cycle_squad_session_get_ui_game_mode();
extern void __cdecl network_session_interface_handle_message(long session_network_message);
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

