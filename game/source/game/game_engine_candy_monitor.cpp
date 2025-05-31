#include "game/game_engine_candy_monitor.hpp"

#include "items/items.hpp"
#include "memory/thread_local.hpp"

//.text:0058F660 ; 
//.text:0058F680 ; 
//.text:0058F690 ; 
//.text:0058F6A0 ; 
//.text:0058F6C0 ; 
//.text:0058F6E0 ; 
//.text:0058F6F0 ; public: void __cdecl c_candy_monitor::delete_object(bool)
//.text:0058F720 ; public: void __cdecl c_multiplayer_candy_monitor_manager::detach_object(int32)
//.text:0058F790 ; public: void __cdecl c_multiplayer_candy_monitor_manager::dispose_from_old_round()
//.text:0058F840 ; 
//.text:0058F850 ; 

bool __cdecl game_engine_any_player_nearby_and_looking_at_sphere(const real_point3d* sphere_center, real32 sphere_radius, real32 distance)
{
	return INVOKE(0x0058F860, game_engine_any_player_nearby_and_looking_at_sphere, sphere_center, sphere_radius, distance);
}

void __cdecl game_engine_candy_monitor_prepare_for_promotion_to_simulation_authority()
{
	INVOKE(0x0058F950, game_engine_candy_monitor_prepare_for_promotion_to_simulation_authority);

	//game_engine_globals->candy_monitor_manager.initialize_for_new_map();
}

bool __cdecl game_engine_object_being_used(int32 object_index)
{
	return INVOKE(0x0058F970, game_engine_object_being_used, object_index);

	//if (object_index != NONE)
	//{
	//	const object_header_datum* object_header = object_header_get(object_index);
	//	if (object_header->datum->object.parent_object_index != NONE)
	//		return true;
	//
	//	if (object_header->object_type == _object_type_vehicle)
	//		return game_engine_vehicle_has_passengers(object_index);
	//
	//	if (object_header->object_type == _object_type_weapon || object_header->object_type == _object_type_equipment)
	//		return object_is_hidden(object_index);
	//}
	//
	//return false;
}

bool __cdecl game_engine_object_has_been_abandoned(int32 object_index)
{
	return INVOKE(0x0058F9E0, game_engine_object_has_been_abandoned, object_index);
}

bool __cdecl game_engine_object_has_been_disturbed(int32 object_index)
{
	return INVOKE(0x0058FB30, game_engine_object_has_been_disturbed, object_index);
}

bool __cdecl game_engine_object_can_be_damaged(int32 object_index)
{
	return INVOKE(0x0058FCA0, game_engine_object_can_be_damaged, object_index);
}

bool __cdecl game_engine_object_is_item_in_inventory(int32 object_index)
{
	return INVOKE(0x0058FCE0, game_engine_object_is_item_in_inventory, object_index);

	//if (object_index != NONE)
	//	return ITEM_GET(object_index)->item.inventory_state != 0;
	//
	//return false;
}

bool __cdecl game_engine_object_is_useless_weapon(int32 object_index)
{
	return INVOKE(0x0058FD40, game_engine_object_is_useless_weapon, object_index);
}

bool __cdecl game_engine_player_is_looking_at_sphere(int32 player_index, const real_point3d* sphere_center, real32 sphere_radius)
{
	return INVOKE(0x0058FDB0, game_engine_player_is_looking_at_sphere, player_index, sphere_center, sphere_radius);
}

bool __cdecl game_engine_player_is_nearby(const real_point3d* point, real32 radius)
{
	return INVOKE(0x0058FEE0, game_engine_player_is_nearby, point, radius);
}

void __cdecl game_engine_register_object(int32 object_index)
{
	INVOKE(0x0058FFB0, game_engine_register_object, object_index);
}

int32 __cdecl game_engine_remap_vehicle_definition_index(int32 vehicle_definition_index)
{
	return INVOKE(0x00590060, game_engine_remap_vehicle_definition_index, vehicle_definition_index);
}

int32 __cdecl game_engine_remap_weapon_definition_index(int32 weapon_definition_index)
{
	return INVOKE(0x005900B0, game_engine_remap_weapon_definition_index, weapon_definition_index);
}

void __cdecl game_engine_unregister_map_variant_object(int32 map_variant_index)
{
	INVOKE(0x00590110, game_engine_unregister_map_variant_object, map_variant_index);
}

void __cdecl game_engine_unregister_object(int32 object_index)
{
	INVOKE(0x00590180, game_engine_unregister_object, object_index);
}

bool __cdecl game_engine_vehicle_has_passengers(int32 object_index)
{
	return INVOKE(0x005901C0, game_engine_vehicle_has_passengers, object_index);
}

//.text:00590230 ; 
//.text:00590240 ; public: int32 __cdecl c_multiplayer_candy_monitor_manager::get_active_object_count(int32, int32)
//.text:00590300 ; public: int32 __cdecl c_multiplayer_candy_monitor_manager::get_candy_monitor_object_definition_index(int16)
//.text:00590390 ; 
//.text:005903A0 ; 
//.text:005903B0 ; 
//.text:005903C0 ; 
//.text:005903D0 ; 
//.text:005903E0 ; 
//.text:005903F0 ; 
//.text:00590400 ; 
//.text:00590410 ; public: void __cdecl c_candy_monitor::initialize(int32)
//.text:00590450 ; public: void __cdecl c_candy_spawner::initialize(int32)
//.text:00590560 ; 
//.text:00590580 ; public: void __cdecl c_multiplayer_candy_monitor_manager::initialize_for_new_map()
//.text:005905F0 ; public: void __cdecl c_multiplayer_candy_monitor_manager::initialize_for_new_round()
//.text:00590600 ; public: void __cdecl c_multiplayer_candy_monitor_manager::monitor_object(int32, int32)
//.text:00590730 ; public: c_candy_monitor* __cdecl c_multiplayer_candy_monitor_manager::monitor_try_and_get(int32)
//.text:00590780 ; 
//.text:00590790 ; 

int32 __cdecl object_get_respawn_time_seconds(int32 object_index)
{
	return INVOKE(0x005907A0, object_get_respawn_time_seconds, object_index);
}

//.text:005908B0 ; public: bool __cdecl c_multiplayer_candy_monitor_manager::quotas_permit_spawn(int16, bool*)
//.text:005909F0 ; 
//.text:00590A00 ; 
//.text:00590A10 ; public: void __cdecl c_candy_spawner::respawn_timer_update()

int32 __cdecl scenario_object_get_abandonment_time_seconds(int32 object_index)
{
	return INVOKE(0x00590A80, scenario_object_get_abandonment_time_seconds, object_index);
}

//.text:00590B40 ; 
//.text:00590C30 ; 

bool __cdecl scenario_object_is_significant(e_object_type object_type, int32 scenario_datum_index)
{
	return INVOKE(0x00590C60, scenario_object_is_significant, object_type, scenario_datum_index);
}

//.text:00590CA0 ; 
//.text:00590CC0 ; 
//.text:00590CD0 ; 
//.text:00590CE0 ; public: bool __cdecl c_candy_spawner::spawn_object(bool)
//.text:00591090 ; 
//.text:005910D0 ; 
//.text:005910F0 ; public: void __cdecl c_multiplayer_candy_monitor_manager::try_to_spawn_new_object(int16, c_candy_spawner*)
//.text:005911A0 ; public: void __cdecl c_candy_monitor::update()
//.text:00591290 ; public: void __cdecl c_candy_spawner::update()
//.text:005913E0 ; public: void __cdecl c_multiplayer_candy_monitor_manager::update()

