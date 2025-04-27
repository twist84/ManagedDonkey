#include "ai/clump.hpp"

char const* const group_state_names[4]
{
	"Idle",
	"Postcombat",
	"Searching",
	"Attacking"
};

//.text:014AB100 ; void __cdecl clump_absorb_memory(int32, int32)
//.text:014AB350 ; void __cdecl clump_activate(int32)
//.text:014AB390 ; bool __cdecl clump_actor_berserk_allowed(int32, int32, int32)
//.text:014AB620 ; void __cdecl clump_actor_get_position(int32, real_point3d*)
//.text:014AB660 ; void __cdecl clump_actor_iterator_new(clump_actor_iterator*, int32)
//.text:014AB6A0 ; void __cdecl clump_actor_iterator_new(clump_actor_iterator*, clump_datum*)
//.text:014AB6C0 ; actor_datum* __cdecl clump_actor_iterator_next(clump_actor_iterator*)
//.text:014AB710 ; bool __cdecl clump_actor_kamikaze_allowed(int32, int32)
//.text:014AB7B0 ; void __cdecl clump_add_actor(int32, int32)
//.text:014ABB50 ; void __cdecl clump_add_prop(int32, int32)
//.text:014ABD50 ; void __cdecl clump_add_prop_ref(int32, int32)
//.text:014ABDD0 ; int32 __cdecl clump_aquire_prop(int32, int32, e_clump_importance)
//.text:014ABE90 ; int32 __cdecl clump_aquire_prop_ref(int32, int32, e_clump_importance)
//.text:014ABF40 ; int16 __cdecl clump_calculate_postombat_result(int32)
//.text:014AC060 ; bool __cdecl clump_compatible(clump_datum const*, actor_datum*)
//.text:014AC090 ; real32 __cdecl clump_consider_object(int32, int32, int32, temp_prop*, e_clump_importance)
//.text:014AC700 ; int16 __cdecl clump_cull_props(int32, real32)
//.text:014AC7D0 ; void __cdecl clump_deactivate(int32)
//.text:014AC810 ; void __cdecl clump_delete(int32)
//.text:014AC910 ; void __cdecl clump_delete_prop(clump_datum*, int32)
//.text:014ACA00 ; int32 __cdecl clump_get_clump_prop(int32, int32)
//.text:014ACAC0 ; real32 __cdecl clump_get_player_attenuation(int32, real32)
//.text:014ACB60 ; int32 __cdecl clump_get_prop_datum(int32, int32)
//.text:014ACC10 ; void __cdecl clump_handle_deleted_object(int32)
//.text:014ACE80 ; void __cdecl clump_invitations_update(int32)
//.text:014ACF10 ; void __cdecl clump_iterator_new(clump_iterator*)
//.text:014ACF50 ; clump_datum* __cdecl clump_iterator_next(clump_iterator*)
//.text:014ACFA0 ; void __cdecl clump_mark_relevance(int32)
//.text:014ACFE0 ; int32 __cdecl clump_new(real_point3d const*, int16, bool)
//.text:014AD1A0 ; int32 __cdecl clump_new_prop(int32, int16, real32, int32, prop_datum const*)
//.text:014AD260 ; void __cdecl clump_new_prop_absorb(prop_datum*, prop_datum const*)
//.text:014AD310 ; void __cdecl clump_new_prop_init(int32, int32, int16, real32, int32)
//.text:014AD570 ; int32 __cdecl clump_new_prop_internal(clump_datum*, real32)
//.text:014AD780 ; void __cdecl clump_notify_death(int32)
//.text:014AD7B0 ; void __cdecl clump_notify_grenade_throw(int32, int32)
//.text:014AD7F0 ; void __cdecl clump_notify_kill(int32, int32, int32)
//.text:014AD850 ; int16 __cdecl clump_num_props(int32)
//.text:014AD8B0 ; void __cdecl clump_perception_update(int32)
//.text:014ADD10 ; void __cdecl clump_prop_acknowledge(prop_datum*, prop_state const*)
//.text:014ADD60 ; bool __cdecl clump_prop_acknowledged_by_someone(int32)
//.text:014ADDD0 ; void __cdecl clump_prop_iterator_new(clump_prop_iterator*, int32)
//.text:014ADE10 ; prop_datum* __cdecl clump_prop_iterator_next(clump_prop_iterator*)
//.text:014ADE60 ; void __cdecl clump_prop_ref_iterator_new(clump_prop_ref_iterator*, int32)
//.text:014ADEA0 ; prop_ref_datum* __cdecl clump_prop_ref_iterator_next(clump_prop_ref_iterator*)
//.text:014ADEF0 ; int32 __cdecl clump_prop_ref_new(int32)
//.text:014ADF60 ; void __cdecl clump_prop_update(int32, int32)
//.text:014AE0B0 ; void __cdecl clump_props_update(int32)
//.text:014AE180 ; int __cdecl clump_qsort_compare_temp_props(void const*, void const*)
//.text:014AE1C0 ; void __cdecl clump_refresh_props(int32)
//.text:014AE280 ; void __cdecl clump_remove_actor(int32, int32)
//.text:014AE340 ; void __cdecl clump_remove_dead_references(int32)
//.text:014AE3C0 ; void __cdecl clump_remove_prop_ref(prop_ref_datum*)
//.text:014AE450 ; bool __cdecl clump_request_grenade_throw(int32, int32)
//.text:014AE470 ; void __cdecl clump_reset_combat_state(int32)
//.text:014AE4C0 ; void __cdecl clump_update_activation(int32)
//.text:014AE530 ; void __cdecl clump_update_behavior_state(int32)
//.text:014AE840 ; void __cdecl clump_update_player_distance(int32)
//.text:014AE950 ; void __cdecl clump_update_position(int32)
//.text:014AEA70 ; void __cdecl clumps_create_for_new_map()

void __cdecl clumps_dispose()
{
	INVOKE(0x014AEA80, clumps_dispose);
}

void __cdecl clumps_dispose_from_old_map()
{
	INVOKE(0x014AEA90, clumps_dispose_from_old_map);
}

void __cdecl clumps_initialize()
{
	INVOKE(0x014AEAB0, clumps_initialize);
}

void __cdecl clumps_initialize_for_new_map()
{
	INVOKE(0x014AEAF0, clumps_initialize_for_new_map);
}

void __cdecl clumps_update()
{
	INVOKE(0x014AEB10, clumps_update);
}

void __cdecl clumps_update_actor_membership()
{
	INVOKE(0x014AECD0, clumps_update_actor_membership);
}

void __cdecl clumps_update_state()
{
	INVOKE(0x014AEFA0, clumps_update_state);
}

//.text:014AF040 ; void __cdecl cull_dead_clumps()
//.text:014AF300 ; t_restricted_allocation_manager<1,0,0,&void __cdecl __tls_set_g_clump_data_allocator(void*)>::deallocate
//.text:014AF330 ; 
//.text:014AF360 ; 
//.text:014AF370 ; 
//.text:014AF380 ; int16 __cdecl group_state_update(c_clump_behavior_state*, real32*, real32*)
//.text:014AF400 ; 
//.text:014AF420 ; void __cdecl prop_ref_initialize(prop_ref_datum*, prop_datum const*)
//.text:014AF4B0 ; void __cdecl recursive_clump_consider_object(int32, int32, temp_prop*, int16*)
//.text:014AF570 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_clump_data_allocator(void*)>::reserve_memory
//.text:014AF5B0 ; public: void __cdecl c_clump_behavior_state::reset()
//.text:014AF5D0 ; 
//.text:014AF6C0 ; 
//.text:014AF6F0 ; public: void __cdecl c_clump_behavior_state::set_state(int16)

