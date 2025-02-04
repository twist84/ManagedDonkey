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
//.text:014A6890 ; void __cdecl actor_disregard_orphans(long)
//.text:014A6910 ; short __cdecl actor_flush_prop_tracks(long, real)
//.text:014A6AC0 ; void __cdecl actor_prop_abandon_search(long, long, short)
//.text:014A6B80 ; void __cdecl actor_prop_update_new_position(long, long, bool)
//.text:014A6E20 ; void __cdecl actor_unacknowledge_orphans(long)
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
//.text:014A71D0 ; bool __cdecl pref_unassailable(long)
//.text:014A7230 ; bool __cdecl pref_unreachable(long)
//.text:014A7290 ; bool __cdecl prop_acknowledge(long, long, bool)
//.text:014A7450 ; bool __cdecl prop_acknowledged_once(prop_ref_datum*)
//.text:014A74B0 ; bool __cdecl prop_allow_state_update(prop_datum const*)
//.text:014A74F0 ; bool __cdecl prop_essential(long, long)
//.text:014A7540 ; short __cdecl prop_get_audibility(long, long, actor_position_data*, real_point3d const*, s_location const*, short)
//.text:014A7650 ; bool __cdecl prop_get_battle_vector(long, real_vector3d*, bool*)
//.text:014A7720 ; short __cdecl prop_get_ineffability(long, long)
//.text:014A77C0 ; long __cdecl prop_get_target_object_index(long)
//.text:014A7880 ; short __cdecl prop_get_visibility(long, long, bool, short, actor_position_data*, actor_position_data const*, bool*)
//.text:014A7A20 ; void __cdecl prop_iterator_new(prop_iterator*)
//.text:014A7A50 ; prop_datum* __cdecl prop_iterator_next(prop_iterator*)
//.text:014A7A80 ; long __cdecl prop_object_get_actor_index(long)
//.text:014A7AF0 ; void __cdecl prop_orphan(long, long, bool)
//.text:014A7DA0 ; bool __cdecl prop_perceive(long, long, short, long, long, long)
//.text:014A7DD0 ; bool __cdecl prop_perceive(long, long, short, e_prop_stimulus, short, long, long, long)
//.text:014A8250 ; void __cdecl prop_ref_delete(long, long, long)
//.text:014A82D0 ; long __cdecl prop_ref_get_acknowledged_by_object_index(long, long)
//.text:014A8380 ; long __cdecl prop_ref_get_by_object_index(long, long)
//.text:014A83A0 ; long __cdecl prop_ref_get_by_object_index(long, long, bool, e_clump_importance)
//.text:014A84E0 ; void __cdecl prop_ref_reset_search(long, bool)
//.text:014A8570 ; void __cdecl prop_remove_tracking(long, long)
//.text:014A85D0 ; bool __cdecl prop_should_refresh_state(prop_ref_datum const*)
//.text:014A8600 ; bool __cdecl prop_should_refresh_status(prop_ref_datum const*)
//.text:014A8660 ; prop_state* __cdecl prop_state_get(long)
//.text:014A86D0 ; prop_state* __cdecl prop_state_get(prop_ref_datum const*)
//.text:014A8720 ; void __cdecl prop_state_new(prop_state*)
//.text:014A87A0 ; bool __cdecl prop_state_refresh(prop_state*, long, short, actor_position_data*)
//.text:014A8AD0 ; long __cdecl prop_track(long, long)
//.text:014A8C60 ; real __cdecl prop_track_weight(long)
//.text:014A8D50 ; void __cdecl prop_unacknowledge(long, long)
//.text:014A8DD0 ; prop_view* __cdecl prop_view_get(long)
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
//.text:014A9110 ; bool __cdecl tracking_data_cleanup(long, real)
//.text:014A9320 ; 
//.text:014A9350 ; 
//.text:014A9380 ; bool __cdecl uninspected_orphan(prop_ref_datum*)

void __cdecl actor_prop_ref_iterator_new(actor_prop_ref_iterator* iterator, long actor_index)
{
	INVOKE(0x014AAEC0, actor_prop_ref_iterator_new, iterator, actor_index);
}

prop_ref_datum* __cdecl actor_prop_ref_iterator_next(actor_prop_ref_iterator* iterator)
{
	return INVOKE(0x014AAF00, actor_prop_ref_iterator_next, iterator);
}

