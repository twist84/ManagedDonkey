#include "render/render_debug.hpp"

#include "interface/interface.hpp"
#include "interface/interface_constants.hpp"
#include "math/color_math.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/views/render_view.hpp"
#include "render/render_cameras.hpp"
#include "render/render_lights.hpp"
#include "render/render_visibility.hpp"
#include "text/draw_string.hpp"

struct s_render_debug_globals
{
	bool use_simple_font;
};

static s_render_debug_globals* g_render_debug_globals = new s_render_debug_globals
{
	.use_simple_font = false
};

void __cdecl rasterizer_debug_line(real_point3d const* p0, real_point3d const* p1, real_argb_color const* color0, real_argb_color const* color1)
{
	ASSERT(p0 && p1 && color0);

	real_argb_color color0_ = *color0;
	real_argb_color color1_ = *color1;
	color0_.alpha = 1.0f;
	color1_.alpha = 1.0f;

	for (long i = 0; i < NUMBEROF(color0_.color.n); i++)
	{
		color0_.color.n[i] = color0_.color.n[i] * color0_.color.n[i];
		color1_.color.n[i] = color1_.color.n[i] * color1_.color.n[i];
	}

	c_rasterizer::draw_debug_line(*p0, *p1, real_argb_color_to_pixel32(&color0_), real_argb_color_to_pixel32(&color1_));
}

void __cdecl rasterizer_debug_line2d(real_point2d const* p0, real_point2d const* p1, real_argb_color const* color0, real_argb_color const* color1)
{
	ASSERT(p0 && p1 && color0);

	real_point3d p0_{};
	real_point3d p1_{};
	set_real_point3d(&p0_, p0->x, p0->y, 0.0f);
	set_real_point3d(&p1_, p1->x, p1->y, 0.0f);

	real_argb_color color0_ = *color0;
	real_argb_color color1_ = *color1;
	color0_.alpha = 1.0f;
	color1_.alpha = 1.0f;

	c_rasterizer::draw_debug_line2d(p0_, p1_, real_argb_color_to_pixel32(&color0_), real_argb_color_to_pixel32(&color1_));
}

void __cdecl rasterizer_debug_triangle(real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color)
{
	rasterizer_vertex_debug vertex_debug[1]{};
	vertex_debug[0].point0 = *point0;
	vertex_debug[0].point1 = *point1;
	vertex_debug[0].point2 = *point2;

	dword _color = real_argb_color_to_pixel32(color);
	vertex_debug[0].color0 = _color;
	vertex_debug[0].color1 = _color;
	vertex_debug[0].color2 = _color;

	c_rasterizer::draw_debug_polygon(vertex_debug, NUMBEROF(vertex_debug), c_rasterizer_index_buffer::_primitive_type_triangle_strip); // D3DPT_TRIANGLESTRIP
}

//render_debug_random_color
//render_debug_unique_color
//render_debug_spray

void __cdecl render_debug_point2d(bool draw_immediately, plane3d const* plane, short projection_axis, bool a4, real_point2d const* point, real scale, real_argb_color const* color, real a8)
{
	ASSERT(plane);
	ASSERT(point);
	ASSERT(color);

	real_point3d q{};
	project_point2d(point, plane, projection_axis, a4, &q);
	ASSERT(VALID_INDEX(projection_axis, NUMBEROF(q.n)));

	q.n[projection_axis] += a4 ? a8 : -a8;

	render_debug_point(draw_immediately, &q, scale, color);
}

void __cdecl render_debug_line2d(real_point2d const* point0, real_point2d const* point1, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color);

	if (render_debug_draw_immediately(color))
	{
		rasterizer_debug_line2d(point0, point1, color, color);
	}
	else
	{
		render_debug_add_cache_entry(3, point0, point1, color, color);
	}
}

void __cdecl render_debug_line2d(bool draw_immediately, plane3d const* plane, short projection_axis, bool a4, real_point2d const* p0, real_point2d const* p1, real_argb_color const* color, real a8)
{
	ASSERT(plane);
	ASSERT(p0);
	ASSERT(p1);
	ASSERT(color);

	real_point3d point0{};
	real_point3d point1{};

	project_point2d(p0, plane, projection_axis, a4, &point0);
	project_point2d(p1, plane, projection_axis, a4, &point1);

	point0.n[projection_axis] += a4 ? a8 : -a8;
	point1.n[projection_axis] += a4 ? a8 : -a8;

	render_debug_line(draw_immediately, &point0, &point1, color);
}

void __cdecl render_debug_vector2d(bool draw_immediately, plane3d const* plane, short projection_axis, bool a4, real_point2d const* point, vector2d const* vector, real scale, real_argb_color const* color, real a9)
{
	ASSERT(plane);
	ASSERT(point);
	ASSERT(vector);
	ASSERT(color);

	real_point2d point1{};
	point_from_line2d(point, vector, scale, &point1);
	render_debug_line2d(draw_immediately, plane, projection_axis, a4, point, &point1, color, a9);
}

void __cdecl render_debug_circle(bool draw_immediately, plane3d const* plane, short projection_axis, bool a4, real_point2d const* center, real radius, real_argb_color const* color, real a8)
{
	ASSERT(plane);
	ASSERT(center);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		real_point2d circle_points[CIRCLE_DIVISIONS + 1]{};
		render_debug_build_circle_points(radius, circle_points, CIRCLE_DIVISIONS + 1);
		for (long i = 0; i < CIRCLE_DIVISIONS; i++)
		{
			real_point2d* circle_point0 = &circle_points[i];
			real_point2d* circle_point1 = &circle_points[i + 1];

			real_point2d point0{};
			real_point2d point1{};

			set_real_point2d(&point0, (center->x + circle_point0->x), (center->y + circle_point0->y));
			set_real_point2d(&point1, (center->x + circle_point1->x), (center->y + circle_point1->y));
			render_debug_line2d(draw_immediately, plane, projection_axis, a4, &point0, &point1, color, a8);
		}
	}
	else
	{
		render_debug_add_cache_entry(0, plane, projection_axis, a4, center, radius, color);
	}
}

void __cdecl render_debug_circle(bool draw_immediately, real_point3d const* center, vector3d const* normal, real radius, real_argb_color const* color)
{
	render_debug_polygon_regular(draw_immediately, center, CIRCLE_DIVISIONS, normal, radius, color);
}

void __cdecl render_debug_polygon_regular(bool draw_immediately, real_point3d const* center, long point_count, vector3d const* normal, real radius, real_argb_color const* color)
{
	ASSERT(center != NULL);
	ASSERT(normal != NULL);
	ASSERT(point_count <= CIRCLE_DIVISIONS);

	real_matrix4x3 matrix{};
	matrix.matrix.forward = *normal;
	generate_up_vector3d(&matrix.matrix.forward, &matrix.matrix.up);
	cross_product3d(&matrix.matrix.up, &matrix.matrix.forward, &matrix.matrix.left);
	matrix.center = *global_origin3d;
	matrix.scale = 1.0f;

	// asserts

	real_point2d circle_points[CIRCLE_DIVISIONS + 1]{};
	render_debug_build_circle_points(radius, circle_points, point_count + 1);

	for (long i = 0; i < point_count; i++)
	{
		real_point3d point0{};
		real_point3d point1{};

		set_real_point3d(&point0, 0.0f, circle_points[i].x, circle_points[i].y);
		set_real_point3d(&point1, 0.0f, circle_points[i + 1].x, circle_points[i + 1].y);

		matrix4x3_transform_point(&matrix, &point0, &point0);
		matrix4x3_transform_point(&matrix, &point1, &point1);

		point0.x += center->x;
		point0.y += center->y;
		point0.z += center->z;

		point1.x += center->x;
		point1.y += center->y;
		point1.z += center->z;

		render_debug_line(draw_immediately, &point0, &point1, color);
	}

}

void __cdecl render_debug_point(bool draw_immediately, real_point3d const* point, real scale, real_argb_color const* color)
{
	ASSERT(point);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		scale *= 0.5f;
		real_point3d points[6]{};
		set_real_point3d(points, point->x - scale, point->y, point->z);
		set_real_point3d(&points[1], point->x + scale, point->y, point->z);
		set_real_point3d(&points[2], point->x, point->y - scale, point->z);
		set_real_point3d(&points[3], point->x, point->y + scale, point->z);
		set_real_point3d(&points[4], point->x, point->y, point->z - scale);
		set_real_point3d(&points[5], point->x, point->y, point->z + scale);

		for (long i = 0; i < 6; i += 2)
			render_debug_line(true, &points[i], &points[i + 1], color);
	}
	else
	{
		render_debug_add_cache_entry(1, point, scale, color);
	}
}

void __cdecl render_debug_line(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		rasterizer_debug_line(point0, point1, color, color);
	}
	else
	{
		render_debug_add_cache_entry(2, point0, point1, color, color);
	}
}

void __cdecl render_debug_line_shaded(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color0, real_argb_color const* color1)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color0);
	ASSERT(color1);

	if (draw_immediately || (render_debug_draw_immediately(color0) && render_debug_draw_immediately(color1)))
	{
		rasterizer_debug_line(point0, point1, color0, color1);
	}
	else
	{
		render_debug_add_cache_entry(2, point0, point1, color0, color1);
	}
}

void __cdecl render_debug_line2d_shaded(real_point2d const* point0, real_point2d const* point1, real_argb_color const* color0, real_argb_color const* color1)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color0);
	ASSERT(color1);

	if (render_debug_draw_immediately(color0) && render_debug_draw_immediately(color1))
	{
		rasterizer_debug_line2d(point0, point1, color0, color1);
	}
	else
	{
		render_debug_add_cache_entry(3, point0, point1, color0, color1);
	}
}

//render_debug_line_unclipped
//render_debug_line_non_occluded

void __cdecl render_debug_vector(bool draw_immediately, real_point3d const* point, vector3d const* vector, real scale, real_argb_color const* color)
{
	ASSERT(point);
	ASSERT(vector);
	ASSERT(color);

	real_point3d point1{};
	point_from_line3d(point, vector, scale, &point1);
	render_debug_line(draw_immediately, point, &point1, color);
}

//render_debug_tick
//render_debug_line_offset
//render_debug_vectors
//render_debug_quaternion
//render_debug_matrix
//render_debug_matrix3x3

void __cdecl render_debug_triangle(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		rasterizer_debug_triangle(point0, point1, point2, color);
	}
	else
	{
		render_debug_add_cache_entry(9, point0, point1, point2, color);
	}
}

void __cdecl render_debug_triangle_outline(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(color);

	render_debug_line(draw_immediately, point0, point1, color);
	render_debug_line(draw_immediately, point1, point2, color);
	render_debug_line(draw_immediately, point2, point0, color);
}

void __cdecl render_debug_quadrilateral(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_point3d const* point3, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(point3);
	ASSERT(color);

	render_debug_triangle(draw_immediately, point0, point1, point2, color);
	render_debug_triangle(draw_immediately, point0, point2, point3, color);
}

void __cdecl render_debug_sphere(bool draw_immediately, real_point3d const* center, real radius, real_argb_color const* color)
{
	ASSERT(center);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		if (render_sphere_visible(center, radius))
		{
			real_point2d circle_points[CIRCLE_DIVISIONS + 1]{};
			render_debug_build_circle_points(radius, circle_points, CIRCLE_DIVISIONS + 1);
			for (long i = 0; i < CIRCLE_DIVISIONS; i++)
			{
				real_point2d* circle_point0 = &circle_points[i];
				real_point2d* circle_point1 = &circle_points[i + 1];

				real_point3d point0{};
				real_point3d point1{};

				set_real_point3d(&point0, (center->x + circle_point0->x), (center->y + circle_point0->y), center->z);
				set_real_point3d(&point1, (center->x + circle_point1->x), (center->y + circle_point1->y), center->z);
				render_debug_line(draw_immediately, &point0, &point1, color);

				set_real_point3d(&point0, (center->x + circle_point0->y), center->y, (center->z + circle_point0->x));
				set_real_point3d(&point1, (center->x + circle_point1->y), center->y, (center->z + circle_point1->x));
				render_debug_line(draw_immediately, &point0, &point1, color);

				set_real_point3d(&point0, center->x, (center->y + circle_point0->x), (center->z + circle_point0->y));
				set_real_point3d(&point1, center->x, (center->y + circle_point1->x), (center->z + circle_point1->y));
				render_debug_line(draw_immediately, &point0, &point1, color);
			}
		}
	}
	else
	{
		render_debug_add_cache_entry(4, center, radius, color);
	}
}

void __cdecl render_debug_cylinder(bool draw_immediately, real_point3d const* base, vector3d const* height, real radius, real_argb_color const* color)
{
	ASSERT(base);
	ASSERT(height);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		real_point3d points0[CIRCLE_DIVISIONS + 1]{};
		real_point3d points1[CIRCLE_DIVISIONS + 1]{};

		render_debug_build_pill_points(base, height, radius, points0, points1, nullptr, nullptr, nullptr, nullptr);

		for (long i = 0; i < CIRCLE_DIVISIONS; i++)
		{
			render_debug_line(draw_immediately, &points0[i], &points0[i + 1], color);
			render_debug_line(draw_immediately, &points1[i], &points1[i + 1], color);
		}

		for (long i = 0; i < CIRCLE_DIVISIONS; i += 4)
			render_debug_line(draw_immediately, &points0[i], &points1[i], color);

		for (long i = 0; i < 8; i += 4)
		{
			render_debug_line(draw_immediately, &points0[i], &points0[i + 8], color);
			render_debug_line(draw_immediately, &points1[i], &points1[i + 8], color);
		}
	}
	else
	{
		render_debug_add_cache_entry(5, base, height, radius, color);
	}
}

void __cdecl render_debug_pill(bool draw_immediately, real_point3d const* base, vector3d const* height, real radius, real_argb_color const* color)
{
	ASSERT(base);
	ASSERT(height);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		real_point3d points0[CIRCLE_DIVISIONS + 1]{};
		real_point3d points1[CIRCLE_DIVISIONS + 1]{};
		real_point3d points2[9]{};
		real_point3d points3[9]{};
		real_point3d points4[9]{};
		real_point3d points5[9]{};

		render_debug_build_pill_points(base, height, radius, points0, points1, points2, points3, points4, points5);

		for (long i = 0; i < CIRCLE_DIVISIONS; ++i)
		{
			render_debug_line(draw_immediately, &points0[i], &points0[i + 1], color);
			render_debug_line(draw_immediately, &points1[i], &points1[i + 1], color);
		}

		for (long i = 0; i < CIRCLE_DIVISIONS; i += 4)
			render_debug_line(draw_immediately, &points0[i], &points1[i], color);

		for (long i = 0; i < 8; i++)
		{
			render_debug_line(draw_immediately, &points2[i], &points2[i + 1], color);
			render_debug_line(draw_immediately, &points3[i], &points3[i + 1], color);
			render_debug_line(draw_immediately, &points4[i], &points4[i + 1], color);
			render_debug_line(draw_immediately, &points5[i], &points5[i + 1], color);
		}
	}
	else
	{
		render_debug_add_cache_entry(6, base, height, radius, color);
	}
}

void __cdecl render_debug_box2d_outline(bool draw_immediately, real_rectangle2d const* bounds, real_argb_color const* color)
{
	ASSERT(bounds);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		real_point3d points[4]{};

		set_real_point3d(points, bounds->x.lower, bounds->y.lower, -1.0);
		set_real_point3d(&points[1], bounds->x.upper, bounds->y.lower, -1.0);
		set_real_point3d(&points[2], bounds->x.upper, bounds->y.upper, -1.0);
		set_real_point3d(&points[3], bounds->x.lower, bounds->y.upper, -1.0);

		render_projection const* projection = c_player_view::get_global_player_view()->get_rasterizer_projection();
		matrix4x3_transform_point(&projection->view_to_world, points, points);
		matrix4x3_transform_point(&projection->view_to_world, &points[1], &points[1]);
		matrix4x3_transform_point(&projection->view_to_world, &points[2], &points[2]);
		matrix4x3_transform_point(&projection->view_to_world, &points[3], &points[3]);
		render_debug_polygon_edges(points, NUMBEROF(points), color);
	}
	else
	{
		render_debug_add_cache_entry(12, bounds, color);
	}
}

void __cdecl render_debug_box(bool draw_immediately, real_rectangle3d const* bounds, real_argb_color const* color)
{
	ASSERT(bounds);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		real_point3d faces[k_faces_per_cube_count][4]{};
		rectangle3d_build_faces(bounds, k_faces_per_cube_count, faces);

		for (long i = 0; i < k_faces_per_cube_count; i++)
			render_debug_quadrilateral(1, faces[i], &faces[i][1], &faces[i][2], &faces[i][3], color);
	}
	else
	{
		render_debug_add_cache_entry(7, bounds, color);
	}
}

void __cdecl render_debug_box_oriented(bool draw_immediately, real_rectangle3d const* bounds, real_matrix4x3 const* matrix, real_argb_color const* color)
{
	real_point3d faces[k_faces_per_cube_count][4]{};
	matrix4x3_transform_points(matrix, k_faces_per_cube_count * 4, (real_point3d*)faces, (real_point3d*)faces);
	rectangle3d_build_faces(bounds, k_faces_per_cube_count, faces);

	for (long i = 0; i < k_faces_per_cube_count; i++)
		render_debug_quadrilateral(draw_immediately, faces[i], &faces[i][1], &faces[i][2], &faces[i][3], color);
}

void __cdecl render_debug_box_outline_oriented(bool draw_immediately, real_rectangle3d const* bounds, real_matrix4x3 const* matrix, real_argb_color const* color)
{
	ASSERT(bounds);
	ASSERT(matrix);
	ASSERT(color);

	real_point3d edges[k_edges_per_cube_count][2]{};
	rectangle3d_build_edges(bounds, k_edges_per_cube_count, edges);
	for (long i = 0; i < k_edges_per_cube_count; i++)
	{
		matrix4x3_transform_point(matrix, edges[i], edges[i]);
		matrix4x3_transform_point(matrix, &edges[i][1], &edges[i][1]);
		render_debug_line(draw_immediately, edges[i], &edges[i][1], color);
	}
}

void __cdecl render_debug_polygon(real_point3d const* points, short total_point_count, real_argb_color const* color)
{
	if (total_point_count > 1)
	{
		ASSERT(points);
		ASSERT(color);

		for (short i = 1; i < total_point_count - 1; i++)
			render_debug_triangle(true, points, &points[i], &points[i + 1], color);
	}
}

void __cdecl render_debug_polygon_edges(real_point3d const* points, short total_point_count, real_argb_color const* color)
{
	if (total_point_count > 1)
	{
		ASSERT(points);
		ASSERT(color);

		if (total_point_count > 2)
		{
			render_debug_line(true, &points[total_point_count - 1], points, color);
			for (short i = 1; i < total_point_count; i++)
				render_debug_line(true, &points[i - 1], &points[i], color);
		}
	}
}

void __cdecl render_debug_k_graph(real_point3d const* points, short total_point_count, real_argb_color const* color)
{
	ASSERT(points);
	ASSERT(color);

	if (total_point_count > 2)
	{
		for (short i = 0; i < total_point_count; i++)
		{
			for (short j = i + 1; j < total_point_count; j++)
				render_debug_line(1, &points[i], &points[j], color);
		}
	}
}

void __cdecl render_debug_cone_outline(bool draw_immediately, real_point3d const* point, vector3d const* direction, real radius, real cone_angle, real_argb_color const* color)
{
	vector3d normalized_cone_direction = *direction;
	normalize3d(&normalized_cone_direction);

	real cone_vertex_offset = sinf(cone_angle) * radius;
	real cone_base_offset = cosf(cone_angle) * radius;

	real_point3d vertex_on_cone{};
	point_from_line3d(point, &normalized_cone_direction, cone_base_offset, &vertex_on_cone);

	vector3d perpendicular_to_cone_direction{};
	perpendicular3d(&normalized_cone_direction, &perpendicular_to_cone_direction);
	normalize3d(&perpendicular_to_cone_direction);

	vector3d perpendicular_cross_product{};
	cross_product3d(&perpendicular_to_cone_direction, &normalized_cone_direction, &perpendicular_cross_product);
	normalize3d(&perpendicular_cross_product);

	real_point3d cone_outline_points[10]{};
	for (long i = 0; i < NUMBEROF(cone_outline_points); i++)
	{
		real angle = real(TWO_PI * i) / NUMBEROF(cone_outline_points);

		real_point3d* outline_point = &cone_outline_points[i];
		point_from_line3d(&vertex_on_cone, &perpendicular_to_cone_direction, (cone_vertex_offset * sinf(angle)), outline_point);

		real_point3d* cross_product_point = &cone_outline_points[i];
		point_from_line3d(&cone_outline_points[i], &perpendicular_cross_product, (cone_vertex_offset * cosf(angle)), cross_product_point);
	}

	for (long i = 0; i < NUMBEROF(cone_outline_points); i++)
	{
		long next_point_index = (i + 1) % NUMBEROF(cone_outline_points);
		render_debug_line(draw_immediately, &cone_outline_points[i], &cone_outline_points[next_point_index], color);
		render_debug_line(draw_immediately, &cone_outline_points[i], point, color);
	}
}

void __cdecl render_debug_string(char const* string)
{
	ASSERT(string);

	render_debug_string_immediate(true, nullptr, 0, string);

	//render_debug_add_cache_entry(10, string);
}

void __cdecl render_debug_string_at_point(real_point3d const* point, char const* string, union real_argb_color const* color)
{
	ASSERT(point);
	ASSERT(string);
	ASSERT(color);

	render_debug_string_at_point_immediate(point, string, color, 0.0f);

	//render_debug_add_cache_entry(11, string, point, color, 0.0f);
}

void __cdecl render_debug_string_immediate(bool draw_immediately, short const* tab_stops, short tab_stop_count, char const* string)
{
	ASSERT(string);
	//ASSERT(g_render_debug_globals);

	if (draw_immediately || g_render_debug_globals->use_simple_font)
	{
		c_simple_font_draw_string draw_string;
		draw_string.set_tab_stops(tab_stops, tab_stop_count);
		interface_set_bitmap_text_draw_mode(&draw_string, 0, -1, 0, 0, 5, 0);
		draw_string.draw(nullptr, string);
	}
	else
	{
		c_rasterizer_draw_string draw_string;
		c_font_cache_mt_safe font_cache;

		draw_string.set_tab_stops(tab_stops, tab_stop_count);
		interface_set_bitmap_text_draw_mode(&draw_string, 0, -1, 0, 0, 5, 0);
		draw_string.draw(&font_cache, string);
	}
}

void __cdecl render_debug_string_at_point_immediate(real_point3d const* point, char const* string, real_argb_color const* color, real scale)
{
	ASSERT(point);
	ASSERT(string);
	ASSERT(color);

	render_camera const* camera = c_player_view::x_global_player_views[0].get_rasterizer_camera();
	render_projection const* projection = c_player_view::x_global_player_views[0].get_rasterizer_projection();

	vector2d aspect_ratio_scale{};
	vector2d aspect_ratio_scaling = interface_get_aspect_ratio_scaling();
	aspect_ratio_scale.i = 1.0f / aspect_ratio_scaling.i;
	aspect_ratio_scale.j = 1.0f / aspect_ratio_scaling.j;

	short_rectangle2d window_display_bounds = camera->window_pixel_bounds;
	interface_scale_rectangle2d_for_xenon_scaler(&window_display_bounds, &aspect_ratio_scale);

	real_point2d screen_point{};
	if (render_camera_world_to_window(camera, projection, &window_display_bounds, point, &screen_point))
	{
		real text_scale = aspect_ratio_scale.i;
		if (scale > 0.0099999998f)
		{
			real v9 = point->y - camera->position.y;
			real v10 = point->z - camera->position.z;
			text_scale = scale / (square_root(((v9 * v9) + ((point->x - camera->position.x) * (point->x - camera->position.x))) + (v10 * v10)) + scale);
		}

		short_rectangle2d bounds{};
		bounds.x0 = static_cast<short>(screen_point.x - window_display_bounds.x0);
		bounds.y0 = static_cast<short>(screen_point.y - window_display_bounds.y0);
		bounds.x1 = INT16_MAX;
		bounds.y1 = INT16_MAX;

		if (g_render_debug_globals->use_simple_font)
		{
			c_simple_font_draw_string draw_string;
			interface_set_bitmap_text_draw_mode(&draw_string, 0, -1, 0, 0, 5, 0);
			draw_string.set_color(color);
			draw_string.set_bounds(&bounds);
			if (scale > 0.0099999998f)
				draw_string.set_scale(text_scale);
			draw_string.draw(nullptr, string);
		}
		else
		{
			c_rasterizer_draw_string draw_string;
			c_font_cache_mt_safe font_cache;

			interface_set_bitmap_text_draw_mode(&draw_string, 0, -1, 0, 0, 5, 0);
			draw_string.set_color(color);
			draw_string.set_bounds(&bounds);
			if (scale > 0.0099999998f)
				draw_string.set_scale(text_scale);
			draw_string.draw(&font_cache, string);
		}
	}
}

bool __cdecl render_debug_draw_immediately(real_argb_color const* color)
{
	if (color)
	{
		return color->alpha >= 1.0f;
	}
	else
	{
		return true;
	}

	return false;
}

void __cdecl render_debug_add_cache_entry(short type, ...)
{

}

void __cdecl render_debug_cache_draw(bool a1)
{

}

// `sound/game_sound_spatialization.cpp`
void __cdecl render_debug_polygon_fan(real_point3d const* points, short total_point_count, real_argb_color const* color)
{
	if (total_point_count > 1)
	{
		ASSERT(points);
		ASSERT(color);

		real_argb_color color_ = *color;
		color_.alpha = 1.0f;

		for (short i = 1; i < total_point_count - 1; i++)
		{
			render_debug_triangle(true, points, &points[i], &points[i + 1], color);
			render_debug_line(true, points, &points[i], &color_);
			render_debug_line(true, &points[i], &points[i + 1], &color_);
			render_debug_line(true, points, &points[i + 1], &color_);
		}
	}
}

real __cdecl build_height_matrix(real_point3d const* base, vector3d const* height, real_matrix4x3* out_matrix)
{
	out_matrix->scale = 1.0f;
	out_matrix->matrix.up = *height;
	perpendicular3d(&out_matrix->matrix.up, &out_matrix->matrix.left);
	real result = normalize3d(&out_matrix->matrix.up);
	normalize3d(&out_matrix->matrix.left);
	cross_product3d(&out_matrix->matrix.left, &out_matrix->matrix.up, &out_matrix->matrix.forward);
	out_matrix->center = *base;

	return result;
}

void __cdecl render_debug_build_circle_points(real radius, real_point2d* points, long total_point_count)
{
	ASSERT(total_point_count > 0);

	real v8 = real(TWO_PI / total_point_count - 1);
	real v7 = sinf(v8);
	real v10 = cosf(v8);
	set_real_point2d(points, radius, 0.0f);
	for (long i = 0; i + 1 < total_point_count; i++)
		rotate_vector2d((vector2d*)&points[i], v7, v10, (vector2d*)&points[i + 1]);

	real_point2d v6 = *points;
	points[total_point_count - 1] = v6;
}

void __cdecl render_debug_build_pill_points(real_point3d const* base, vector3d const* height, real radius, real_point3d* points0, real_point3d* points1, real_point3d* points2, real_point3d* points3, real_point3d* points4, real_point3d* points5)
{
	real_matrix4x3 height_matrix{};
	real normalized_up = build_height_matrix(base, height, &height_matrix);

	real_point2d circle_points[CIRCLE_DIVISIONS + 1]{};
	render_debug_build_circle_points(radius, circle_points, CIRCLE_DIVISIONS + 1);

	if (points0 && points1)
	{
		for (long i = 0; i < CIRCLE_DIVISIONS; i++)
		{
			real_point2d* circle_point = &circle_points[i];

			set_real_point3d(&points0[i], circle_point->x, circle_point->y, normalized_up);
			set_real_point3d(&points1[i], circle_point->x, circle_point->y, 0.0f);

			matrix4x3_transform_point(&height_matrix, &points0[i], &points0[i]);
			matrix4x3_transform_point(&height_matrix, &points1[i], &points1[i]);
		}
	}

	if (points2 && points3 && points4 && points5)
	{
		for (long i = 0; i < 9; i++)
		{
			real_point2d* circle_point1 = &circle_points[i];
			real_point2d* circle_point0 = &circle_points[i + 8];

			set_real_point3d(&points2[i], 0.0, circle_point1->x, circle_point1->y + normalized_up);
			set_real_point3d(&points3[i], 0.0, circle_point0->x, circle_point0->y);
			set_real_point3d(&points4[i], circle_point1->x, 0.0f, circle_point1->y + normalized_up);
			set_real_point3d(&points5[i], circle_point0->x, 0.0f, circle_point0->y);

			matrix4x3_transform_point(&height_matrix, &points2[i], &points2[i]);
			matrix4x3_transform_point(&height_matrix, &points3[i], &points3[i]);
			matrix4x3_transform_point(&height_matrix, &points4[i], &points4[i]);
			matrix4x3_transform_point(&height_matrix, &points5[i], &points5[i]);
		}
	}
}

