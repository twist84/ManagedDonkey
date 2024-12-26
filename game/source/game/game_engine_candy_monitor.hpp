#pragma once

#include "cseries/cseries.hpp"

struct c_candy_spawner
{
	char m_flags;
	long m_map_variant_index;
	long m_last_object_spawned_index;
	short m_respawn_timer_seconds;
	short pad;
};
static_assert(sizeof(c_candy_spawner) == 0x10);

struct c_candy_monitor
{
	long m_spawned_object_index;
	short m_abandoned_seconds;
	bool m_disturbed_from_initial_placement;
	bool m_pad[3];
	short m_abandonment_timeout;
};
static_assert(sizeof(c_candy_monitor) == 0xC);

struct c_multiplayer_candy_monitor_manager
{
	enum
	{
		k_dummy_spawn_monitor_index = 0x1971,
		k_maximum_number_of_monitored_pieces_of_candy = 512,
		k_insignificant_object_abandonment_threshold_seconds = 10,
	};

	c_static_array<c_candy_spawner, k_maximum_number_of_monitored_pieces_of_candy> m_candy_spawners;
	c_static_array<c_candy_monitor, k_maximum_number_of_monitored_pieces_of_candy> m_candy_monitors;
};
static_assert(sizeof(c_multiplayer_candy_monitor_manager) == 0x3800);

enum e_object_type;

extern bool __cdecl game_engine_any_player_nearby_and_looking_at_sphere(real_point3d const* sphere_center, real sphere_radius, real distance);
extern void __cdecl game_engine_candy_monitor_prepare_for_promotion_to_simulation_authority();
extern bool __cdecl game_engine_object_being_used(long object_index);
extern bool __cdecl game_engine_object_has_been_abandoned(long object_index);
extern bool __cdecl game_engine_object_has_been_disturbed(long object_index);
extern bool __cdecl game_engine_object_can_be_damaged(long object_index);
extern bool __cdecl game_engine_object_is_item_in_inventory(long object_index);
extern bool __cdecl game_engine_object_is_useless_weapon(long object_index);
extern bool __cdecl game_engine_player_is_looking_at_sphere(long player_index, real_point3d const* sphere_center, real sphere_radius);
extern bool __cdecl game_engine_player_is_nearby(real_point3d const* point, real radius);
extern void __cdecl game_engine_register_object(long object_index);
extern long __cdecl game_engine_remap_vehicle_definition_index(long vehicle_definition_index);
extern long __cdecl game_engine_remap_weapon_definition_index(long weapon_definition_index);
extern void __cdecl game_engine_unregister_map_variant_object(long map_variant_index);
extern void __cdecl game_engine_unregister_object(long object_index);
extern bool __cdecl game_engine_vehicle_has_passengers(long object_index);
extern long __cdecl object_get_respawn_time_seconds(long object_index);
extern long __cdecl scenario_object_get_abandonment_time_seconds(long object_index);
extern bool __cdecl scenario_object_is_significant(e_object_type object_type, long scenario_datum_index);

