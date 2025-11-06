#include "render/simple_font.hpp"

#include "bitmaps/bitmap_group_tag_definition.hpp"
#include "bitmaps/bitmaps.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"

#include <string.h>

c_simple_font_screen_display::c_simple_font_screen_display() :
	m_print_session_open(false),
	m_base_x(),
	m_base_y(),
	m_total_width(),
	m_total_height(),
	m_char_width(),
	m_char_height(),
	m_max_column(),
	m_max_row(),
	buffer()
{
}

c_simple_font_screen_display::~c_simple_font_screen_display()
{
	close_session();
}

void c_simple_font_screen_display::close_session()
{
	INVOKE_CLASS_MEMBER(0x00A767F0, c_simple_font_screen_display, close_session);

	if (m_print_session_open)
	{
		simple_font::end_rendering();
		m_print_session_open = false;
	}
}

void c_simple_font_screen_display::draw(int32 column, int32 row, uns32 color, const char* format, ...)
{
	//return INVOKE_CLASS_MEMBER(0x00A76960, c_simple_font_screen_display, draw, column, row, color, format);

	va_list va;
	va_start(va, format);

	cvsnzprintf(buffer, sizeof(buffer) - 1, format, va);
	simple_font::print(
		m_base_x + column * m_char_width,
		m_base_y + row * m_char_height,
		color,
		buffer,
		strlen_debug(buffer),
		false);
}

bool c_simple_font_screen_display::open_session(real32 scale)
{
	//return INVOKE_CLASS_MEMBER(0x00A76FA0, c_simple_font_screen_display, open_session, scale);

	if (m_print_session_open = simple_font::begin_rendering(scale, true))
	{
		rectangle2d display_bounds{};
		c_rasterizer::get_fullscreen_render_title_safe_pixel_bounds(&display_bounds);
		m_base_x = display_bounds.x0;
		m_base_y = display_bounds.y0;
		m_total_width = rectangle2d_width(&display_bounds);
		m_total_height = rectangle2d_height(&display_bounds);
		m_char_width = simple_font::get_width();
		m_char_height = simple_font::get_height();
		m_max_column = m_total_width / m_char_width;
		m_max_row = m_total_height / m_char_height;
		csmemset(buffer, 0, sizeof(buffer));

		return m_print_session_open;
	}

	return false;
}

namespace simple_font
{
	HOOK_DECLARE_CLASS_MEMBER(0x00A76B50, s_font_data, install);
	HOOK_DECLARE(0x00A76C60, install);
	HOOK_DECLARE_CLASS_MEMBER(0x00A77390, s_font_data, remove);
	HOOK_DECLARE(0x00A773C0, remove);

	REFERENCE_DECLARE_ARRAY(0x0191BFD0, s_font_data*, g_fonts, 2);
	REFERENCE_DECLARE(0x0191C000, s_font_data, g_font_6x10);
	REFERENCE_DECLARE(0x0191C03C, s_font_data, g_font_4x6);
	REFERENCE_DECLARE(0x0524B6B4, s_simple_font_globals, g_simple_font_globals);
	REFERENCE_DECLARE(0x0524B6B8, s_font_data*, g_activeFont);

	// static variables from `simple_font::draw_quads`
	REFERENCE_DECLARE(0x0524B6BC, bool, x_initialized);
	REFERENCE_DECLARE(0x0524B6C0, real32, x_one_over_window_bounds_width);
	REFERENCE_DECLARE(0x0524B6C4, real32, x_one_over_window_bounds_height);
	REFERENCE_DECLARE(0x0524B6C8, real_vector2d, x_aspect_ratio_scale);
}

bool __cdecl simple_font::begin_rendering(real32 scale, bool drop_shadow)
{
	return INVOKE(0x00A76690, simple_font::begin_rendering, scale, drop_shadow);

	//if (simple_font::g_activeFont->installed)
	//{
	//	bool rendering = c_rasterizer_draw_string::begin_rendering();
	//	c_rasterizer::set_sampler_texture(0, simple_font::g_activeFont->hardware_format);
	//	simple_font::g_activeFont->char_scale = PIN(scale, 0.1f, 10.0f);
	//	simple_font::g_simple_font_globals.render_drop_shadow = drop_shadow;
	//	return rendering;
	//}
	//
	//return false;
}

void __cdecl simple_font::draw_quads(rasterizer_vertex_screen* triangle_vertices, int32 verts_used, bool apply_display_scalar_correction)
{
	INVOKE(0x00A769F0, simple_font::draw_quads, triangle_vertices, verts_used, apply_display_scalar_correction);
}

void __cdecl simple_font::end_rendering()
{
	INVOKE(0x00A76AE0, simple_font::end_rendering);

	//if (simple_font::g_activeFont->installed)
	//	c_rasterizer_draw_string::end_rendering();
}

int32 __cdecl simple_font::get_height()
{
	return INVOKE(0x00A76AF0, simple_font::get_height);

	//return int32(real32(simple_font::g_activeFont->char_height) * simple_font::g_activeFont->char_scale);
}

int32 __cdecl simple_font::get_width()
{
	return INVOKE(0x00A76B30, simple_font::get_width);

	//return int32(real32(simple_font::g_activeFont->char_width) * simple_font::g_activeFont->char_scale);
}

void simple_font::s_font_data::install()
{
	//INVOKE_CLASS_MEMBER(0x00A76B50, simple_font::s_font_data, install);

	if (!installed && c_rasterizer::g_device) // c_rasterizer::get_device()
	{
		if (bitmap = bitmap_2d_new((int16)texture_width, (int16)texture_height, 0, _bitmap_format_a8y8, 0))
		{
			bitmap->internal_hardware_format = hardware_format = c_rasterizer_texture_ref::allocate(hardware_format, bitmap, "simple_font", true);
			bitmap->flags.set(_bitmap_hardware_format_is_tracked_externally_bit, true);

			pitch = texture_width;

			int32 source_offset = 0;
			for (int32 y = 0; y < source_height; y++)
			{
				for (int32 x = 0; x < source_width; x++)
				{
					char source_char = source_data[source_offset++];
					char* dest_char = (char*)bitmap_2d_address(bitmap, (int16)x, (int16)y, 0);

					char char_value = (source_char != '#') - 1;
					dest_char[0] = char_value;
					dest_char[1] = char_value;
				}
			}

			rectangle2d rect{};
			rect.x1 = (int16)source_width;
			rect.y1 = (int16)source_height;
			rasterizer_bitmap_2d_changed_rect(bitmap, hardware_format, &rect, true);
		}
	}

	installed = true;
}

void __cdecl simple_font::install()
{
	//INVOKE(0x00A76C60, simple_font::install);

	for (s_font_data* font_data : g_fonts)
		font_data->install();

	g_simple_font_globals.render_drop_shadow = false;
}

//.text:00A76DB0 ; bool __cdecl c_simple_font_draw_string::s_character_group_render_data::is_full() const

int32 __cdecl simple_font::make_quad(int32 x1, int32 y1, real32 u1, real32 v1, int32 x2, int32 y2, real32 u2, real32 v2, uns32 color, int32 shadow_offset, rasterizer_vertex_screen* triangle_vertices)
{
	return INVOKE(0x00A76DB0, simple_font::make_quad, x1, y1, u1, v1, x2, y2, u2, v2, color, shadow_offset, triangle_vertices);
}

void __cdecl simple_font::print(int32 x, int32 y, uns32 color, const char* c, int32 count, bool apply_display_scalar_correction)
{
	//INVOKE(0x00A770E0, print, x, y, color, c, count, apply_display_scalar_correction);

	simple_font::render_text(g_activeFont, x, y, 1024, 0, color, c, count, apply_display_scalar_correction);
}

void __cdecl simple_font::print_block(int32 x, int32 y, int32 width, int32 height, uns32 color, const char* c)
{
	INVOKE(0x00A77110, print_block, x, y, width, height, color, c);
}

void __cdecl simple_font::print_white(int32 x, int32 y, const char* c, int32 count)
{
	INVOKE(0x00A77140, print_white, x, y, c, count);
}

//.text:00A77160
void __cdecl simple_font::printf(int32 x, int32 y, uns32 color, const char* c, ...)
{
	va_list va;
	va_start(va, c);
	simple_font::vprintf(x, y, 3, color, c, va);
	va_end(va);
}

//.text:00A771D0
void __cdecl simple_font::printf(int32 x, int32 y, const char* c, ...)
{
	va_list va;
	va_start(va, c);
	simple_font::vprintf(x, y, 3, 0xFFFFFFFF, c, va);
	va_end(va);
}

//.text:00A77240
void __cdecl simple_font::printf_down(int32 x, int32 y, uns32 color, const char* c, ...)
{
	va_list va;
	va_start(va, c);
	simple_font::vprintf(x, y, 11, color, c, va);
	va_end(va);
}

//.text:00A772B0
void __cdecl simple_font::printf_left(int32 x, int32 y, uns32 color, const char* c, ...)
{
	va_list va;
	va_start(va, c);
	simple_font::vprintf(x, y, 19, color, c, va);
	va_end(va);
}

//.text:00A77320
void __cdecl simple_font::printf_up(int32 x, int32 y, uns32 color, const char* c, ...)
{
	va_list va;
	va_start(va, c);
	simple_font::vprintf(x, y, 27, color, c, va);
	va_end(va);
}

void simple_font::s_font_data::remove()
{
	//INVOKE_CLASS_MEMBER(0x00A77390, simple_font::s_font_data, remove);

	if (installed)
	{
		bitmap_delete(bitmap);
		c_rasterizer_texture_ref::release(hardware_format);
		bitmap = NULL;
		installed = false;
	}
}

void __cdecl simple_font::remove()
{
	//INVOKE(0x00A773C0, simple_font::remove);

	for (s_font_data* font_data : g_fonts)
		font_data->remove();
}

void __cdecl simple_font::render_text(const s_font_data* font, int32 pixelX, int32 pixelY, int32 pixelPitch, int32 flags, uns32 color, const char* text, unsigned int charCount, bool apply_display_scalar_correction)
{
	INVOKE(0x00A77480, render_text, font, pixelX, pixelY, pixelPitch, flags, color, text, charCount, apply_display_scalar_correction);
}

void __cdecl simple_font::vprintf(int32 x, int32 y, int32 flags, uns32 color, const char* c, char* arglist)
{
	//INVOKE(0x00A779B0, simple_font::vprintf, x, y, flags, color, c, arglist);

	char debugString[256]{};
	csnzprintf(debugString, sizeof(debugString), c, arglist);
	simple_font::render_text(simple_font::g_activeFont, x, y, 1024, flags, color, debugString, strlen_debug(debugString), false);
}

/*
simple_font::s_font_data simple_font::g_font_6x10 =
{
	.installed = false,

	.char_width = 6,
	.char_height = 10,
	.char_pitch = 16,
	.char_start = 0x20,
	.char_end = 0x7F,
	.char_scale = 1.0f,

	.texture_width = 128,
	.texture_height = 64,
	.bitmap = NULL,
	.hardware_format = c_rasterizer_texture_ref(),
	.pitch = 0,

	.source_width = 96,
	.source_height = 64,
	.source_data =
		"......" "......" ".#.#.." "......" "......" "......" "......" "..#..." "...#.." ".#...." "......" "......" "......" "......" "......" "....#."
		"......" "..#..." ".#.#.." "......" "..#..." "......" ".##..." "..#..." "..#..." "..#..." "......" "......" "......" "......" "......" "....#."
		"......" "..#..." ".#.#.." ".#.#.." ".####." ".#...#" "#..#.." "..#..." ".#...." "...#.." "..#..." "..#..." "......" "......" "......" "...#.."
		"......" "..#..." "......" "#####." "#.#..." "#.#.#." "#..#.." "......" ".#...." "...#.." "#.#.#." "..#..." "......" "......" "......" "...#.."
		"......" "..#..." "......" ".#.#.." ".###.." ".#.#.." ".##..." "......" ".#...." "...#.." ".###.." "#####." "......" "#####." "......" "..#..."
		"......" "..#..." "......" ".#.#.." "..#.#." "..#.#." "#..#.#" "......" ".#...." "...#.." "#.#.#." "..#..." "......" "......" "......" "..#..."
		"......" "......" "......" "#####." "####.." ".#.#.#" "#...#." "......" ".#...." "...#.." "..#..." "..#..." "......" "......" "......" ".#...."
		"......" "..#..." "......" ".#.#.." "..#..." "#...#." ".###.#" "......" "..#..." "..#..." "......" "......" "..#..." "......" "..#..." ".#...."
		"......" "......" "......" "......" "......" "......" "......" "......" "...#.." ".#...." "......" "......" ".#...." "......" "......" "#....."
		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"

		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
		".###.." "..#..." ".###.." ".###.." "...#.." "#####." "..##.." "#####." ".###.." ".###.." "......" "......" "......" "......" "......" ".###.."
		"#...#." ".##..." "#...#." "#...#." "..##.." "#....." ".#...." "....#." "#...#." "#...#." "......" "......" "....#." "......" "#....." "#...#."
		"#...#." "..#..." "....#." "....#." ".#.#.." "#....." "#....." "...#.." "#...#." "#...#." "..#..." "..#..." "..##.." "#####." ".##..." "....#."
		"#.#.#." "..#..." "..##.." "..##.." "#..#.." "####.." "####.." "...#.." ".###.." ".####." "......" "......" "##...." "......" "...##." "...#.."
		"#...#." "..#..." ".#...." "....#." "#####." "....#." "#...#." "..#..." "#...#." "....#." "......" "......" "..##.." "#####." ".##..." "..#..."
		"#...#." "..#..." "#....." "#...#." "...#.." "#...#." "#...#." "..#..." "#...#." "...#.." "..#..." "..#..." "....#." "......" "#....." "......"
		".###.." ".###.." "#####." ".###.." "...#.." ".###.." ".###.." ".#...." ".###.." ".##..." "......" ".#...." "......" "......" "......" "..#..."
		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"

		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
		".####." "..#..." "####.." ".###.." "###..." "#####." "#####." ".###.." "#...#." ".###.." "..###." "#...#." "#....." "#...#." "##..#." ".###.."
		"#....#" "..#..." "#...#." "#...#." "#..#.." "#....." "#....." "#...#." "#...#." "..#..." "....#." "#..#.." "#....." "#...#." "##..#." "#...#."
		"#.##.#" ".#.#.." "#...#." "#....." "#...#." "#....." "#....." "#....." "#...#." "..#..." "....#." "#.#..." "#....." "##.##." "#.#.#." "#...#."
		"#.##.#" ".#.#.." "####.." "#....." "#...#." "####.." "####.." "#..##." "#####." "..#..." "....#." "##...." "#....." "##.##." "#.#.#." "#...#."
		"#.####" ".###.." "#...#." "#....." "#...#." "#....." "#....." "#...#." "#...#." "..#..." "....#." "#.#..." "#....." "#.#.#." "#.#.#." "#...#."
		"#....." "#...#." "#...#." "#...#." "#..#.." "#....." "#....." "#...#." "#...#." "..#..." "....#." "#..#.." "#....." "#.#.#." "#..##." "#...#."
		".#####" "#...#." "####.." ".###.." "###..." "#####." "#....." ".####." "#...#." ".###.." "####.." "#...#." "#####." "#...#." "#..##." ".###.."
		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"

		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" ".###.." "#....." ".###.." "......" "......"
		"####.." ".###.." "####.." ".###.." "#####." "#...#." "#...#." "#...#." "#...#." "#...#." "#####." ".#...." "#....." "...#.." "..#..." "......"
		"#...#." "#...#." "#...#." "#...#." "..#..." "#...#." "#...#." "#.#.#." "#...#." "#...#." "....#." ".#...." ".#...." "...#.." ".#.#.." "......"
		"#...#." "#...#." "#...#." "#....." "..#..." "#...#." "#...#." "#.#.#." ".#.#.." "#...#." "...#.." ".#...." ".#...." "...#.." "#...#." "......"
		"####.." "#...#." "####.." ".###.." "..#..." "#...#." ".#.#.." "#.#.#." "..#..." ".###.." "..#..." ".#...." "..#..." "...#.." "......" "......"
		"#....." "#...#." "#...#." "....#." "..#..." "#...#." ".#.#.." ".#.#.." ".#.#.." "..#..." ".#...." ".#...." "..#..." "...#.." "......" "......"
		"#....." "#..##." "#...#." "#...#." "..#..." "#...#." "..#..." ".#.#.." "#...#." "..#..." "#....." ".#...." "...#.." "...#.." "......" "......"
		"#....." ".####." "#...#." ".###.." "..#..." ".###.." "..#..." ".#.#.." "#...#." "..#..." "#####." ".#...." "...#.." "...#.." "......" "......"
		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" ".###.." "....#." ".###.." "......" "######"
		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"

		"......" "......" "#....." "......" "....#." "......" "..###." "......" "#....." "......" "......" ".#...." ".##..." "......" "......" "......"
		"..#..." "......" "#....." "......" "....#." "......" ".#...." "......" "#....." "..#..." "...#.." ".#...." "..#..." "......" "......" "......"
		"...#.." "......" "#....." "......" "....#." "......" ".#...." "......" "#....." "......" "......" ".#...." "..#..." "......" "......" "......"
		"......" ".###.." "####.." ".####." ".####." ".###.." "####.." ".###.." "####.." ".##..." "..##.." ".#..#." "..#..." "##.#.." "####.." ".###.."
		"......" "....#." "#...#." "#....." "#...#." "#...#." ".#...." "#...#." "#...#." "..#..." "...#.." ".#.#.." "..#..." "#.#.#." "#...#." "#...#."
		"......" ".####." "#...#." "#....." "#...#." "#####." ".#...." "#...#." "#...#." "..#..." "...#.." ".##..." "..#..." "#.#.#." "#...#." "#...#."
		"......" "#...#." "#...#." "#....." "#...#." "#....." ".#...." "#...#." "#...#." "..#..." "...#.." ".#.#.." "..#..." "#.#.#." "#...#." "#...#."
		"......" ".####." "####.." ".####." ".####." ".####." ".#...." ".####." "#...#." "..#..." "...#.." ".#..#." "..#..." "#.#.#." "#...#." ".###.."
		"......" "......" "......" "......" "......" "......" "......" "....#." "......" "......" "...#.." "......" "......" "......" "......" "......"
		"......" "......" "......" "......" "......" "......" "......" ".###.." "......" "......" "###..." "......" "......" "......" "......" "......"

		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "...#.." "..#..." ".#...." "......" "......"
		"......" "......" "......" "......" ".#...." "......" "......" "......" "......" "......" "......" "..#..." "..#..." "..#..." "......" "......"
		"......" "......" "......" "......" ".#...." "......" "......" "......" "......" "......" "......" "..#..." "..#..." "..#..." "......" "......"
		"####.." ".####." "#.##.." ".####." ".###.." "#...#." "#...#." "#...#." "#...#." "#...#." "#####." "..#..." "..#..." "..#..." "......" "......"
		"#...#." "#...#." "##..#." "#....." ".#...." "#...#." "#...#." "#.#.#." ".#.#.." "#...#." "...#.." ".#...." "..#..." "...#.." ".##..#" "......"
		"#...#." "#...#." "#....." ".###.." ".#...." "#...#." ".#.#.." "#.#.#." "..#..." "#...#." "..#..." "..#..." "..#..." "..#..." "#..##." "......"
		"#...#." "#...#." "#....." "....#." ".#...." "#...#." ".#.#.." ".#.#.." ".#.#.." "#...#." ".#...." "..#..." "..#..." "..#..." "......" "......"
		"####.." ".####." "#....." "####.." "..###." ".####." "..#..." ".#.#.." "#...#." ".####." "#####." "..#..." "..#..." "..#..." "......" "......"
		"#....." "....#." "......" "......" "......" "......" "......" "......" "......" "....#." "......" "...#.." "..#..." ".#...." "......" "......"
		"#....." "....#." "......" "......" "......" "......" "......" "......" "......" ".###.." "......" "......" "......" "......" "......" "......"

		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
		"......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......" "......"
};

simple_font::s_font_data simple_font::g_font_4x6 =
{
	.installed = false,

	.char_width = 4,
	.char_height = 6,
	.char_pitch = 16,
	.char_start = 0x20,
	.char_end = 0x7F,
	.char_scale = 1.0f,

	.texture_width = 128,
	.texture_height = 64,
	.bitmap = NULL,
	.hardware_format = c_rasterizer_texture_ref(),
	.pitch = 0,

	.source_width = 64,
	.source_height = 36,
	.source_data =
		"...." ".#.." "#.#." "#.#." ".#.." "#..." ".#.." "##.." ".#.." "#..." "#.#." "...." "...." "...." "...." "..#."
		"...." ".#.." "#.#." "###." ".##." "..#." "#.#." "#..." "#..." ".#.." ".#.." ".#.." "...." "...." "...." "..#."
		"...." ".#.." "...." "#.#." "##.." ".#.." ".##." "...." "#..." ".#.." "###." "###." "...." "###." "...." ".#.."
		"...." "...." "...." "###." ".##." "#..." "#.#." "...." "#..." ".#.." ".#.." ".#.." "...." "...." "...." "#..."
		"...." ".#.." "...." "#.#." "##.." "..#." "###." "...." ".#.." "#..." "#.#." "...." "##.." "...." ".#.." "#..."
		"...." "...." "...." "...." ".#.." "...." "...." "...." "...." "...." "...." "...." "#..." "...." "...." "...."

		".##." ".#.." "##.." "##.." "..#." "###." ".#.." "###." ".#.." ".#.." "...." "...." "..#." "...." "#..." "##.."
		"#.#." "##.." "..#." "..#." "#.#." "#..." "#..." "..#." "#.#." "#.#." "...." "...." ".#.." "...." ".#.." "..#."
		"#.#." ".#.." ".#.." ".#.." "###." "##.." "##.." ".##." ".#.." ".##." ".#.." ".#.." "#..." "###." "..#." ".#.."
		"#.#." ".#.." "#..." "..#." "..#." "..#." "#.#." ".#.." "#.#." "..#." "...." "...." ".#.." "...." ".#.." "...."
		"##.." "###." "###." "##.." "..#." "##.." ".#.." ".#.." ".#.." ".#.." ".#.." "##.." "..#." "###." "#..." ".#.."
		"...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "#..." "...." "...." "...." "...."

		"###." ".#.." "##.." ".##." "##.." "###." "###." ".##." "#.#." "###." "..#." "#.#." "#..." "#.#." "#.#." ".#.."
		"#.#." "#.#." "#.#." "#..." "#.#." "#..." "#..." "#..." "#.#." ".#.." "..#." "#.#." "#..." "###." "###." "#.#."
		"#.#." "###." "##.." "#..." "#.#." "##.." "##.." "#.#." "###." ".#.." "..#." "##.." "#..." "###." "#.#." "#.#."
		"#..." "#.#." "#.#." "#..." "#.#." "#..." "#..." "#.#." "#.#." ".#.." "#.#." "#.#." "#..." "#.#." "#.#." "#.#."
		"###." "#.#." "##.." ".##." "##.." "###." "#..." ".##." "#.#." "###." ".#.." "#.#." "###." "#.#." "#.#." ".#.."
		"...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...."

		"##.." ".#.." "##.." ".##." "###." "#.#." "#.#." "#.#." "#.#." "#.#." "###." "##.." "#..." "##.." ".#.." "...." // ".#.." "...." possible mistake with ^ and _
		"#.#." "#.#." "#.#." "#..." ".#.." "#.#." "#.#." "#.#." "#.#." "#.#." "..#." "#..." "#..." ".#.." "#.#." "...." // "#.#." "...." 
		"##.." "#.#." "##.." "###." ".#.." "#.#." "#.#." "###." ".#.." ".#.." ".#.." "#..." ".#.." ".#.." "...." "...." // "...." "...." 
		"#..." "###." "#.#." "..#." ".#.." "#.#." "#.#." "###." "#.#." ".#.." "#..." "#..." "..#." ".#.." "...." "...." // "...." "...." 
		"#..." ".##." "#.#." "##.." ".#.." "###." ".#.." "#.#." "#.#." ".#.." "###." "##.." "..#." "##.." "...." "...." // "...." "...." 
		"...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...#" "###." // "...." "####" 

		"##.." "...." "#..." "...." "..#." "...." "..#." "...." "#..." ".#.." ".#.." "#..." ".#.." "...." "...." "...."
		".#.." "...." "#..." "...." "..#." "...." ".#.." "...." "#..." "...." "...." "#..." ".#.." "...." "...." "...."
		"...." ".##." "##.." ".##." ".##." "###." "###." "###." "##.." ".#.." ".#.." "#.#." ".#.." "###." "##.." ".#.."
		"...." "#.#." "#.#." "#..." "#.#." "##.." ".#.." "#.#." "#.#." ".#.." ".#.." "##.." ".#.." "###." "#.#." "#.#."
		"...." "###." "##.." ".##." ".##." ".##." ".#.." "..#." "#.#." ".#.." ".#.." "#.#." ".#.." "#.#." "#.#." ".#.."
		"...." "...." "...." "...." "...." "...." "...." "###." "...." "...." "##.." "...." "...." "...." "...." "...."

		"...." "...." "...." "...." "...." "...." "...." "...." "...." "...." "...." ".##." ".#.." "##.." "#.#." "...." // "##.." ".#.#" possible mistake with } and ~
		"...." "...." "...." "...." ".#.." "...." "...." "...." "...." "...." "...." ".#.." ".#.." ".#.#" ".#.." "...." // ".#.." "#.#." 
		"##.." ".##." "##.." ".##." "###." "#.#." "#.#." "#.#." "#.#." "#.#." "##.." "##.." ".#.." ".##." "...." "...." // ".##." "...." 
		"#.#." "#.#." "#..." ".#.." ".#.." "#.#." "#.#." "###." ".#.." "#.#." ".#.." ".#.." ".#.." ".#.." "...." "...." // ".#.." "...." 
		"##.." ".##." "#..." "##.." ".##." "###." ".#.." "###." "#.#." ".#.." ".##." ".##." ".#.." "##.." "...." "...." // "##.." "...." 
		"#..." "..#." "...." "...." "...." "...." "...." "...." "...." "#..." "...." "...." "...." "...." "...." "...." // "...." "...." 
};
*/

