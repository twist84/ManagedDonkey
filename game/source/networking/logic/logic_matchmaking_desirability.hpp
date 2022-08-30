#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"

// TODO: find the actual name for this as well as a home
struct s_player_configuration_with_identifier
{
	s_player_identifier identifier;
	s_player_configuration configuration;
};
static_assert(sizeof(s_player_configuration_with_identifier) == 0x1628);

struct s_matchmaking_session_preferences
{
	long search_preference;
	long search_min_skill;
	long search_max_skill;
};
static_assert(sizeof(s_matchmaking_session_preferences) == 0xC);

struct s_matchmaking_session_player_properties
{
	long peer_index;
	long party_index;
};
static_assert(sizeof(s_matchmaking_session_player_properties) == 0x8);

struct s_matchmaking_session_properties
{
	word hopper_identifier;
	bool hopper_is_ranked;
	byte : 8;

	long player_count;
	dword_flags player_valid_flags;
	c_static_array<s_matchmaking_session_player_properties, 16> player_properties;
	long avg_skill;
	long maximum_skill_level;
	long minimum_adjusted_skill_level;
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
static_assert(sizeof(s_matchmaking_session_properties) == 0xC4);

struct s_matchmaking_gather_party_properties
{
	dword gather_seconds;
	s_matchmaking_session_preferences gather_party_preferences;
	s_matchmaking_session_properties matchmaking_desirability;
};
static_assert(sizeof(s_matchmaking_gather_party_properties) == 0xD4);

struct s_matchmaking_search_party_properties
{
	dword search_seconds;
	dword search_latency;
	s_matchmaking_session_preferences search_party_preferences;
	s_matchmaking_session_properties matchmaking_desirability;
};
static_assert(sizeof(s_matchmaking_search_party_properties) == 0xD8);
