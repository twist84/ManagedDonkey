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
	real width;
};
static_assert(sizeof(s_rasterizer_implicit_cylinder_object) == sizeof(real));

struct s_rasterizer_implicit_sphere_object
{
	real width;
	real absolute_positive_height;
	real absolute_negative_height;
};
static_assert(sizeof(s_rasterizer_implicit_sphere_object) == sizeof(real) * 3);

struct s_rasterizer_implicit_box_object
{
	real width;
	real length;
	real absolute_positive_height;
	real absolute_negative_height;
};
static_assert(sizeof(s_rasterizer_implicit_box_object) == sizeof(real) * 4);

struct s_rasterizer_implicit_object
{
	c_enum<e_implicit_transparent_type, long, _implicit_transparent_type_cylinder, k_implicit_transparent_type_count> transparent_type;
	long render_method_index;
	real_argb_color color;
	real_matrix4x3 transform;

	union
	{
		s_rasterizer_implicit_cylinder_object cylinder;
		s_rasterizer_implicit_sphere_object sphere;
		s_rasterizer_implicit_box_object box;
	};
};
static_assert(sizeof(s_rasterizer_implicit_object) == 0x5C);

struct s_implicit_vertex
{
	real_point3d position;
	real_point2d texcoord;
};
static_assert(sizeof(s_implicit_vertex) == 0x14);

struct rasterizer_vertex_implicit
{
	s_implicit_vertex bottom;
	s_implicit_vertex top;
};
static_assert(sizeof(rasterizer_vertex_implicit) == 0x28);

struct s_rasterizer_implicit_geometry_globals
{
	long implicit_object_count;
	s_rasterizer_implicit_object implicit_objects[64];
	rasterizer_vertex_implicit implicit_vertex_data[64];
};
static_assert(sizeof(s_rasterizer_implicit_geometry_globals) == 0x2104);

//void initialize_circle_strip()
//{
//	TLS_DATA_GET_VALUE_REFERENCE(g_rasterizer_implicit_geometry_globals);
//
//	for (long i = 0; i < NUMBEROF(g_rasterizer_implicit_geometry_globals->implicit_vertex_data); i++)
//	{
//		rasterizer_vertex_implicit* vertex_data = &g_rasterizer_implicit_geometry_globals->implicit_vertex_data[i];
//
//		real angle_scaling_factor = i * 63.0f;
//		real circle_x_shift = sinf(angle_scaling_factor * TWO_PI);
//		real circle_y_shift = cosf(angle_scaling_factor * TWO_PI);
//
// 
//		// MS23
//
//		vertex_data->bottom.position.x = circle_x_shift;
//		vertex_data->bottom.position.y = circle_y_shift;
//		vertex_data->bottom.position.z = 0.0f;
//		vertex_data->bottom.texcoord.x = angle_scaling_factor;
//		vertex_data->bottom.texcoord.y = 0.0f;
//
//		vertex_data->top.position.x = circle_x_shift;
//		vertex_data->top.position.y = circle_y_shift;
//		vertex_data->top.position.z = 1.0f;
//		vertex_data->top.texcoord.x = angle_scaling_factor;
//		vertex_data->top.texcoord.y = 1.0f;
//
// 
//		// H3EK
//		for (long j = 0; j < 2; j += 2)
//		{
//			vertex_data->bottom.position.x = circle_x_shift;
//			vertex_data->bottom.position.y = circle_y_shift;
//			vertex_data->bottom.position.z = j ? 1.0f : 0.0f;
//			vertex_data->bottom.texcoord.x = angle_scaling_factor;
//			vertex_data->bottom.texcoord.y = j ? 1.0f : 0.0f;
//
//			vertex_data->top.position.x = circle_x_shift;
//			vertex_data->top.position.y = circle_y_shift;
//			vertex_data->top.position.z = j == -1 ? 0.0f : 1.0f;
//			vertex_data->top.texcoord.x = angle_scaling_factor;
//			vertex_data->top.texcoord.y = j == -1 ? 0.0f : 1.0f;
//		}
//	}
//}

