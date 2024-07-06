#pragma once

#include "cseries/cseries.hpp"

struct s_damage_owner
{
	long player_index;
	long object_index;
	byte team;
};
static_assert(sizeof(s_damage_owner) == 0xC);

extern s_damage_owner const* const& global_damage_owner_unknown;

