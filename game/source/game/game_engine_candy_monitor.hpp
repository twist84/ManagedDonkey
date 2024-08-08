#pragma once

#include "cseries/cseries.hpp"

struct c_candy_spawner
{
	byte_flags m_flags;
	byte __pad1[0x3];
	long m_map_variant_index;
	long m_last_object_spawned_index;
	short m_respawn_timer_seconds;
	byte __padE[0x2];
};
static_assert(sizeof(c_candy_spawner) == 0x10);

struct c_candy_monitor
{
	long m_spawned_object_index;
	short m_abandoned_seconds;
	bool m_disturbed_from_initial_placement;
	bool __unknown7;
	byte __pad8[0x2];
	short m_abandonment_time_seconds;
};
static_assert(sizeof(c_candy_monitor) == 0xC);

struct c_multiplayer_candy_monitor_manager
{
	c_candy_spawner m_spawners[512];
	c_candy_monitor m_monitors[512];
};
static_assert(sizeof(c_multiplayer_candy_monitor_manager) == 0x3800);

enum e_object_type;

extern bool __cdecl game_engine_any_player_nearby_and_looking_at_sphere(real_point3d const* position, real nearby_radius, real looking_at_radius);
extern void __cdecl game_engine_candy_monitor_prepare_for_promotion_to_simulation_authority();
extern bool __cdecl game_engine_object_being_used(long object_index);
extern bool __cdecl game_engine_object_has_been_abandoned(long object_index);
extern bool __cdecl game_engine_object_has_been_disturbed(long object_index);
extern bool __cdecl game_engine_object_can_be_damaged(long object_index);
extern bool __cdecl game_engine_object_is_item_in_inventory(long object_index);
extern bool __cdecl game_engine_object_is_useless_weapon(long object_index);
extern bool __cdecl game_engine_player_is_looking_at_sphere(long player_index, real_point3d const* position, real radius);
extern bool __cdecl game_engine_player_is_nearby(real_point3d const* position, real radius);
extern void __cdecl game_engine_register_object(long object_index);
extern long __cdecl game_engine_remap_vehicle_definition_index(long vehicle_definition_index);
extern long __cdecl game_engine_remap_weapon_definition_index(long weapon_definition_index);
extern void __cdecl game_engine_unregister_map_variant_object(long map_variant_index);
extern void __cdecl game_engine_unregister_object(long object_index);
extern bool __cdecl game_engine_vehicle_has_passengers(long object_index);
extern long __cdecl object_get_respawn_time_seconds(long object_index);
extern long __cdecl scenario_object_get_abandonment_time_seconds(long object_index);
extern bool __cdecl scenario_object_is_significant(e_object_type object_type, long placement_index);

