#pragma once

#include "cseries/cseries.hpp"
#include "physics/physics_model_definitions.hpp"
#include "tag_files/tag_groups.hpp"

struct s_character_physics_ground_definition
{
	real32 maximum_slope_angle; // degrees
	real32 downhill_falloff_angle; // degrees
	real32 downhill_cutoff_angle; // degrees
	real32 uphill_falloff_angle; // degrees
	real32 uphill_cutoff_angle; // degrees

	real32 downhill_velocity_scale;
	real32 uphill_velocity_scale;

	real32 runtime_minimum_normal_k;
	real32 runtime_downhill_k0;
	real32 runtime_downhill_k1;
	real32 runtime_uphill_k0;
	real32 runtime_uphill_k1;

	// angle for bipeds at which climb direction changes between up and down
	real32 climb_inflection_angle;

	// scale on the time for the entity to realize it is airborne
	real32 scale_airborne_reaction_time;

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
	real32 bank_angle; // degrees

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
	real32 angular_velocity_maximum; // degrees per second

	// turn acceleration rate
	real32 angular_acceleration_maximum; // degrees per second squared

	// how much slower we fly if crouching (zero = same speed)
	real32 crouch_velocity_modifier; // [0,1]
};
static_assert(sizeof(s_character_physics_flying_definition) == 0x2C);

struct s_character_physics_definition
{
	enum e_flag
	{
		_centered_at_origin_bit,
		_shape_spherical_bit,
		_use_player_physics_bit,
		_climb_any_surface_bit,
		_flying_bit,
		_not_physical_bit,
		_always_dead_character_collision_group_bit,
		_suppress_ground_planes_on_bipeds_bit,
		_physical_ragdoll_bit,
		_do_not_resize_dead_spheres_bit,

		k_flag_count
	};

	c_flags<e_flag, uns32, k_flag_count> flags;

	real32 collision_height_standing;
	real32 collision_height_crouching;
	real32 collision_radius;
	real32 collision_mass;

	// collision material used when character is alive
	c_string_id collision_global_material_name;

	// collision material used when character is dead
	c_string_id dead_collision_global_material_name;

	// runtime
	int16 collision_global_material_type;

	// runtime
	int16 dead_collision_global_material_type;

	// don't be a jerk, edit something else
	c_typed_tag_block<s_physics_model_sphere> dead_spheres;

	// don't be a jerk, edit something else
	c_typed_tag_block<s_physics_model_pill> pills;

	// don't be a jerk, edit something else
	c_typed_tag_block<s_physics_model_sphere> spheres;

	// ground
	s_character_physics_ground_definition ground;

	// flying
	s_character_physics_flying_definition flying;
};
static_assert(sizeof(s_character_physics_definition) == 0xB4);

