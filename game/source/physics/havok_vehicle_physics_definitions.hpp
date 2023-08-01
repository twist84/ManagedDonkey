#pragma once

#include "cseries/cseries.hpp"
#include "physics/physics_model_definitions.hpp"
#include "tag_files/tag_groups.hpp"

enum e_havok_vehicle_physics_definition_flags
{
	_havok_vehicle_physics_definition_flags_invalid = 0,

	k_havok_vehicle_physics_definition_flags
};

struct s_anti_gravity_point_definition;
struct s_friction_point_definition;
struct s_vehicle_phantom_shape_definition;
struct s_havok_vehicle_physics_definition
{
	c_flags<e_havok_vehicle_physics_definition_flags, dword_flags, k_havok_vehicle_physics_definition_flags> flags;

	// this sucks.
	// for friction based vehicles only
	real ground_friction;
	real ground_depth;
	real ground_damp_factor;
	real ground_moving_friction;


	// degrees 0-90
	real ground_maximum_slope_0;

	// degrees 0-90.
	// and greater than slope 0
	real ground_maximum_slope_1;

	// 0 defaults to 3, this prevents the physics from becoming unstable when hucked against a surface
	real maximum_normal_force_contribution;

	// lift per WU.
	real anti_gravity_bank_lift;

	// how quickly we bank when we steer
	real steering_bank_reaction_scale;

	// value of 0 defaults to 1.  .5 is half gravity
	real gravity_scale;

	// generated from the radius of the hkConvexShape for this vehicle
	real radius;

	// WU.  if a friciton point moves more than this distance it must update
	real maximum_update_distance;

	// Seconds. a friction point of this vehicle must update a least this often when controlled by a local player.
	real maximum_update_period;

	// Seconds. a friction point of this vehicle must update a least this often when controlled by an ai or remote player.
	real maximum_remote_update_period;

	c_typed_tag_block<s_anti_gravity_point_definition> anti_gravity_points;
	c_typed_tag_block<s_friction_point_definition> friction_points;
	c_typed_tag_block<s_vehicle_phantom_shape_definition> shape_phantom_shape;
};
static_assert(sizeof(s_havok_vehicle_physics_definition) == 0x60);

struct s_havok_vector4 // hkVector4
{
	vector3d sphere;
	real havok_w_sphere;
};

struct c_multi_sphere_shape
{
	s_havok_shape sphere_rep_shape;
	long num_spheres;
	char cock_suck[0xC];
	s_havok_vector4 four_vectors_storage[8];
};

struct c_list_shape
{
	struct ChildInfo
	{
		long child_shape_pointer;
		long collision_filter_info;
		long shape_size;
		long num_child_shapes;
	};
};

enum e_vehicle_phantom_shape_flags
{
	_vehicle_phantom_shape_flag_has_aabb_phantom_bit = 0,
	_vehicle_phantom_shape_flag_has_suspension_bit,

	k_vehicle_phantom_shape_flags
};

struct s_vehicle_phantom_shape_definition
{
	s_havok_shape_collection list_shape;
	long child_shapes_pointer;
	long child_shapes_size;
	long child_shapes_capacity;
	byte nail_in_dick[0xC]; // pad
	vector3d aabb_half_extents;
	real havok_w_aabb_half_extents;
	vector3d aabb_center;
	real havok_w_aabb_center;
	long multisphere_count;
	c_flags<e_vehicle_phantom_shape_flags, dword_flags, k_vehicle_phantom_shape_flags> flags;
	real_rectangle3d bounds;
	c_multi_sphere_shape multispheres[4];
	c_list_shape::ChildInfo list_shape_children[4];
};
static_assert(sizeof(s_vehicle_phantom_shape_definition) == 0x330);

