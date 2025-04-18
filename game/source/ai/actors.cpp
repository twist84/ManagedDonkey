#include "ai/actors.hpp"

#include "ai/swarms.hpp"
#include "memory/thread_local.hpp"

//.text:01428390 ; 
//.text:014283B0 ; 
//.text:014283D0 ; 
//.text:014283F0 ; 
//.text:01428400 ; 
//.text:01428410 ; 

real_argb_color const* __cdecl actor_activation_debug_color(long actor_index)
{
	return INVOKE(0x01428470, actor_activation_debug_color, actor_index);
}

//.text:014284B0 ; void __cdecl actor_attach_unit(long, long)
//.text:01428590 ; bool __cdecl actor_attacking_target(long, real_vector3d*)
//.text:01428640 ; void __cdecl actor_berserk_update(long)
//.text:01428700 ; void __cdecl actor_braindead(long, bool)
//.text:01428760 ; void __cdecl actor_break(long, bool)
//.text:014287B0 ; void __cdecl actor_broken_update(long)
//.text:01428820 ; bool __cdecl actor_bubbleshield_attached(long)
//.text:014288D0 ; void __cdecl actor_change_squad(long, short)
//.text:014289F0 ; void __cdecl actor_clear_orders(long)
//.text:01428A50 ; void __cdecl actor_clear_output(long)
//.text:01428B20 ; void __cdecl actor_clear_spawn_definition(actor_spawn_definition*)
//.text:01428BC0 ; void __cdecl actor_cover_update(long)
//.text:01428CA0 ; long __cdecl actor_create(long, long, long, bool)
//.text:01428CC0 ; long __cdecl actor_create_internal(long, long, long, long, bool)
//.text:01428E80 ; bool __cdecl actor_create_kamikaze_grenades(long)
//.text:01428EE0 ; void __cdecl actor_customize_unit(long, long, long, long, short, long)
//.text:014291D0 ; void __cdecl actor_customize_weapon(long, long)

bool __cdecl actor_datum_available_to_current_thread()
{
	//return INVOKE(0x01429350, actor_datum_available_to_current_thread);

	TLS_DATA_GET_VALUE_REFERENCE(actor_data);
	return actor_data != NULL;
}

void __cdecl actor_delete(long actor_index, bool died)
{
	INVOKE(0x01429370, actor_delete, actor_index, died);
}

//.text:014296F0 ; real __cdecl actor_destination_tolerance(long)
//.text:01429810 ; void __cdecl actor_detach_from_unit(long)
//.text:01429870 ; void __cdecl actor_died(long)
//.text:01429DB0 ; void __cdecl actor_dont_do_avoidance(long, bool)

void __cdecl actor_erase(long actor_index, bool delete_immediately)
{
	//INVOKE(0x01429DF0, actor_erase, actor_index, delete_immediately);

	TLS_DATA_GET_VALUE_REFERENCE(actor_data);

	actor_datum* actor = DATUM_GET(actor_data, actor_datum, actor_index);
	long unit_index = actor->meta.unit_index;
	long swarm_index = actor->meta.swarm_index;
	
	actor_delete(actor_index, false);
	if (unit_index == NONE)
	{
		if (swarm_index != NONE)
			swarm_delete(swarm_index);
	}
	else if (delete_immediately)
	{
		object_delete_immediately(unit_index);
	}
	else
	{
		object_delete(unit_index);
	}
}

//.text:01429E60 ; void __cdecl actor_estimate_position(long, short, real_point3d const*, real_vector3d const*, real_vector3d const*, real_vector3d const*, real_point3d*)
//.text:01429ED0 ; void __cdecl actor_firing_position_hash_set_address(void*)
//.text:01429F00 ; void __cdecl actor_flush_position_indices(long, bool)
//.text:0142A010 ; void __cdecl actor_flush_prop(long, long)
//.text:0142A1C0 ; void __cdecl actor_flush_structure_indices(long)
//.text:0142A300 ; void __cdecl actor_force_combat_status(long, short)
//.text:0142A370 ; void __cdecl actor_freeze(long)
//.text:0142A420 ; void __cdecl actor_freeze_unit(long, long)

bool __cdecl actor_general_update(long actor_index)
{
	return INVOKE(0x0142A480, actor_general_update, actor_index);
}

//.text:0142A630 ; bool __cdecl actor_get_active_camo_state(long)
//.text:0142A680 ; long __cdecl actor_get_backpack_weapon(long)
//.text:0142A6C0 ; bool __cdecl actor_get_crouch_velocity(long, real*)
//.text:0142A710 ; short __cdecl actor_get_desired_area_type(long)
//.text:0142A770 ; short __cdecl actor_get_enemies_in_sphere(long, real_point3d*, real, long*, short)
//.text:0142A860 ; double __cdecl actor_get_gravity_scale(long)
//.text:0142A8D0 ; double __cdecl actor_get_maximum_damage_velocity(long)
//.text:0142A970 ; short __cdecl actor_get_rank(long)
//.text:0142A9C0 ; bool __cdecl actor_get_running_blind_vector(long, real_vector3d*)
//.text:0142AAF0 ; long __cdecl actor_get_secondary_weapon(long)
//.text:0142AB50 ; short __cdecl actor_get_standing(long)
//.text:0142ABD0 ; bool __cdecl actor_get_task_direction(long, real_vector3d*)
//.text:0142AC90 ; void __cdecl actor_get_timeslices(long)
//.text:0142ADB0 ; long __cdecl actor_get_weapon(long)
//.text:0142AE50 ; void __cdecl actor_handle_change_internal(long, bool, bool)
//.text:0142AF40 ; void __cdecl actor_handle_damage(long, short, long, long, real, real_vector3d const*)
//.text:0142B2C0 ; void __cdecl actor_handle_deleted_actor(long, long)
//.text:0142B350 ; void __cdecl actor_handle_deleted_object(long, long)
//.text:0142B530 ; void __cdecl actor_handle_equipment_delete(long)
//.text:0142B560 ; void __cdecl actor_handle_get_attention(long, long)
//.text:0142B600 ; bool __cdecl actor_handle_kill_volume(long)
//.text:0142B6A0 ; void __cdecl actor_handle_order_change(long, orders_definition*)
//.text:0142B6E0 ; void __cdecl actor_handle_projectile_attach(long, long)
//.text:0142B770 ; void __cdecl actor_handle_stun(long, real, real_vector3d const*)
//.text:0142B8E0 ; void __cdecl actor_handle_task_change(long, s_task*)
//.text:0142B940 ; void __cdecl actor_handle_weapon_trade(long, long, long, long)
//.text:0142BA20 ; bool __cdecl actor_has_followers(long)
//.text:0142BB80 ; bool __cdecl actor_can_do_ranged_damage(long)
//.text:0142BC20 ; bool __cdecl actor_in_combat(long)
//.text:0142BCA0 ; long __cdecl actor_index_from_object(long)
//.text:0142BD10 ; void __cdecl actor_initiate_combat(long)
//.text:0142BD90 ; void __cdecl actor_input_sample_position(long, long, actor_position_data*)
//.text:0142BE50 ; void __cdecl actor_input_update(long)

bool __cdecl actor_is_active(actor_datum const* actor)
{
	//return INVOKE(0x0142C690, actor_is_active, actor);

	return actor->meta.active;
}

bool __cdecl actor_is_blind(long actor_index)
{
	return INVOKE(0x0142C6A0, actor_is_blind, actor_index);

	//TLS_DATA_GET_VALUE_REFERENCE(actor_data);
	//actor_datum* actor = DATUM_GET(actor_data, actor_datum, actor_index);
	//if (actor->state.blind || !actor->meta.active || actor->state.mode == 1)
	//	return true;
	//
	//long squad_index = actor->meta.squad_index;
	//return squad_index != NONE && squad_is_blind(squad_index);
}

bool __cdecl actor_is_deaf(long actor_index)
{
	return INVOKE(0x0142C700, actor_is_deaf, actor_index);

	//TLS_DATA_GET_VALUE_REFERENCE(actor_data);
	//actor_datum* actor = DATUM_GET(actor_data, actor_datum, actor_index);
	//if (actor->state.deaf || !actor->meta.active)
	//	return true;
	//
	//long squad_index = actor->meta.squad_index;
	//return squad_index != NONE && squad_is_deaf(squad_index);
}

//.text:0142C750 ; bool __cdecl actor_is_fighting(long)
//.text:0142C770 ; bool __cdecl actor_is_giant(long)
//.text:0142C7A0 ; bool __cdecl actor_is_giant(actor_datum*)
//.text:0142C820 ; bool __cdecl actor_is_hero(long)
//.text:0142C870 ; bool __cdecl actor_is_leaping(long)
//.text:0142C890 ; bool __cdecl actor_is_noncombat(long)
//.text:0142C8D0 ; bool __cdecl actor_is_unsuspecting(long)

void __cdecl actor_iterator_new(actor_iterator* iterator, bool active_only)
{
	//INVOKE(0x0142C950, actor_iterator_new, iterator, active_only);

	TLS_DATA_GET_VALUE_REFERENCE(actor_data);
	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	
	if (ai_globals->ai_initialized_for_map)
	{
		iterator->iterator.begin(actor_data);
		iterator->active_only = active_only;
	}
}

actor_datum* __cdecl actor_iterator_next(actor_iterator* iterator)
{
	//return INVOKE(0x0142C990, actor_iterator_next, iterator);
	
	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);

	actor_datum* actor = NULL;
	if (ai_globals->ai_initialized_for_map)
	{
		while (iterator->iterator.next())
		{
			if (!iterator->active_only || actor_is_active(iterator->iterator.m_datum))
			{
				actor = iterator->iterator.m_datum;
				break;
			}
		}
		iterator->index = iterator->iterator.get_index();
	}

	ASSERT(((iterator->index == NONE) && (actor == NULL)) || ((iterator->index != NONE) && (actor != NULL)));

	return actor;
}

void __cdecl actor_kill(long actor_index, bool silent, bool delayed)
{
	INVOKE(0x0142CA00, actor_kill, actor_index, silent, delayed);
}

//.text:0142CAA0 ; void __cdecl actor_memory_update(long)
//.text:0142CC20 ; void __cdecl actor_nearby_actors_iterator_new(actor_nearby_actors_iterator*, real_point3d const*, real, short, short)
//.text:0142CC80 ; actor_datum* __cdecl actor_nearby_actors_iterator_next(actor_nearby_actors_iterator*)
//.text:0142CE20 ; long __cdecl actor_new(long)
//.text:0142D590 ; long __cdecl actor_place(short, actor_spawn_definition const*)
//.text:0142E080 ; void __cdecl actor_preupdate(long)
//.text:0142E0B0 ; void __cdecl actor_randomly_control_unit(long, long)
//.text:0142E170 ; void __cdecl actor_relinquish_pathfinding_timeslice(long)
//.text:0142E1A0 ; bool __cdecl actor_runtime_within_structure_bsp(long)
//.text:0142E280 ; long __cdecl actor_select_character_variant(long)
//.text:0142E410 ; void __cdecl actor_select_character_voice(long, long)

bool __cdecl actor_set_active(long actor_index, bool active)
{
	return INVOKE(0x0142E6E0, actor_set_active, actor_index, active);
}

//.text:0142E7F0 ; bool __cdecl actor_set_active_camo(long actor_index, bool active)
//.text:0142E810 ; bool __cdecl actor_set_active_camo(long actor_index, bool active, real transition_time)
//.text:0142E8B0 ; bool __cdecl actor_set_bring_forward(long, bool, real, real)
//.text:0142E9C0 ; void __cdecl actor_set_covered(long, bool)
//.text:0142EA10 ; void __cdecl actor_set_mission_critical(long, long)
//.text:0142EAD0 ; void __cdecl actor_set_team(long, short)
//.text:0142EBA0 ; long __cdecl actor_spawn_death_character(long, long, long, short, short, real_point3d const*)
//.text:0142ED80 ; bool __cdecl actor_spawn_unit_character(long, long)
//.text:0142EDC0 ; void __cdecl actor_suppress_combat(long)
//.text:0142EDF0 ; void __cdecl actor_suppress_combat_update(long)

bool __cdecl actor_switch_bsp(long actor_index)
{
	return INVOKE(0x0142EED0, actor_switch_bsp, actor_index);
}

//.text:0142EFC0 ; long __cdecl actor_target_unit_index(long)
//.text:0142F010 ; bool __cdecl actor_test_activation_cluster_bit_vector(long, s_game_cluster_bit_vectors const*)
//.text:0142F150 ; void __cdecl actor_unit_control(long)
//.text:0142F2C0 ; void __cdecl actor_unit_control_boost(long, bool)
//.text:0142F340 ; void __cdecl actor_unit_control_crouch(long, bool)
//.text:0142F390 ; void __cdecl actor_unit_control_exact_facing(long, bool)
//.text:0142F3D0 ; void __cdecl actor_unit_control_force_field(long, bool)
//.text:0142F420 ; void __cdecl actor_unit_control_jump(long)
//.text:0142F450 ; void __cdecl actor_unit_control_lower_weapon(long, bool)
//.text:0142F4B0 ; bool __cdecl actor_unit_is_mission_critical(long)
//.text:0142F500 ; void __cdecl actor_update(long)
//.text:0142F640 ; void __cdecl actor_update_goal_status(long)
//.text:0142F6E0 ; bool __cdecl actor_uses_gravemind(long)
//.text:0142F6F0 ; bool __cdecl actor_vehicle_charge_allowed(long)
//.text:0142F860 ; void __cdecl actor_vehicle_update(long)

void __cdecl actors_dispose()
{
	INVOKE(0x0142FD10, actors_dispose);
}

void __cdecl actors_dispose_from_old_map()
{
	INVOKE(0x0142FD30, actors_dispose_from_old_map);
}

void __cdecl actors_freeze()
{
	INVOKE(0x0142FD50, actors_freeze);
}

void __cdecl actors_initialize()
{
	INVOKE(0x0142FDC0, actors_initialize);
}

void __cdecl actors_initialize_for_new_map()
{
	INVOKE(0x0142FE80, actors_initialize_for_new_map);
}

void __cdecl actors_move_randomly()
{
	INVOKE(0x0142FEC0, actors_move_randomly);
}

void __cdecl actors_update()
{
	INVOKE(0x0142FF30, actors_update);
}

//.text:014302E0 ; bool __cdecl ai_area_type_allowed(short, area_definition const*)
//.text:01430340 ; 
//.text:01430350 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_actor_data_allocator(void*)>::allocate
//.text:01430390 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_ai_reference_frame_data_allocator(void*)>::allocate
//.text:014303D0 ; t_restricted_allocation_manager<1,0,0,&void actor_firing_position_hash_set_address(void*)>::allocate
//.text:01430410 ; 
//.text:01430430 ; 
//.text:01430450 ; 
//.text:01430460 ; 
//.text:01430470 ; 
//.text:01430480 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_actor_data_allocator(void*)>::deallocate
//.text:014304B0 ; t_restricted_allocation_manager<1,0,0,&void __cdecl __tls_set_g_ai_reference_frame_data_allocator(void*)>::deallocate
//.text:014304E0 ; t_restricted_allocation_manager<1,0,0,&void __cdecl actor_firing_position_hash_set_address(void*)>::deallocate
//.text:01430510 ; 
//.text:01430520 ; 
//.text:01430550 ; 
//.text:01430580 ; 
//.text:014305B0 ; 
//.text:014305C0 ; 
//.text:014305D0 ; 
//.text:014305F0 ; 
//.text:01430610 ; 
//.text:01430620 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_actor_data_allocator(void*)>::reserve_memory
//.text:01430660 ; t_restricted_allocation_manager<1,0,0,&void __tls_set_g_ai_reference_frame_data_allocator(void*)>::reserve_memory
//.text:014306A0 ; 
//.text:014306E0 ; long __cdecl actor_create(long, long, long, bool)
//.text:01430840 ; 
//.text:01430860 ; 
//.text:01430880 ; 
//.text:01430890 ; 
//.text:014308B0 ; 

