#pragma once

#include "cseries/cseries.hpp"

enum e_implicit_transparent_type
{
	_implicit_transparent_type_cylinder = 0,
	_implicit_transparent_type_sphere,
	_implicit_transparent_type_box,

	k_implicit_transparent_type_count
};

struct s_rasterizer_implicit_cylinder_object
{
	real32 width;
};
COMPILE_ASSERT(sizeof(s_rasterizer_implicit_cylinder_object) == sizeof(real32));

struct s_rasterizer_implicit_sphere_object
{
	real32 width;
	real32 absolute_positive_height;
	real32 absolute_negative_height;
};
COMPILE_ASSERT(sizeof(s_rasterizer_implicit_sphere_object) == sizeof(real32) * 3);

struct s_rasterizer_implicit_box_object
{
	real32 width;
	real32 length;
	real32 absolute_positive_height;
	real32 absolute_negative_height;
};
COMPILE_ASSERT(sizeof(s_rasterizer_implicit_box_object) == sizeof(real32) * 4);

struct s_rasterizer_implicit_object
{
	c_enum<e_implicit_transparent_type, int32, _implicit_transparent_type_cylinder, k_implicit_transparent_type_count> transparent_type;
	int32 render_method_index;
	real_argb_color color;
	real_matrix4x3 transform;

	union
	{
		s_rasterizer_implicit_cylinder_object cylinder;
		s_rasterizer_implicit_sphere_object sphere;
		s_rasterizer_implicit_box_object box;
	};
};
COMPILE_ASSERT(sizeof(s_rasterizer_implicit_object) == 0x5C);

struct s_rasterizer_implicit_vertex
{
	real_point3d position;
	real_point2d texcoord;
};
COMPILE_ASSERT(sizeof(s_rasterizer_implicit_vertex) == 0x14);

struct s_rasterizer_implicit_geometry_globals
{
	int32 implicit_object_count;
	s_rasterizer_implicit_object implicit_objects[64];
	s_rasterizer_implicit_vertex circle_strip[128];
};
COMPILE_ASSERT(sizeof(s_rasterizer_implicit_geometry_globals) == 0x2104);

/*
void initialize_circle_strip()
{
	for (int32 i = 0; i < NUMBEROF(g_rasterizer_implicit_geometry_globals->circle_strip); i++)
	{
		s_rasterizer_implicit_vertex* vertex_data = &g_rasterizer_implicit_geometry_globals->circle_strip[i];

		real32 angle_scaling_factor = i * 63.0f;
		real32 circle_x_shift = sine(angle_scaling_factor * TWO_PI);
		real32 circle_y_shift = cosine(angle_scaling_factor * TWO_PI);

 
		// MS23
		vertex_data[0].position.x = circle_x_shift;
		vertex_data[0].position.y = circle_y_shift;
		vertex_data[0].position.z = 0.0f;
		vertex_data[0].texcoord.x = angle_scaling_factor;
		vertex_data[0].texcoord.y = 0.0f;

		vertex_data[1].position.x = circle_x_shift;
		vertex_data[1].position.y = circle_y_shift;
		vertex_data[1].position.z = 1.0f;
		vertex_data[1].texcoord.x = angle_scaling_factor;
		vertex_data[1].texcoord.y = 1.0f;

 
		// H3EK
		for (int32 j = 0; j < 2; j += 2)
		{
			vertex_data[0].position.x = circle_x_shift;
			vertex_data[0].position.y = circle_y_shift;
			vertex_data[0].position.z = j ? 1.0f : 0.0f;
			vertex_data[0].texcoord.x = angle_scaling_factor;
			vertex_data[0].texcoord.y = j ? 1.0f : 0.0f;

			vertex_data[1].position.x = circle_x_shift;
			vertex_data[1].position.y = circle_y_shift;
			vertex_data[1].position.z = j == -1 ? 0.0f : 1.0f;
			vertex_data[1].texcoord.x = angle_scaling_factor;
			vertex_data[1].texcoord.y = j == -1 ? 0.0f : 1.0f;
		}
	}
}
*/

