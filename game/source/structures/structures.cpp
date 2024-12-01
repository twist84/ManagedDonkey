#include "structures/structures.hpp"

//.text:006771E0 ; 
//.text:00677240 ; 
//.text:00677470 ; bool __cdecl structure_cluster_mark(s_cluster_reference)
//.text:006774B0 ; void __cdecl structure_cluster_marker_begin()
//.text:006774D0 ; void __cdecl structure_cluster_marker_end()
//.text:006774F0 ; 
//.text:00677530 ; 
//.text:006775D0 ; 
//.text:006778A0 ; long __cdecl structure_clusters_in_sphere(s_cluster_reference, real_point3d const*, real, long, s_cluster_reference*, long*)
//.text:00677C20 ; 
//.text:00677D20 ; 
//.text:00677EE0 ; float __cdecl structure_distance_to_portal(structure_bsp const*, cluster_portal const*, real_point3d const*)
//.text:00678020 ; 
//.text:006781A0 ; 
//.text:006781C0 ; 
//.text:00678240 ; bool __cdecl structure_instanced_geometry_mark(long, long)
//.text:00678290 ; bool __cdecl structure_instanced_geometry_mark_get_user_data(long, long, long*)
//.text:006782E0 ; bool __cdecl structure_instanced_geometry_mark_with_user_data(long, long, long)
//.text:00678310 ; void __cdecl structure_instanced_geometry_marker_begin()
//.text:00678350 ; void __cdecl structure_instanced_geometry_marker_end()
//.text:00678370 ; bool __cdecl structure_instanced_geometry_unmarked(long, long)

void __cdecl structures_dispose()
{
	INVOKE(0x006783B0, structures_dispose);
}

void __cdecl structures_dispose_from_old_map()
{
	INVOKE(0x006783C0, structures_dispose_from_old_map);
}

void __cdecl structures_dispose_from_old_structure_bsp(dword deactivating_structure_bsp_mask)
{
	INVOKE(0x006783D0, structures_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl structures_initialize()
{
	INVOKE(0x006783E0, structures_initialize);
}

void __cdecl structures_initialize_for_new_map()
{
	INVOKE(0x006783F0, structures_initialize_for_new_map);
}

void __cdecl structures_initialize_for_new_structure_bsp(dword activating_structure_bsp_mask)
{
	INVOKE(0x00678400, structures_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

void render_debug_fog_planes()
{
}

