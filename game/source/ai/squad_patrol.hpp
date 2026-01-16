#pragma once

#include "cseries/cseries.hpp"

struct s_squad_patrol_datum :
	s_datum_header
{
	byte __data[0x6C2];
};
COMPILE_ASSERT(sizeof(s_squad_patrol_datum) == 0x6C4);

struct s_squad_patrol_definition;

extern void __cdecl squad_patrol_dispose();
extern void __cdecl squad_patrol_dispose_from_old_map();
extern void __cdecl squad_patrol_initialize();
extern void __cdecl squad_patrol_initialize_for_new_map();
extern void __cdecl squad_patrol_update();

