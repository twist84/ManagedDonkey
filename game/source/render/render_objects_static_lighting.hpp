#pragma once

#include "cseries/cseries.hpp"

struct render_first_person_model
{
	long render_model_definition_index;
	long object_index;
	dword_flags flags;
	c_static_array<real_matrix4x3, 150> render_model_node_matrices;
};
static_assert(sizeof(render_first_person_model) == 0x1E84);

struct s_render_object_first_person_globals
{
	long camera_object_index;
	long model_count;
	c_static_array<render_first_person_model, 6> models;
};
static_assert(sizeof(s_render_object_first_person_globals) == 0xB720);

extern s_render_object_first_person_globals& render_object_globals;

