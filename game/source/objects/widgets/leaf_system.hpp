#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_word_quantized_position
{
	word m_position[3];
};
static_assert(sizeof(c_word_quantized_position) == 0x6);

struct c_dword_unit_vector
{
	dword m_vector;
};
static_assert(sizeof(c_dword_unit_vector) == 0x4);

struct c_short_quaternion
{
	short m_quaternion[4];
};
static_assert(sizeof(c_short_quaternion) == 0x8);

struct s_leaf
{
	c_word_quantized_position position;
	byte_flags flags;
	byte leaf_type_index;
	c_dword_unit_vector physics_normal;
	c_dword_unit_vector pendulum_axis;
	c_short_quaternion render_orientation;
	real velocity_scale;
	c_dword_unit_vector unit_velocity;
	word age_in_quarter_ticks;
	word rotation;
};
static_assert(sizeof(s_leaf) == 0x24);

struct s_leaf_system_datum :
	s_datum_header
{
	long object_index;
	long definition_index;
	c_static_array<s_leaf, 64> leaves;
	real_rectangle3d movement_bounding_box;
	real_point3d origin;
	s_location location;
	real next_emission_time_left;
	real total_leaf_frequency;
	bool valid_lighting_sample;
	vector3d lighting_sample;
};
static_assert(sizeof(s_leaf_system_datum) == 0x94C);

