#pragma once

#include "cseries/cseries.hpp"
#include "networking/online/online_guide_pc.hpp"
#include "networking/transport/transport_security.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"
#include "shell/shell.hpp"

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
#pragma pack(pop)

extern long __cdecl network_squad_session_get_countdown_timer();
extern void __cdecl network_session_interface_handle_message(long session_network_message);