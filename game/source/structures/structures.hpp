#pragma once

#include "cseries/cseries.hpp"
#include "structures/structure_bsp_constants.hpp"

typedef c_static_array<c_static_array<int16, MAXIMUM_CLUSTERS_PER_STRUCTURE>, 16> t_cluster_activation_reason;

extern void __cdecl structures_dispose();
extern void __cdecl structures_dispose_from_old_map();
extern void __cdecl structures_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl structures_initialize();
extern void __cdecl structures_initialize_for_new_map();
extern void __cdecl structures_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);

extern void render_debug_fog_planes();

