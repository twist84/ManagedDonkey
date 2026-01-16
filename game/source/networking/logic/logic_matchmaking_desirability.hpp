#pragma once

#include "cseries/cseries.hpp"

enum e_gamer_zone;
enum e_gamer_region;
enum e_language;
enum e_matchmaking_search_preference;

struct s_matchmaking_session_player_properties
{
	int32 peer_index;
	int32 party_index;
};
COMPILE_ASSERT(sizeof(s_matchmaking_session_player_properties) == 0x8);

enum e_network_session_matchmaking_composition_flags
{
	_network_session_matchmaking_composition_session_of_quitters_bit = 0,
	_network_session_matchmaking_composition_mixed_skill_bit,
	_network_session_matchmaking_composition_enable_mixed_skill_restriction_bit,
	_network_session_matchmaking_composition_valid_bit,

	k_network_session_matchmaking_composition_flags_count,
};

typedef c_flags_no_init<e_network_session_matchmaking_composition_flags, uns8, k_network_session_matchmaking_composition_flags_count> c_network_session_matchmaking_composition_flags;

struct s_matchmaking_session_properties
{
	uns16 hopper_identifier;
	bool hopper_ranked;
	int32 player_count;
	c_static_flags_no_init<16> player_valid_flags;
	c_static_array<s_matchmaking_session_player_properties, 16> players;
	int32 average_adjusted_skill_level;
	int32 maximum_skill_level;
	int32 minimum_adjusted_skill_level;
	real32 average_mu;
	int32 average_experience_rank;
	uns32 party_needed_mask;
	uns32 party_can_join_mask;
	int32 good_host_count;
	e_online_nat_type most_restrictive_nat_type;
	e_gamer_zone gamer_zone;
	e_gamer_region gamer_region;
	e_language language;
	e_online_nat_type nat_type;
	c_network_session_matchmaking_composition_flags flags;
};
COMPILE_ASSERT(sizeof(s_matchmaking_session_properties) == 0xC4);

struct s_matchmaking_search_party_preferences
{
	e_matchmaking_search_preference preference;
	int32 minimum_skill_level;
	int32 maximum_skill_level;
};
COMPILE_ASSERT(sizeof(s_matchmaking_search_party_preferences) == 0xC);

struct s_matchmaking_gather_party_properties
{
	int32 gather_time_seconds;
	s_matchmaking_search_party_preferences preferences;
	s_matchmaking_session_properties session_properties;
};
COMPILE_ASSERT(sizeof(s_matchmaking_gather_party_properties) == 0xD4);

struct s_matchmaking_search_party_properties
{
	int32 searching_time_seconds;
	int32 connection_latency_ms;
	s_matchmaking_search_party_preferences preferences;
	s_matchmaking_session_properties session_properties;
};
COMPILE_ASSERT(sizeof(s_matchmaking_search_party_properties) == 0xD8);

