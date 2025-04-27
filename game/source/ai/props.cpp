#include "ai/props.hpp"

//.text:014A6770 ; 
//.text:014A6790 ; 
//.text:014A67B0 ; 
//.text:014A67D0 ; 
//.text:014A67E0 ; 
//.text:014A67F0 ; 
//.text:014A6800 ; void __cdecl __tls_set_g_prop_data_allocator(void*)
//.text:014A6830 ; void __cdecl __tls_set_g_prop_ref_data_allocator(void*)
//.text:014A6860 ; void __cdecl __tls_set_g_tracking_data_allocator(void*)
//.text:014A6890 ; void __cdecl actor_disregard_orphans(int32)
//.text:014A6910 ; int16 __cdecl actor_flush_prop_tracks(int32, real32)
//.text:014A6AC0 ; void __cdecl actor_prop_abandon_search(int32, int32, int16)
//.text:014A6B80 ; void __cdecl actor_prop_update_new_position(int32, int32, bool)
//.text:014A6E20 ; void __cdecl actor_unacknowledge_orphans(int32)
//.text:014A6EF0 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_prop_data_allocator(void*)>::allocate
//.text:014A6F30 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_prop_ref_data_allocator(void*)>::allocate
//.text:014A6F70 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_tracking_data_allocator(void*)>::allocate
//.text:014A6FB0 ; bool __cdecl allow_orphanhood(prop_ref_datum*)
//.text:014A7040 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_prop_data_allocator(void*)>::deallocate
//.text:014A7070 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_prop_ref_data_allocator(void*)>::deallocate
//.text:014A70A0 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_tracking_data_allocator(void*)>::deallocate
//.text:014A70D0 ; 
//.text:014A7100 ; 
//.text:014A7130 ; 
//.text:014A7160 ; 
//.text:014A7170 ; 
//.text:014A7180 ; bool __cdecl pref_inspected_orphan(prop_ref_datum*)
//.text:014A71D0 ; bool __cdecl pref_unassailable(int32)
//.text:014A7230 ; bool __cdecl pref_unreachable(int32)
//.text:014A7290 ; bool __cdecl prop_acknowledge(int32, int32, bool)
//.text:014A7450 ; bool __cdecl prop_acknowledged_once(prop_ref_datum*)
//.text:014A74B0 ; bool __cdecl prop_allow_state_update(prop_datum const*)
//.text:014A74F0 ; bool __cdecl prop_essential(int32, int32)
//.text:014A7540 ; int16 __cdecl prop_get_audibility(int32, int32, actor_position_data*, real_point3d const*, s_location const*, int16)
//.text:014A7650 ; bool __cdecl prop_get_battle_vector(int32, real_vector3d*, bool*)
//.text:014A7720 ; int16 __cdecl prop_get_ineffability(int32, int32)
//.text:014A77C0 ; int32 __cdecl prop_get_target_object_index(int32)
//.text:014A7880 ; int16 __cdecl prop_get_visibility(int32, int32, bool, int16, actor_position_data*, actor_position_data const*, bool*)
//.text:014A7A20 ; void __cdecl prop_iterator_new(prop_iterator*)
//.text:014A7A50 ; prop_datum* __cdecl prop_iterator_next(prop_iterator*)
//.text:014A7A80 ; int32 __cdecl prop_object_get_actor_index(int32)
//.text:014A7AF0 ; void __cdecl prop_orphan(int32, int32, bool)
//.text:014A7DA0 ; bool __cdecl prop_perceive(int32, int32, int16, int32, int32, int32)
//.text:014A7DD0 ; bool __cdecl prop_perceive(int32, int32, int16, e_prop_stimulus, int16, int32, int32, int32)
//.text:014A8250 ; void __cdecl prop_ref_delete(int32, int32, int32)
//.text:014A82D0 ; int32 __cdecl prop_ref_get_acknowledged_by_object_index(int32, int32)
//.text:014A8380 ; int32 __cdecl prop_ref_get_by_object_index(int32, int32)
//.text:014A83A0 ; int32 __cdecl prop_ref_get_by_object_index(int32, int32, bool, e_clump_importance)
//.text:014A84E0 ; void __cdecl prop_ref_reset_search(int32, bool)
//.text:014A8570 ; void __cdecl prop_remove_tracking(int32, int32)
//.text:014A85D0 ; bool __cdecl prop_should_refresh_state(prop_ref_datum const*)
//.text:014A8600 ; bool __cdecl prop_should_refresh_status(prop_ref_datum const*)
//.text:014A8660 ; prop_state* __cdecl prop_state_get(int32)

prop_state* __cdecl prop_state_get(prop_ref_datum const* pref)
{
	return INVOKE(0x014A86D0, prop_state_get, pref);
}

//.text:014A8720 ; void __cdecl prop_state_new(prop_state*)
//.text:014A87A0 ; bool __cdecl prop_state_refresh(prop_state*, int32, int16, actor_position_data*)
//.text:014A8AD0 ; int32 __cdecl prop_track(int32, int32)
//.text:014A8C60 ; real32 __cdecl prop_track_weight(int32)
//.text:014A8D50 ; void __cdecl prop_unacknowledge(int32, int32)
//.text:014A8DD0 ; prop_view* __cdecl prop_view_get(int32)
//.text:014A8E20 ; prop_view* __cdecl prop_view_get(prop_ref_datum const*)
//.text:014A8E60 ; void __cdecl prop_view_new(prop_view*)
//.text:014A8EF0 ; void __cdecl prop_view_reset_search(prop_view*)

void __cdecl props_dispose()
{
	INVOKE(0x014A8F20, props_dispose);
}

void __cdecl props_dispose_from_old_map()
{
	INVOKE(0x014A8F30, props_dispose_from_old_map);
}

void __cdecl props_initialize()
{
	INVOKE(0x014A8F70, props_initialize);
}

void __cdecl props_initialize_for_new_map()
{
	INVOKE(0x014A8FF0, props_initialize_for_new_map);
}

//.text:014A9030 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_prop_data_allocator(void*)>::reserve_memory
//.text:014A9070 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_prop_ref_data_allocator(void*)>::reserve_memory
//.text:014A90B0 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_tracking_data_allocator(void*)>::reserve_memory
//.text:014A90F0 ; bool __cdecl track_prop_ref(prop_ref_datum const*)
//.text:014A9110 ; bool __cdecl tracking_data_cleanup(int32, real32)
//.text:014A9320 ; 
//.text:014A9350 ; 
//.text:014A9380 ; bool __cdecl uninspected_orphan(prop_ref_datum*)

void __cdecl actor_prop_ref_iterator_new(actor_prop_ref_iterator* iterator, int32 actor_index)
{
	INVOKE(0x014AAEC0, actor_prop_ref_iterator_new, iterator, actor_index);
}

prop_ref_datum* __cdecl actor_prop_ref_iterator_next(actor_prop_ref_iterator* iterator)
{
	return INVOKE(0x014AAF00, actor_prop_ref_iterator_next, iterator);
}

