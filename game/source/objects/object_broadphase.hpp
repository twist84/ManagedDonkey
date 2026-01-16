#pragma once

#include "cseries/cseries.hpp"

struct s_object_broadphase
{
	byte __data[0x32450];
};
COMPILE_ASSERT(sizeof(s_object_broadphase) == 0x32450);

extern void __cdecl object_broadphase_dispose();
extern void __cdecl object_broadphase_dispose_from_old_map();
extern void __cdecl object_broadphase_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl object_broadphase_initialize();
extern void __cdecl object_broadphase_initialize_for_new_map();
extern void __cdecl object_broadphase_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern void __cdecl object_broadphase_update_object(int32 object_index);

