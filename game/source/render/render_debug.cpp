#include "render/render_debug.hpp"

#include "math/color_math.hpp"
#include "rasterizer/rasterizer.hpp"

void __cdecl rasterizer_debug_line(real_point3d const* p0, real_point3d const* p1, real_argb_color const* color0, real_argb_color const* color1)
{
	ASSERT(p0 && p1 && color0);

	real_argb_color _color0 = *color0;
	real_argb_color _color1 = *color1;

	_color0.alpha = 1.0f;
	_color1.alpha = 1.0f;

	for (long i = 0; i < NUMBEROF(_color0.color.n); i++)
	{
		_color0.color.n[i] = _color0.color.n[i] * _color0.color.n[i];
		_color1.color.n[i] = _color1.color.n[i] * _color1.color.n[i];
	}

	c_rasterizer::draw_debug_line(*p0, *p1, real_argb_color_to_pixel32(&_color0), real_argb_color_to_pixel32(&_color1));
}

void __cdecl render_debug_line(bool a1, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color);

	rasterizer_debug_line(point0, point1, color, color);
}

void __cdecl render_debug_polygon_edges(real_point3d const* points, short point_count, real_argb_color const* color)
{
	if (point_count > 1)
	{
		ASSERT(points);
		ASSERT(color);

		if (point_count > 2)
		{
			render_debug_line(1, &points[point_count - 1], points, color);
			for (short i = 1; i < point_count; ++i)
				render_debug_line(1, &points[i - 1], &points[i], color);
		}
	}
}

void __cdecl rasterizer_debug_triangle(real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color)
{
	rasterizer_vertex_debug vertex_debug{};
	vertex_debug.point0 = *point0;
	vertex_debug.point1 = *point1;
	vertex_debug.point2 = *point2;

	dword _color = real_argb_color_to_pixel32(color);
	vertex_debug.color0 = _color;
	vertex_debug.color1 = _color;
	vertex_debug.color2 = _color;
	c_rasterizer::draw_debug_polygon(&vertex_debug, 1, 5); // D3DPT_TRIANGLESTRIP
}

void __cdecl render_debug_triangle(bool a1, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(color);

	rasterizer_debug_triangle(point0, point1, point2, color);
}

void __cdecl render_debug_polygon(real_point3d const* points, short point_count, real_argb_color const* color)
{
	if (point_count > 1)
	{
		ASSERT(points);
		ASSERT(color);

		for (short i = 1; i < point_count - 1; ++i)
			render_debug_triangle(1, points, &points[i], &points[i + 1], color);
	}
}

