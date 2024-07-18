#include "rasterizer/dx9/rasterizer_dx9_dynamic_geometry.hpp"

#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"

REFERENCE_DECLARE(0x01914BBC, real, g_screenspace_scale_x);
REFERENCE_DECLARE(0x01914BC0, real, g_screenspace_scale_y);
REFERENCE_DECLARE(0x01914BC4, real, g_screenspace_pixel_center);
REFERENCE_DECLARE(0x05106FA4, real, g_screenspace_offset_x);
REFERENCE_DECLARE(0x05106FA8, real, g_screenspace_offset_y);

void(__cdecl* rasterizer_draw_textured_screen_quad1)(rasterizer_vertex_screen const*, bool) = c_rasterizer::draw_textured_screen_quad;
void(__cdecl* rasterizer_draw_textured_screen_quad0)(real, real, real, real) = c_rasterizer::draw_textured_screen_quad;
void(__cdecl* rasterizer_draw_worldspace_polygon0)(real_point3d const*, long) = c_rasterizer::draw_worldspace_polygon;
void(__cdecl* rasterizer_draw_worldspace_polygon1)(rasterizer_vertex_world const*, long) = c_rasterizer::draw_worldspace_polygon;

//HOOK_DECLARE_CLASS(0x00A456A0, c_rasterizer, draw_debug_line2d);
HOOK_DECLARE_CLASS(0x00A45830, c_rasterizer, draw_debug_line);
//HOOK_DECLARE_CLASS(0x00A458B0, c_rasterizer, draw_debug_line_list2d_explicit);
HOOK_DECLARE_CLASS(0x00A45920, c_rasterizer, draw_debug_line_list_explicit);
//HOOK_DECLARE_CLASS(0x00A45970, c_rasterizer, draw_debug_linestrip2d);
HOOK_DECLARE_CLASS(0x00A45B40, c_rasterizer, draw_debug_polygon2d);
HOOK_DECLARE_CLASS(0x00A45B90, c_rasterizer, draw_debug_polygon);
HOOK_DECLARE_CLASS(0x00A45EE0, c_rasterizer, draw_fullscreen_quad);
HOOK_DECLARE(0x00A45FE0, draw_tesselated_quad);
HOOK_DECLARE_CLASS(0x00A460A0, c_rasterizer, draw_fullscreen_quad_with_texture_xform);
HOOK_DECLARE(0x00A46520, rasterizer_draw_textured_screen_quad0);
//HOOK_DECLARE(0x00A465F0, rasterizer_draw_textured_screen_quad1);
HOOK_DECLARE_CLASS(0x00A46640, c_rasterizer, draw_textured_screen_triangle_list);
//HOOK_DECLARE_CLASS(0x00A46680, c_rasterizer, draw_textured_transparent_polygon);
//HOOK_DECLARE_CLASS(0x00A46750, c_rasterizer, draw_textured_transparent_quad);
//HOOK_DECLARE(0x00A46820, rasterizer_draw_worldspace_polygon0);
HOOK_DECLARE(0x00A46890, rasterizer_draw_worldspace_polygon1);

void __cdecl c_rasterizer::draw_debug_line2d(real_point3d const& p0, real_point3d const& p1, dword color0, dword color1)
{
	INVOKE(0x00A456A0, draw_debug_line2d, p0, p1, color0, color1);
}

void __cdecl c_rasterizer::draw_debug_line(real_point3d const& p0, real_point3d const& p1, dword color0, dword color1)
{
	//INVOKE(0x00A45830, draw_debug_line, p0, p1, color0, color1);

	rasterizer_vertex_debug point_list[2]{};
	point_list[0].point = p0;
	point_list[0].color.value = color0;
	point_list[1].point = p1;
	point_list[1].color.value = color1;

	if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_explicit_shader_debug))
	{
		set_cull_mode(_cull_mode_none);
		set_z_buffer_mode(_z_buffer_mode_unknown1);
		set_indices(NULL);
		draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_line_list, 1, point_list, sizeof(rasterizer_vertex_debug));
		set_cull_mode(_cull_mode_clockwise);
	}
}

void __cdecl c_rasterizer::draw_debug_line_list2d_explicit(rasterizer_vertex_debug const* vertex_debug, long primitive_count)
{
	INVOKE(0x00A458B0, draw_debug_line_list2d_explicit, vertex_debug, primitive_count);

	//if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_explicit_shader_debug2d))
	//{
	//	vector4d constants[1]
	//	{
	//		{ 1.0f, 1.0f, 1.0f, 1.0f }
	//	};
	//
	//	set_cull_mode(_cull_mode_none);
	//	set_z_buffer_mode(_z_buffer_mode_unknown2);
	//	set_pixel_shader_constant(1, NUMBEROF(constants), constants);
	//	set_indices(NULL);
	//	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_line_list, primitive_count, vertex_debug, sizeof(rasterizer_vertex_debug));
	//	set_cull_mode(_cull_mode_clockwise);
	//}
}

void __cdecl c_rasterizer::draw_debug_line_list_explicit(rasterizer_vertex_debug const* vertex_debug, long primitive_count)
{
	//INVOKE(0x00A45920, draw_debug_line_list_explicit, vertex_debug, primitive_count);

	if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_explicit_shader_debug))
	{
		set_cull_mode(_cull_mode_none);
		set_z_buffer_mode(_z_buffer_mode_unknown1);
		set_indices(NULL);
		draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_line_list, primitive_count, vertex_debug, sizeof(rasterizer_vertex_debug));
		set_cull_mode(_cull_mode_clockwise);
	}
}

void __cdecl c_rasterizer::draw_debug_linestrip2d(int16_point2d const* points, long point_count, dword color)
{
	INVOKE(0x00A45970, draw_debug_linestrip2d, points, point_count, color);
}

void __cdecl c_rasterizer::draw_debug_polygon2d(rasterizer_vertex_debug const* polygon2d, long primitive_count, c_rasterizer_index_buffer::e_primitive_type primitive_type)
{
	//INVOKE(0x00A45B40, draw_debug_polygon2d, polygon2d, primitive_count, primitive_type);

	if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_explicit_shader_debug2d))
	{
		set_cull_mode(_cull_mode_none);
		set_z_buffer_mode(_z_buffer_mode_unknown2);
		set_indices(NULL);
		draw_primitive_up(primitive_type, primitive_count, polygon2d, sizeof(rasterizer_vertex_debug));
		set_cull_mode(_cull_mode_clockwise);
	}
}

void __cdecl c_rasterizer::draw_debug_polygon(rasterizer_vertex_debug const* polygon, long primitive_count, c_rasterizer_index_buffer::e_primitive_type primitive_type)
{
	//INVOKE(0x00A45B90, draw_debug_polygon, polygon, primitive_count, primitive_type);

	if (primitive_count > 0)
	{
		if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_explicit_shader_debug))
		{
			set_cull_mode(_cull_mode_none);
			set_z_buffer_mode(_z_buffer_mode_unknown1);
			set_indices(NULL);
			draw_primitive_up(primitive_type, primitive_count, polygon, sizeof(rasterizer_vertex_debug));
			set_cull_mode(_cull_mode_clockwise);
		}
	}
}

void __cdecl c_rasterizer::draw_fullscreen_quad(int width, int height)
{
	//INVOKE(0x00A45EE0, draw_fullscreen_quad, width, height);

	real x = -1.0f / width;
	real y = 1.0f / height;

	rasterizer_vertex_screen triangle_fan[4]{};

	triangle_fan[0].position.x = x - 1.0f;
	triangle_fan[0].position.y = y + 1.0f;
	triangle_fan[0].texcoord.x = 0.0f;
	triangle_fan[0].texcoord.y = 0.0f;
	triangle_fan[0].color.value = 0xFFFFFFFF;

	triangle_fan[1].position.x = x + 1.0f;
	triangle_fan[1].position.y = y + 1.0f;
	triangle_fan[1].texcoord.x = 1.0f;
	triangle_fan[1].texcoord.y = 0.0f;
	triangle_fan[1].color.value = 0xFFFFFFFF;

	triangle_fan[2].position.x = x - 1.0f;
	triangle_fan[2].position.y = y - 1.0f;
	triangle_fan[2].texcoord.x = 0.0f;
	triangle_fan[2].texcoord.y = 1.0f;
	triangle_fan[2].color.value = 0xFFFFFFFF;

	triangle_fan[3].position.x = x + 1.0f;
	triangle_fan[3].position.y = y - 1.0f;
	triangle_fan[3].texcoord.x = 1.0f;
	triangle_fan[3].texcoord.y = 1.0f;
	triangle_fan[3].color.value = 0xFFFFFFFF;

	set_cull_mode(_cull_mode_none);
	set_indices(NULL);
	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_strip, 2, triangle_fan, sizeof(rasterizer_vertex_screen));
	set_cull_mode(_cull_mode_clockwise);
}

void __cdecl draw_tesselated_quad()
{
	//INVOKE(0x00A45FE0, draw_tesselated_quad);

	rasterizer_vertex_screen triangle_fan[4]{};

	triangle_fan[0].position.x = -1.0f;
	triangle_fan[0].position.y = 0.0f;
	triangle_fan[0].texcoord.x = 1.0f;
	triangle_fan[0].texcoord.y = 0.0f;
	triangle_fan[0].color.value = 0xFFFFFFFF;

	triangle_fan[1].position.x = 1.0f;
	triangle_fan[1].position.y = 1.0f;
	triangle_fan[1].texcoord.x = 1.0f;
	triangle_fan[1].texcoord.y = 0.0f;
	triangle_fan[1].color.value = 0xFFFFFFFF;

	triangle_fan[2].position.x = -1.0f;
	triangle_fan[2].position.y = -1.0f;
	triangle_fan[2].texcoord.x = 0.0f;
	triangle_fan[2].texcoord.y = 1.0f;
	triangle_fan[2].color.value = 0xFFFFFFFF;

	triangle_fan[3].position.x = 1.0f;
	triangle_fan[3].position.y = -1.0f;
	triangle_fan[3].texcoord.x = 1.0f;
	triangle_fan[3].texcoord.y = 1.0f;
	triangle_fan[3].color.value = 0xFFFFFFFF;

	c_rasterizer::set_cull_mode(c_rasterizer::e_cull_mode::_cull_mode_none);
	c_rasterizer::set_indices(NULL);
	c_rasterizer::draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_strip, 2, triangle_fan, sizeof(rasterizer_vertex_screen));
	c_rasterizer::set_cull_mode(c_rasterizer::e_cull_mode::_cull_mode_clockwise);
}

void __cdecl c_rasterizer::draw_fullscreen_quad_with_texture_xform(int width, int height, real_rectangle2d const* bounds)
{
	//INVOKE(0x00A460A0, draw_fullscreen_quad_with_texture_xform, width, height, bounds);

	real x = -1.0f / width;
	real y = 1.0f / height;

	rasterizer_vertex_screen triangle_fan[4]{};

	triangle_fan[0].position.x = x - 1.0f;
	triangle_fan[0].position.y = y + 1.0f;
	triangle_fan[0].texcoord.x = bounds->x.lower;
	triangle_fan[0].texcoord.y = bounds->y.lower;
	triangle_fan[0].color.value = 0xFFFFFFFF;

	triangle_fan[1].position.x = x + 1.0f;
	triangle_fan[1].position.y = y + 1.0f;
	triangle_fan[1].texcoord.y = bounds->y.lower;
	triangle_fan[1].texcoord.x = bounds->x.upper;
	triangle_fan[1].color.value = 0xFFFFFFFF;

	triangle_fan[2].position.x = x - 1.0f;
	triangle_fan[2].position.y = y - 1.0f;
	triangle_fan[2].texcoord.x = bounds->x.lower;
	triangle_fan[2].texcoord.y = bounds->y.upper;
	triangle_fan[2].color.value = 0xFFFFFFFF;

	triangle_fan[3].position.x = x + 1.0f;
	triangle_fan[3].position.y = y - 1.0f;
	triangle_fan[3].texcoord.x = bounds->x.upper;
	triangle_fan[3].texcoord.y = bounds->y.upper;
	triangle_fan[3].color.value = 0xFFFFFFFF;

	set_cull_mode(_cull_mode_none);
	set_indices(NULL);
	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_strip, 2, triangle_fan, sizeof(rasterizer_vertex_screen));
	set_cull_mode(_cull_mode_clockwise);
}

//.text:00A461B0
//.text:00A46300

void __cdecl c_rasterizer::draw_textured_screen_quad(real a1, real a2, real a3, real a4)
{
	//DECLFUNC(0x00A46520, void, __cdecl, real, real, real, real)(a1, a2, a3, a4);

	rasterizer_vertex_screen triangle_fan[4]{};

	triangle_fan[0].position.x = a1;
	triangle_fan[0].position.y = a2;
	triangle_fan[0].texcoord.x = 0.0f;
	triangle_fan[0].texcoord.y = 1.0f;
	triangle_fan[0].color.value = 0xFFFFFFFF;

	triangle_fan[1].position.x = a3;
	triangle_fan[1].position.y = a2;
	triangle_fan[1].texcoord.x = 1.0f;
	triangle_fan[1].texcoord.y = 1.0f;
	triangle_fan[1].color.value = 0xFFFFFFFF;

	triangle_fan[2].position.x = a1;
	triangle_fan[2].position.y = a4;
	triangle_fan[2].texcoord.x = 0.0f;
	triangle_fan[2].texcoord.y = 0.0f;
	triangle_fan[2].color.value = 0xFFFFFFFF;

	triangle_fan[3].position.x = a3;
	triangle_fan[3].position.y = a4;
	triangle_fan[3].texcoord.x = 1.0f;
	triangle_fan[3].texcoord.y = 0.0f;
	triangle_fan[3].color.value = 0xFFFFFFFF;

	set_cull_mode(_cull_mode_none);
	set_indices(NULL);
	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_strip, 2, triangle_fan, sizeof(rasterizer_vertex_screen));
	set_cull_mode(_cull_mode_clockwise);
}

void __cdecl c_rasterizer::draw_textured_screen_quad(rasterizer_vertex_screen const* textured_screen_quad, bool a2)
{
	DECLFUNC(0x00A465F0, void, __cdecl, rasterizer_vertex_screen const*, bool)(textured_screen_quad, a2);
}

void __cdecl c_rasterizer::draw_textured_screen_triangle_list(rasterizer_vertex_screen const* textured_screen_triangle_list, long primitive_count)
{
	//INVOKE(0x00A46640, draw_textured_screen_triangle_list, textured_screen_triangle_list, primitive_count);

	set_cull_mode(_cull_mode_none);
	set_z_buffer_mode(_z_buffer_mode_unknown2);
	set_indices(NULL);
	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_list, primitive_count, textured_screen_triangle_list, sizeof(rasterizer_vertex_screen));
	set_cull_mode(_cull_mode_clockwise);
}

void __cdecl c_rasterizer::draw_textured_transparent_polygon(rasterizer_vertex_transparent const* textured_transparent_polygon, long polygon_count, e_alpha_blend_mode alpha_blend_mode)
{
	DECLFUNC(0x00A46680, void, __cdecl, rasterizer_vertex_transparent const*, long, e_alpha_blend_mode)(textured_transparent_polygon, polygon_count, alpha_blend_mode);
}

void __cdecl c_rasterizer::draw_textured_transparent_quad(rasterizer_vertex_transparent const* textured_transparent_quad, e_alpha_blend_mode alpha_blend_mode)
{
	DECLFUNC(0x00A46750, void, __cdecl, rasterizer_vertex_transparent const*, e_alpha_blend_mode)(textured_transparent_quad, alpha_blend_mode);
}

void __cdecl c_rasterizer::draw_worldspace_polygon(real_point3d const* worldspace_polygon, long polygon_count)
{
	DECLFUNC(0x00A46820, void, __cdecl, real_point3d const*, long)(worldspace_polygon, polygon_count);

	//rasterizer_vertex_world triangle_fan[128]{};
	//
	//for (long i = 0; i < polygon_count; i++)
	//	triangle_fan[i].position = *worldspace_polygon;
	//
	//draw_worldspace_polygon(triangle_fan, polygon_count);
}

void __cdecl c_rasterizer::draw_worldspace_polygon(rasterizer_vertex_world const* worldspace_polygon, long polygon_count)
{
	//DECLFUNC(0x00A46890, void, __cdecl, rasterizer_vertex_world const*, long)(worldspace_polygon, polygon_count);

	set_cull_mode(_cull_mode_none);
	set_indices(NULL);
	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_fan, polygon_count - 2, worldspace_polygon, sizeof(rasterizer_vertex_world));
	set_cull_mode(_cull_mode_clockwise);
}

void __cdecl rasterizer_quad_screenspace(int16_point2d const(&points)[4], dword color, s_tag_reference const* reference, short bitmap_index, bool a5)
{
	INVOKE(0x00A46DA0, rasterizer_quad_screenspace, points, color, reference, bitmap_index, a5);
}

bool __cdecl rasterizer_set_explicit_debug_shader(c_rasterizer_globals::e_explicit_shader explicit_shader)
{
	return INVOKE(0x00A46FB0, rasterizer_set_explicit_debug_shader, explicit_shader);
}

