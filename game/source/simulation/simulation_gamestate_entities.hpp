#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_simulation_gamestate_entity : s_datum_header
{
	word __unknown2;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(s_simulation_gamestate_entity) == 0x10);

