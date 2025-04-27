#pragma once

#include "cseries/cseries.hpp"
#include "physics/physics_model_definitions.hpp"
#include "tag_files/tag_groups.hpp"

enum e_character_physics_flags
{
	_character_physics_centered_at_origin_bit,
	_character_physics_shape_spherical_bit,
	_character_physics_use_player_physics_bit,
	_character_physics_climb_any_surface_bit,
	_character_physics_flying_bit,
	_character_physics_not_physical_bit,
	_character_physics_dead_character_collision_group_bit,
	_character_physics_suppress_ground_planes_on_bipeds_bit,
	_character_physics_physical_ragdoll_bit,
	_character_physics_do_not_resize_dead_spheres_bit,

	k_number_of_character_physics_flags
};

struct s_character_physics_ground_definition
{
	angle maximum_slope_angle; // degrees
	angle downhill_falloff_angle; // degrees
	angle downhill_cutoff_angle; // degrees
	angle uphill_falloff_angle; // degrees
	angle uphill_cutoff_angle; // degrees

	real32 downhill_velocity_scale;
	real32 uphill_velocity_scale;

	real32 runtime_minimum_normal_k;
	real32 runtime_downhill_k[2];
	real32 runtime_uphill_k[2];

	// angle for bipeds at which climb direction changes between up and down
	real32 climb_inflection_angle;

	// scale on the time for the entity to realize it is airborne
	angle scale_airborne_reaction_time;

	// scale on velocity with which the entity is pushed back into its ground plane
	real32 scale_ground_adhesion_velocity;

	// scale on gravity for this entity
	real32 gravity_scale;

	real32 airborne_acceleration_scale;
};
static_assert(sizeof(s_character_physics_ground_definition) == 0x44);

struct s_character_physics_flying_definition
{
	// angle at which we bank left/right when sidestepping or turning while moving forwards
	angle bank_angle; // degrees

	// time it takes us to apply a bank
	real32 bank_apply_time; // seconds

	// time it takes us to recover from a bank
	real32 bank_decay_time; // seconds

	// amount that we pitch up/down when moving up or down
	real32 pitch_ratio;

	// max velocity when not crouching
	real32 max_velocity; // world units per second

	// max sideways or up/down velocity when not crouching
	real32 max_sidestep_velocity; // world units per second

	real32 acceleration; // world units per second squared
	real32 deceleration; // world units per second squared

	// turn rate
	angle angular_velocity_maximum; // degrees per second

	// turn acceleration rate
	angle angular_acceleration_maximum; // degrees per second squared

	// how much slower we fly if crouching (zero = same speed)
	real32 crouch_velocity_modifier; // [0,1]
};
static_assert(sizeof(s_character_physics_flying_definition) == 0x2C);

struct s_character_physics_definition
{
	c_flags<e_character_physics_flags, uint32, k_number_of_character_physics_flags> flags;

	real32 height_standing;
	real32 height_crouching;

	real32 radius;
	real32 mass;

	// collision material used when character is alive
	c_string_id living_material_name;

	// collision material used when character is dead
	c_string_id dead_material_name;

	// runtime
	short global_material_type;

	// runtime
	short dead_global_material_type;

	// don't be a jerk, edit something else
	c_typed_tag_block<s_physics_model_sphere> dead_sphere_shapes;

	// don't be a jerk, edit something else
	c_typed_tag_block<s_physics_model_pill> pill_shapes;

	// don't be a jerk, edit something else
	c_typed_tag_block<s_physics_model_sphere> sphere_shapes;

	// ground
	s_character_physics_ground_definition ground_physics;

	// flying
	s_character_physics_flying_definition flying_physics;
};
static_assert(sizeof(s_character_physics_definition) == 0xB4);

