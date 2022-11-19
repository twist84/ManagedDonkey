#pragma once

#include "cseries/cseries.hpp"

struct s_multiplayer_object_boundary_geometry_data
{
	long boundary_shape;

	// shader used for boundary geometry
	long standard_shader_or_opaque_shader_index;

	real boundary_width;
	real boundary_box_length;
	real boundary_positive_height;
	real boundary_negative_height;
	real_matrix4x3 boundary_matrix;
	real boundary_raius;
};
static_assert(sizeof(s_multiplayer_object_boundary_geometry_data) == 0x50);

