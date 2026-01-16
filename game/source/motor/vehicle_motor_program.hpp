#pragma once

#include "cseries/cseries.hpp"
#include "game/materials.hpp"
#include "physics/havok_vehicle_physics_definitions.hpp"

struct s_vehicle_physics_point_datum
{
	real_matrix4x3 rotation_matrix;
	real_point3d position;
	real_vector3d radius;
	real_vector3d velocity;
	real_vector3d force;
	real_vector3d torque;
	bool moving_surface;
	real_vector3d surface_velocity;
	int32 object_index;
};
COMPILE_ASSERT(sizeof(s_vehicle_physics_point_datum) == 0x84);

struct s_anti_gravity_vehicle_physics_point_datum :
	s_vehicle_physics_point_datum
{
	bool anti_gravitating;
	bool valid;
	real_vector3d local_offset;
	real_vector3d powered_force;
	real32 antigrav_fraction;
	real32 antigrav_error;
	real32 antigrav_height_fraction;
};
COMPILE_ASSERT(sizeof(s_anti_gravity_vehicle_physics_point_datum) == sizeof(s_vehicle_physics_point_datum) + 0x28);

struct s_friction_datum
{
	real_vector3d friction;
	real_vector3d parallel;
	real_vector3d perpendicular;
};
COMPILE_ASSERT(sizeof(s_friction_datum) == 0x24);

struct s_friction_vehicle_physics_point_datum :
	s_vehicle_physics_point_datum
{
	bool on_ground;
	bool sliding;
	bool emergency_braking;
	bool braking;
	real32 ground_depth;
	real32 current_radius;
	real32 normal_force_magnitude;
	real32 ground_friction_velocity;
	real32 ground_tire_relative_velocity;
	real_vector3d normal_force;
	real_vector3d ground_plane;
	real_point3d ground_position;
	int32 plane_object_index;
	c_global_material_type ground_global_material_type;
	bool pad;
	bool disabled;
	s_friction_datum ground_friction;
};
COMPILE_ASSERT(sizeof(s_friction_vehicle_physics_point_datum) == sizeof(s_vehicle_physics_point_datum) + 0x68);

struct s_havok_vehicle_physics_instance
{
	int32 vehicle_index;
	s_havok_vehicle_physics_definition* physics;
	real_matrix4x3 world_matrix;
	real_matrix3x3 inertia_tensor;
	real_point3d world_center_of_mass;
	real32 mass;
	real32 wheel_total_frictional_torque;
	real_vector3d throttle;
	real32 steering;
	bool is_e_braking;
	bool antigravity_disabled;
	bool cache_invalid;
	int32 anti_gravity_points_on_slip_surface_count;
	int32 friction_points_on_slip_surface_count;

	s_anti_gravity_vehicle_physics_point_datum anti_gravity_points[16];
	s_friction_vehicle_physics_point_datum friction_points[16];
};
COMPILE_ASSERT(sizeof(s_havok_vehicle_physics_instance) == 0x1A10);

