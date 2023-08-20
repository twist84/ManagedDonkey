#include "render/render_debug.hpp"

#include "interface/interface.hpp"
#include "interface/interface_constants.hpp"
#include "math/color_math.hpp"
#include "math/random_math.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/views/render_view.hpp"
#include "render/render_cameras.hpp"
#include "render/render_lights.hpp"
#include "render/render_visibility.hpp"
#include "text/draw_string.hpp"

#define MAXIMUM_CACHE_STRING_LENGTH 4096

struct s_render_debug_cache_entry
{
	short type;
	short layer;
	real __unknown4;

	union
	{
		struct // type 0
		{
			plane3d plane;
			short projection_axis;
			bool a4;
			real_point2d center;
			real radius;
			real_argb_color color;
		} circle;

		struct // type 1
		{
			real_point3d point;
			real scale;
			real_argb_color color;
		} point;

		struct // type 2
		{
			real_point3d point0;
			real_point3d point1;
			real_argb_color color0;
			real_argb_color color1;
		} line;

		struct // type 3
		{
			real_point2d point0;
			real_point2d point1;
			real_argb_color color0;
			real_argb_color color1;
		} line2d;

		struct // type 4
		{
			real_point3d center;
			real radius;
			real_argb_color color;
		} sphere;

		struct // type 5
		{
			real_point3d base;
			vector3d height;
			real radius;
			real_argb_color color;
		} cylinder;

		struct // type 6
		{
			real_point3d base;
			vector3d height;
			real radius;
			real_argb_color color;
		} pill;

		struct // type 7, 8
		{
			real_rectangle3d bounds;
			real_argb_color color;
		} box;

		struct // type 9
		{
			real_point3d point0;
			real_point3d point1;
			real_point3d point2;
			real_argb_color color;
		} triangle;

		struct // type 10
		{
			long string_offset;
			short tab_stops[16];
			short tab_stop_count;
		} string;
		
		struct // type 11
		{
			long string_offset;
			real_point3d point;
			real_argb_color color;
			real scale;
		} string_at_point;

		struct // type 12
		{
			real_rectangle2d bounds;
			real_argb_color color;
		} box2d_outline;

		byte type_storage[0x38];
	};
};

struct s_render_debug_globals
{
	bool active;
	bool drawing_cached_geometry;
	bool use_simple_font;

	real __unknown4;

	long group_ids[2];
	long group_level;

	short cache_count;
	short cache_count2;

	bool inside_game_tick;
	char __data19[3];

	s_render_debug_cache_entry cache_entries[8192];

	short cache_string_length;
	short __unknown8001E;

	char cache_string[MAXIMUM_CACHE_STRING_LENGTH];
};
static_assert(sizeof(s_render_debug_globals) == 0x81020);

static s_render_debug_globals render_debug_globals
{
	.use_simple_font = false
};

thread_local s_render_debug_globals* g_render_debug_globals = &render_debug_globals;

long type_list[] = { 0, 0, 0, 3, 0, 0, 0, 1, 0, 1, 2, 2, 0 };

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

long __cdecl render_debug_add_cache_string(char const* string)
{
	ASSERT(g_render_debug_globals);

	long string_offset = 0;
	long result = -1;

	if (g_render_debug_globals->cache_string_length > 0)
	{
		while (csstrcmp(string, &g_render_debug_globals->cache_string[string_offset]))
		{
			if (++string_offset >= g_render_debug_globals->cache_string_length)
				break;

			result = string_offset;
			if (string_offset != -1)
				return string_offset;
		}
	}

	short cache_string_length = g_render_debug_globals->cache_string_length;
	if (cache_string_length >= 4095)
	{
		static bool render_debug_cache_string_overflow = false;
		if (!render_debug_cache_string_overflow)
		{
			c_console::write_line("render debug cache string overflow");
			render_debug_cache_string_overflow = true;
		}
	}
	else
	{
		result = cache_string_length;
		csstrnzcpy(&g_render_debug_globals->cache_string[cache_string_length], string, 4096 - cache_string_length);
		short v7 = (short)strlen(string);
		// length assert

		g_render_debug_globals->cache_string_length += v7 + 1;
		short v9 = g_render_debug_globals->cache_string_length;
		if (v9 > 4095)
			v9 = 4095;
		g_render_debug_globals->cache_string_length = v9;
	}

	return result;
}

real_argb_color const* __cdecl render_debug_random_color(real_argb_color* color)
{
	return render_debug_unique_color(_random(get_local_random_seed_address(), 0, __FILE__, __LINE__), color);
}

real_argb_color const* __cdecl render_debug_unique_color(long index, real_argb_color* color)
{
	ASSERT(color);

	static real_argb_color unique_colors[]
	{
		{ 1.0, { 1.0, 0.0, 1.0 } },
		{ 1.0, { 0.0, 1.0, 1.0 } },
		{ 1.0, { 1.0, 0.5, 0.0 } },
		{ 1.0, { 0.0, 1.0, 0.5 } },
		{ 1.0, { 0.5, 0.0, 1.0 } },
		{ 1.0, { 1.0, 0.0, 0.5 } },
		{ 1.0, { 0.5, 1.0, 0.0 } },
		{ 1.0, { 0.0, 0.5, 1.0 } },
		{ 1.0, { 0.5, 0.0, 0.0 } },
		{ 1.0, { 0.0, 0.5, 0.0 } },
		{ 1.0, { 0.0, 0.0, 0.5 } },
		{ 1.0, { 1.0, 1.0, 0.5 } },
		{ 1.0, { 1.0, 0.5, 1.0 } },
		{ 1.0, { 0.5, 1.0, 1.0 } },
		{ 1.0, { 0.5, 0.5, 0.0 } },
		{ 1.0, { 0.0, 0.5, 0.5 } },
		{ 1.0, { 0.5, 0.0, 0.5 } },
	};

	*color = unique_colors[abs(index) % NUMBEROF(unique_colors)];

	return color;
}

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

void __cdecl render_debug_line_unclipped(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);

	real_matrix4x3 camera{};
	c_player_view::get_player_render_camera_orientation(&camera);

	real_point3d p0 = *point0;
	real_point3d p1 = *point1;

	vector3d vector0{};
	vector3d vector1{};
	vector_from_points3d(&camera.center, &p0, &vector0);
	vector_from_points3d(&camera.center, &p1, &vector1);

	real clip_distance = magnitude3d(&vector0);
	if (magnitude3d(&vector0) <= magnitude3d(&vector1))
		clip_distance = magnitude3d(&vector1);

	real z_far = c_player_view::get_global_player_view(0)->get_rasterizer_camera()->z_far;
	if (clip_distance > (0.5f * z_far))
	{
		real distance = (0.5f * z_far) / clip_distance;
		point_from_line3d(&camera.center, &vector0, distance, &p0);
		point_from_line3d(&camera.center, &vector1, distance, &p1);
	}
	render_debug_line(draw_immediately, &p0, &p1, color);
}

void __cdecl render_debug_line_non_occluded(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);

	real_matrix4x3 camera{};
	c_player_view::get_player_render_camera_orientation(&camera);

	real_point3d p0 = *point0;
	real_point3d p1 = *point1;

	vector3d vector0{};
	vector3d vector1{};
	vector_from_points3d(&camera.center, &p0, &vector0);
	vector_from_points3d(&camera.center, &p1, &vector1);

	real clip_distance = magnitude3d(&vector0);
	if (magnitude3d(&vector0) <= magnitude3d(&vector1))
		clip_distance = magnitude3d(&vector1);

	real z_near = c_player_view::get_global_player_view(0)->get_rasterizer_camera()->z_near;
	if (clip_distance > (0.5f * z_near))
	{
		real distance = (0.5f * z_near) / clip_distance;
		point_from_line3d(&camera.center, &vector0, distance, &p0);
		point_from_line3d(&camera.center, &vector1, distance, &p1);
	}
	render_debug_line(draw_immediately, &p0, &p1, color);
}

void __cdecl render_debug_vector(bool draw_immediately, real_point3d const* point, vector3d const* vector, real scale, real_argb_color const* color)
{
	ASSERT(point);
	ASSERT(vector);
	ASSERT(color);

	real_point3d point1{};
	point_from_line3d(point, vector, scale, &point1);
	render_debug_line(draw_immediately, point, &point1, color);
}

void __cdecl render_debug_tick(bool draw_immediately, real_point3d const* point, vector3d const* vector, real scale, real_argb_color const* color)
{
	real_point3d point0{};
	real_point3d point1{};
	point_from_line3d(point, vector, scale, &point0);
	point_from_line3d(point, vector, scale, &point1);
	render_debug_line(draw_immediately, &point0, &point1, color);
}

void __cdecl render_debug_line_offset(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color, real scale)
{
	real_point3d p0{};
	real_point3d p1{};
	point_from_line3d(point0, global_up3d, scale, &p0);
	point_from_line3d(point1, global_up3d, scale, &p1);
	render_debug_line(draw_immediately, &p0, &p1, color);
}

void __cdecl render_debug_vectors(bool draw_immediately, real_point3d const* point, vector3d const* forward, vector3d const* up, real radius)
{
	real_matrix4x3 matrix{};
	matrix4x3_from_point_and_vectors(&matrix, point, forward, up);
	render_debug_matrix(draw_immediately, &matrix, radius);
}

void __cdecl render_debug_quaternion(bool draw_immediately, real_point3d const* point, real_quaternion const* quaternion, real radius)
{
	real_matrix4x3 matrix{};
	matrix4x3_from_point_and_quaternion(&matrix, point, quaternion);
	render_debug_matrix(draw_immediately, &matrix, radius);
}

void __cdecl render_debug_matrix(bool draw_immediately, real_matrix4x3 const* matrix, real radius)
{
	render_debug_vector(draw_immediately, &matrix->center, &matrix->matrix.forward, radius * matrix->scale, global_real_argb_red);
	render_debug_vector(draw_immediately, &matrix->center, &matrix->matrix.left, radius * matrix->scale, global_real_argb_green);
	render_debug_vector(draw_immediately, &matrix->center, &matrix->matrix.up, radius * matrix->scale, global_real_argb_blue);
}

void __cdecl render_debug_matrix3x3(bool draw_immediately, matrix3x3 const* matrix, real_point3d const* point, real radius)
{
	render_debug_vector(draw_immediately, point, &matrix->forward, radius, global_real_argb_red);
	render_debug_vector(draw_immediately, point, &matrix->left, radius, global_real_argb_green);
	render_debug_vector(draw_immediately, point, &matrix->up, radius, global_real_argb_blue);
}

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
		real_point3d points2[(CIRCLE_DIVISIONS / 2) + 1]{};
		real_point3d points3[(CIRCLE_DIVISIONS / 2) + 1]{};
		real_point3d points4[(CIRCLE_DIVISIONS / 2) + 1]{};
		real_point3d points5[(CIRCLE_DIVISIONS / 2) + 1]{};

		render_debug_build_pill_points(base, height, radius, points0, points1, points2, points3, points4, points5);

		for (long i = 0; i < CIRCLE_DIVISIONS; ++i)
		{
			render_debug_line(draw_immediately, &points0[i], &points0[i + 1], color);
			render_debug_line(draw_immediately, &points1[i], &points1[i + 1], color);
		}

		for (long i = 0; i < CIRCLE_DIVISIONS; i += 4)
			render_debug_line(draw_immediately, &points0[i], &points1[i], color);

		for (long i = 0; i < CIRCLE_DIVISIONS / 2; i++)
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

		render_projection const* projection = c_player_view::get_global_player_view(0)->get_rasterizer_projection();
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

void __cdecl render_debug_box_outline(bool draw_immediately, real_rectangle3d const* bounds, real_argb_color const* color)
{
	ASSERT(bounds);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		real_point3d edges[k_edges_per_cube_count][2]{};
		rectangle3d_build_edges(bounds, k_edges_per_cube_count, edges);
		for (long i = 0; i < k_edges_per_cube_count; i++)
			render_debug_line(true, edges[i], &edges[i][1], color);
	}
	else
	{
		render_debug_add_cache_entry(8, bounds, color);
	}
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

	render_debug_string_at_point_immediate(point, string, color, 0.6f);

	//render_debug_add_cache_entry(11, string, point, color, 0.0f);
}

void __cdecl render_debug_string_immediate(bool draw_immediately, short const* tab_stops, short tab_stop_count, char const* string)
{
	ASSERT(string);
	ASSERT(g_render_debug_globals);

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

	render_camera const* camera = c_player_view::get_global_player_view(0)->get_rasterizer_camera();
	render_projection const* projection = c_player_view::get_global_player_view(0)->get_rasterizer_projection();

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
		if (scale > 0.01f)
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
			draw_string.set_shadow_color(global_real_argb_black);
			draw_string.set_color(color);
			draw_string.set_bounds(&bounds);
			if (scale > 0.01f)
				draw_string.set_scale(scale);
			draw_string.draw(nullptr, string);
		}
		else
		{
			c_rasterizer_draw_string draw_string;
			c_font_cache_mt_safe font_cache;

			interface_set_bitmap_text_draw_mode(&draw_string, 0, -1, 0, 0, 5, 0);
			draw_string.set_shadow_color(global_real_argb_black);
			draw_string.set_color(color);
			draw_string.set_bounds(&bounds);
			if (scale > 0.01f)
				draw_string.set_scale(scale);
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
	ASSERT(g_render_debug_globals);

	if (g_render_debug_globals->cache_count < NUMBEROF(g_render_debug_globals->cache_entries))
	{
		s_render_debug_cache_entry* entry = &g_render_debug_globals->cache_entries[g_render_debug_globals->cache_count++];

		real alpha = 1.0f;

		va_list list;
		va_start(list, type);

		switch (type)
		{
		case 0:
		{
			entry->circle.plane = va_arg(list, decltype(entry->circle.plane));
			entry->circle.projection_axis = va_arg(list, decltype(entry->circle.projection_axis));
			entry->circle.a4 = va_arg(list, decltype(entry->circle.a4));
			entry->circle.center = va_arg(list, decltype(entry->circle.center));
			entry->circle.radius = va_arg(list, decltype(entry->circle.radius));
			entry->circle.color = va_arg(list, decltype(entry->circle.color));
			alpha = entry->circle.color.alpha;

			real_point3d centroid{};
			project_point2d(&entry->circle.center, &entry->circle.plane, entry->circle.projection_axis, entry->circle.a4, &centroid);
			ASSERT(VALID_INDEX(entry->circle.projection_axis, NUMBEROF(centroid.n)));
		}
		break;
		case 1:
		{
			entry->point.point = va_arg(list, decltype(entry->point.point));
			entry->point.scale = va_arg(list, decltype(entry->point.scale));
			entry->point.color = va_arg(list, decltype(entry->point.color));
			alpha = entry->point.color.alpha;
		}
		break;
		case 2:
		{
			
			entry->line.point0 = va_arg(list, decltype(entry->line.point0));
			entry->line.point1 = va_arg(list, decltype(entry->line.point1));
			entry->line.color0 = va_arg(list, decltype(entry->line.color0));
			entry->line.color1 = va_arg(list, decltype(entry->line.color1));
			alpha = fminf(entry->line.color1.alpha, entry->line.color0.alpha);
		}
		break;
		case 3:
		{
			entry->line2d.point0 = va_arg(list, decltype(entry->line2d.point0));
			entry->line2d.point1 = va_arg(list, decltype(entry->line2d.point1));
			entry->line2d.color0 = va_arg(list, decltype(entry->line2d.color0));
			entry->line2d.color1 = va_arg(list, decltype(entry->line2d.color1));
			alpha = fminf(entry->line.color1.alpha, entry->line.color0.alpha);
		}
		break;
		case 4:
		{
			entry->sphere.center = va_arg(list, decltype(entry->sphere.center));
			entry->sphere.radius = va_arg(list, decltype(entry->sphere.radius));
			entry->sphere.color = va_arg(list, decltype(entry->sphere.color));
			alpha = entry->sphere.color.alpha;
		}
		break;
		case 5:
		{
			entry->cylinder.base = va_arg(list, decltype(entry->cylinder.base));
			entry->cylinder.height = va_arg(list, decltype(entry->cylinder.height));
			entry->cylinder.radius = va_arg(list, decltype(entry->cylinder.radius));
			entry->cylinder.color = va_arg(list, decltype(entry->cylinder.color));
			alpha = entry->sphere.color.alpha;
		}
		break;
		case 6:
		{
			entry->pill.base = va_arg(list, decltype(entry->pill.base));
			entry->pill.height = va_arg(list, decltype(entry->pill.height));
			entry->pill.radius = va_arg(list, decltype(entry->pill.radius));
			entry->pill.color = va_arg(list, decltype(entry->pill.color));
			alpha = entry->pill.color.alpha;
		}
		break;
		case 7:
		case 8:
		{
			entry->box.bounds = va_arg(list, decltype(entry->box.bounds));
			entry->box.color = va_arg(list, decltype(entry->box.color));
			alpha = entry->box.color.alpha;
		}
		break;
		case 9:
		{
			entry->triangle.point0 = va_arg(list, decltype(entry->triangle.point0));
			entry->triangle.point1 = va_arg(list, decltype(entry->triangle.point1));
			entry->triangle.point2 = va_arg(list, decltype(entry->triangle.point2));
			entry->triangle.color = va_arg(list, decltype(entry->triangle.color));
			alpha = entry->triangle.color.alpha;
		}
		break;
		case 10:
		{
			const char* string = va_arg(list, decltype(string));
			long string_offset = render_debug_add_cache_string(string);
			if (string_offset == -1)
				break;

			entry->string.string_offset = string_offset;
			entry->string.tab_stop_count = 0;

			//LABEL_46
			entry->type = type;
			entry->layer = 0;
			entry->__unknown4 = 0.0f;
			if (g_render_debug_globals->group_level > 0)
				entry->__unknown4 = g_render_debug_globals->__unknown4;
			return;
		}
		break;
		case 11:
		{
			const char* string = va_arg(list, decltype(string));
			long string_offset = render_debug_add_cache_string(string);
			if (string_offset == -1)
				break;

			entry->string_at_point.string_offset = string_offset;
			entry->string_at_point.point = va_arg(list, decltype(entry->string_at_point.point));
			entry->string_at_point.color = va_arg(list, decltype(entry->string_at_point.color));
			entry->string_at_point.scale = va_arg(list, decltype(entry->string_at_point.scale));
			alpha = entry->string_at_point.color.alpha;

		}
		break;
		case 12:
		{
			entry->box2d_outline.bounds = va_arg(list, decltype(entry->box2d_outline.bounds));
			entry->box2d_outline.color = va_arg(list, decltype(entry->box2d_outline.color));
			alpha = entry->box2d_outline.color.alpha;

			//LABEL_46
			entry->type = type;
			entry->layer = 0;
			entry->__unknown4 = 0.0f;
			if (g_render_debug_globals->group_level > 0)
				entry->__unknown4 = g_render_debug_globals->__unknown4;
			return;
		}
		break;
		default:
		{
			entry->type = type;
			entry->layer = 0;
			entry->__unknown4 = 0.0f;
			if (g_render_debug_globals->group_level > 0)
				entry->__unknown4 = g_render_debug_globals->__unknown4;
			return;
		}
		break;
		}

		entry->type = type;

		if (alpha > 0.0f)
			entry->layer = alpha < 1.0f ? 1 : 0;

		g_render_debug_globals->cache_count--;

		entry->__unknown4 = g_render_debug_globals->group_level > 0 ? g_render_debug_globals->__unknown4 : 0.0f;
	}

	static bool render_debug_cache_overflow = false;
	if (!render_debug_cache_overflow)
	{
		c_console::write_line("render debug cache overflow.");
		render_debug_cache_overflow = true;
	}
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

	real angle = real(TWO_PI / (total_point_count - 1));
	real sin_angle = sinf(angle);
	real cos_angle = cosf(angle);
	set_real_point2d(points, radius, 0.0f);
	for (long i = 0; i + 1 < total_point_count; i++)
		rotate_vector2d((vector2d*)&points[i], sin_angle, cos_angle, (vector2d*)&points[i + 1]);

	points[total_point_count - 1] = *points;
}

void __cdecl render_debug_build_pill_points(real_point3d const* base, vector3d const* height, real radius, real_point3d* points0, real_point3d* points1, real_point3d* points2, real_point3d* points3, real_point3d* points4, real_point3d* points5)
{
	real_matrix4x3 height_matrix{};
	real normalized_up = build_height_matrix(base, height, &height_matrix);

	real_point2d circle_points[CIRCLE_DIVISIONS + 1]{};
	render_debug_build_circle_points(radius, circle_points, CIRCLE_DIVISIONS + 1);

	if (points0 && points1)
	{
		for (long i = 0; i < CIRCLE_DIVISIONS + 1; i++)
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
		for (long i = 0; i < (CIRCLE_DIVISIONS / 2) + 1; i++)
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

word __cdecl _random(dword* seed, char const* string, char const* file, dword line)
{
	//random_seed_debug(seed, "random", string, file, line);
	*seed = 0x19660D * *seed + 0x3C6EF35F;

	return HIWORD(*seed);
}

