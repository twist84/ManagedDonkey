#include "rasterizer/dx9/rasterizer_dx9_dynamic_geometry.hpp"

#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"

REFERENCE_DECLARE(0x01914BBC, real, g_screenspace_scale_x);
REFERENCE_DECLARE(0x01914BC0, real, g_screenspace_scale_y);
REFERENCE_DECLARE(0x01914BC4, real, g_screenspace_pixel_center);
REFERENCE_DECLARE(0x05106FA4, real, g_screenspace_offset_x);
REFERENCE_DECLARE(0x05106FA8, real, g_screenspace_offset_y);

void __cdecl c_rasterizer::draw_debug_line2d(real_point3d const& p0, real_point3d const& p1, dword color0, dword color1)
{
	INVOKE(0x00A456A0, draw_debug_line, p0, p1, color0, color1);
}

void __cdecl c_rasterizer::draw_debug_line(real_point3d const& p0, real_point3d const& p1, dword color0, dword color1)
{
	INVOKE(0x00A45830, draw_debug_line, p0, p1, color0, color1);
}

void __cdecl c_rasterizer::draw_debug_line_list2d_explicit(rasterizer_vertex_debug const* vertex_debug, long primitive_count)
{
	INVOKE(0x00A458B0, draw_debug_line_list2d_explicit, vertex_debug, primitive_count);
}

void __cdecl c_rasterizer::draw_debug_line_list_explicit(rasterizer_vertex_debug const* vertex_debug, long primitive_count)
{
	INVOKE(0x00A45920, draw_debug_line_list_explicit, vertex_debug, primitive_count);
}

void __cdecl c_rasterizer::draw_debug_linestrip2d(int16_point2d const* points, long point_count, dword color)
{
	INVOKE(0x00A45970, draw_debug_linestrip2d, points, point_count, color);
}

void __cdecl c_rasterizer::draw_debug_polygon2d(rasterizer_vertex_debug const* vertex_debug, long primitive_count)
{
	INVOKE(0x00A45B40, draw_debug_polygon2d, vertex_debug, primitive_count);
}

void __cdecl c_rasterizer::draw_debug_polygon(rasterizer_vertex_debug const* vertex_debug, long primitive_count, c_rasterizer_index_buffer::e_primitive_type type)
{
	INVOKE(0x00A45B90, draw_debug_polygon, vertex_debug, primitive_count, type);
}

void __cdecl c_rasterizer::draw_fullscreen_quad(int width, int height)
{
	INVOKE(0x00A45EE0, draw_fullscreen_quad, width, height);
}

void __cdecl draw_tesselated_quad()
{
	INVOKE(0x00A45FE0, draw_tesselated_quad);
}

void __cdecl c_rasterizer::draw_fullscreen_quad_with_texture_xform(int width, int height, real_rectangle2d const* bounds)
{
	INVOKE(0x00A460A0, draw_fullscreen_quad_with_texture_xform, width, height, bounds);
}

//.text:00A461B0
//.text:00A46300

void __cdecl c_rasterizer::draw_textured_screen_quad(real a1, real a2, real a3, real a4)
{
	DECLFUNC(0x00A46520, void, __cdecl, real, real, real, real)(a1, a2, a3, a4);
}

void __cdecl c_rasterizer::draw_textured_screen_quad(rasterizer_vertex_screen const* vertex_screen, bool a2)
{
	DECLFUNC(0x00A465F0, void, __cdecl, rasterizer_vertex_screen const*, bool)(vertex_screen, a2);
}

void __cdecl c_rasterizer::draw_textured_screen_triangle_list(rasterizer_vertex_screen const* vertex_screen, long primitive_count)
{
	INVOKE(0x00A46640, draw_textured_screen_triangle_list, vertex_screen, primitive_count);
}

void __cdecl c_rasterizer::draw_textured_transparent_polygon(rasterizer_vertex_transparent const* vertex_transparent, long polygon_count, e_alpha_blend_mode alpha_blend_mode)
{
	DECLFUNC(0x00A46680, void, __cdecl, rasterizer_vertex_transparent const*, long, e_alpha_blend_mode)(vertex_transparent, polygon_count, alpha_blend_mode);
}

void __cdecl c_rasterizer::draw_textured_transparent_polygon(rasterizer_vertex_transparent const* vertex_transparent, e_alpha_blend_mode alpha_blend_mode)
{
	DECLFUNC(0x00A46750, void, __cdecl, rasterizer_vertex_transparent const*, e_alpha_blend_mode)(vertex_transparent, alpha_blend_mode);
}

void __cdecl c_rasterizer::draw_worldspace_polygon(real_point3d const* polygon, long polygon_count)
{
	DECLFUNC(0x00A46820, void, __cdecl, real_point3d const*, long)(polygon, polygon_count);
}

void __cdecl c_rasterizer::draw_worldspace_polygon(rasterizer_vertex_world const* vertex_world, long polygon_count)
{
	//DECLFUNC(0x00A46890, void, __cdecl, rasterizer_vertex_world const*, long)(vertex_world, polygon_count);

	set_cull_mode(_cull_mode_none);
	set_indices(NULL);
	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_list, polygon_count - 2, vertex_world, sizeof(rasterizer_vertex_world));
	set_cull_mode(_cull_mode_clockwise);
}
using rasterizer_draw_worldspace_polygon_t = void __cdecl(rasterizer_vertex_world const*, long);
rasterizer_draw_worldspace_polygon_t* rasterizer_draw_worldspace_polygon = c_rasterizer::draw_worldspace_polygon;
HOOK_DECLARE_CALL(0x00A5A205, rasterizer_draw_worldspace_polygon);

void __cdecl rasterizer_quad_screenspace(int16_point2d const(&points)[4], dword color, s_tag_reference const* reference, short bitmap_index, bool a5)
{
	INVOKE(0x00A46DA0, rasterizer_quad_screenspace, points, color, reference, bitmap_index, a5);
}

