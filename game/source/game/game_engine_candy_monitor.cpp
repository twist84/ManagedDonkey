#include "game/game_engine_candy_monitor.hpp"

bool __cdecl game_engine_any_player_nearby_and_looking_at_sphere(real_point3d const* position, real nearby_radius, real looking_at_radius)
{
	return INVOKE(0x0058F860, game_engine_any_player_nearby_and_looking_at_sphere, position, nearby_radius, looking_at_radius);
}

void __cdecl game_engine_candy_monitor_prepare_for_promotion_to_simulation_authority()
{
	INVOKE(0x0058F950, game_engine_candy_monitor_prepare_for_promotion_to_simulation_authority);
}

bool __cdecl game_engine_object_being_used(long object_index)
{
	return INVOKE(0x0058F970, game_engine_object_being_used, object_index);
}

bool __cdecl game_engine_object_has_been_abandoned(long object_index)
{
	return INVOKE(0x0058F9E0, game_engine_object_has_been_abandoned, object_index);
}

bool __cdecl game_engine_object_has_been_disturbed(long object_index)
{
	return INVOKE(0x0058FB30, game_engine_object_has_been_disturbed, object_index);
}

//.text:0058FCA0 ; 

bool __cdecl game_engine_object_is_item_in_inventory(long object_index)
{
	return INVOKE(0x0058FCE0, game_engine_object_is_item_in_inventory, object_index);
}

bool __cdecl game_engine_object_is_useless_weapon(long object_index)
{
	return INVOKE(0x0058FD40, game_engine_object_is_useless_weapon, object_index);
}

bool __cdecl game_engine_player_is_looking_at_sphere(long player_index, real_point3d const* position, real radius)
{
	return INVOKE(0x0058FDB0, game_engine_player_is_looking_at_sphere, player_index, position, radius);
}

bool __cdecl game_engine_player_is_nearby(real_point3d const* position, real radius)
{
	return INVOKE(0x0058FEE0, game_engine_player_is_nearby, position, radius);
}

void __cdecl game_engine_register_object(long object_index)
{
	INVOKE(0x0058FFB0, game_engine_register_object, object_index);
}

long __cdecl game_engine_remap_vehicle_definition_index(long vehicle_definition_index)
{
	return INVOKE(0x00590060, game_engine_remap_vehicle_definition_index, vehicle_definition_index);
}

long __cdecl game_engine_remap_weapon_definition_index(long weapon_definition_index)
{
	return INVOKE(0x005900B0, game_engine_remap_weapon_definition_index, weapon_definition_index);
}

void __cdecl game_engine_unregister_map_variant_object(long map_variant_index)
{
	INVOKE(0x00590110, game_engine_unregister_map_variant_object, map_variant_index);
}

void __cdecl game_engine_unregister_object(long object_index)
{
	INVOKE(0x00590180, game_engine_unregister_object, object_index);
}

bool __cdecl game_engine_vehicle_has_passengers(long object_index)
{
	return INVOKE(0x005901C0, game_engine_vehicle_has_passengers, object_index);
}

