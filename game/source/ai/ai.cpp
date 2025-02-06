#include "ai/ai.hpp"

#include "ai/ai_reference_frame.hpp"
#include "memory/thread_local.hpp"

//.text:014308D0 ; void __cdecl __tls_set_g_ai_globals_allocator(void*)

long __cdecl actor_endangering_player(bool must_be_attacking, bool build_player_mask, dword* player_mask_out)
{
	return INVOKE(0x014308F0, actor_endangering_player, must_be_attacking, build_player_mask, player_mask_out);
}

//.text:01430DB0 ; bool __cdecl actor_resurrection_biped_safe(long)
//.text:014310A0 ; bool __cdecl ai_adjust_damage(long, long, s_damage_data*, real*)
//.text:014312F0 ; double __cdecl ai_chance_this_tick(real)
//.text:01431360 ; void __cdecl ai_create_mounted_weapons_for_unit(long)
//.text:014313B0 ; double __cdecl ai_danger_level(short)
//.text:014314C0 ; void __cdecl ai_debug_update(void)

void __cdecl ai_dispose()
{
	//INVOKE(0x014314D0, ai_dispose);

	ai_script_dispose();
	squads_dispose();
	clumps_dispose();
	joint_behavior_dispose();
	cs_dispose();
	props_dispose();
	actors_dispose();
	paths_dispose();
	ai_profile_dispose();
}

void __cdecl ai_dispose_from_old_map()
{
	//INVOKE(0x01431500, ai_dispose_from_old_map);

	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	if (ai_globals->ai_initialized_for_map)
	{
		ai_script_dispose_from_old_map();
		squads_dispose_from_old_map();
		clumps_dispose_from_old_map();
		joint_behavior_dispose_from_old_map();
		cs_dispose_from_old_map();
		dynamic_firing_sets_dispose_from_old_map();
		squad_patrol_dispose_from_old_map();
		formations_dispose_from_old_map();
		props_dispose_from_old_map();
		actors_dispose_from_old_map();
		swarms_dispose_from_old_map();
		paths_dispose_from_old_map();
		ai_profile_dispose_from_old_map();
		ai_dialogue_dispose_from_old_map();
		flocks_dispose_from_old_map();
		ai_globals->ai_initialized_for_map = false;
	}
}

void __cdecl ai_dispose_from_old_structure_bsp(dword deactivating_structure_bsp_mask)
{
	//INVOKE(0x01431570, ai_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);

	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	if (ai_globals->ai_initialized_for_map)
	{
		flocks_dispose_from_old_structure_bsp();
	
		actor_iterator actor_iter{};
		actor_iterator_new(&actor_iter, false);
		for (actor_datum* actor = actor_iterator_next(&actor_iter); actor; actor = actor_iterator_next(&actor_iter))
		{
			char bsp_index = actor->input.position.body_location.cluster_reference.bsp_index;
			if (bsp_index != NONE && TEST_BIT(deactivating_structure_bsp_mask, bsp_index))
			{
				actor_set_active(actor_iter.index, true);
				actor->input.position.body_location.cluster_reference.bsp_index = NONE;
				actor->input.position.body_location.cluster_reference.cluster_index = NONE;
			}
		}
	
		squad_iterator squad_iter{};
		squad_iterator_new(&squad_iter);
		for (squad_datum* squad = squad_iterator_next(&squad_iter); squad; squad = squad_iterator_next(&squad_iter))
		{
			if ((signed char)(squad->flags & 0xFF) < 0)
			{
				squad_actor_iterator squad_actor_iter{};
				squad_actor_iterator_new(&squad_actor_iter, squad_iter.squad_index, true);
				if (!squad_actor_iterator_next(&squad_actor_iter))
				{
					squad_deactivate(squad_iter.squad_index);
				}
			}
		}
	}
}

bool __cdecl ai_enemies_attacking_players(long* attacking_object_index, dword* player_mask_out)
{
	return INVOKE(0x01431670, ai_enemies_attacking_players, attacking_object_index, player_mask_out);

	//long actor_index = actor_endangering_player(true, true, player_mask_out);
	//if (actor_index != NONE)
	//{
	//	actor_datum* actor = actor_get(actor_index);
	//	*attacking_object_index = actor->meta.unit_index;
	//}
	//return *player_mask_out != 0;
}

bool __cdecl ai_enemies_can_see_player(long* object_index)
{
	return INVOKE(0x014316D0, ai_enemies_can_see_player, object_index);

	//long actor_index = actor_endangering_player(false, false, NULL);
	//if (actor_index != NONE)
	//{
	//	actor_datum* actor = actor_get(actor_index);
	//	*object_index = actor->meta.unit_index;
	//	return true;
	//}
	//return false;
}

void __cdecl ai_erase(long squad_index, bool delete_immediately)
{
	//INVOKE(0x01431720, ai_erase, squad_index, delete_immediately);

	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	if (ai_globals->ai_initialized_for_map)
	{
		if (squad_index == NONE)
		{
			actor_iterator iterator{};
			actor_iterator_new(&iterator, false);
			while (actor_iterator_next(&iterator))
				actor_erase(iterator.index, delete_immediately);
		}
		else
		{
			squad_actor_iterator iterator{};
			squad_actor_iterator_new(&iterator, squad_index, false);
			while (squad_actor_iterator_next(&iterator))
				actor_erase(iterator.index, delete_immediately);
		}
	}
}

//.text:014317D0 ; void __cdecl ai_find_inactive_squads(long, byte*, long)
//.text:014318F0 ; void __cdecl ai_find_line_of_fire_friend_pills(long, long, line_of_fire_pill*)
//.text:01431B50 ; bool __cdecl ai_find_location(real_point3d const*, s_location*)

void __cdecl ai_flush_spatial_effects()
{
	//INVOKE(0x01431BC0, ai_flush_spatial_effects);

	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	ai_globals->spatial_effects_last_index = 0;
	ai_globals->spatial_effects_first_index = 0;
	csmemset(ai_globals->spatial_effects, 0, sizeof(ai_globals->spatial_effects));
}

//.text:01431C00 ; void __cdecl ai_generate_line_of_fire_pill(long, long, line_of_fire_pill*)

bool __cdecl ai_get_active_clusters(long structure_bsp_index, dword* activation_bitvector, long cluster_count)
{
	return INVOKE(0x01431C90, ai_get_active_clusters, structure_bsp_index, activation_bitvector, cluster_count);
}

//.text:01431D50 ; void __cdecl ai_get_center_of_mass(long, real_point3d*)
//.text:01431DE0 ; long __cdecl ai_get_unit_responsible_for_damage(long, bool)
//.text:01431E30 ; void __cdecl ai_globals_dialogue_enable(bool)
//.text:01431E50 ; bool __cdecl ai_globals_dialogue_enabled()
//.text:01431E80 ; void __cdecl ai_globals_dialogue_suppress(real)
//.text:01431EC0 ; bool __cdecl ai_globals_get_ai_active()
//.text:01431EE0 ; void __cdecl ai_globals_grenades_enabled(bool)
//.text:01431F10 ; void __cdecl ai_globals_infection_suppress(long)

void __cdecl ai_globals_initialize()
{
	INVOKE(0x01431F60, ai_globals_initialize);
}

void __cdecl ai_globals_initialize_for_new_map()
{
	INVOKE(0x01431F90, ai_globals_initialize_for_new_map);
}

//.text:01432120 ; void __cdecl ai_globals_player_dialogue_enable(bool)
//.text:01432140 ; bool __cdecl ai_globals_player_dialogue_enabled()

void __cdecl ai_globals_set_ai_active(bool enable)
{
	INVOKE(0x01432160, ai_globals_set_ai_active, enable);
}

//.text:01432180 ; void __cdecl ai_globals_set_fast_and_dumb(bool)
//.text:014321A0 ; s_ai_globals_data* __cdecl ai_globals_try_and_get_data(s_ai_globals_definition const*)

void __cdecl ai_globals_update()
{
	INVOKE(0x014321C0, ai_globals_update);
}

//.text:01432690 ; void __cdecl ai_handle_ai_point_move(real_point3d const*, c_ai_point3d*)
//.text:014326E0 ; void __cdecl ai_handle_allegiance_status_changed(e_game_team, e_game_team, bool, bool, bool)
//.text:01432890 ; void __cdecl ai_handle_areas_delete(short, dword const*)

void __cdecl ai_handle_bump(long biped_index, long object_index, real_vector3d const* old_velocity)
{
	INVOKE(0x014328E0, ai_handle_bump, biped_index, object_index, old_velocity);
}

//.text:01432980 ; void __cdecl ai_handle_cs_data_point_move(short, short)
//.text:01432990 ; void __cdecl ai_handle_damage(long, long, short, long, real, real_vector3d const*, bool)
//.text:01432B20 ; void __cdecl ai_handle_death(long, long, short, long, real)
//.text:01433040 ; void __cdecl ai_handle_deleted_object(long)
//.text:014332F0 ; void __cdecl ai_handle_effect_creation(long, short, long, short, real, real, real_matrix4x3 const*)
//.text:01433390 ; void __cdecl ai_handle_equipment_release(long, long)
//.text:01433490 ; void __cdecl ai_handle_firing_position_move(short, short, real_vector3d const*)
//.text:014334A0 ; void __cdecl ai_handle_get_attention(long, long)
//.text:014334D0 ; bool __cdecl ai_handle_killing_spree(long, short)
//.text:01433530 ; void __cdecl ai_handle_noisemaker(long)
//.text:01433690 ; void __cdecl ai_handle_objectives_editing()
//.text:014336A0 ; void __cdecl ai_handle_projectile_attach(long, long)
//.text:01433790 ; void __cdecl ai_handle_projectile_impact(long, long)
//.text:014337C0 ; void __cdecl ai_handle_sector_point_move(short, short, short, real_vector3d const*, s_tag_block_definition const*)
//.text:014337D0 ; void __cdecl ai_handle_spatial_effect(long, real_point3d const*, short, short, short)
//.text:01433A70 ; void __cdecl ai_handle_spawn_point_change(short, short)
//.text:01433A80 ; void __cdecl ai_handle_squad_addition(long)
//.text:01433A90 ; void __cdecl ai_handle_squad_deletion(dword const*)
//.text:01433AA0 ; void __cdecl ai_handle_squad_editing()
//.text:01433AB0 ; void __cdecl ai_handle_squad_group_addition(long)
//.text:01433AC0 ; void __cdecl ai_handle_squad_group_deletion(dword const*)
//.text:01433AD0 ; void __cdecl ai_handle_squad_patrol_editing()
//.text:01433AE0 ; void __cdecl ai_handle_squad_patrol_waypoint_addition(long, long, long)
//.text:01433B90 ; void __cdecl ai_handle_squad_patrol_waypoint_deletion(long, long, long)
//.text:01433C80 ; void __cdecl ai_handle_stun(long, real, real_vector3d const*)
//.text:01433CE0 ; void __cdecl ai_handle_tasks_addition(long)
//.text:01433CF0 ; void __cdecl ai_handle_tasks_deletion(dword const*)
//.text:01433D00 ; void __cdecl ai_handle_unit_effect(long, e_sound_type, e_ai_sound_volume)
//.text:01433E40 ; void __cdecl ai_handle_weapon_trade(long, long, long, long)
//.text:01433E70 ; void __cdecl ai_handle_well_point_move(short, short, real_vector3d const*)
//.text:01433E80 ; void __cdecl ai_handle_zone_addition(long)
//.text:01433E90 ; void __cdecl ai_handle_zone_deletion(dword const*)

void __cdecl ai_initialize()
{
	//INVOKE(0x01433EA0, ai_initialize);

	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	ai_globals = (ai_globals_type*)g_ai_globals_allocator.allocate(sizeof(ai_globals_type), "ai globals");

	ai_globals_initialize();
	ai_player_initialize();
	ai_profile_initialize();
	paths_initialize();
	actors_initialize();
	swarms_initialize();
	props_initialize();
	squads_initialize();
	objectives_initialize();
	clumps_initialize();
	joint_behavior_initialize();
	event_handling_initialize();
	dynamic_firing_sets_initialize();
	squad_patrol_initialize();
	formations_initialize();
	cs_initialize();
	activities_initialize();
	ai_script_initialize();
	ai_dialogue_initialize();
	actor_move_initialize();
	behaviors_initialize();
	flocks_initialize();
}

void __cdecl ai_initialize_for_new_map()
{
	//INVOKE(0x01433F70, ai_initialize_for_new_map);

	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	ai_globals_initialize_for_new_map();
	ai_players_initialize_for_new_map();
	ai_profile_initialize_for_new_map();
	paths_initialize_for_new_map();
	actors_initialize_for_new_map();
	swarms_initialize_for_new_map();
	props_initialize_for_new_map();
	squads_initialize_for_new_map();
	objectives_initialize_for_new_map();
	clumps_initialize_for_new_map();
	joint_behavior_initialize_for_new_map();
	dynamic_firing_sets_initialize_for_new_map();
	cs_initialize_for_new_map();
	ai_script_initialize_for_new_map();
	ai_dialogue_initialize_for_new_map();
	flocks_initialize_for_new_map();
	ai_flush_spatial_effects();
	ai_globals->ai_initialized_for_map = true;
	ai_verify_tags();
}

void __cdecl ai_initialize_for_new_structure_bsp(dword activating_structure_bsp_mask)
{
	INVOKE(0x01434010, ai_initialize_for_new_structure_bsp, activating_structure_bsp_mask);

	//TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	//if (ai_globals->ai_initialized_for_map)
	//{
	//	actor_iterator iterator{};
	//	actor_iterator_new(&iterator, false);
	//	for (actor_datum* actor = actor_iterator_next(&iterator); actor; actor = actor_iterator_next(&iterator))
	//	{
	//		if (actor_switch_bsp(iterator.index))
	//		{
	//			long squad_index = actor->meta.squad_index;
	//			if (squad_index != NONE && (signed char)(squad_get(squad_index)->flags & 0xFF) < 0)
	//			{
	//				actor_set_active(iterator.index, true);
	//			}
	//		}
	//	}
	//	ai_refresh_reference_frames();
	//	objectives_initialize_for_new_structure_bsp(activating_structure_bsp_mask);
	//
	//	// $TODO: stucture bsp pathfinding data validation loop
	//
	//	flocks_initialize_for_new_structure_bsp(activating_structure_bsp_mask);
	//	cs_initialize_for_new_structure_bsp((long)activating_structure_bsp_mask);
	//}
}

//.text:01434150 ; void __cdecl ai_initialize_for_saved_game(long)
//.text:01434180 ; void __cdecl ai_place(long)
//.text:014341B0 ; long __cdecl ai_release_inactive_squads(long, byte*, long, bool*, char*, long)
//.text:01434230 ; long __cdecl ai_release_inactive_swarms(long, byte*, long, bool*, char*, long)
//.text:014342E0 ; void __cdecl ai_reset()
//.text:014343B0 ; void* __cdecl ai_scratch_allocate(unsigned int)
//.text:01434450 ; void __cdecl ai_scratch_free(void*)
//.text:014344B0 ; bool __cdecl ai_test_ballistic_line_of_fire(long, real_point3d const*, real, real_vector3d const*, real, long, bool)
//.text:014346F0 ; bool __cdecl ai_test_line_of_fire(long, long, real_point3d const*, real_vector3d const*, long*)
//.text:014347D0 ; bool __cdecl ai_test_line_of_fire_geometry(long, long, real_point3d const*, real_point3d const*)
//.text:01434AD0 ; short __cdecl ai_test_line_of_sight(real_point3d const*, s_cluster_reference, real_point3d const*, s_cluster_reference, short, bool, long, long, bool, bool, long*, bool*)
//.text:014350F0 ; bool __cdecl ai_try_vehicle_eviction(long, long, bool)

void __cdecl ai_update()
{
	//INVOKE(0x014351F0, ai_update);

	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	if (ai_globals->ai_initialized_for_map && !ai_profile.disable_ai && !game_is_predicted())
	{
		ai_profile_update();
		if (ai_profile.move_randomly)
		{
			actors_move_randomly();
			ai_globals->ai_has_control_data = true;
		}
		else if (ai_globals->ai_active)
		{
			ai_player_state_update();
			ai_globals_update();
			formations_update();
			squad_patrol_update();
			squads_update();
			objectives_update();
			clumps_update();
			joint_behaviors_update();
			swarm_spawners_update();
			actors_update();
			ai_dialogue_update();
			flocks_update();
			ai_globals->ai_has_control_data = true;
		}
		else if (ai_globals->ai_has_control_data)
		{
			actors_freeze();
			ai_globals->ai_has_control_data = false;
		}
	}
	else if (game_is_predicted())
	{
		flocks_update();
	}
}

//.text:01435290 ; void __cdecl ai_update_team_status()

void __cdecl ai_verify_tags()
{
	// $TODO: implement me?
}

//.text:014353A0 ; t_restricted_allocation_manager<3,0,0,&void __tls_set_g_ai_globals_allocator(void*)>::allocate
//.text:014353E0 ; 
//.text:01435400 ; 
//.text:01435420 ; 
//.text:01435430 ; 
//.text:01435450 ; bool __cdecl compare_releasable_inactive_squads(void const*, void const*, void const*)
//.text:01435480 ; 
//.text:014354B0 ; 
//.text:014354E0 ; 
//.text:014354F0 ; 
//.text:01435500 ; 
//.text:01435510 ; 
//.text:01435530 ; 
//.text:01435550 ; 
//.text:01435560 ; 
//.text:01435570 ; 
//.text:01435590 ; 
//.text:014355B0 ; 

