#pragma once

#include "cseries/cseries.hpp"
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

	real downhill_velocity_scale;
	real uphill_velocity_scale;

	real runtime_minimum_normal_k;
	real runtime_downhill_k[2];
	real runtime_uphill_k[2];

	// angle for bipeds at which climb direction changes between up and down
	real climb_inflection_angle;

	// scale on the time for the entity to realize it is airborne
	angle scale_airborne_reaction_time;

	// scale on velocity with which the entity is pushed back into its ground plane
	real scale_ground_adhesion_velocity;

	// scale on gravity for this entity
	real gravity_scale;

	real airborne_acceleration_scale;
};
static_assert(sizeof(s_character_physics_ground_definition) == 0x44);

struct s_character_physics_flying_definition
{
	// angle at which we bank left/right when sidestepping or turning while moving forwards
	angle bank_angle; // degrees

	// time it takes us to apply a bank
	real bank_apply_time; // seconds

	// time it takes us to recover from a bank
	real bank_decay_time; // seconds

	// amount that we pitch up/down when moving up or down
	real pitch_ratio;

	// max velocity when not crouching
	real max_velocity; // world units per second

	// max sideways or up/down velocity when not crouching
	real max_sidestep_velocity; // world units per second

	real acceleration; // world units per second squared
	real deceleration; // world units per second squared

	// turn rate
	angle angular_velocity_maximum; // degrees per second

	// turn acceleration rate
	angle angular_acceleration_maximum; // degrees per second squared

	// how much slower we fly if crouching (zero = same speed)
	real crouch_velocity_modifier; // [0,1]
};
static_assert(sizeof(s_character_physics_flying_definition) == 0x2C);

struct s_physics_model_sphere;
struct s_physics_model_pill;
struct s_character_physics_definition
{
	c_flags<e_character_physics_flags, dword_flags, k_number_of_character_physics_flags> flags;

	real height_standing;
	real height_crouching;

	real radius;
	real mass;

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

enum e_primitive_flags
{
	_primitive_flag_unused_bit = 0,
	_primitive_flag_bit1,
	_primitive_flag_bit2,
	_primitive_flag_bit3,
	_primitive_flag_bit4,
	_primitive_flag_bit5,
	_primitive_flag_bit6,
	_primitive_flag_bit7,
	_primitive_flag_bit8,

	k_primitive_flags
};

struct s_physics_model_primitive
{
	c_string_id name;
	short material;
	c_flags<e_primitive_flags, word_flags, k_primitive_flags> flags;
	real relative_mass_scale;
	real_fraction friction;
	real_fraction restitution;
	real volume;
	real mass;
	short mass_distribution_index;
	char phantom;
	char runtime_collision_group;
};
static_assert(sizeof(s_physics_model_primitive) == 0x20);

struct s_havok_shape // hkShape
{
	void* field_pointer_skip;
	short size;
	short count;
	long user_data;
	long type;
};
static_assert(sizeof(s_havok_shape) == 0x10);

struct s_havok_convex_shape // hkConvexShape
{
	s_havok_shape base;
	real radius;
};
static_assert(sizeof(s_havok_convex_shape) == 0x14);

enum e_havok_shape
{
	_havok_shape_sphere = 0,
	_havok_shape_pill,
	_havok_shape_box,
	_havok_shape_triangle,
	_havok_shape_polyhedron,
	_havok_shape_multi_sphere,
	_havok_shape_unused_0,
	_havok_shape_unused_1,
	_havok_shape_unused_2,
	_havok_shape_unused_3,
	_havok_shape_unused_4,
	_havok_shape_unused_5,
	_havok_shape_unused_6,
	_havok_shape_unused_7,
	_havok_shape_list,
	_havok_shape_mopp,

	k_havok_shape_count
};

struct s_havok_shape_reference // hkShape *
{
	c_enum<e_havok_shape, short, _havok_shape_sphere, k_havok_shape_count> shape_type;
	short shape;
};
static_assert(sizeof(s_havok_shape_reference) == 0x4);

struct s_havok_convex_translate_shape // hkConvexTranslateShape
{
	s_havok_convex_shape convex;
	void* field_pointer_skip;
	s_havok_shape_reference havok_shape_reference_struct;
	long child_shape_size;
	vector3d translation;
	real havok_w_translation;
};
static_assert(sizeof(s_havok_convex_translate_shape) == 0x30);

struct s_physics_model_sphere
{
	s_physics_model_primitive base;
	s_havok_convex_shape sphere_shape;
	byte algn344[0xC]; // pad
	s_havok_convex_translate_shape translate_shape;
};
static_assert(sizeof(s_physics_model_sphere) == 0x70);

struct s_physics_model_pill
{
	s_physics_model_primitive base;
	s_havok_convex_shape capsule_shape;
	byte algn243[0xC]; // pad
	vector3d bottom;
	real havok_w_bottom;
	vector3d top;
	real havok_w_top;
};
static_assert(sizeof(s_physics_model_pill) == 0x60);

