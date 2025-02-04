#include "ai/actor_dynamic_firing_position.hpp"

//.text:014A4680 ; 
//.text:014A46A0 ; 
//.text:014A46B0 ; void __cdecl __tls_set_g_dynamic_firing_set_allocator(void*)
//.text:014A46E0 ; void __cdecl actor_dynamic_firing_positions_update(long)
//.text:014A48B0 ; void __cdecl actor_dynamic_firing_set_support_object(long, long)
//.text:014A4910 ; 
//.text:014A4920 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_dynamic_firing_set_allocator(void*)>::allocate
//.text:014A4960 ; 
//.text:014A4980 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_dynamic_firing_set_allocator(void*)>::deallocate
//.text:014A49B0 ; bool __cdecl dynamic_firing_point_initialize(long, short, short, c_ai_point3d*, c_sector_ref, real_vector3d*)
//.text:014A4B50 ; bool __cdecl dynamic_firing_position_find_test_point(long, long, short, short, object_marker*, real_point3d*, c_ai_point3d*, c_sector_ref*)
//.text:014A4D90 ; s_firing_position_definition* __cdecl dynamic_firing_position_ref_get_definition(s_firing_position_ref)
//.text:014A4E00 ; bool __cdecl dynamic_firing_positions_find(long, firing_position_evaluation_context*, bool, bool)
//.text:014A5320 ; void __cdecl dynamic_firing_set_attach(long, long)
//.text:014A5390 ; void __cdecl dynamic_firing_set_discard(long)
//.text:014A5410 ; long __cdecl dynamic_firing_set_find(long, short)
//.text:014A5480 ; void __cdecl dynamic_firing_set_handle_deleted_object(long)
//.text:014A54F0 ; void __cdecl dynamic_firing_set_handle_detach(long)
//.text:014A5530 ; void __cdecl dynamic_firing_set_iterator_new(dynamic_firing_set_iterator*)
//.text:014A5560 ; dynamic_firing_set_datum* __cdecl dynamic_firing_set_iterator_next(dynamic_firing_set_iterator*)
//.text:014A5590 ; long __cdecl dynamic_firing_set_new_automatic(long)
//.text:014A5950 ; long __cdecl dynamic_firing_set_new_cover_markers(long)
//.text:014A59E0 ; long __cdecl dynamic_firing_set_new_internal(long, short)

void __cdecl dynamic_firing_sets_dispose_from_old_map()
{
	INVOKE(0x014A5AE0, dynamic_firing_sets_dispose_from_old_map);
}

void __cdecl dynamic_firing_sets_initialize()
{
	INVOKE(0x014A5B00, dynamic_firing_sets_initialize);
}

void __cdecl dynamic_firing_sets_initialize_for_new_map()
{
	INVOKE(0x014A5B40, dynamic_firing_sets_initialize_for_new_map);
}

//.text:014A5B60 ; bool __cdecl firing_set_update(long)
//.text:014A5CC0 ; 
//.text:014A5CF0 ; 
//.text:014A5D10 ; 
//.text:014A5D30 ; short __cdecl object_get_dynamic_cover_points(long, object_marker*, short*, short)
//.text:014A5E40 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_dynamic_firing_set_allocator(void*)>::reserve_memory
//.text:014A5E80 ; bool __cdecl support_object_accessible(long, long, bool, bool)

