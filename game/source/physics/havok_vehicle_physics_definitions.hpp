#pragma once

#include "cseries/cseries.hpp"
#include "game/materials.hpp"
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
	c_flags<e_havok_vehicle_physics_definition_flags, uns32, k_havok_vehicle_physics_definition_flags> flags;

	// this sucks.
	// for friction based vehicles only
	real32 ground_friction;
	real32 ground_depth;
	real32 ground_damp_factor;
	real32 ground_moving_friction;


	// degrees 0-90
	real32 ground_maximum_slope_0;

	// degrees 0-90.
	// and greater than slope 0
	real32 ground_maximum_slope_1;

	// 0 defaults to 3, this prevents the physics from becoming unstable when hucked against a surface
	real32 maximum_normal_force_contribution;

	// lift per WU.
	real32 anti_gravity_bank_lift;

	// how quickly we bank when we steer
	real32 steering_bank_reaction_scale;

	// value of 0 defaults to 1.  .5 is half gravity
	real32 gravity_scale;

	// generated from the radius of the hkConvexShape for this vehicle
	real32 radius;

	// WU.  if a friciton point moves more than this distance it must update
	real32 maximum_update_distance;

	// Seconds. a friction point of this vehicle must update a least this often when controlled by a local player.
	real32 maximum_update_period;

	// Seconds. a friction point of this vehicle must update a least this often when controlled by an ai or remote player.
	real32 maximum_remote_update_period;

	c_typed_tag_block<s_anti_gravity_point_definition> anti_gravity_points;
	c_typed_tag_block<s_friction_point_definition> friction_points;
	c_typed_tag_block<s_vehicle_phantom_shape_definition> shape_phantom_shape;
};
COMPILE_ASSERT(sizeof(s_havok_vehicle_physics_definition) == 0x60);

struct s_anti_gravity_point_definition
{
	c_string_id marker_name;
	uns32 flags;
	real32 antigrav_strength;
	real32 antigrav_offset;
	real32 antigrav_height;
	real32 antigrav_damp_factor;
	real32 antigrav_normal_k1;
	real32 antigrav_normal_k0;
	real32 radius;

	// pad
	byte OX[0xC];

	// pad
	byte BB[0x2];

	int16 WU;
	c_string_id damage_source_region_name;
	real32 model_state_error[k_number_of_model_states];
};
COMPILE_ASSERT(sizeof(s_anti_gravity_point_definition) == 0x4C);

enum e_friction_point_flags
{
	_friction_point_gets_damage_from_region_bit = 0,
	_friction_point_powered_bit,
	_friction_point_front_turning_bit,
	_friction_point_rear_turning_bit,
	_friction_point_attached_to_e_brake_bit,
	_friction_point_can_be_destroyed_bit,

	k_friction_point_flags
};

enum e_friction_type
{
	_friction_type_point = 0,
	_friction_type_forward,

	k_friction_type_count
};

struct s_friction_point_definition
{
	c_string_id marker_name;
	c_flags<e_friction_point_flags, uns32, k_friction_point_flags> flags;
	real32 fraction_of_total_mass; // (0.0-1.0) fraction of total vehicle mass
	real32 radius;
	real32 damaged_radius; // radius when the tire is blown off.
	c_enum<e_friction_type, int16, _friction_type_point, k_friction_type_count> friction_type;

	// pad
	byte BTUPMKNC[0x2];

	real32 moving_friction_velocity_diff;
	real32 e_brake_moving_friction;
	real32 e_brake_friction;
	real32 e_brake_moving_friction_vel_diff;

	// pad
	byte ESECRABPX[0x14];

	c_string_id collision_global_material_name;
	c_global_material_type runtime_global_material_index;

	// friction point destruction data
	c_enum<e_model_state, int16, _model_state_standard, k_number_of_model_states> model_state_destroyed; // only need point can destroy flag set
	c_string_id region_name; // only need point can destroy flag set
	int32 runtime_region_index;
};
COMPILE_ASSERT(sizeof(s_friction_point_definition) == 0x4C);

struct s_havok_vector4 // hkVector4
{
	real_vector3d sphere;
	real32 havok_w_sphere;
};

class c_multi_sphere_shape
{
public:
	s_havok_shape sphere_rep_shape;
	int32 num_spheres;
	int8 cock_suck[0xC];
	s_havok_vector4 four_vectors_storage[8];
};

class c_list_shape
{
public:
	struct ChildInfo
	{
		int32 child_shape_pointer;
		int32 collision_filter_info;
		int32 shape_size;
		int32 num_child_shapes;
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
	int32 child_shapes_pointer;
	int32 child_shapes_size;
	int32 child_shapes_capacity;
	byte nail_in_dick[0xC]; // pad
	real_vector3d aabb_half_extents;
	real32 havok_w_aabb_half_extents;
	real_vector3d aabb_center;
	real32 havok_w_aabb_center;
	int32 multisphere_count;
	c_flags<e_vehicle_phantom_shape_flags, uns32, k_vehicle_phantom_shape_flags> flags;
	real_rectangle3d bounds;
	c_multi_sphere_shape multispheres[4];
	c_list_shape::ChildInfo list_shape_children[4];
};
COMPILE_ASSERT(sizeof(s_vehicle_phantom_shape_definition) == 0x330);

