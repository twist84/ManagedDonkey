#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_security.hpp"

// TODO: find a home
struct s_player_identifier
{
	long parts[2];
};
static_assert(sizeof(s_player_identifier) == 0x8);

// TODO: find a home
struct s_player_configuration_from_client
{
	wchar_t player_name[16];

	byte __data[0x10];
};
static_assert(sizeof(s_player_configuration_from_client) == 0x30);

// TODO: find a home
struct s_player_configuration_from_host
{
	s_player_identifier machine_identifier;
	wchar_t player_name[16];

	byte __data[0x15C8];
};
static_assert(sizeof(s_player_configuration_from_host) == 0x15F0);

// TODO: find a home
struct s_player_configuration
{
	s_player_configuration_from_client client;
	s_player_configuration_from_host host;
};
static_assert(sizeof(s_player_configuration) == 0x1620);

// TODO: find the actual name for this as well as a home
struct s_player_configuration_with_identifier
{
	s_player_identifier identifier;
	s_player_configuration configuration;
};
static_assert(sizeof(s_player_configuration_with_identifier) == 0x1628);

// TODO: find a home
struct s_matchmaking_session_preferences
{
	long search_preference;
	long search_min_skill;
	long search_max_skill;
};
static_assert(sizeof(s_matchmaking_session_preferences) == 0xC);

// TODO: find a home
struct s_matchmaking_session_player_properties
{
	long peer_index;
	long party_index;
};
static_assert(sizeof(s_matchmaking_session_player_properties) == 0x8);

// TODO: find a home
struct s_matchmaking_desirability
{
	word hopper_identifier;
	bool hopper_is_ranked;
	byte : 8;

	long player_count;
	dword_flags player_valid_flags;
	s_matchmaking_session_player_properties player_properties[16];
	long avg_skill;
	long max_skill;
	long min_skill;
	long avg_mu;
	long avg_rank;
	long party_needed;
	long party_join;
	long good_host_count;
	long party_nat_type;
	long gamer_zone;
	long gamer_region;
	long language;
	long nat_type;

	byte_flags flags;
	byte : 8;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_matchmaking_desirability) == 0xC4);

// TODO: find a home
struct s_matchmaking_gather_party_properties
{
	dword gather_seconds;
	s_matchmaking_session_preferences gather_party_preferences;
	s_matchmaking_desirability matchmaking_desirability;
};
static_assert(sizeof(s_matchmaking_gather_party_properties) == 0xD4);

struct s_matchmaking_search_party_properties
{
	dword search_seconds;
	dword search_latency;
	s_matchmaking_session_preferences search_party_preferences;
	s_matchmaking_desirability matchmaking_desirability;
};
static_assert(sizeof(s_matchmaking_search_party_properties) == 0xD8);

// TODO: find a home
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
	short life_cycle_state;
	s_transport_secure_identifier session_id;
	s_transport_secure_address host_address;
	s_transport_secure_key key;
	short public_slots;
	short private_slots;
	short public_players;
	short private_players;
	short game_mode;
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
	short player_encoded_count;
	s_player_configuration_with_identifier players[16];
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	long game_timer_type;
	long game_timer_seconds;
	long team_mask;
	long team_scores[8];
	s_matchmaking_gather_party_properties gather_party_properties;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_network_squad_status_data) == 0x164C0);
#pragma pack(pop)

// TODO: find a home
struct s_available_session
{
	bool initialized;
	byte : 8;
	byte : 8;
	byte : 8;
	dword time;
	bool has_time;
	byte : 8;
	byte : 8;
	byte : 8;
	char __dataC[0x2C];
	s_network_squad_status_data status_data;
};
static_assert(sizeof(s_available_session) == 0x164F8);

struct s_network_search_globals
{
	long search_category;
	long reference_count;
	long maximum_sessions;
	dword_flags squad_search_flags;
	struct c_allocation_base* allocation;
	long available_session_count;
	s_available_session* available_sessions;
};
static_assert(sizeof(s_network_search_globals) == 0x1C);

extern s_network_search_globals& g_network_search_globals;