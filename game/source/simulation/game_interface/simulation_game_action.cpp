#include "simulation/game_interface/simulation_game_action.hpp"

//.text:004B2440 ; void __cdecl simulation_action_game_ai_update(dword)
//.text:004B2490 ; void __cdecl simulation_action_game_engine_globals_create()
//.text:004B2510 ; void __cdecl simulation_action_game_engine_globals_delete()

void __cdecl simulation_action_game_engine_globals_update(c_flags<long, qword, 64>& flags)
{
	INVOKE(0x004B2570, simulation_action_game_engine_globals_update, flags);
}

//.text:004B25C0 ; void __cdecl simulation_action_game_engine_player_create(short)
//.text:004B2640 ; void __cdecl simulation_action_game_engine_player_delete(short)

void __cdecl simulation_action_game_engine_player_update(long player_index, c_flags<long, qword, 64>& flags)
{
	INVOKE(0x004B26B0, simulation_action_game_engine_player_update, player_index, flags);
}

//.text:004B2BE0 ; void __cdecl simulation_action_hit_marker(long)
//.text:004B2C00 ; void __cdecl simulation_action_multiplayer_event(s_game_engine_event_data const*)

void __cdecl simulation_action_object_create(long object_index)
{
	INVOKE(0x004B2CD0, simulation_action_object_create, object_index);
}

//.text:004B2D90 ; void __cdecl simulation_action_object_create_build_entity_types(long, long, long, long*, e_simulation_entity_type*, long*)
//.text:004B2EA0 ; void __cdecl simulation_action_object_delete(long)
//.text:004B2F40 ; void __cdecl simulation_action_object_detach_from_gamestate_and_delete(long)
//.text:004B2FA0 ; void __cdecl simulation_action_object_force_update(long, c_simulation_object_update_flags)
//.text:004B3010 ; void __cdecl simulation_action_object_update_internal(long, c_simulation_object_update_flags)
//.text:004B3080 ; void __cdecl simulation_action_pickup_ammo(long, long, long, long)
//.text:004B3140 ; void __cdecl simulation_action_pickup_equipment(long, long)
//.text:004B31C0 ; void __cdecl simulation_action_pickup_powerup(long, long)
//.text:004B3240 ; void __cdecl simulation_action_pickup_weapon(long, long)
//.text:004B3240 ; simulation_action_player_taunt
//.text:004B3300 ; void __cdecl simulation_action_projectile_attached(long, long, short, real_point3d const*, s_location const*)

//.text:004B4650 ; void __cdecl simulation_player_force_base_respawn(long)
//.text:004B4690 ; bool __cdecl simulation_query_control(long, long, long*)
//.text:004B47D0 ; bool __cdecl simulation_query_local_control(long, long)
//.text:004B4810 ; bool __cdecl simulation_query_local_predictive_control(long, long)

bool __cdecl simulation_query_object_is_predicted(long object_index)
{
	return INVOKE(0x004B4850, simulation_query_object_is_predicted, object_index);
}

bool __cdecl simulation_query_object_placement(object_placement_data const* data)
{
	return INVOKE(0x004B48A0, simulation_query_object_placement, data);
}

//.text:004B48E0 ; bool __cdecl simulation_query_predictive_event(long, long, long*)
//.text:004B4950 ; bool __cdecl simulation_request_actor_create(long, long)
//.text:004B49E0 ; void __cdecl simulation_request_actor_delete(long)
//.text:004B49F0 ; void __cdecl simulation_request_autopickup_ammo(long, long, long)
//.text:004B4A00 ; 
//.text:004B4A10 ; void __cdecl simulation_request_autopickup_powerup(long, long)
//.text:004B4A20 ; void __cdecl simulation_request_autopickup_grenade(long, long)
//.text:004B4A30 ; void __cdecl simulation_request_drop_weapon(long, short)
//.text:004B4B10 ; void __cdecl simulation_request_flip_vehicle(long, long, bool)
//.text:004B4B70 ; void __cdecl simulation_request_object_refresh(long)
//.text:004B4CA0 ; void __cdecl simulation_request_pickup_equipment(long, long)
//.text:004B4D00 ; void __cdecl simulation_request_pickup_weapon(long, long, e_weapon_addition_method)
//.text:004B4D90 ; bool __cdecl simulation_request_player_attach(long, long)
//.text:004B4E10 ; void __cdecl simulation_request_player_boot(long, long)
//.text:004B4E50 ; void __cdecl simulation_request_player_detach(long)
//.text:004B4E90 ; void __cdecl simulation_request_player_editor(long, e_player_editor_request_type, bool, long)
//.text:004B4F10 ; void __cdecl simulation_request_player_event(s_player_editor_request_event_data const*)
//.text:004B4F70 ; 
//.text:004B4FF0 ; 
//.text:004B5020 ; void __cdecl simulation_request_unit_current_equipment_use(long, long)
//.text:004B5080 ; void __cdecl simulation_request_projectile_supercombine(long)
//.text:004B5100 ; void __cdecl simulation_request_put_away_weapon(long, short)
//.text:004B51E0 ; void __cdecl simulation_request_touch_device(long, long)
//.text:004B51E0 ; void __cdecl simulation_request_touch_device(long, long)
//.text:004B5240 ; void __cdecl simulation_request_unit_assassinate(long, long, long, real_point3d const*, vector3d const*, vector3d const*, s_simulation_biped_melee_damage_event_data const*)
//.text:004B5300 ; void __cdecl simulation_request_unit_enter_vehicle(long, long, short)
//.text:004B5360 ; void __cdecl simulation_request_unit_exit_and_detach_vehicle(long)
//.text:004B53F0 ; void __cdecl simulation_request_unit_exit_vehicle(long)
//.text:004B5480 ; void __cdecl simulation_request_unit_melee_damage(long, long, long, s_unit_player_melee_damage_target const*)
//.text:004B5630 ; void __cdecl simulation_request_unit_melee_damage_with_prebuilt_event_data(long, long, s_simulation_biped_melee_damage_event_data*)
//.text:004B56C0 ; void __cdecl simulation_request_unit_throw_release(long, short, long, real_point3d const*, vector3d const*)
//.text:004B5810 ; void __cdecl simulation_request_vehicle_trick(long, long)

