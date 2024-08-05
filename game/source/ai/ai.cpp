#include "ai/ai.hpp"

#include "memory/thread_local.hpp"

//.text:014308D0 ; void __cdecl __tls_set_g_ai_globals_allocator(void*)
//.text:014308F0 ; long __cdecl actor_endangering_player(bool, bool, dword*)
//.text:01430DB0 ; bool __cdecl actor_resurrection_biped_safe(long)
//.text:014310A0 ; bool __cdecl ai_adjust_damage(long, long, s_damage_data*, real*)
//.text:014312F0 ; double __cdecl ai_chance_this_tick(real)
//.text:01431360 ; 
//.text:014313B0 ; double __cdecl ai_danger_level(short)
//.text:014314C0 ; 
//.text:014314D0 ; void __cdecl ai_dispose()
//.text:01431500 ; void __cdecl ai_dispose_from_old_map()
//.text:01431570 ; void __cdecl ai_dispose_from_old_structure_bsp(dword)
//.text:01431670 ; bool __cdecl ai_enemies_attacking_players(long*, dword*)

bool __cdecl ai_enemies_can_see_player(long* out_unit_index)
{
	return INVOKE(0x014316D0, ai_enemies_can_see_player, out_unit_index);
}

void __cdecl ai_erase(long squad_index, bool delete_immediately)
{
	INVOKE(0x01431720, ai_erase, squad_index, delete_immediately);

	//TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	//if (ai_globals->__unknown1)
	//{
	//	if (squad_index == NONE)
	//	{
	//		actor_iterator actor_iter{};
	//		actor_iterator_new(&actor_iter, false);
	//		while (actor_iterator_next(&actor_iter))
	//			actor_erase(actor_iter.index, delete_immediately);
	//	}
	//	else
	//	{
	//		squad_actor_iterator squad_actor_iter{};
	//		squad_actor_iterator_new(&squad_actor_iter, squad_index, false);
	//		while (squad_actor_iterator_next(&squad_actor_iter))
	//			actor_erase(squad_actor_iter.actor_index, delete_immediately);
	//	}
	//}
}

//.text:014317D0 ; void __cdecl ai_find_inactive_squads(long, byte*, long)
//.text:014318F0 ; void __cdecl ai_find_line_of_fire_friend_pills(long, long, line_of_fire_pill*)
//.text:01431B50 ; bool __cdecl ai_find_location(real_point3d const*, s_location*)
//.text:01431BC0 ; 
//.text:01431C00 ; void __cdecl ai_generate_line_of_fire_pill(long, long, line_of_fire_pill*)
//.text:01431C90 ; bool __cdecl ai_get_active_clusters(long, dword*, long)
//.text:01431D50 ; void __cdecl ai_get_center_of_mass(long, real_point3d*)
//.text:01431DE0 ; long __cdecl ai_get_unit_responsible_for_damage(long, bool)
//.text:01431E30 ; void __cdecl ai_globals_dialogue_enable(bool)
//.text:01431E50 ; bool __cdecl ai_globals_dialogue_enabled()
//.text:01431E80 ; void __cdecl ai_globals_dialogue_suppress(real)
//.text:01431EC0 ; bool __cdecl ai_globals_get_ai_active()
//.text:01431EE0 ; void __cdecl ai_globals_grenades_enabled(bool)
//.text:01431F10 ; void __cdecl ai_globals_infection_suppress(long)
//.text:01431F60 ; ai_globals_initialize?
//.text:01431F90 ; void __cdecl ai_globals_initialize_for_new_map(void)
//.text:01432120 ; void __cdecl ai_globals_player_dialogue_enable(bool)
//.text:01432140 ; bool __cdecl ai_globals_player_dialogue_enabled()

void __cdecl ai_globals_set_ai_active(bool ai_active)
{
	INVOKE(0x01432160, ai_globals_set_ai_active, ai_active);
}

//.text:01432180 ; void __cdecl ai_globals_set_fast_and_dumb(bool)
//.text:014321A0 ; 
//.text:014321C0 ; void __cdecl ai_globals_update()
//.text:01432690 ; 
//.text:014326E0 ; void __cdecl ai_handle_allegiance_status_changed(e_campaign_team, e_campaign_team, bool, bool, bool)
//.text:01432890 ; 

void __cdecl ai_handle_bump(long object_index, long bump_object_index, vector3d const* linear_velocity)
{
	INVOKE(0x014328E0, ai_handle_bump, object_index, bump_object_index, linear_velocity);
}

//.text:01432980 ; 
//.text:01432990 ; void __cdecl ai_handle_damage(long, long, short, long, real, vector3d const*, bool)
//.text:01432B20 ; void __cdecl ai_handle_death(long, long, short, long, real)
//.text:01433040 ; void __cdecl ai_handle_deleted_object(long)
//.text:014332F0 ; void __cdecl ai_handle_effect_creation(long, short, long, short, real, real, real_matrix4x3 const*)
//.text:01433390 ; void __cdecl ai_handle_equipment_release(long, long)
//.text:01433490 ; 
//.text:014334A0 ; void __cdecl ai_handle_get_attention(long, long)
//.text:014334D0 ; bool __cdecl ai_handle_killing_spree(long, short)
//.text:01433530 ; void __cdecl ai_handle_noisemaker(long)
//.text:01433690 ; void __cdecl ai_handle_objectives_editing()
//.text:014336A0 ; void __cdecl ai_handle_projectile_attach(long, long)
//.text:01433790 ; void __cdecl ai_handle_projectile_impact(long, long)
//.text:014337C0 ; 
//.text:014337D0 ; void __cdecl ai_handle_spatial_effect(long, real_point3d const*, short, short, short)
//.text:01433A70 ; 
//.text:01433A80 ; 
//.text:01433A90 ; 
//.text:01433AA0 ; 
//.text:01433AB0 ; 
//.text:01433AC0 ; 
//.text:01433AD0 ; 
//.text:01433AE0 ; 
//.text:01433B90 ; 
//.text:01433C80 ; void __cdecl ai_handle_stun(long, real, vector3d const*)
//.text:01433CE0 ; 
//.text:01433CF0 ; 
//.text:01433D00 ; void __cdecl ai_handle_unit_effect(long, e_sound_type, e_ai_sound_volume)
//.text:01433E40 ; void __cdecl ai_handle_weapon_trade(long, long, long, long)
//.text:01433E70 ; 
//.text:01433E80 ; 
//.text:01433E90 ; 
//.text:01433EA0 ; void __cdecl ai_initialize()
//.text:01433F70 ; void __cdecl ai_initialize_for_new_map()
//.text:01434010 ; void __cdecl ai_initialize_for_new_structure_bsp(dword)
//.text:01434150 ; void __cdecl ai_initialize_for_saved_game(long)
//.text:01434180 ; void __cdecl ai_place(long)
//.text:014341B0 ; long __cdecl ai_release_inactive_squads(long, byte*, long, bool*, char*, long)
//.text:01434230 ; long __cdecl ai_release_inactive_swarms(long, byte*, long, bool*, char*, long)
//.text:014342E0 ; void __cdecl ai_reset()
//.text:014343B0 ; void * __cdecl ai_scratch_allocate(unsigned int)
//.text:01434450 ; void __cdecl ai_scratch_free(void*)
//.text:014344B0 ; bool __cdecl ai_test_ballistic_line_of_fire(long, real_point3d const*, real, vector3d const*, real, long, bool)
//.text:014346F0 ; bool __cdecl ai_test_line_of_fire(long, long, real_point3d const*, vector3d const*, long*)
//.text:014347D0 ; ai_test_line_of_sight
//.text:01434AD0 ; ai_test_line_of_sight
//.text:014350F0 ; bool __cdecl ai_try_vehicle_eviction(long, long, bool)

void __cdecl ai_update()
{
	INVOKE(0x014351F0, ai_update);
}

//.text:01435290 ; void __cdecl ai_update_team_status()

