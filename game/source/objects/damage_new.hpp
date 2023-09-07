#pragma once

#include "cseries/cseries.hpp"

struct s_damage_globals
{
	byte __data[0x810];
};
static_assert(sizeof(s_damage_globals) == 0x810);

