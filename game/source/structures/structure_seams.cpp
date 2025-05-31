#include "structures/structure_seams.hpp"

#include "memory/thread_local.hpp"
#include "structures/structure_seam_definitions.hpp"

//.text:005F3D00 ; 
//.text:005F3D20 ; 
//.text:005F3D30 ; 
//.text:005F3D40 ; 
//.text:005F3D50 ; void __cdecl __tls_set_g_structure_seam_globals_allocator(void*)
//.text:005F3D70 ; 
//.text:005F3DB0 ; 
//.text:005F3DD0 ; const s_collision_materials_extant_flags* __cdecl collision_materials_extant_flags_get(int32)
//.text:005F3DF0 ; void __cdecl collision_materials_extant_flags_initialize(int32, s_collision_materials_extant_flags*, int32)
//.text:005F3ED0 ; 
//.text:005F3F00 ; 
//.text:005F3F10 ; 
//.text:005F3F20 ; 
//.text:005F3F40 ; 
//.text:005F3F50 ; 
//.text:005F3F80 ; 
//.text:005F3F90 ; 
//.text:005F3FA0 ; 
//.text:005F3FE0 ; const c_static_flags<64>* __cdecl structure_seams_active_seams_mask_get()
//.text:005F4000 ; c_static_flags<64> __cdecl structure_seams_calculate_active_seams_mask()
//.text:005F4080 ; void __cdecl structure_seams_connect(int32, int32)
//.text:005F4350 ; bool __cdecl structure_seams_connected_cluster_references_get(int32, s_cluster_reference, bool, c_static_stack<s_cluster_reference, 32>*)
//.text:005F45B0 ; bool __cdecl structure_seams_connected_edge_get(int32, int32, int32*, int32*)
//.text:005F46C0 ; int32 __cdecl structure_seams_connected_structure_bsp_get(int32, int32)
//.text:005F4720 ; void __cdecl structure_seams_connected_structure_bsp_indices_get(int32, int32, int32*, int32*)
//.text:005F4760 ; void __cdecl structure_seams_disconnect(int32, int32)

void __cdecl structure_seams_dispose()
{
	//INVOKE(0x005F4930, structure_seams_dispose);
}

void __cdecl structure_seams_dispose_from_old_map()
{
	//INVOKE(0x005F4940, structure_seams_dispose_from_old_map);
}

void __cdecl structure_seams_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x005F4950, structure_seams_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl structure_seams_initialize()
{
	//INVOKE(0x005F4960, structure_seams_initialize);

	g_structure_seam_globals = (s_structure_seam_globals*)g_structure_seam_globals_allocator.allocate(sizeof(s_structure_seam_globals), "s_structure_seam_globals");
}

void __cdecl structure_seams_initialize_for_new_map()
{
	INVOKE(0x005F49C0, structure_seams_initialize_for_new_map);
}

void __cdecl structure_seams_initialize_for_new_structure_bsp(int32 scenario_index, const s_structure_seams* structure_seams, uns32 activating_structure_bsp_mask)
{
	INVOKE(0x005F49D0, structure_seams_initialize_for_new_structure_bsp, scenario_index, structure_seams, activating_structure_bsp_mask);
}

//.text:005F4A60 ; void __cdecl structure_seams_initialize_no_game()
//.text:005F4A90 ; structure_seam_globals_dispose_internal?
//.text:005F4B40 ; 

