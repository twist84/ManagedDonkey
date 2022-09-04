#pragma once

#include "cseries/cseries.hpp"

enum e_game_progression_level
{
	_game_progression_level_none = 0,
	_game_progression_level_normal,
	_game_progression_level_hub_and_level_is_hub,
	_game_progression_level_spoke_and_level_is_spoke,

	k_game_progression_level_count
};

struct s_persistent_campaign_player_weapon
{
	short damage_reporting_type;
	short rounds_loaded_maximum;
	short runtime_rounds_inventory_maximum;
	short rounds_loaded_amount;
};
static_assert(sizeof(s_persistent_campaign_player_weapon) == 0x8);

struct s_persistent_campaign_player
{
	bool valid;
	byte : 8;

	s_persistent_campaign_player_weapon primary_weapon;
	s_persistent_campaign_player_weapon backpack_weapon;
	s_persistent_campaign_player_weapon secondary_weapon;

	byte __unknown1A[0x4];
};
static_assert(sizeof(s_persistent_campaign_player) == 0x1E);

struct s_campaign_armaments
{
	s_persistent_campaign_player players[4];
};
static_assert(sizeof(s_campaign_armaments) == 0x78);

struct s_campaign_game_progression
{
	byte __data[0x80];
};
static_assert(sizeof(s_campaign_game_progression) == 0x80);

struct s_hub_progression
{
	s_campaign_armaments hub_armaments;
	long hub_return_to_insertion_point;
	bool hub_progression_valid;
	byte : 8;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_hub_progression) == 0x80);