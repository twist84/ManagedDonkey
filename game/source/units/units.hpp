#pragma once

#include "cseries/cseries.hpp"

enum e_weapon_set
{
	_weapon_set_primary = 0,
	_weapon_set_secondary,

	k_weapon_set_count
};

struct s_unit_weapon_set
{
	word set_identifier;
	c_static_array<byte, k_weapon_set_count> weapon_indices;
};

extern bool __cdecl unit_add_weapon_to_inventory(long unit_index, long object_index, long weapon_addition_method);
extern bool __cdecl unit_has_weapon_definition_index(long unit_index, long weapon_definition_index);

