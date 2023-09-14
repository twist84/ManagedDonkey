#include "rasterizer/rasterizer.hpp"

void __cdecl rasterizer_quad_screenspace(int16_point2d const(&points)[4], dword color, s_tag_reference const* reference, short bitmap_index, bool a5)
{
	INVOKE(0x00A46DA0, rasterizer_quad_screenspace, points, color, reference, bitmap_index, a5);
}

void __cdecl c_rasterizer::draw_debug_line2d(real_point3d const& p0, real_point3d const& p1, dword color0, dword color1)
{
	INVOKE(0x00A456A0, draw_debug_line, p0, p1, color0, color1);
}

void __cdecl c_rasterizer::draw_debug_line(real_point3d const& p0, real_point3d const& p1, dword color0, dword color1)
{
	INVOKE(0x00A45830, draw_debug_line, p0, p1, color0, color1);
}

// sub_A458B0

void __cdecl c_rasterizer::draw_debug_line_list_explicit(rasterizer_vertex_debug const* vertex_debug, long primitive_count)
{
	INVOKE(0x00A45920, draw_debug_line_list_explicit, vertex_debug, primitive_count);
}

void __cdecl c_rasterizer::draw_debug_linestrip2d(int16_point2d const* points, long point_count, dword color)
{
	INVOKE(0x00A45970, draw_debug_linestrip2d, points, point_count, color);
}

void __cdecl c_rasterizer::draw_debug_line_list2d_explicit(rasterizer_vertex_debug const* vertex_debug, long primitive_count)
{
	INVOKE(0x00A45B40, draw_debug_line_list2d_explicit, vertex_debug, primitive_count);
}

void __cdecl c_rasterizer::draw_debug_polygon(rasterizer_vertex_debug const* vertex_debug, long primitive_count, c_rasterizer_index_buffer::e_primitive_type type)
{
	INVOKE(0x00A45B90, draw_debug_polygon, vertex_debug, primitive_count, type);
}

