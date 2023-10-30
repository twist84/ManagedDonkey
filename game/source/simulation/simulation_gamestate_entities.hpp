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

extern void __cdecl simulation_gamestate_entities_dispose();
extern void __cdecl simulation_gamestate_entities_dispose_from_old_map();
extern void __cdecl simulation_gamestate_entities_initialize();
extern void __cdecl simulation_gamestate_entities_initialize_for_new_map();

