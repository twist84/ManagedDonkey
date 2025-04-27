#pragma once

#include "game/aim_target.hpp"
#include "objects/objects.hpp"

struct s_projectile_targeting_data
{
	c_aim_target_object object;
};
static_assert(sizeof(s_projectile_targeting_data) == sizeof(c_aim_target_object));

struct _projectile_datum
{
	uint32 flags;
	char action;
	char consecutive_collision_counter;
	c_global_material_type hit_global_material;
	real_vector3d hit_normal;
	long ignore_object_index;
	long secondary_ignore_object_index;
	s_projectile_targeting_data targeting_data;
	long weapon_index;
	long simulation_collision_object_index;
	long simulation_collision_object_node_index;
	real_point3d simulation_collision_object_position;
	real32 detonation_timer;
	real32 detonation_timer_delta;
	real32 super_detonation_check_timer;
	real32 super_detonation_check_timer_delta;
	real32 arming_time;
	real32 arming_time_delta;
	real32 odometer;
	real32 bounce_odometer;
	real32 velocity_scale;
	real32 maximum_damage_distance;
	real_vector3d rotation_axis;
	real32 rotation_sine;
	real32 rotation_cosine;
	real32 damage_scale;
	real32 guided_angular_velocity;
	real32 last_thrust_speed;
	real32 thrust_odometer;
	real_point3d start_point;
	short shot_id;
	short time_of_flight_ticks;
};
static_assert(sizeof(_projectile_datum) == 0x9C);

struct projectile_datum
{
	long definition_index;
	_object_datum object;
	_projectile_datum projectile;
};
static_assert(sizeof(projectile_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_projectile_datum));

extern bool __cdecl dangerous_projectiles_near_player(long* out_projectile_index);

extern void render_debug_projectiles();

