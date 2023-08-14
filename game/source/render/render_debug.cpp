#include "render/render_debug.hpp"

#include "math/color_math.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/render_lights.hpp"

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

void __cdecl render_debug_point(bool a1, real_point3d const* point, real scale, real_argb_color const* color)
{
	ASSERT(point);
	ASSERT(color);

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

void __cdecl render_debug_line(bool a1, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color);

	rasterizer_debug_line(point0, point1, color, color);
}

void __cdecl render_debug_line_shaded(bool a1, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color0, real_argb_color const* color1)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color0);
	ASSERT(color1);

	rasterizer_debug_line(point0, point1, color0, color1);
}

void __cdecl render_debug_line2d(real_point2d const* point0, real_point2d const* point1, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color);

	rasterizer_debug_line2d(point0, point1, color, color);
}

void __cdecl render_debug_line2d_shaded(real_point2d const* point0, real_point2d const* point1, real_argb_color const* color0, real_argb_color const* color1)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color0);
	ASSERT(color1);

	rasterizer_debug_line2d(point0, point1, color0, color1);
}

void __cdecl render_debug_vector(bool a1, real_point3d const* point, vector3d const* vector, real scale, real_argb_color const* color)
{
	ASSERT(point);
	ASSERT(vector);
	ASSERT(color);

	real_point3d point1{};
	point_from_line3d(point, vector, scale, &point1);
	render_debug_line(a1, point, &point1, color);
}

void __cdecl render_debug_triangle(bool a1, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(color);

	rasterizer_debug_triangle(point0, point1, point2, color);
}

void __cdecl render_debug_triangle_outline(bool a1, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(color);

	render_debug_line(a1, point0, point1, color);
	render_debug_line(a1, point1, point2, color);
	render_debug_line(a1, point2, point0, color);
}

void __cdecl render_debug_quadrilateral(bool a1, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_point3d const* point3, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(point3);
	ASSERT(color);

	render_debug_triangle(a1, point0, point1, point2, color);
	render_debug_triangle(a1, point0, point2, point3, color);
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

