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
	dword_flags flags;
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
	real detonation_timer;
	real detonation_timer_delta;
	real super_detonation_check_timer;
	real super_detonation_check_timer_delta;
	real arming_time;
	real arming_time_delta;
	real odometer;
	real bounce_odometer;
	real velocity_scale;
	real maximum_damage_distance;
	real_vector3d rotation_axis;
	real rotation_sine;
	real rotation_cosine;
	real damage_scale;
	real guided_angular_velocity;
	real last_thrust_speed;
	real thrust_odometer;
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

