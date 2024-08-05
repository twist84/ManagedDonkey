#include "ai/swarms.hpp"

//.text:01494390 ; 
//.text:014943B0 ; 
//.text:014943D0 ; 
//.text:014943F0 ; 
//.text:01494400 ; 
//.text:01494410 ; 
//.text:01494420 ; void __cdecl __tls_set_g_spawner_globals_allocator(void*)
//.text:01494440 ; void __cdecl __tls_set_g_swarm_data_allocator(void*)
//.text:01494470 ; void __cdecl __tls_set_g_swarm_spawners_allocator(void*)
//.text:01494490 ; void __cdecl actor_attach_swarm(long, long)
//.text:01494570 ; void __cdecl actor_detach_swarm(long)
//.text:01494620 ; bool __cdecl ai_allow_attachment(long, bool*)
//.text:014946E0 ; bool __cdecl ai_swarm_creature_handle_attach(long, long)
//.text:014948A0 ; 
//.text:014948E0 ; 
//.text:01494920 ; 
//.text:01494960 ; 
//.text:01494980 ; 
//.text:014949A0 ; void __cdecl creature_generate_up_vector(long, swarm_creature_state*)
//.text:01494BC0 ; void __cdecl creature_state_refresh(long, swarm_creature_state*)
//.text:01494C20 ; 
//.text:01494C50 ; 
//.text:01494C80 ; 
//.text:01494CB0 ; 
//.text:01494CE0 ; 
//.text:01494D10 ; 
//.text:01494D40 ; 
//.text:01494D50 ; 
//.text:01494D60 ; void __cdecl perlin_state_reset(s_perlin_state*)
//.text:01494DE0 ; 
//.text:01494E00 ; 
//.text:01494E10 ; 
//.text:01494E50 ; 
//.text:01494E90 ; 
//.text:01494ED0 ; void __cdecl spawner_initialize_creature(short, long)
//.text:01494FA0 ; void __cdecl swarm_accelerate(long, vector3d const*)
//.text:01495020 ; bool __cdecl swarm_action_execute_command_script(long, long, vector3d*, vector3d*)
//.text:01495120 ; bool __cdecl swarm_add_creature(long, long)
//.text:014951D0 ; long __cdecl swarm_create_creature(long, long, actor_spawn_definition const*, bool)
//.text:01495660 ; bool __cdecl swarm_creature_aim_jump(long, vector3d*)
//.text:014956C0 ; void __cdecl swarm_creature_control(long)
//.text:014957B0 ; void __cdecl swarm_creature_handle_action_finished(long, long, long)
//.text:01495910 ; bool __cdecl swarm_creature_infect(long, long, object_marker const*, vector3d const*)
//.text:01495B80 ; void __cdecl swarm_creature_iterator_new(long, swarm_creature_iterator*)
//.text:01495BC0 ; creature_datum* __cdecl swarm_creature_iterator_next(swarm_creature_iterator*)
//.text:01495C20 ; bool __cdecl swarm_creature_moving_jump(long, real, real, real_point3d const*, vector3d*)
//.text:01495D20 ; void __cdecl swarm_creature_update(long, long)
//.text:014968A0 ; void __cdecl swarm_creature_update_timers(long, long, swarm_creature_state*)

void __cdecl swarm_delete(long swarm_index)
{
	INVOKE(0x014968F0, swarm_delete, swarm_index);
}

//.text:014969A0 ; void __cdecl swarm_flush_prop(long, long)
//.text:01496A20 ; void __cdecl swarm_flush_structure_indices(long)
//.text:01496A90 ; void __cdecl swarm_freeze(long)
//.text:01496B30 ; 
//.text:01496B70 ; void __cdecl swarm_handle_creature_death(long, long, short)
//.text:01496C50 ; void __cdecl swarm_handle_delete_object(long)
//.text:01496CF0 ; void __cdecl swarm_input_update(long)
//.text:01496E90 ; 
//.text:01496EE0 ; void __cdecl swarm_iterator_new(swarm_iterator*)
//.text:01496F20 ; swarm_datum* __cdecl swarm_iterator_next(swarm_iterator*)
//.text:01496F70 ; void __cdecl swarm_kill(long, bool)
//.text:01497000 ; void __cdecl swarm_movement_update(long, long)
//.text:01497380 ; short __cdecl swarm_num_spawners_of_type(long)
//.text:014973C0 ; long __cdecl swarm_perception_creature_from_swarm(long, actor_position_data*, long)
//.text:014974D0 ; bool __cdecl swarm_perception_find_sense_position(long, real_point3d const*, actor_position_data*)
//.text:014975E0 ; 
//.text:01497710 ; bool __cdecl swarm_remove_creature(long, long, bool, bool)
//.text:01497870 ; void __cdecl swarm_set_team(long, e_campaign_team)
//.text:014978E0 ; 
//.text:014979B0 ; void __cdecl swarm_spawner_definition_new(swarm_spawner_definition*)
//.text:01497A30 ; void __cdecl swarm_spawner_end(short)
//.text:01497A90 ; short __cdecl swarm_spawner_new(swarm_spawner_definition*)
//.text:01497B30 ; 
//.text:01497BA0 ; 
//.text:01497C30 ; bool __cdecl swarm_spawner_starting_location_new(actor_starting_location_definition*, short)
//.text:01497E40 ; void __cdecl swarm_spawner_new_from_defintion(swarm_spawner_definition*, s_swarm_spawner*)
//.text:01497F50 ; void __cdecl swarm_spawner_new_swarm(short)
//.text:014980D0 ; void __cdecl swarm_spawner_start(short)
//.text:01498110 ; void __cdecl swarm_spawners_handle_deleted_object(long)
//.text:01498180 ; void __cdecl swarm_spawners_update()
//.text:01498290 ; bool __cdecl swarm_target_get_info(short, long, long*, real_point3d*, real_point3d*)
//.text:014983D0 ; void __cdecl swarm_update(long)
//.text:01498580 ; void __cdecl swarms_dispose_from_old_map()
//.text:01498610 ; void __cdecl swarms_initialize()
//.text:014986C0 ; void __cdecl swarms_initialize_for_new_map()

