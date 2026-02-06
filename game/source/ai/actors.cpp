#include "ai/actors.hpp"

#include "ai/swarms.hpp"
#include "memory/thread_local.hpp"

//.text:01428390 ; 
//.text:014283B0 ; 
//.text:014283D0 ; 
//.text:014283F0 ; 
//.text:01428400 ; 
//.text:01428410 ; 

const real_argb_color* __cdecl actor_activation_debug_color(int32 actor_index)
{
	return INVOKE(0x01428470, actor_activation_debug_color, actor_index);
}

//.text:014284B0 ; void __cdecl actor_attach_unit(int32, int32)
//.text:01428590 ; bool __cdecl actor_attacking_target(int32, real_vector3d*)
//.text:01428640 ; void __cdecl actor_berserk_update(int32)
//.text:01428700 ; void __cdecl actor_braindead(int32, bool)
//.text:01428760 ; void __cdecl actor_break(int32, bool)
//.text:014287B0 ; void __cdecl actor_broken_update(int32)
//.text:01428820 ; bool __cdecl actor_bubbleshield_attached(int32)
//.text:014288D0 ; void __cdecl actor_change_squad(int32, int16)
//.text:014289F0 ; void __cdecl actor_clear_orders(int32)
//.text:01428A50 ; void __cdecl actor_clear_output(int32)
//.text:01428B20 ; void __cdecl actor_clear_spawn_definition(actor_spawn_definition*)
//.text:01428BC0 ; void __cdecl actor_cover_update(int32)
//.text:01428CA0 ; int32 __cdecl actor_create(int32, int32, int32, bool)
//.text:01428CC0 ; int32 __cdecl actor_create_internal(int32, int32, int32, int32, bool)
//.text:01428E80 ; bool __cdecl actor_create_kamikaze_grenades(int32)
//.text:01428EE0 ; void __cdecl actor_customize_unit(int32, int32, int32, int32, int16, int32)
//.text:014291D0 ; void __cdecl actor_customize_weapon(int32, int32)

bool __cdecl actor_datum_available_to_current_thread()
{
	//return INVOKE(0x01429350, actor_datum_available_to_current_thread);

	return actor_data != nullptr;
}

void __cdecl actor_delete(int32 actor_index, bool died)
{
	INVOKE(0x01429370, actor_delete, actor_index, died);
}

//.text:014296F0 ; real32 __cdecl actor_destination_tolerance(int32)
//.text:01429810 ; void __cdecl actor_detach_from_unit(int32)
//.text:01429870 ; void __cdecl actor_died(int32)
//.text:01429DB0 ; void __cdecl actor_dont_do_avoidance(int32, bool)

void __cdecl actor_erase(int32 actor_index, bool delete_immediately)
{
	//INVOKE(0x01429DF0, actor_erase, actor_index, delete_immediately);

	actor_datum* actor = DATUM_GET(actor_data, actor_datum, actor_index);
	int32 unit_index = actor->meta.unit_index;
	int32 swarm_index = actor->meta.swarm_index;
	
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

//.text:01429E60 ; void __cdecl actor_estimate_position(int32, int16, const real_point3d*, const real_vector3d*, const real_vector3d*, const real_vector3d*, real_point3d*)
//.text:01429ED0 ; void __cdecl actor_firing_position_hash_set_address(void*)
//.text:01429F00 ; void __cdecl actor_flush_position_indices(int32, bool)
//.text:0142A010 ; void __cdecl actor_flush_prop(int32, int32)
//.text:0142A1C0 ; void __cdecl actor_flush_structure_indices(int32)
//.text:0142A300 ; void __cdecl actor_force_combat_status(int32, int16)
//.text:0142A370 ; void __cdecl actor_freeze(int32)
//.text:0142A420 ; void __cdecl actor_freeze_unit(int32, int32)

bool __cdecl actor_general_update(int32 actor_index)
{
	return INVOKE(0x0142A480, actor_general_update, actor_index);
}

//.text:0142A630 ; bool __cdecl actor_get_active_camo_state(int32)
//.text:0142A680 ; int32 __cdecl actor_get_backpack_weapon(int32)
//.text:0142A6C0 ; bool __cdecl actor_get_crouch_velocity(int32, real32*)
//.text:0142A710 ; int16 __cdecl actor_get_desired_area_type(int32)
//.text:0142A770 ; int16 __cdecl actor_get_enemies_in_sphere(int32, real_point3d*, real32, int32*, int16)
//.text:0142A860 ; real64 __cdecl actor_get_gravity_scale(int32)
//.text:0142A8D0 ; real64 __cdecl actor_get_maximum_damage_velocity(int32)
//.text:0142A970 ; int16 __cdecl actor_get_rank(int32)
//.text:0142A9C0 ; bool __cdecl actor_get_running_blind_vector(int32, real_vector3d*)
//.text:0142AAF0 ; int32 __cdecl actor_get_secondary_weapon(int32)
//.text:0142AB50 ; int16 __cdecl actor_get_standing(int32)
//.text:0142ABD0 ; bool __cdecl actor_get_task_direction(int32, real_vector3d*)
//.text:0142AC90 ; void __cdecl actor_get_timeslices(int32)
//.text:0142ADB0 ; int32 __cdecl actor_get_weapon(int32)
//.text:0142AE50 ; void __cdecl actor_handle_change_internal(int32, bool, bool)
//.text:0142AF40 ; void __cdecl actor_handle_damage(int32, int16, int32, int32, real32, const real_vector3d*)
//.text:0142B2C0 ; void __cdecl actor_handle_deleted_actor(int32, int32)
//.text:0142B350 ; void __cdecl actor_handle_deleted_object(int32, int32)

void __cdecl actor_handle_equipment_delete(int32 actor_index)
{
	INVOKE(0x0142B530, actor_handle_equipment_delete, actor_index);
}

//.text:0142B560 ; void __cdecl actor_handle_get_attention(int32, int32)
//.text:0142B600 ; bool __cdecl actor_handle_kill_volume(int32)
//.text:0142B6A0 ; void __cdecl actor_handle_order_change(int32, orders_definition*)
//.text:0142B6E0 ; void __cdecl actor_handle_projectile_attach(int32, int32)
//.text:0142B770 ; void __cdecl actor_handle_stun(int32, real32, const real_vector3d*)
//.text:0142B8E0 ; void __cdecl actor_handle_task_change(int32, s_task*)
//.text:0142B940 ; void __cdecl actor_handle_weapon_trade(int32, int32, int32, int32)
//.text:0142BA20 ; bool __cdecl actor_has_followers(int32)
//.text:0142BB80 ; bool __cdecl actor_can_do_ranged_damage(int32)
//.text:0142BC20 ; bool __cdecl actor_in_combat(int32)
//.text:0142BCA0 ; int32 __cdecl actor_index_from_object(int32)
//.text:0142BD10 ; void __cdecl actor_initiate_combat(int32)
//.text:0142BD90 ; void __cdecl actor_input_sample_position(int32, int32, actor_position_data*)
//.text:0142BE50 ; void __cdecl actor_input_update(int32)

bool __cdecl actor_is_active(const actor_datum* actor)
{
	//return INVOKE(0x0142C690, actor_is_active, actor);

	return actor->meta.active;
}

bool __cdecl actor_is_blind(int32 actor_index)
{
	return INVOKE(0x0142C6A0, actor_is_blind, actor_index);

	//actor_datum* actor = DATUM_GET(actor_data, actor_datum, actor_index);
	//if (actor->state.blind || !actor->meta.active || actor->state.mode == 1)
	//	return true;
	//
	//int32 squad_index = actor->meta.squad_index;
	//return squad_index != NONE && squad_is_blind(squad_index);
}

bool __cdecl actor_is_deaf(int32 actor_index)
{
	return INVOKE(0x0142C700, actor_is_deaf, actor_index);

	//actor_datum* actor = DATUM_GET(actor_data, actor_datum, actor_index);
	//if (actor->state.deaf || !actor->meta.active)
	//	return true;
	//
	//int32 squad_index = actor->meta.squad_index;
	//return squad_index != NONE && squad_is_deaf(squad_index);
}

//.text:0142C750 ; bool __cdecl actor_is_fighting(int32)
//.text:0142C770 ; bool __cdecl actor_is_giant(int32)
//.text:0142C7A0 ; bool __cdecl actor_is_giant(actor_datum*)
//.text:0142C820 ; bool __cdecl actor_is_hero(int32)
//.text:0142C870 ; bool __cdecl actor_is_leaping(int32)
//.text:0142C890 ; bool __cdecl actor_is_noncombat(int32)
//.text:0142C8D0 ; bool __cdecl actor_is_unsuspecting(int32)

void __cdecl actor_iterator_new(actor_iterator* iterator, bool active_only)
{
	//INVOKE(0x0142C950, actor_iterator_new, iterator, active_only);

	if (ai_globals->ai_initialized_for_map)
	{
		iterator->iterator.begin(actor_data);
		iterator->active_only = active_only;
	}
}

actor_datum* __cdecl actor_iterator_next(actor_iterator* iterator)
{
	//return INVOKE(0x0142C990, actor_iterator_next, iterator);
	
	actor_datum* actor = nullptr;
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

	ASSERT(((iterator->index == NONE) && (actor == nullptr)) || ((iterator->index != NONE) && (actor != nullptr)));

	return actor;
}

void __cdecl actor_kill(int32 actor_index, bool silent, bool delayed)
{
	INVOKE(0x0142CA00, actor_kill, actor_index, silent, delayed);
}

//.text:0142CAA0 ; void __cdecl actor_memory_update(int32)
//.text:0142CC20 ; void __cdecl actor_nearby_actors_iterator_new(actor_nearby_actors_iterator*, const real_point3d*, real32, int16, int16)
//.text:0142CC80 ; actor_datum* __cdecl actor_nearby_actors_iterator_next(actor_nearby_actors_iterator*)
//.text:0142CE20 ; int32 __cdecl actor_new(int32)
//.text:0142D590 ; int32 __cdecl actor_place(int16, const actor_spawn_definition*)
//.text:0142E080 ; void __cdecl actor_preupdate(int32)
//.text:0142E0B0 ; void __cdecl actor_randomly_control_unit(int32, int32)
//.text:0142E170 ; void __cdecl actor_relinquish_pathfinding_timeslice(int32)
//.text:0142E1A0 ; bool __cdecl actor_runtime_within_structure_bsp(int32)
//.text:0142E280 ; int32 __cdecl actor_select_character_variant(int32)
//.text:0142E410 ; void __cdecl actor_select_character_voice(int32, int32)

bool __cdecl actor_set_active(int32 actor_index, bool active)
{
	return INVOKE(0x0142E6E0, actor_set_active, actor_index, active);
}

//.text:0142E7F0 ; bool __cdecl actor_set_active_camo(int32 actor_index, bool active)
//.text:0142E810 ; bool __cdecl actor_set_active_camo(int32 actor_index, bool active, real32 transition_time)
//.text:0142E8B0 ; bool __cdecl actor_set_bring_forward(int32, bool, real32, real32)
//.text:0142E9C0 ; void __cdecl actor_set_covered(int32, bool)
//.text:0142EA10 ; void __cdecl actor_set_mission_critical(int32, int32)
//.text:0142EAD0 ; void __cdecl actor_set_team(int32, int16)
//.text:0142EBA0 ; int32 __cdecl actor_spawn_death_character(int32, int32, int32, int16, int16, const real_point3d*)
//.text:0142ED80 ; bool __cdecl actor_spawn_unit_character(int32, int32)
//.text:0142EDC0 ; void __cdecl actor_suppress_combat(int32)
//.text:0142EDF0 ; void __cdecl actor_suppress_combat_update(int32)

bool __cdecl actor_switch_bsp(int32 actor_index)
{
	return INVOKE(0x0142EED0, actor_switch_bsp, actor_index);
}

//.text:0142EFC0 ; int32 __cdecl actor_target_unit_index(int32)
//.text:0142F010 ; bool __cdecl actor_test_activation_cluster_bit_vector(int32, const s_game_cluster_bit_vectors*)
//.text:0142F150 ; void __cdecl actor_unit_control(int32)
//.text:0142F2C0 ; void __cdecl actor_unit_control_boost(int32, bool)
//.text:0142F340 ; void __cdecl actor_unit_control_crouch(int32, bool)
//.text:0142F390 ; void __cdecl actor_unit_control_exact_facing(int32, bool)
//.text:0142F3D0 ; void __cdecl actor_unit_control_force_field(int32, bool)
//.text:0142F420 ; void __cdecl actor_unit_control_jump(int32)
//.text:0142F450 ; void __cdecl actor_unit_control_lower_weapon(int32, bool)
//.text:0142F4B0 ; bool __cdecl actor_unit_is_mission_critical(int32)
//.text:0142F500 ; void __cdecl actor_update(int32)
//.text:0142F640 ; void __cdecl actor_update_goal_status(int32)
//.text:0142F6E0 ; bool __cdecl actor_uses_gravemind(int32)
//.text:0142F6F0 ; bool __cdecl actor_vehicle_charge_allowed(int32)
//.text:0142F860 ; void __cdecl actor_vehicle_update(int32)

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

//.text:014302E0 ; bool __cdecl ai_area_type_allowed(int16, const area_definition*)
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
//.text:014306E0 ; int32 __cdecl actor_create(int32, int32, int32, bool)
//.text:01430840 ; 
//.text:01430860 ; 
//.text:01430880 ; 
//.text:01430890 ; 
//.text:014308B0 ; 

