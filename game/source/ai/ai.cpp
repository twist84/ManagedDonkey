#include "ai/ai.hpp"

#include "ai/ai_reference_frame.hpp"
#include "memory/thread_local.hpp"

//.text:014308D0 ; void __cdecl __tls_set_g_ai_globals_allocator(void*)

int32 __cdecl actor_endangering_player(bool must_be_attacking, bool build_player_mask, uns32* player_mask_out)
{
	return INVOKE(0x014308F0, actor_endangering_player, must_be_attacking, build_player_mask, player_mask_out);
}

//.text:01430DB0 ; bool __cdecl actor_resurrection_biped_safe(int32)
//.text:014310A0 ; bool __cdecl ai_adjust_damage(int32, int32, s_damage_data*, real32*)
//.text:014312F0 ; real64 __cdecl ai_chance_this_tick(real32)
//.text:01431360 ; void __cdecl ai_create_mounted_weapons_for_unit(int32)
//.text:014313B0 ; real64 __cdecl ai_danger_level(int16)

void __cdecl ai_debug_update()
{
	//INVOKE(0x014314C0, ai_debug_update);
}

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

void __cdecl ai_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	//INVOKE(0x01431570, ai_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);

	if (ai_globals->ai_initialized_for_map)
	{
		flocks_dispose_from_old_structure_bsp();
	
		actor_iterator actor_iter{};
		actor_iterator_new(&actor_iter, false);
		for (actor_datum* actor = actor_iterator_next(&actor_iter); actor; actor = actor_iterator_next(&actor_iter))
		{
			int8 bsp_index = actor->input.position.body_location.cluster_reference.bsp_index;
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

bool __cdecl ai_enemies_attacking_players(int32* attacking_object_index, uns32* player_mask_out)
{
	return INVOKE(0x01431670, ai_enemies_attacking_players, attacking_object_index, player_mask_out);

	//int32 actor_index = actor_endangering_player(true, true, player_mask_out);
	//if (actor_index != NONE)
	//{
	//	actor_datum* actor = DATUM_GET(actor_data, actor_datum, actor_index);
	//	*attacking_object_index = actor->meta.unit_index;
	//}
	//return *player_mask_out != 0;
}

bool __cdecl ai_enemies_can_see_player(int32* object_index)
{
	return INVOKE(0x014316D0, ai_enemies_can_see_player, object_index);

	//int32 actor_index = actor_endangering_player(false, false, NULL);
	//if (actor_index != NONE)
	//{
	//	actor_datum* actor = DATUM_GET(actor_data, actor_datum, actor_index);
	//	*object_index = actor->meta.unit_index;
	//	return true;
	//}
	//return false;
}

void __cdecl ai_erase(int32 squad_index, bool delete_immediately)
{
	//INVOKE(0x01431720, ai_erase, squad_index, delete_immediately);

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

//.text:014317D0 ; void __cdecl ai_find_inactive_squads(int32, uns8*, int32)
//.text:014318F0 ; void __cdecl ai_find_line_of_fire_friend_pills(int32, int32, line_of_fire_pill*)
//.text:01431B50 ; bool __cdecl ai_find_location(const real_point3d*, s_location*)

void __cdecl ai_flush_spatial_effects()
{
	//INVOKE(0x01431BC0, ai_flush_spatial_effects);

	ai_globals->spatial_effects_last_index = 0;
	ai_globals->spatial_effects_first_index = 0;
	csmemset(ai_globals->spatial_effects, 0, sizeof(ai_globals->spatial_effects));
}

//.text:01431C00 ; void __cdecl ai_generate_line_of_fire_pill(int32, int32, line_of_fire_pill*)

bool __cdecl ai_get_active_clusters(int32 structure_bsp_index, uns32* activation_bitvector, int32 cluster_count)
{
	return INVOKE(0x01431C90, ai_get_active_clusters, structure_bsp_index, activation_bitvector, cluster_count);
}

//.text:01431D50 ; void __cdecl ai_get_center_of_mass(int32, real_point3d*)
//.text:01431DE0 ; int32 __cdecl ai_get_unit_responsible_for_damage(int32, bool)
//.text:01431E30 ; void __cdecl ai_globals_dialogue_enable(bool)
//.text:01431E50 ; bool __cdecl ai_globals_dialogue_enabled()
//.text:01431E80 ; void __cdecl ai_globals_dialogue_suppress(real32)
//.text:01431EC0 ; bool __cdecl ai_globals_get_ai_active()
//.text:01431EE0 ; void __cdecl ai_globals_grenades_enabled(bool)
//.text:01431F10 ; void __cdecl ai_globals_infection_suppress(int32)

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
//.text:014321A0 ; s_ai_globals_data* __cdecl ai_globals_try_and_get_data(const s_ai_globals_definition*)

void __cdecl ai_globals_update()
{
	INVOKE(0x014321C0, ai_globals_update);
}

//.text:01432690 ; void __cdecl ai_handle_ai_point_move(const real_point3d*, c_ai_point3d*)
//.text:014326E0 ; void __cdecl ai_handle_allegiance_status_changed(e_game_team, e_game_team, bool, bool, bool)
//.text:01432890 ; void __cdecl ai_handle_areas_delete(int16, const uns32*)

void __cdecl ai_handle_bump(int32 biped_index, int32 object_index, const real_vector3d* old_velocity)
{
	INVOKE(0x014328E0, ai_handle_bump, biped_index, object_index, old_velocity);
}

//.text:01432980 ; void __cdecl ai_handle_cs_data_point_move(int16, int16)
//.text:01432990 ; void __cdecl ai_handle_damage(int32, int32, int16, int32, real32, const real_vector3d*, bool)
//.text:01432B20 ; void __cdecl ai_handle_death(int32, int32, int16, int32, real32)
//.text:01433040 ; void __cdecl ai_handle_deleted_object(int32)
//.text:014332F0 ; void __cdecl ai_handle_effect_creation(int32, int16, int32, int16, real32, real32, const real_matrix4x3*)
//.text:01433390 ; void __cdecl ai_handle_equipment_release(int32, int32)
//.text:01433490 ; void __cdecl ai_handle_firing_position_move(int16, int16, const real_vector3d*)
//.text:014334A0 ; void __cdecl ai_handle_get_attention(int32, int32)
//.text:014334D0 ; bool __cdecl ai_handle_killing_spree(int32, int16)
//.text:01433530 ; void __cdecl ai_handle_noisemaker(int32)
//.text:01433690 ; void __cdecl ai_handle_objectives_editing()
//.text:014336A0 ; void __cdecl ai_handle_projectile_attach(int32, int32)
//.text:01433790 ; void __cdecl ai_handle_projectile_impact(int32, int32)
//.text:014337C0 ; void __cdecl ai_handle_sector_point_move(int16, int16, int16, const real_vector3d*, const s_tag_block_definition*)
//.text:014337D0 ; void __cdecl ai_handle_spatial_effect(int32, const real_point3d*, int16, int16, int16)
//.text:01433A70 ; void __cdecl ai_handle_spawn_point_change(int16, int16)
//.text:01433A80 ; void __cdecl ai_handle_squad_addition(int32)
//.text:01433A90 ; void __cdecl ai_handle_squad_deletion(const uns32*)
//.text:01433AA0 ; void __cdecl ai_handle_squad_editing()
//.text:01433AB0 ; void __cdecl ai_handle_squad_group_addition(int32)
//.text:01433AC0 ; void __cdecl ai_handle_squad_group_deletion(const uns32*)
//.text:01433AD0 ; void __cdecl ai_handle_squad_patrol_editing()
//.text:01433AE0 ; void __cdecl ai_handle_squad_patrol_waypoint_addition(int32, int32, int32)
//.text:01433B90 ; void __cdecl ai_handle_squad_patrol_waypoint_deletion(int32, int32, int32)
//.text:01433C80 ; void __cdecl ai_handle_stun(int32, real32, const real_vector3d*)
//.text:01433CE0 ; void __cdecl ai_handle_tasks_addition(int32)
//.text:01433CF0 ; void __cdecl ai_handle_tasks_deletion(const uns32*)
//.text:01433D00 ; void __cdecl ai_handle_unit_effect(int32, e_sound_type, e_ai_sound_volume)
//.text:01433E40 ; void __cdecl ai_handle_weapon_trade(int32, int32, int32, int32)
//.text:01433E70 ; void __cdecl ai_handle_well_point_move(int16, int16, const real_vector3d*)
//.text:01433E80 ; void __cdecl ai_handle_zone_addition(int32)
//.text:01433E90 ; void __cdecl ai_handle_zone_deletion(const uns32*)

void __cdecl ai_initialize()
{
	//INVOKE(0x01433EA0, ai_initialize);

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

void __cdecl ai_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x01434010, ai_initialize_for_new_structure_bsp, activating_structure_bsp_mask);

	//if (ai_globals->ai_initialized_for_map)
	//{
	//	actor_iterator iterator{};
	//	actor_iterator_new(&iterator, false);
	//	for (actor_datum* actor = actor_iterator_next(&iterator); actor; actor = actor_iterator_next(&iterator))
	//	{
	//		if (actor_switch_bsp(iterator.index))
	//		{
	//			int32 squad_index = actor->meta.squad_index;
	//			if (squad_index != NONE && (signed char)(squad_get(squad_index)->flags & 0xFF) < 0)
	//			{
	//				actor_set_active(iterator.index, true);
	//			}
	//		}
	//	}
	//	ai_refresh_reference_frames();
	//	objectives_initialize_for_new_structure_bsp(activating_structure_bsp_mask);
	//
	//	// $TODO stucture bsp pathfinding data validation loop
	//
	//	flocks_initialize_for_new_structure_bsp(activating_structure_bsp_mask);
	//	cs_initialize_for_new_structure_bsp((int32)activating_structure_bsp_mask);
	//}
}

//.text:01434150 ; void __cdecl ai_initialize_for_saved_game(int32)
//.text:01434180 ; void __cdecl ai_place(int32)
//.text:014341B0 ; int32 __cdecl ai_release_inactive_squads(int32, uns8*, int32, bool*, char*, int32)
//.text:01434230 ; int32 __cdecl ai_release_inactive_swarms(int32, uns8*, int32, bool*, char*, int32)
//.text:014342E0 ; void __cdecl ai_reset()
//.text:014343B0 ; void* __cdecl ai_scratch_allocate(unsigned int)
//.text:01434450 ; void __cdecl ai_scratch_free(void*)
//.text:014344B0 ; bool __cdecl ai_test_ballistic_line_of_fire(int32, const real_point3d*, real32, const real_vector3d*, real32, int32, bool)

bool __cdecl ai_test_line_of_fire(int32 actor_index, int32 ignore_unit_index, const real_point3d* origin, const real_vector3d* vector, int32* prop_index_reference)
{
	return INVOKE(0x014346F0, ai_test_line_of_fire, actor_index, ignore_unit_index, origin, vector, prop_index_reference);
}

//.text:014347D0 ; bool __cdecl ai_test_line_of_fire_geometry(int32, int32, const real_point3d*, const real_point3d*)

int16 __cdecl ai_test_line_of_sight(const real_point3d* p0, s_cluster_reference p0_cluster_ref, const real_point3d* p1, s_cluster_reference p1_cluster_ref, int16 mode, bool test_line_of_fire, int32 ignore_object_index, int32 ignore_object_index2, bool ignore_vehicles, bool allow_early_out, int32* blocking_object_index_ref, bool* two_sided_obstruction_ref)
{
	return INVOKE(0x01434AD0, ai_test_line_of_sight, p0, p0_cluster_ref, p1, p1_cluster_ref, mode, test_line_of_fire, ignore_object_index, ignore_object_index2, ignore_vehicles, allow_early_out, blocking_object_index_ref, two_sided_obstruction_ref);
}

//.text:014350F0 ; bool __cdecl ai_try_vehicle_eviction(int32, int32, bool)

void __cdecl ai_update()
{
	//INVOKE(0x014351F0, ai_update);

	if (ai_globals->ai_initialized_for_map && !ai_profile.disable_ai && !game_is_predicted())
	{
		ai_debug_update();
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
	// $IMPLEMENT
}

//.text:014353A0 ; t_restricted_allocation_manager<3,0,0,&void __tls_set_g_ai_globals_allocator(void*)>::allocate
//.text:014353E0 ; 
//.text:01435400 ; 
//.text:01435420 ; 
//.text:01435430 ; 
//.text:01435450 ; bool __cdecl compare_releasable_inactive_squads(const void*, const void*, const void*)
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

