#pragma once

#include "cseries/cseries.hpp"

struct s_breakable_surface_globals
{
	uint8 __data[0x3CE18];
};
static_assert(sizeof(s_breakable_surface_globals) == 0x3CE18);

struct s_breakable_surface_set_broken_event :
	s_datum_header
{
	uint8 __data[0x462];
};
static_assert(sizeof(s_breakable_surface_set_broken_event) == 0x464);

extern void __cdecl breakable_surfaces_dispose();
extern void __cdecl breakable_surfaces_dispose_from_old_map();
extern void __cdecl breakable_surfaces_dispose_from_old_structure_bsp(uint32 deactivating_structure_bsp_mask);
extern void __cdecl breakable_surfaces_initialize();
extern void __cdecl breakable_surfaces_initialize_for_new_map();
extern void __cdecl breakable_surfaces_initialize_for_new_structure_bsp(uint32 activating_structure_bsp_mask);
extern void __cdecl breakable_surfaces_update();

