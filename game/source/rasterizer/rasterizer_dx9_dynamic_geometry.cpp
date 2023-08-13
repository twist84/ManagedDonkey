#include "rasterizer/rasterizer.hpp"

void __cdecl c_rasterizer::draw_debug_line(real_point3d const& p0, real_point3d const& p1, dword color0, dword color1)
{
	INVOKE(0x00A45830, draw_debug_line, p0, p1, color0, color1);
}

void c_rasterizer::draw_debug_polygon(rasterizer_vertex_debug const* vertex_debug, long primitive_count, long primitive_type)
{
	INVOKE(0x00A45B90, draw_debug_polygon, vertex_debug, primitive_count, primitive_type);
}

