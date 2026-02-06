#include "objects/object_early_movers.hpp"

#include "memory/thread_local.hpp"
#include "objects/objects.hpp"
#include "render/render_debug.hpp"

//.text:00B97CE0 ; 
//.text:00B97D00 ; 
//.text:00B97D10 ; 
//.text:00B97D20 ; 
//.text:00B97D30 ; void __cdecl __tls_set_g_object_early_movers_globals_allocator(void*)
//.text:00B97D50 ; 
//.text:00B97D90 ; 
//.text:00B97DC0 ; 
//.text:00B97E00 ; void __cdecl object_early_mover_delete(int32)
//.text:00B97F90 ; void __cdecl object_early_mover_delete_early_mover_childern(int32)
//.text:00B98010 ; bool __cdecl object_early_mover_get_node_and_rigid_body_index(int32, int32*, int32*)

bool __cdecl object_early_mover_get_obb(int32 object_index_array_index, s_object_early_mover_obb* obb)
{
	return INVOKE(0x00B98190, object_early_mover_get_obb, object_index_array_index, obb);
}

//.text:00B982B0 ; int32 __cdecl object_early_mover_local_space_index(int32)
//.text:00B98340 ; void __cdecl object_early_mover_move(int32, int32)
//.text:00B98580 ; void __cdecl object_early_mover_new(int32)

void __cdecl object_early_mover_notify_local_objects(int32 early_mover_object_index)
{
	INVOKE(0x00B98600, object_early_mover_notify_local_objects, early_mover_object_index);
}

//.text:00B98800 ; bool __cdecl object_early_mover_obb_test_point(const s_object_early_mover_obb*, const real_point3d*)

void __cdecl object_early_mover_update(int32 object_index, int32 early_mover_array_index)
{
	INVOKE(0x00B988B0, object_early_mover_update, object_index, early_mover_array_index);
}

void __cdecl object_early_movers_dispose()
{
	INVOKE(0x00B989E0, object_early_movers_dispose);
}

void __cdecl object_early_movers_dispose_from_old_map()
{
	INVOKE(0x00B98A00, object_early_movers_dispose_from_old_map);
}

void __cdecl object_early_movers_initialize()
{
	INVOKE(0x00B98A20, object_early_movers_initialize);
}

void __cdecl object_early_movers_initialize_for_new_map()
{
	INVOKE(0x00B98A90, object_early_movers_initialize_for_new_map);
}

//.text:00B98AD0 ; bool __cdecl object_get_early_mover_local_space_acceleration(int32, const real_point3d*, real_vector3d*, real_vector3d*, bool, bool)

bool __cdecl object_get_early_mover_local_space_velocity(int32 moving_object_index, real_vector3d* linear_local_space_velocity, real_vector3d* angular_local_space_velocity, bool localized_phyiscs_early_movers_only, bool current)
{
	return INVOKE(0x00B98CF0, object_get_early_mover_local_space_velocity, moving_object_index, linear_local_space_velocity, angular_local_space_velocity, localized_phyiscs_early_movers_only, current);
}

//.text:00B98D30 ; bool __cdecl object_get_early_mover_local_space_velocity(int32, const real_point3d*, real_vector3d*, real_vector3d*, bool, bool)

void __cdecl object_get_early_movers(const int32** object_early_movers, int32* object_early_movers_count)
{
	INVOKE(0x00B98EB0, object_get_early_movers, object_early_movers, object_early_movers_count);

	//ASSERT(g_object_early_movers_globals != nullptr);
	//ASSERT(g_object_early_movers_globals->map_initialized);
	//
	//ASSERT(object_early_movers != nullptr);
	//ASSERT(object_early_movers_count != nullptr);
	//
	//ASSERT(IN_RANGE_INCLUSIVE(g_object_early_movers_globals->object_index_count, 0, MAXIMUM_OBJECT_EARLY_MOVERS_PER_MAP));
	//
	//*object_early_movers = g_object_early_movers_globals->object_indices;
	//*object_early_movers_count = g_object_early_movers_globals->object_index_count;
}

//.text:00B98EF0 ; void __cdecl object_in_early_mover_detach(int32)

void __cdecl object_in_early_mover_join(int32 object_index, int32 joining_object_index)
{
	INVOKE(0x00B98FB0, object_in_early_mover_join, object_index, joining_object_index);
}

//.text:00B99090 ; void __cdecl object_in_early_mover_join(int32 early_mover_object_index, int32 joining_object_index, bool accept)
//.text:00B99100 ; void __cdecl object_in_early_mover_physics_move(int32, void (__cdecl *)(int32))
//.text:00B993C0 ; void __cdecl object_in_early_mover_physics_update(int32)
//.text:00B995B0 ; bool __cdecl object_in_early_mover_transform_get(int32, real_matrix4x3*)
//.text:00B99610 ; bool __cdecl object_is_valid_early_mover(int32, bool)
//.text:00B996A0 ; bool __cdecl object_move_in_early_mover_local_space(int32, real_vector3d*, real_vector3d*, bool)
//.text:00B99B90 ; 

void __cdecl object_early_mover_render_debug()
{
	if (!debug_objects)
		return;

	if (!debug_objects_early_movers)
		return;

	for (int32 i = 0; i < g_object_early_movers_globals->object_index_count; i++)
	{
		s_object_early_mover_obb obb{};
		if (object_early_mover_get_obb(i, &obb))
			render_debug_box_outline_oriented(true, &obb.aabb, &obb.transform, global_real_argb_blue);
	}
}

