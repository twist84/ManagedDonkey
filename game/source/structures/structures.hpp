#pragma once

#include "cseries/cseries.hpp"

extern void __cdecl structures_dispose();
extern void __cdecl structures_dispose_from_old_map();
extern void __cdecl structures_dispose_from_old_structure_bsp(dword old_structure_bsp_mask);
extern void __cdecl structures_initialize();
extern void __cdecl structures_initialize_for_new_map();
extern void __cdecl structures_initialize_for_new_structure_bsp(dword new_structure_bsp_mask);

extern void render_debug_fog_planes();

