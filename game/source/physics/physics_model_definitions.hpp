#pragma once

#include "cseries/cseries.hpp"

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
	real friction;
	real restitution;
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
	real_vector3d translation;
	real havok_w_translation;
};
static_assert(sizeof(s_havok_convex_translate_shape) == 0x30);

struct s_havok_shape_collection // hkShapeCollection
{
	s_havok_shape base;
	void* field_pointer_skip;
	char disable_welding;
	char collection_type;
	byte VDVAPBSS[0x2]; // pad
};
static_assert(sizeof(s_havok_shape_collection) == 0x18);

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
	real_vector3d bottom;
	real havok_w_bottom;
	real_vector3d top;
	real havok_w_top;
};
static_assert(sizeof(s_physics_model_pill) == 0x60);

