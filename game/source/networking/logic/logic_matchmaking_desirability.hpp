#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"

struct s_matchmaking_session_preferences
{
	int32 search_preference;
	int32 search_min_skill;
	int32 search_max_skill;
};
static_assert(sizeof(s_matchmaking_session_preferences) == 0xC);

struct s_matchmaking_session_player_properties
{
	int32 peer_index;
	int32 party_index;
};
static_assert(sizeof(s_matchmaking_session_player_properties) == 0x8);

struct s_matchmaking_session_properties
{
	uint16 hopper_identifier;
	bool hopper_is_ranked;
	uint8 : 8;

	int32 player_count;
	uint32 player_valid_flags;
	c_static_array<s_matchmaking_session_player_properties, 16> player_properties;
	int32 avg_skill;
	int32 maximum_skill_level;
	int32 minimum_adjusted_skill_level;
	int32 avg_mu;
	int32 avg_rank;
	int32 party_needed;
	int32 party_join;
	int32 good_host_count;
	int32 party_nat_type;
	int32 gamer_zone;
	int32 gamer_region;
	uint32 language;
	int32 nat_type;

	uint8 flags;
	uint8 : 8;
	uint8 : 8;
	uint8 : 8;
};
static_assert(sizeof(s_matchmaking_session_properties) == 0xC4);

struct s_matchmaking_gather_party_properties
{
	uint32 gather_seconds;
	s_matchmaking_session_preferences gather_party_preferences;
	s_matchmaking_session_properties matchmaking_desirability;
};
static_assert(sizeof(s_matchmaking_gather_party_properties) == 0xD4);

struct s_matchmaking_search_party_properties
{
	uint32 search_seconds;
	uint32 search_latency;
	s_matchmaking_session_preferences search_party_preferences;
	s_matchmaking_session_properties matchmaking_desirability;
};
static_assert(sizeof(s_matchmaking_search_party_properties) == 0xD8);
