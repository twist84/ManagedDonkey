#pragma once

#include "cseries/cseries.hpp"

class c_candy_spawner
{
public:
	int8 m_flags;
	int32 m_map_variant_index;
	int32 m_last_object_spawned_index;
	int16 m_respawn_timer_seconds;
	int16 pad;
};
COMPILE_ASSERT(sizeof(c_candy_spawner) == 0x10);

class c_candy_monitor
{
public:
	int32 m_spawned_object_index;
	int16 m_abandoned_seconds;
	bool m_disturbed_from_initial_placement;
	bool m_pad[0x3];
	int16 m_abandonment_timeout;
};
COMPILE_ASSERT(sizeof(c_candy_monitor) == 0xC);

class c_multiplayer_candy_monitor_manager
{
public:
	enum
	{
		k_dummy_spawn_monitor_index = 0x1971,
		k_maximum_number_of_monitored_pieces_of_candy = 512,
		k_insignificant_object_abandonment_threshold_seconds = 10,
	};

	c_static_array<c_candy_spawner, k_maximum_number_of_monitored_pieces_of_candy> m_candy_spawners;
	c_static_array<c_candy_monitor, k_maximum_number_of_monitored_pieces_of_candy> m_candy_monitors;
};
COMPILE_ASSERT(sizeof(c_multiplayer_candy_monitor_manager) == 0x3800);

extern bool __cdecl game_engine_any_player_nearby_and_looking_at_sphere(const real_point3d* sphere_center, real32 sphere_radius, real32 distance);
extern void __cdecl game_engine_candy_monitor_prepare_for_promotion_to_simulation_authority();
extern bool __cdecl game_engine_object_being_used(int32 object_index);
extern bool __cdecl game_engine_object_has_been_abandoned(int32 object_index);
extern bool __cdecl game_engine_object_has_been_disturbed(int32 object_index);
extern bool __cdecl game_engine_object_can_be_damaged(int32 object_index);
extern bool __cdecl game_engine_object_is_item_in_inventory(int32 object_index);
extern bool __cdecl game_engine_object_is_useless_weapon(int32 object_index);
extern bool __cdecl game_engine_player_is_looking_at_sphere(int32 player_index, const real_point3d* sphere_center, real32 sphere_radius);
extern bool __cdecl game_engine_player_is_nearby(const real_point3d* point, real32 radius);
extern void __cdecl game_engine_register_object(int32 object_index);
extern int32 __cdecl game_engine_remap_vehicle_definition_index(int32 vehicle_definition_index);
extern int32 __cdecl game_engine_remap_weapon_definition_index(int32 weapon_definition_index);
extern void __cdecl game_engine_unregister_map_variant_object(int32 map_variant_index);
extern void __cdecl game_engine_unregister_object(int32 object_index);
extern bool __cdecl game_engine_vehicle_has_passengers(int32 object_index);
extern int32 __cdecl object_get_respawn_time_seconds(int32 object_index);
extern int32 __cdecl scenario_object_get_abandonment_time_seconds(int32 object_index);
extern bool __cdecl scenario_object_is_significant(e_object_type object_type, int32 scenario_datum_index);

