#include "objects/object_early_movers.hpp"

#include "memory/thread_local.hpp"
#include "objects/objects.hpp"
#include "render/render_debug.hpp"

//.text:00B97D30 ; void __cdecl __tls_set_g_object_early_movers_globals_allocator(void*)
//.text:00B97D50 ; 
//.text:00B97D90 ; 
//.text:00B97DC0 ; 
//.text:00B97E00 ; void __cdecl object_early_mover_delete(long)
//.text:00B97F90 ; 
//.text:00B98010 ; bool __cdecl object_early_mover_get_node_and_rigid_body_index(long, long*, long*)

bool __cdecl object_early_mover_get_obb(long object_index_array_index, s_object_early_mover_obb* obb)
{
	return INVOKE(0x00B98190, object_early_mover_get_obb, object_index_array_index, obb);
}

//.text:00B982B0 ; long __cdecl object_early_mover_local_space_index(long)
//.text:00B98340 ; void __cdecl object_early_mover_move(long, long)
//.text:00B98580 ; void __cdecl object_early_mover_new(long)

void __cdecl object_early_mover_notify_local_objects(long early_mover_object_index)
{
	INVOKE(0x00B98600, object_early_mover_notify_local_objects, early_mover_object_index);
}

//.text:00B98800 ; 

void __cdecl object_early_mover_update(long object_index, long early_mover_array_index)
{
	INVOKE(0x00B988B0, object_early_mover_update, object_index, early_mover_array_index);
}

//.text:00B989E0 ; void __cdecl object_early_movers_dispose()
//.text:00B98A00 ; void __cdecl object_early_movers_dispose_from_old_map()
//.text:00B98A20 ; void __cdecl object_early_movers_initialize()
//.text:00B98A90 ; void __cdecl object_early_movers_initialize_for_new_map()
//.text:00B98AD0 ; bool __cdecl object_get_early_mover_local_space_acceleration(long, real_point3d const*, vector3d*, vector3d*, bool, bool)
//.text:00B98CF0 ; bool __cdecl object_get_early_mover_local_space_velocity(long, vector3d*, vector3d*, bool, bool)
//.text:00B98D30 ; bool __cdecl object_get_early_mover_local_space_velocity(long, real_point3d const*, vector3d*, vector3d*, bool, bool)

void __cdecl object_get_early_movers(long const** object_early_movers, long* object_early_movers_count)
{
	INVOKE(0x00B98EB0, object_get_early_movers, object_early_movers, object_early_movers_count);

	//TLS_DATA_GET_VALUE_REFERENCE(g_object_early_movers_globals);
	//
	//ASSERT(g_object_early_movers_globals != NULL);
	//ASSERT(g_object_early_movers_globals->map_initialized);
	//
	//ASSERT(object_early_movers != NULL);
	//ASSERT(object_early_movers_count != NULL);
	//
	//ASSERT(IN_RANGE_INCLUSIVE(g_object_early_movers_globals->object_index_count, 0, MAXIMUM_OBJECT_EARLY_MOVERS_PER_MAP));
	//
	//*object_early_movers = g_object_early_movers_globals->object_indices;
	//*object_early_movers_count = g_object_early_movers_globals->object_index_count;
}

//.text:00B98EF0 ; void __cdecl object_in_early_mover_detach(long)
//.text:00B98FB0 ; void __cdecl object_in_early_mover_join(long, long)
//.text:00B99090 ; void __cdecl object_in_early_mover_join(long, long, bool)
//.text:00B99100 ; void __cdecl object_in_early_mover_physics_move(long, void (__cdecl *)(long))
//.text:00B993C0 ; void __cdecl object_in_early_mover_physics_update(long)
//.text:00B995B0 ; bool __cdecl object_in_early_mover_transform_get(long, real_matrix4x3*)
//.text:00B99610 ; bool __cdecl object_is_valid_early_mover(long, bool)
//.text:00B996A0 ; 
//.text:00B99B90 ; 

void __cdecl object_early_mover_render_debug()
{
	if (!debug_objects)
		return;

	if (!debug_objects_early_movers)
		return;

	TLS_DATA_GET_VALUE_REFERENCE(g_object_early_movers_globals);
	for (long i = 0; i < g_object_early_movers_globals->object_index_count; i++)
	{
		s_object_early_mover_obb obb{};
		if (object_early_mover_get_obb(i, &obb))
			render_debug_box_outline_oriented(true, &obb.bounds, &obb.matrix, global_real_argb_blue);
	}
}

