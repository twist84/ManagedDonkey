#pragma once

#include "cseries/cseries.hpp"

#pragma warning(push)
#pragma warning(disable : 26495)
struct s_campaign_armaments_weapon
{
	c_enum<e_damage_reporting_type, int16, _damage_reporting_type_unknown, k_damage_reporting_type_count> damage_reporting_type;
	int16 rounds_loaded_maximum;
	int16 runtime_rounds_inventory_maximum;
	int16 rounds_loaded_amount;
};
COMPILE_ASSERT(sizeof(s_campaign_armaments_weapon) == 0x8);

struct s_campaign_armaments_player
{
	bool valid;
	uns8 : 8;

	s_campaign_armaments_weapon primary_weapon;
	s_campaign_armaments_weapon backpack_weapon;
	s_campaign_armaments_weapon secondary_weapon;
	c_static_array<uns8, 4> grenade_counts;
};
COMPILE_ASSERT(sizeof(s_campaign_armaments_player) == 0x1E);

struct s_campaign_armaments
{
	c_static_array<s_campaign_armaments_player, 4> players;
};
COMPILE_ASSERT(sizeof(s_campaign_armaments) == 0x78);

struct s_campaign_game_progression
{
	// int32[32]?
	c_static_array<uns32, 32> integer_names;
};
COMPILE_ASSERT(sizeof(s_campaign_game_progression) == 0x80);

struct s_hub_progression
{
	s_campaign_armaments hub_armaments;
	int32 hub_return_to_insertion_point;
	bool hub_progression_valid;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;
};
COMPILE_ASSERT(sizeof(s_hub_progression) == 0x80);
#pragma warning(pop)

extern int32 __cdecl game_progression_get_last_level();
extern bool __cdecl game_progression_level_has_gameplay(int32 level_index);
extern bool __cdecl game_progression_level_is_hub(int32 level_index);

