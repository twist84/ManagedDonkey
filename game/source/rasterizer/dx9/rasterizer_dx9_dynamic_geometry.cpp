#include "rasterizer/dx9/rasterizer_dx9_dynamic_geometry.hpp"

#include "bitmaps/bitmap_group_tag_definition.hpp"
#include "cache/cache_files.hpp"
#include "game/game_globals.hpp"
#include "interface/interface_constants.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_resource_definitions.hpp"
#include "scenario/scenario.hpp"

REFERENCE_DECLARE(0x01914BBC, real32, g_screenspace_scale_x);
REFERENCE_DECLARE(0x01914BC0, real32, g_screenspace_scale_y);
REFERENCE_DECLARE(0x01914BC4, real32, g_screenspace_pixel_center);
REFERENCE_DECLARE(0x05106FA4, real32, g_screenspace_offset_x);
REFERENCE_DECLARE(0x05106FA8, real32, g_screenspace_offset_y);

void(__cdecl* rasterizer_draw_textured_screen_quad0)(real32, real32, real32, real32) = c_rasterizer::draw_textured_screen_quad;
void(__cdecl* rasterizer_draw_textured_screen_quad1)(const rasterizer_vertex_screen*, bool) = c_rasterizer::draw_textured_screen_quad;
void(__cdecl* rasterizer_draw_worldspace_polygon0)(const real_point3d*, int32) = c_rasterizer::draw_worldspace_polygon;
void(__cdecl* rasterizer_draw_worldspace_polygon1)(const rasterizer_vertex_world*, int32) = c_rasterizer::draw_worldspace_polygon;

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
HOOK_DECLARE(0x00A46E50, rasterizer_quad_screenspace_explicit);
HOOK_DECLARE(0x00A46FB0, rasterizer_set_explicit_debug_shader);

void __cdecl c_rasterizer::draw_debug_line2d(const real_point3d& point1, const real_point3d& point2, uns32 color0, uns32 color1)
{
	INVOKE(0x00A456A0, draw_debug_line2d, point1, point2, color0, color1);
}

void __cdecl c_rasterizer::draw_debug_line(const real_point3d& point1, const real_point3d& point2, uns32 color0, uns32 color1)
{
	//INVOKE(0x00A45830, draw_debug_line, point1, point2, color0, color1);

	rasterizer_vertex_debug point_list[2]{};
	point_list[0].position = point1;
	point_list[0].color = color0;
	point_list[1].position = point2;
	point_list[1].color = color1;

	if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_shader_debug))
	{
		set_cull_mode(_cull_mode_off);
		set_z_buffer_mode(_z_buffer_mode_read);
		set_indices(NULL);
		draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_line_list, 1, point_list, sizeof(rasterizer_vertex_debug));
		set_cull_mode(_cull_mode_cw);
	}
}

void __cdecl c_rasterizer::draw_debug_line_list2d_explicit(const rasterizer_vertex_debug* points, int32 line_count)
{
	//INVOKE(0x00A458B0, draw_debug_line_list2d_explicit, points, line_count);

	if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_shader_debug2d))
	{
		real_vector4d fill_color[1]
		{
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		};

		set_cull_mode(_cull_mode_off);
		set_z_buffer_mode(_z_buffer_mode_off);
		set_pixel_shader_constant(1, NUMBEROF(fill_color), fill_color);
		set_indices(NULL);
		draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_line_list, line_count, points, sizeof(rasterizer_vertex_debug));
		set_cull_mode(_cull_mode_cw);
	}
}

void __cdecl c_rasterizer::draw_debug_line_list_explicit(const rasterizer_vertex_debug* points, int32 line_count)
{
	//INVOKE(0x00A45920, draw_debug_line_list_explicit, points, line_count);

	if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_shader_debug))
	{
		set_cull_mode(_cull_mode_off);
		set_z_buffer_mode(_z_buffer_mode_read);
		set_indices(NULL);
		draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_line_list, line_count, points, sizeof(rasterizer_vertex_debug));
		set_cull_mode(_cull_mode_cw);
	}
}

void __cdecl c_rasterizer::draw_debug_linestrip2d(const point2d* points, int32 point_count, uns32 color)
{
	INVOKE(0x00A45970, draw_debug_linestrip2d, points, point_count, color);
}

void __cdecl c_rasterizer::draw_debug_polygon2d(const rasterizer_vertex_debug* polygon2d, int32 primitive_count, c_rasterizer_index_buffer::e_primitive_type primitive_type)
{
	//INVOKE(0x00A45B40, draw_debug_polygon2d, polygon2d, primitive_count, primitive_type);

	if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_shader_debug2d))
	{
		set_cull_mode(_cull_mode_off);
		set_z_buffer_mode(_z_buffer_mode_off);
		set_indices(NULL);
		draw_primitive_up(primitive_type, primitive_count, polygon2d, sizeof(rasterizer_vertex_debug));
		set_cull_mode(_cull_mode_cw);
	}
}

void __cdecl c_rasterizer::draw_debug_polygon(const rasterizer_vertex_debug* vertices, int32 primitive_count, c_rasterizer_index_buffer::e_primitive_type primitive_type)
{
	//INVOKE(0x00A45B90, draw_debug_polygon, vertices, primitive_count, primitive_type);

	if (primitive_count > 0)
	{
		if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_shader_debug))
		{
			set_cull_mode(_cull_mode_off);
			set_z_buffer_mode(_z_buffer_mode_read);
			set_indices(NULL);
			draw_primitive_up(primitive_type, primitive_count, vertices, sizeof(rasterizer_vertex_debug));
			set_cull_mode(_cull_mode_cw);
		}
	}
}

void __cdecl c_rasterizer::draw_fullscreen_quad(int width, int height)
{
	//INVOKE(0x00A45EE0, draw_fullscreen_quad, width, height);

	real32 x = -1.0f / width;
	real32 y = 1.0f / height;

	rasterizer_vertex_screen triangle_fan[4]{};

	triangle_fan[0].position.x = x - 1.0f;
	triangle_fan[0].position.y = y + 1.0f;
	triangle_fan[0].texcoord.i = 0.0f;
	triangle_fan[0].texcoord.j = 0.0f;
	triangle_fan[0].color = 0xFFFFFFFF;

	triangle_fan[1].position.x = x + 1.0f;
	triangle_fan[1].position.y = y + 1.0f;
	triangle_fan[1].texcoord.i = 1.0f;
	triangle_fan[1].texcoord.j = 0.0f;
	triangle_fan[1].color = 0xFFFFFFFF;

	triangle_fan[2].position.x = x - 1.0f;
	triangle_fan[2].position.y = y - 1.0f;
	triangle_fan[2].texcoord.i = 0.0f;
	triangle_fan[2].texcoord.j = 1.0f;
	triangle_fan[2].color = 0xFFFFFFFF;

	triangle_fan[3].position.x = x + 1.0f;
	triangle_fan[3].position.y = y - 1.0f;
	triangle_fan[3].texcoord.i = 1.0f;
	triangle_fan[3].texcoord.j = 1.0f;
	triangle_fan[3].color = 0xFFFFFFFF;

	set_cull_mode(_cull_mode_off);
	set_indices(NULL);
	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_strip, 2, triangle_fan, sizeof(rasterizer_vertex_screen));
	set_cull_mode(_cull_mode_cw);
}

void __cdecl draw_tesselated_quad()
{
	//INVOKE(0x00A45FE0, draw_tesselated_quad);

	rasterizer_vertex_screen triangle_fan[4]{};

	triangle_fan[0].position.x = -1.0f;
	triangle_fan[0].position.y = 1.0f;
	triangle_fan[0].texcoord.i = 0.0f;
	triangle_fan[0].texcoord.j = 0.0f;
	triangle_fan[0].color = 0xFFFFFFFF;

	triangle_fan[1].position.x = 1.0f;
	triangle_fan[1].position.y = 1.0f;
	triangle_fan[1].texcoord.i = 1.0f;
	triangle_fan[1].texcoord.j = 0.0f;
	triangle_fan[1].color = 0xFFFFFFFF;

	triangle_fan[2].position.x = -1.0f;
	triangle_fan[2].position.y = -1.0f;
	triangle_fan[2].texcoord.i = 0.0f;
	triangle_fan[2].texcoord.j = 1.0f;
	triangle_fan[2].color = 0xFFFFFFFF;

	triangle_fan[3].position.x = 1.0f;
	triangle_fan[3].position.y = -1.0f;
	triangle_fan[3].texcoord.i = 1.0f;
	triangle_fan[3].texcoord.j = 1.0f;
	triangle_fan[3].color = 0xFFFFFFFF;

	c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_off);
	c_rasterizer::set_indices(NULL);
	c_rasterizer::draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_strip, 2, triangle_fan, sizeof(rasterizer_vertex_screen));
	c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_cw);
}

void __cdecl c_rasterizer::draw_fullscreen_quad_with_texture_xform(int width, int height, const real_rectangle2d* bounds)
{
	//INVOKE(0x00A460A0, draw_fullscreen_quad_with_texture_xform, width, height, bounds);

	real32 x = -1.0f / width;
	real32 y = 1.0f / height;

	rasterizer_vertex_screen vertices[4]{};

	vertices[0].position.x = x - 1.0f;
	vertices[0].position.y = y + 1.0f;
	vertices[0].texcoord.i = bounds->x0;
	vertices[0].texcoord.j = bounds->y0;
	vertices[0].color = 0xFFFFFFFF;

	vertices[1].position.x = x + 1.0f;
	vertices[1].position.y = y + 1.0f;
	vertices[1].texcoord.j = bounds->y0;
	vertices[1].texcoord.i = bounds->x1;
	vertices[1].color = 0xFFFFFFFF;

	vertices[2].position.x = x - 1.0f;
	vertices[2].position.y = y - 1.0f;
	vertices[2].texcoord.i = bounds->x0;
	vertices[2].texcoord.j = bounds->y1;
	vertices[2].color = 0xFFFFFFFF;

	vertices[3].position.x = x + 1.0f;
	vertices[3].position.y = y - 1.0f;
	vertices[3].texcoord.i = bounds->x1;
	vertices[3].texcoord.j = bounds->y1;
	vertices[3].color = 0xFFFFFFFF;

	set_cull_mode(_cull_mode_off);
	set_indices(NULL);
	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_strip, 2, vertices, sizeof(rasterizer_vertex_screen));
	set_cull_mode(_cull_mode_cw);
}

void __cdecl c_rasterizer::draw_screen_quad_with_texture_transform(int target_width, int target_height, const real_rectangle2d* dest_texcoords, const real_rectangle2d* source_texcoords)
{
	//INVOKE(0x00A461B0, c_rasterizer::draw_screen_quad_with_texture_transform, target_width, target_height, dest_texcoords, source_texcoords);

	real32 x = -1.0f / target_width;
	real32 y = 1.0f / target_height;

	real32 x0_offset = ((dest_texcoords->x0 * 2.0f) - 1.0f) + x;
	real32 x1_offset = ((dest_texcoords->x1 * 2.0f) - 1.0f) + x;
	real32 y0_offset = 1.0f - (dest_texcoords->y0 * 2.0f);
	real32 y1_offset = 1.0f - (dest_texcoords->y1 * 2.0f);

	rasterizer_vertex_screen vertices[4]{};

	vertices[0].position.x = x0_offset;
	vertices[0].position.y = y0_offset + y;
	vertices[0].texcoord.i = source_texcoords->x0;
	vertices[0].texcoord.j = source_texcoords->y0;
	vertices[0].color = 0xFFFFFFFF;

	vertices[1].position.x = x1_offset;
	vertices[1].position.y = y0_offset + y;
	vertices[1].texcoord.i = source_texcoords->x1;
	vertices[1].texcoord.j = source_texcoords->y0;
	vertices[1].color = 0xFFFFFFFF;

	vertices[2].position.x = x0_offset;
	vertices[2].position.y = y1_offset + y;
	vertices[2].texcoord.i = source_texcoords->x0;
	vertices[2].texcoord.j = source_texcoords->y1;
	vertices[2].color = 0xFFFFFFFF;

	vertices[3].position.x = x1_offset;
	vertices[3].position.y = y1_offset + y;
	vertices[3].texcoord.i = source_texcoords->x1;
	vertices[3].texcoord.j = source_texcoords->y1;
	vertices[3].color = 0xFFFFFFFF;

	set_cull_mode(_cull_mode_off);
	set_indices(NULL);
	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_strip, 2, vertices, sizeof(rasterizer_vertex_screen));
	set_cull_mode(_cull_mode_cw);
}

//.text:00A46300

void __cdecl c_rasterizer::draw_textured_screen_quad(real32 x0, real32 y0, real32 x1, real32 y1)
{
	//DECLFUNC(0x00A46520, void, __cdecl, real32, real32, real32, real32)(x0, y0, x1, y1);

	rasterizer_vertex_screen vertices[4]{};

	vertices[0].position.x = x0;
	vertices[0].position.y = y0;
	vertices[0].texcoord.i = 0.0f;
	vertices[0].texcoord.j = 1.0f;
	vertices[0].color = 0xFFFFFFFF;

	vertices[1].position.x = y1;
	vertices[1].position.y = y0;
	vertices[1].texcoord.i = 1.0f;
	vertices[1].texcoord.j = 1.0f;
	vertices[1].color = 0xFFFFFFFF;

	vertices[2].position.x = x0;
	vertices[2].position.y = y1;
	vertices[2].texcoord.i = 0.0f;
	vertices[2].texcoord.j = 0.0f;
	vertices[2].color = 0xFFFFFFFF;

	vertices[3].position.x = x1;
	vertices[3].position.y = y1;
	vertices[3].texcoord.i = 1.0f;
	vertices[3].texcoord.j = 0.0f;
	vertices[3].color = 0xFFFFFFFF;

	set_cull_mode(_cull_mode_off);
	set_indices(NULL);
	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_strip, 2, vertices, sizeof(rasterizer_vertex_screen));
	set_cull_mode(_cull_mode_cw);
}

void __cdecl c_rasterizer::draw_textured_screen_quad(const rasterizer_vertex_screen* vertices, bool strip)
{
	DECLFUNC(0x00A465F0, void, __cdecl, const rasterizer_vertex_screen*, bool)(vertices, strip);

	//c_rasterizer::set_cull_mode(_cull_mode_off);
	//c_rasterizer::set_z_buffer_mode(_z_buffer_mode_off);
	//c_vertex_declaration_table::set(_vertex_type_screen, _transfer_vertex_none, 0);
	//c_rasterizer::set_indices(0);
	//c_rasterizer::draw_primitive_up(c_rasterizer_index_buffer::e_primitive_type(strip + c_rasterizer_index_buffer::_primitive_type_triangle_fan), 2, vertices, sizeof(rasterizer_vertex_screen));
	//c_rasterizer::set_z_buffer_mode(_z_buffer_mode_read);
	//c_rasterizer::set_cull_mode(_cull_mode_cw);
}

void __cdecl c_rasterizer::draw_textured_screen_triangle_list(const rasterizer_vertex_screen* textured_screen_triangle_list, int32 primitive_count)
{
	//INVOKE(0x00A46640, draw_textured_screen_triangle_list, textured_screen_triangle_list, primitive_count);

	set_cull_mode(_cull_mode_off);
	set_z_buffer_mode(_z_buffer_mode_off);
	set_indices(NULL);
	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_list, primitive_count, textured_screen_triangle_list, sizeof(rasterizer_vertex_screen));
	set_cull_mode(_cull_mode_cw);
}

void __cdecl c_rasterizer::draw_textured_transparent_polygon(const rasterizer_vertex_transparent* textured_transparent_polygon, int32 polygon_count, e_alpha_blend_mode alpha_blend_mode)
{
	DECLFUNC(0x00A46680, void, __cdecl, const rasterizer_vertex_transparent*, int32, e_alpha_blend_mode)(textured_transparent_polygon, polygon_count, alpha_blend_mode);
}

void __cdecl c_rasterizer::draw_textured_transparent_quad(const rasterizer_vertex_transparent* textured_transparent_quad, e_alpha_blend_mode alpha_blend_mode)
{
	DECLFUNC(0x00A46750, void, __cdecl, const rasterizer_vertex_transparent*, e_alpha_blend_mode)(textured_transparent_quad, alpha_blend_mode);
}

void __cdecl c_rasterizer::draw_worldspace_polygon(const real_point3d* worldspace_polygon, int32 polygon_count)
{
	DECLFUNC(0x00A46820, void, __cdecl, const real_point3d*, int32)(worldspace_polygon, polygon_count);

	//rasterizer_vertex_world vertices[128]{};
	//
	//for (int32 i = 0; i < polygon_count; i++)
	//	vertices[i].position = *worldspace_polygon;
	//
	//draw_worldspace_polygon(vertices, polygon_count);
}

void __cdecl c_rasterizer::draw_worldspace_polygon(const rasterizer_vertex_world* worldspace_polygon, int32 polygon_count)
{
	//DECLFUNC(0x00A46890, void, __cdecl, const rasterizer_vertex_world*, int32)(worldspace_polygon, polygon_count);

	set_cull_mode(_cull_mode_off);
	set_indices(NULL);
	draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_triangle_fan, polygon_count - 2, worldspace_polygon, sizeof(rasterizer_vertex_world));
	set_cull_mode(_cull_mode_cw);
}

//.text:00A46910 ; public: static void __cdecl c_rasterizer::normalize_screenspace_vertices(real32, real32, rasterizer_vertex_screen*, int32)
//.text:00A469D0 ; void __cdecl rasterizer_draw_screen_facing_lens_flare(rasterizer_vertex_screen* const, int32, int32, real32, real32, real32)

void __cdecl rasterizer_psuedo_dynamic_screen_quad_draw(const rasterizer_dynamic_screen_geometry_parameters* parameters, rasterizer_vertex_screen* vertices)
{
	INVOKE(0x00A46AD0, rasterizer_psuedo_dynamic_screen_quad_draw, parameters, vertices);
}

void __cdecl rasterizer_quad_screenspace(point2d const(&points)[4], uns32 color, const s_tag_reference* reference, int16 bitmap_index, bool point_sampled)
{
	INVOKE(0x00A46DA0, rasterizer_quad_screenspace, points, color, reference, bitmap_index, point_sampled);

	//if (!reference)
	//{
	//	c_rasterizer_globals* rasterizer_globals = NULL;
	//	s_game_globals* game_globals = scenario_get_game_globals();
	//	if (game_globals->rasterizer_globals_ref.index == NONE)
	//	{
	//		rasterizer_globals = TAG_GET(RASTERIZER_GLOBALS_TAG, c_rasterizer_globals, game_globals->rasterizer_globals_ref.index);
	//	}
	//	reference = rasterizer_globals->get_default_texture_ref(0);
	//}
	//
	//c_rasterizer_texture_ref hardware_format;
	//if (bitmap_group_get_bitmap_hardware_format(
	//	reference->index,
	//	MIN(MAX(bitmap_index, 0),
	//	TAG_GET(BITMAP_TAG, bitmap_group, bitmap_index)->bitmaps.count - 1),
	//	&hardware_format))
	//{
	//	rasterizer_quad_screenspace_explicit(points, color, hardware_format, point_sampled, c_rasterizer::_alpha_blend_alpha_blend);
	//}
}

void __cdecl rasterizer_quad_screenspace_explicit(point2d const(&points)[4], uns32 color, c_rasterizer_texture_ref texture_ref, bool point_sampled, c_rasterizer::e_alpha_blend_mode blend_mode)
{
	//INVOKE(0x00A46E50, rasterizer_quad_screenspace_explicit, points, color, texture_ref, point_sampled, blend_mode);

	rasterizer_dynamic_screen_geometry_parameters parameters{};
	rasterizer_vertex_screen vertices[4]{};

	real_vector2d const texcoords[4]
	{
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f }
	};

	for (int32 vertex_index = 0; vertex_index < NUMBEROF(vertices); vertex_index++)
	{
		vertices[vertex_index].position.x = (real32)points[vertex_index].x;
		vertices[vertex_index].position.y = (real32)points[vertex_index].y;
		vertices[vertex_index].texcoord = texcoords[vertex_index];
		vertices[vertex_index].color = color;
	}

	real_vector2d aspect_ratio_scale = interface_get_aspect_ratio_scaling();
	interface_scale_screenspace_vertices_for_xenon_scaler(vertices, NUMBEROF(vertices), &aspect_ratio_scale);

	parameters.hardware_formats[0] = texture_ref;
	parameters.framebuffer_blend_function = blend_mode;
	parameters.point_sampled = point_sampled;
	parameters.map_texture_scale[0] = { 1.0f, 1.0f };
	parameters.map_scale[0] = { 1.0f, 1.0f };
	parameters.explicit_override_index = NONE;

	rasterizer_psuedo_dynamic_screen_quad_draw(&parameters, vertices);
}

bool __cdecl rasterizer_set_explicit_debug_shader(c_rasterizer_globals::e_explicit_shader shader_type)
{
	//return INVOKE(0x00A46FB0, rasterizer_set_explicit_debug_shader, shader_type);

	s_game_globals* game_globals = scenario_get_game_globals();
	if (!game_globals)
	{
		return false;
	}
	
	if (game_globals->rasterizer_globals_ref.index == NONE)
	{
		return false;
	}
	
	c_rasterizer_globals* rasterizer_globals = TAG_GET(RASTERIZER_GLOBALS_TAG, c_rasterizer_globals, game_globals->rasterizer_globals_ref.index);

	const s_tag_reference* default_vertex_shader_ref = rasterizer_globals->get_explicit_vertex_shader_ref(shader_type);
	const s_tag_reference* default_pixel_shader_ref = rasterizer_globals->get_explicit_pixel_shader_ref(shader_type);
	if (default_vertex_shader_ref->index == NONE || default_pixel_shader_ref->index == NONE)
	{
		return false;
	}
	
	c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_off);
	c_vertex_declaration_table::set(_vertex_type_debug, _transfer_vertex_none, _entry_point_default);
	
	c_rasterizer_vertex_shader* vertex_shader = TAG_GET(VERTEX_SHADER_TAG, c_rasterizer_vertex_shader, default_vertex_shader_ref->index);
	c_rasterizer::set_vertex_shader(vertex_shader, _vertex_type_debug, _transfer_vertex_none, _entry_point_default);
	
	c_rasterizer_pixel_shader* pixel_shader = TAG_GET(PIXEL_SHADER_TAG, c_rasterizer_pixel_shader, default_pixel_shader_ref->index);
	c_rasterizer::set_pixel_shader(pixel_shader, _entry_point_default);
	
	c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_cw);
	
	return true;
}

