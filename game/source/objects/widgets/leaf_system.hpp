#pragma once

#include "cseries/cseries.hpp"
#include "cseries/location.hpp"

struct s_leaf
{
	c_word_quantized_position position;
	uns8 flags;
	uns8 leaf_type_index;
	c_dword_unit_vector physics_normal;
	c_dword_unit_vector pendulum_axis;
	c_short_quaternion render_orientation;
	real32 velocity_scale;
	c_dword_unit_vector unit_velocity;
	uns16 age_in_quarter_ticks;
	uns16 rotation;
};
COMPILE_ASSERT(sizeof(s_leaf) == 0x24);

struct s_leaf_system_datum :
	s_datum_header
{
	uns16 pad;
	int32 object_index;
	int32 definition_index;
	c_static_array<s_leaf, 64> leaves;
	real_rectangle3d movement_bounding_box;
	real_point3d origin;
	s_location location;
	real32 next_emission_time_left;
	real32 total_leaf_frequency;
	bool valid_lighting_sample;
	real_vector3d lighting_sample;
};
COMPILE_ASSERT(sizeof(s_leaf_system_datum) == 0x94C);

