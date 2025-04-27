#pragma once

#include "cseries/cseries.hpp"

struct s_damage_owner
{
	long player_index;
	long object_index;
	uint8 team;
};
static_assert(sizeof(s_damage_owner) == 0xC);

extern s_damage_owner const* const& global_damage_owner_unknown;

extern void __cdecl damage_compute_damage_owner_from_object_index(long object_index, s_damage_owner* damage_owner);

