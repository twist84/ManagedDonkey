#pragma once

#include "cseries/cseries.hpp"
#include "networking/online/online_guide_pc.hpp"
#include "networking/transport/transport_security.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"
#include "networking/transport/transport_qos.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"

#pragma pack(push, 1)
struct s_network_squad_status_data
{
	short protocol;
	short platform;
	long executable_type;
	long executable_version;
	long compatible_version;
	long session_mode;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	long session_class;
	long session_type;
	long privacy_mode;
	wchar_t session_name[32];
	c_enum<e_life_cycle_state, short, k_life_cycle_state_count> life_cycle_state;
	s_transport_secure_identifier session_id;
	s_transport_secure_address host_address;
	s_transport_secure_key key;
	short public_slots;
	short private_slots;
	short public_players;
	short private_players;
	c_enum<e_game_mode, short, k_game_mode_count> game_mode;
	short game_state;
	short network_game_type;
	short connection_quality;
	char playlist_name[16];
	byte : 8;
	byte : 8;
	long game_engine_type;
	wchar_t game_engine_variant_name[16];
	long campaign_id;
	long campaign_difficulty_level;
	long map_id;
	short insertion_point;
	wchar_t map_variant_name[16];
	wchar_t saved_film_name[16];
	bool game_has_teams;
	byte : 8;
	short maximum_team_count;
	short score_to_win_round;
	short best_player_score;
	short player_count;
	short : 16;
	s_network_session_status_data_player players[16];
	byte : 8;
	byte : 8;
	long player_encoded_count;
	long game_timer_type;
	long game_timer_seconds;
	dword team_mask;
	long team_scores[8];
	s_matchmaking_gather_party_properties gather_party_properties;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_network_squad_status_data) == 0x164C0);

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

extern long __cdecl network_squad_session_get_countdown_timer();
extern void __cdecl network_session_interface_handle_message(long session_network_message);

