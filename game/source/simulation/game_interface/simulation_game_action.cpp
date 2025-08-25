#include "simulation/game_interface/simulation_game_action.hpp"

#include "game/game.hpp"
#include "objects/objects.hpp"
#include "simulation/simulation_gamestate_entities.hpp"
#include "simulation/simulation_type_collection.hpp"

e_simulation_entity_type c_simulation_object_update_flags::get_simulation_entity_type(int32 object_index)
{
	return INVOKE_CLASS_MEMBER(0x004B1C20, c_simulation_object_update_flags, get_simulation_entity_type, object_index);
}

void c_simulation_object_update_flags::set_flag(int32 device_index, e_simulation_device_update_flag flag)
{
	DECLFUNC(0x004B1D80, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_device_update_flag)(this, device_index, flag);

	//if (simulation_object_is_attached_to_distributed_networking(device_index))
	//	ASSERT(get_simulation_entity_type(device_index) == _simulation_entity_type_device);
	//
	//set(flag, true);
}

void c_simulation_object_update_flags::set_flag(int32 generic_index, e_simulation_generic_update_flag flag)
{
	DECLFUNC(0x004B1DB0, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_generic_update_flag)(this, generic_index, flag);

	//if (simulation_object_is_attached_to_distributed_networking(generic_index))
	//	ASSERT(get_simulation_entity_type(generic_index) == _simulation_entity_type_generic || get_simulation_entity_type(generic_index) == _simulation_entity_type_generic_garbage);
	//
	//set(flag, true);
}

void c_simulation_object_update_flags::set_flag(int32 item_index, e_simulation_item_update_flag flag)
{
	DECLFUNC(0x004B1DE0, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_item_update_flag)(this, item_index, flag);

	//if (simulation_object_is_attached_to_distributed_networking(item_index))
	//	ASSERT(get_simulation_entity_type(item_index) == _simulation_entity_type_item || get_simulation_entity_type(item_index) == _simulation_entity_type_weapon);
	//
	//set(flag, true);
}

void c_simulation_object_update_flags::set_flag(int32 object_index, e_simulation_object_update_flag flag)
{
	DECLFUNC(0x004B1E10, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_object_update_flag)(this, object_index, flag);

	//if (simulation_object_is_attached_to_distributed_networking(object_index))
	//	ASSERT(simulation_entity_type_is_object(get_simulation_entity_type(object_index)));
	//
	//set(flag, true);
}

void c_simulation_object_update_flags::set_flag(int32 projectile_index, e_simulation_projectile_update_flag flag)
{
	DECLFUNC(0x004B1E40, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_projectile_update_flag)(this, projectile_index, flag);

	//if (simulation_object_is_attached_to_distributed_networking(projectile_index))
	//	ASSERT(get_simulation_entity_type(projectile_index) == _simulation_entity_type_projectile);
	//
	//set(flag, true);
}

void c_simulation_object_update_flags::set_flag(int32 unit_index, e_simulation_unit_update_flag flag)
{
	DECLFUNC(0x004B1E70, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_unit_update_flag)(this, unit_index, flag);

	//if (simulation_object_is_attached_to_distributed_networking(unit_index))
	//	ASSERT(get_simulation_entity_type(unit_index) == _simulation_entity_type_unit);
	//
	//set(flag, true);
}

void c_simulation_object_update_flags::set_flag(int32 vehicle_index, e_simulation_vehicle_update_flag flag)
{
	DECLFUNC(0x004B1EA0, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_vehicle_update_flag)(this, vehicle_index, flag);

	//if (simulation_object_is_attached_to_distributed_networking(vehicle_index))
	//	ASSERT(get_simulation_entity_type(vehicle_index) == _simulation_entity_type_vehicle);
	//
	//set(flag, true);
}

void c_simulation_object_update_flags::set_flag(int32 weapon_index, e_simulation_weapon_update_flag flag)
{
	DECLFUNC(0x004B1ED0, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_weapon_update_flag)(this, weapon_index, flag);

	//if (simulation_object_is_attached_to_distributed_networking(weapon_index))
	//	ASSERT(get_simulation_entity_type(weapon_index) == _simulation_entity_type_weapon);
	//
	//set(flag, true);
}

//.text:004B2440 ; void __cdecl simulation_action_game_ai_update(uns32)
//.text:004B2490 ; void __cdecl simulation_action_game_engine_globals_create()
//.text:004B2510 ; void __cdecl simulation_action_game_engine_globals_delete()

void __cdecl simulation_action_game_engine_globals_update(c_flags<int32, uns64, 64>& flags)
{
	INVOKE(0x004B2570, simulation_action_game_engine_globals_update, flags);
}

//.text:004B25C0 ; void __cdecl simulation_action_game_engine_player_create(int16)
//.text:004B2640 ; void __cdecl simulation_action_game_engine_player_delete(int16)

void __cdecl simulation_action_game_engine_player_update(int32 player_index, c_flags<int32, uns64, 64>& flags)
{
	INVOKE(0x004B26B0, simulation_action_game_engine_player_update, player_index, flags);
}

//.text:004B2BE0 ; void __cdecl simulation_action_hit_marker(int32)
//.text:004B2C00 ; void __cdecl simulation_action_multiplayer_event(const s_game_engine_event_data*)

void __cdecl simulation_action_object_create(int32 object_index)
{
	INVOKE(0x004B2CD0, simulation_action_object_create, object_index);
}

//.text:004B2D90 ; void __cdecl simulation_action_object_create_build_entity_types(int32, int32, int32, int32*, e_simulation_entity_type*, int32*)

void __cdecl simulation_action_object_delete(int32 object_index)
{
	INVOKE(0x004B2EA0, simulation_action_object_delete, object_index);

	//if (game_is_server() && game_is_distributed())
	//{
	//	object_datum* object = object_get(object_index);
	//	if (simulation_object_is_attached_to_distributed_networking(object_index))
	//	{
	//		if (!game_is_playback())
	//		{
	//			int32 simulation_entity_index = simulation_object_glue_get_simulation_entity_index(object->object.simulation_object_glue_index);
	//			if (simulation_entity_index != NONE)
	//			{
	//				simulation_entity_delete(simulation_entity_index);
	//			}
	//		}
	//		simulation_object_glue_delete(object->object.simulation_object_glue_index);
	//		object_detach_gamestate_entity(object_index, object->object.simulation_object_glue_index);
	//	}
	//}
}

void __cdecl simulation_action_object_detach_from_gamestate_and_delete(int32 object_index)
{
	INVOKE(0x004B2F40, simulation_action_object_detach_from_gamestate_and_delete, object_index);

	//object_datum* object = object_get(object_index);
	//if (simulation_object_is_attached_to_distributed_networking(object_index))
	//{
	//	object_detach_gamestate_entity(object_index, object->object.simulation_object_glue_index);
	//	simulation_gamestate_entity_set_object_index(object->object.simulation_object_glue_index, NONE);
	//}
	//object_delete(object_index);
}

void __cdecl simulation_action_object_force_update(int32 object_index, c_simulation_object_update_flags& flags)
{
	INVOKE(0x004B2FA0, simulation_action_object_force_update, object_index, flags);

	//if (game_is_distributed() && game_is_server() && !game_is_playback())
	//{
	//	object_datum* object = object_get(object_index);
	//	if (simulation_object_is_attached_to_distributed_networking(object_index))
	//	{
	//		int32 simulation_entity_index = simulation_object_glue_get_simulation_entity_index(object->object.simulation_object_glue_index);
	//		if (simulation_entity_index != NONE)
	//		{
	//			simulation_entity_force_update(simulation_entity_index, object_index, flags);
	//		}
	//	}
	//}
}

void simulation_action_object_update_internal(int32 object_index, c_simulation_object_update_flags& flags)
{
	INVOKE(0x004B3010, simulation_action_object_update_internal, object_index, flags);

	//if (game_is_distributed() && game_is_server() && !game_is_playback())
	//{
	//	object_datum* object = object_get(object_index);
	//	if (simulation_object_is_attached_to_distributed_networking(object_index))
	//	{
	//		int32 simulation_entity_index = simulation_object_glue_get_simulation_entity_index(object->object.simulation_object_glue_index);
	//		if (simulation_entity_index != NONE)
	//		{
	//			simulation_entity_update(simulation_entity_index, object_index, flags);
	//		}
	//	}
	//}
}

//.text:004B3080 ; void __cdecl simulation_action_pickup_ammo(int32, int32, int32, int32)
//.text:004B3140 ; void __cdecl simulation_action_pickup_equipment(int32, int32)
//.text:004B31C0 ; void __cdecl simulation_action_pickup_powerup(int32, int32)
//.text:004B3240 ; void __cdecl simulation_action_pickup_weapon(int32, int32)
//.text:004B32C0 ; simulation_action_player_taunt
//.text:004B3300 ; void __cdecl simulation_action_projectile_attached(int32, int32, int16, const real_point3d*, const s_location*)

//.text:004B4650 ; void __cdecl simulation_player_force_base_respawn(int32)
//.text:004B4690 ; bool __cdecl simulation_query_control(int32, int32, int32*)
//.text:004B47D0 ; bool __cdecl simulation_query_local_control(int32, int32)
//.text:004B4810 ; bool __cdecl simulation_query_local_predictive_control(int32, int32)

bool __cdecl simulation_query_object_is_predicted(int32 object_index)
{
	return INVOKE(0x004B4850, simulation_query_object_is_predicted, object_index);
}

bool __cdecl simulation_query_object_placement(const object_placement_data* data)
{
	return INVOKE(0x004B48A0, simulation_query_object_placement, data);
}

//.text:004B48E0 ; bool __cdecl simulation_query_predictive_event(int32, int32, int32*)
//.text:004B4950 ; bool __cdecl simulation_request_actor_create(int32, int32)
//.text:004B49E0 ; void __cdecl simulation_request_actor_delete(int32)
//.text:004B49F0 ; void __cdecl simulation_request_autopickup_ammo(int32, int32, int32)
//.text:004B4A00 ; 
//.text:004B4A10 ; void __cdecl simulation_request_autopickup_powerup(int32, int32)
//.text:004B4A20 ; void __cdecl simulation_request_autopickup_grenade(int32, int32)
//.text:004B4A30 ; void __cdecl simulation_request_drop_weapon(int32, int16)
//.text:004B4B10 ; void __cdecl simulation_request_flip_vehicle(int32, int32, bool)
//.text:004B4B70 ; void __cdecl simulation_request_object_refresh(int32)
//.text:004B4CA0 ; void __cdecl simulation_request_pickup_equipment(int32, int32)
//.text:004B4D00 ; void __cdecl simulation_request_pickup_weapon(int32, int32, e_weapon_addition_method)
//.text:004B4D90 ; bool __cdecl simulation_request_player_attach(int32, int32)
//.text:004B4E10 ; void __cdecl simulation_request_player_boot(int32, int32)
//.text:004B4E50 ; void __cdecl simulation_request_player_detach(int32)
//.text:004B4E90 ; void __cdecl simulation_request_player_editor(int32, e_player_editor_request_type, bool, int32)
//.text:004B4F10 ; void __cdecl simulation_request_player_event(const s_player_editor_request_event_data*)
//.text:004B4F70 ; 
//.text:004B4FF0 ; 
//.text:004B5020 ; void __cdecl simulation_request_unit_current_equipment_use(int32, int32)
//.text:004B5080 ; void __cdecl simulation_request_projectile_supercombine(int32)
//.text:004B5100 ; void __cdecl simulation_request_put_away_weapon(int32, int16)
//.text:004B51E0 ; void __cdecl simulation_request_touch_device(int32, int32)
//.text:004B51E0 ; void __cdecl simulation_request_touch_device(int32, int32)
//.text:004B5240 ; void __cdecl simulation_request_unit_assassinate(int32, int32, int32, const real_point3d*, const real_vector3d*, const real_vector3d*, const s_simulation_biped_melee_damage_event_data*)
//.text:004B5300 ; void __cdecl simulation_request_unit_enter_vehicle(int32, int32, int16)
//.text:004B5360 ; void __cdecl simulation_request_unit_exit_and_detach_vehicle(int32)
//.text:004B53F0 ; void __cdecl simulation_request_unit_exit_vehicle(int32)
//.text:004B5480 ; void __cdecl simulation_request_unit_melee_damage(int32, int32, int32, const s_unit_player_melee_damage_target*)
//.text:004B5630 ; void __cdecl simulation_request_unit_melee_damage_with_prebuilt_event_data(int32, int32, s_simulation_biped_melee_damage_event_data*)
//.text:004B56C0 ; void __cdecl simulation_request_unit_throw_release(int32, int16, int32, const real_point3d*, const real_vector3d*)
//.text:004B5810 ; void __cdecl simulation_request_vehicle_trick(int32, int32)

c_simulation_object_update_flags::c_simulation_object_update_flags() :
	c_flags<int32, uns64, 64>()
{
}

template<typename t_flag_enum>
void simulation_action_object_update(int32 object_index, t_flag_enum flag)
{
	throw "no available 'simulation_action_object_update' templated function";
}

c_simulation_object_update_flags::c_simulation_object_update_flags(int32 object_index, e_simulation_unit_update_flag flag) :
	c_flags<int32, uns64, 64>()
{
	// $TODO find the offset for this function
	
	set_flag(object_index, flag);
}

template<>
void __cdecl simulation_action_object_update<e_simulation_unit_update_flag>(int32 unit_index, e_simulation_unit_update_flag flag)
{
	// $TODO find the unit_indexfor this function

	c_simulation_object_update_flags flags(unit_index, flag);
	simulation_action_object_update_internal(unit_index, flags);
}

c_simulation_object_update_flags::c_simulation_object_update_flags(int32 object_index, e_simulation_object_update_flag flag) :
	c_flags<int32, uns64, 64>()
{
	//DECLFUNC(0x0054CF60, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_object_update_flag)(this, object_index, flag);

	set_flag(object_index, flag);
}

template<>
void __cdecl simulation_action_object_update<e_simulation_object_update_flag>(int32 object_index, e_simulation_object_update_flag flag)
{
	//INVOKE(0x00580AD0, simulation_action_object_update<e_simulation_object_update_flag>, object_index, flag);

	c_simulation_object_update_flags flags(object_index, flag);
	simulation_action_object_update_internal(object_index, flags);
}

template<>
void __cdecl simulation_action_object_update<e_simulation_weapon_update_flag>(int32 weapon_index, e_simulation_weapon_update_flag flag)
{
	//INVOKE(0x005A06A0, simulation_action_object_update<e_simulation_weapon_update_flag>, weapon_index, flag);

	c_simulation_object_update_flags flags(weapon_index, flag);
	simulation_action_object_update_internal(weapon_index, flags);
}

c_simulation_object_update_flags::c_simulation_object_update_flags(int32 object_index, e_simulation_weapon_update_flag flag) :
	c_flags<int32, uns64, 64>()
{
	//DECLFUNC(0x005A06E0, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_weapon_update_flag)(this, object_index, flag);

	set_flag(object_index, flag);
}

template<>
void __cdecl simulation_action_object_update<e_simulation_generic_update_flag>(int32 generic_index, e_simulation_generic_update_flag flag)
{
	//INVOKE(0x00B26CA0, simulation_action_object_update<e_simulation_generic_update_flag>, generic_index, flag);

	c_simulation_object_update_flags flags(generic_index, flag);
	simulation_action_object_update_internal(generic_index, flags);
}

template<>
void __cdecl simulation_action_object_update<e_simulation_item_update_flag>(int32 item_index, e_simulation_item_update_flag flag)
{
	//INVOKE(0x00B26CD0, simulation_action_object_update<e_simulation_item_update_flag>, item_index, flag);

	c_simulation_object_update_flags flags(item_index, flag);
	simulation_action_object_update_internal(item_index, flags);
}

template<>
void __cdecl simulation_action_object_update<e_simulation_projectile_update_flag>(int32 projectile_index, e_simulation_projectile_update_flag flag)
{
	//INVOKE(0x00B26D00, simulation_action_object_update<e_simulation_projectile_update_flag>, projectile_index, flag);

	c_simulation_object_update_flags flags(projectile_index, flag);
	simulation_action_object_update_internal(projectile_index, flags);
}

c_simulation_object_update_flags::c_simulation_object_update_flags(int32 object_index, e_simulation_generic_update_flag flag) :
	c_flags<int32, uns64, 64>()
{
	//DECLFUNC(0x00B26ED0, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_generic_update_flag)(this, object_index, flag);

	set_flag(object_index, flag);
}

c_simulation_object_update_flags::c_simulation_object_update_flags(int32 object_index, e_simulation_item_update_flag flag) :
	c_flags<int32, uns64, 64>()
{
	//DECLFUNC(0x00B26F00, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_item_update_flag)(this, object_index, flag);

	set_flag(object_index, flag);
}

c_simulation_object_update_flags::c_simulation_object_update_flags(int32 object_index, e_simulation_projectile_update_flag flag) :
	c_flags<int32, uns64, 64>()
{
	//DECLFUNC(0x00B26F30, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_projectile_update_flag)(this, object_index, flag);

	set_flag(object_index, flag);
}

template<>
void __cdecl simulation_action_object_update<e_simulation_vehicle_update_flag>(int32 vehicle_index, e_simulation_vehicle_update_flag flag)
{
	//INVOKE(0x00B377D0, simulation_action_object_update<e_simulation_vehicle_update_flag>, vehicle_index, flag);

	c_simulation_object_update_flags flags(vehicle_index, flag);
	simulation_action_object_update_internal(vehicle_index, flags);
}

c_simulation_object_update_flags::c_simulation_object_update_flags(int32 object_index, e_simulation_vehicle_update_flag flag) :
	c_flags<int32, uns64, 64>()
{
	//DECLFUNC(0x00B37810, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_vehicle_update_flag)(this, object_index, flag);

	set_flag(object_index, flag);
}

template<>
void __cdecl simulation_action_object_update<e_simulation_device_update_flag>(int32 device_index, e_simulation_device_update_flag flag)
{
	//INVOKE(0x00B92160, simulation_action_object_update<e_simulation_device_update_flag>, device_index, flag);

	c_simulation_object_update_flags flags(device_index, flag);
	simulation_action_object_update_internal(device_index, flags);
}

c_simulation_object_update_flags::c_simulation_object_update_flags(int32 object_index, e_simulation_device_update_flag flag) :
	c_flags<int32, uns64, 64>()
{
	//DECLFUNC(0x00B921B0, void, __thiscall, c_simulation_object_update_flags*, int32, e_simulation_device_update_flag)(this, object_index, flag);
	
	set_flag(object_index, flag);
}

bool __cdecl simulation_object_is_attached_to_distributed_networking(int32 object_index)
{
	object_datum* object = object_get(object_index);
	return object->object.simulation_object_glue_index != NONE;
}

