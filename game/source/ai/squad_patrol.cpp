#include "ai/squad_patrol.hpp"

//.text:014B97F0 ; 
//.text:014B9810 ; 
//.text:014B9820 ; void __cdecl __tls_set_g_squad_patrol_data_allocator(void*)
//.text:014B9850 ; void __cdecl actor_transition_data_clear(int32)
//.text:014B98A0 ; void __cdecl actor_transition_data_set(int32, int32, int32, bool)
//.text:014B9950 ; void __cdecl actor_transition_data_update(int32)
//.text:014B9C80 ; 
//.text:014B9CE0 ; 
//.text:014B9CF0 ; 
//.text:014B9D30 ; 
//.text:014B9D50 ; 
//.text:014B9D60 ; 
//.text:014B9D80 ; 
//.text:014B9DB0 ; 
//.text:014B9DC0 ; 
//.text:014B9DF0 ; 
//.text:014B9E00 ; 
//.text:014B9E10 ; 
//.text:014B9E30 ; 
//.text:014B9E70 ; 
//.text:014B9EA0 ; bool __cdecl squad_arrival_imminent(int32)
//.text:014B9F40 ; 
//.text:014B9F50 ; void __cdecl squad_patrol_attach_sectors(int32)
//.text:014BA010 ; void __cdecl squad_patrol_clear_sector_indices(int32)

void __cdecl squad_patrol_dispose()
{
	INVOKE(0x014BA0D0, squad_patrol_dispose);
}

void __cdecl squad_patrol_dispose_from_old_map()
{
	INVOKE(0x014BA0E0, squad_patrol_dispose_from_old_map);
}

//.text:014BA100 ; int32 __cdecl squad_patrol_get_squad_member_index(int32, int16)
//.text:014BA140 ; int32 __cdecl squad_patrol_get_squad_patrol_index(int16, int32*)
//.text:014BA200 ; int32 __cdecl squad_patrol_get_transitions(int32, int32, s_patrol_transition*, int32, bool, bool*)

void __cdecl squad_patrol_initialize()
{
	INVOKE(0x014BA650, squad_patrol_initialize);
}

void __cdecl squad_patrol_initialize_for_new_map()
{
	INVOKE(0x014BA690, squad_patrol_initialize_for_new_map);
}

//.text:014BA790 ; void __cdecl squad_patrol_iterator_new(squad_patrol_iterator*)
//.text:014BA7C0 ; s_squad_patrol_datum* __cdecl squad_patrol_iterator_next(squad_patrol_iterator*)
//.text:014BA7F0 ; void __cdecl squad_patrol_member_assign_to_point(int32, int32, const s_patrol_transition*)
//.text:014BA810 ; void __cdecl squad_patrol_member_assign_to_point(int32, int32, const s_patrol_transition*, int16)
//.text:014BAA10 ; bool __cdecl squad_patrol_member_attempt_pass_through(int32, int32)
//.text:014BAC60 ; void __cdecl squad_patrol_member_check_combat_status(int32, int32)
//.text:014BACE0 ; void __cdecl squad_patrol_member_clear_state(s_squad_patrol_member_state*)
//.text:014BAD20 ; bool __cdecl squad_patrol_member_select_point(int32, int32, const real_vector3d*, int16, s_patrol_transition*)
//.text:014BB3A0 ; void __cdecl squad_patrol_member_set_state(int32, int32, int16)
//.text:014BB400 ; bool __cdecl squad_patrol_member_should_run(int32, int32)
//.text:014BB450 ; bool __cdecl squad_patrol_member_should_search(int32, int32)
//.text:014BB4A0 ; void __cdecl squad_patrol_new(int32)
//.text:014BB560 ; void __cdecl squad_patrol_point_clear_state(s_squad_patrol_point_state*)
//.text:014BB580 ; bool __cdecl squad_patrol_setup_path_waypoints(const s_actor_patrol_state*, path_input*)
//.text:014BB6D0 ; void __cdecl squad_patrol_squad_reset(int32)
//.text:014BB710 ; 

void __cdecl squad_patrol_update()
{
	INVOKE(0x014BB720, squad_patrol_update);
}

//.text:014BB7B0 ; void __cdecl squad_patrol_update_member_state(int32, int32)
//.text:014BBD40 ; void __cdecl squad_patrol_update_patrol(int32)
//.text:014BBD80 ; void __cdecl squad_patrol_update_stragglers(int32, int32)
//.text:014B97F0 ; 
//.text:014B97F0 ; 
//.text:014B97F0 ; 
//.text:014B97F0 ; 
//.text:014B97F0 ; 
//.text:014B97F0 ; 

