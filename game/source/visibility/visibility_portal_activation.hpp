#pragma once

#include "cseries/cseries.hpp"

extern void __cdecl portal_activation_dispose();
extern void __cdecl portal_activation_dispose_from_old_map();
extern void __cdecl portal_activation_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl portal_activation_initialize();
extern void __cdecl portal_activation_initialize_for_new_map();
extern void __cdecl portal_activation_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);

