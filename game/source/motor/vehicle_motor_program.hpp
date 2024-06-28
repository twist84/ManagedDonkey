#pragma once

#include "cseries/cseries.hpp"
#include "physics/havok_vehicle_physics_definitions.hpp"

struct s_runtime_anti_gravity_point_definition
{
	s_anti_gravity_point_definition anti_gravity_point;

	// I don't know
	byte runtime_data[0xAC - sizeof(s_anti_gravity_point_definition)];
};
static_assert(sizeof(s_runtime_anti_gravity_point_definition) == 0xAC);

struct s_runtime_friction_point_definition
{
	s_friction_point_definition friction_point;

	// I don't know
	byte runtime_data[0xEC - sizeof(s_friction_point_definition)];
};
static_assert(sizeof(s_runtime_friction_point_definition) == 0xEC);

struct s_havok_vehicle_physics_instance
{
	long vehicle_index;
	s_havok_vehicle_physics_definition* physics;
	real_matrix4x3 world_matrix;
	matrix3x3 inertia_tensor;
	real_point3d center_of_mass;
	real mass;

	real __unknown70;

	vector3d throttle;

	real __unknown80;

	bool is_e_braking;
	bool is_upsides_down;

	byte __unknown86;
	byte __unknown87;
	long __unknown88;
	long __unknown8C;

	s_runtime_anti_gravity_point_definition anti_gravity_points[16];
	s_runtime_friction_point_definition friction_points[16];
};
static_assert(sizeof(s_havok_vehicle_physics_instance) == 0x1A10);

