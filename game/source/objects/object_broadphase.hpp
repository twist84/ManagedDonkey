#pragma once

#include "cseries/cseries.hpp"

struct s_object_broadphase
{
	uint8 __data[0x32450];
};
static_assert(sizeof(s_object_broadphase) == 0x32450);

extern void __cdecl object_broadphase_dispose();
extern void __cdecl object_broadphase_dispose_from_old_map();
extern void __cdecl object_broadphase_dispose_from_old_structure_bsp(uint32 deactivating_structure_bsp_mask);
extern void __cdecl object_broadphase_initialize();
extern void __cdecl object_broadphase_initialize_for_new_map();
extern void __cdecl object_broadphase_initialize_for_new_structure_bsp(uint32 activating_structure_bsp_mask);
extern void __cdecl object_broadphase_update_object(long object_index);

