#include "render/simple_font.hpp"

#include "bitmaps/bitmap_group_tag_definition.hpp"
#include "bitmaps/bitmaps.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"

#include <string.h>

c_simple_font_screen_display::c_simple_font_screen_display() :
	m_rendering(false),
	m_x(),            // not initialized
	m_y(),            // not initialized
	m_width(),        // not initialized
	m_height(),       // not initialized
	m_column_width(), // not initialized
	m_row_height(),   // not initialized
	m_max_column(),   // not initialized
	m_max_row(),      // not initialized
	m_string()        // not initialized
{
}

c_simple_font_screen_display::~c_simple_font_screen_display()
{
	close_session();
}

void c_simple_font_screen_display::close_session()
{
	INVOKE_CLASS_MEMBER(0x00A767F0, c_simple_font_screen_display, close_session);
}

void c_simple_font_screen_display::draw(long a1, long a2, dword a3, char const* format, ...)
{
	//return INVOKE_CLASS_MEMBER(0x00A76960, c_simple_font_screen_display, draw, a1, a2, a3, format);

	va_list list;
	va_start(list, format);

	cvsnzprintf(m_string, sizeof(m_string) - 1, format, list);
	simple_font::print(
		m_x + a1 * m_column_width,
		m_y + a2 * m_row_height,
		a3,
		m_string,
		strlen(m_string),
		false);
}

bool c_simple_font_screen_display::open_session(real scale)
{
	//return INVOKE_CLASS_MEMBER(0x00A76FA0, c_simple_font_screen_display, open_session, scale);

	if (m_rendering = simple_font::begin_rendering(scale, true))
	{
		short_rectangle2d bounds{};
		c_rasterizer::get_fullscreen_render_title_safe_pixel_bounds(&bounds);
		m_x = bounds.x0;
		m_y = bounds.y0;
		m_width = bounds.x1 - bounds.x0;
		m_height = bounds.y1 - bounds.y0;
		m_column_width = simple_font::get_width();
		m_row_height = simple_font::get_height();
		m_max_column = m_width / m_column_width;
		m_max_row = m_height / m_row_height;
		csmemset(m_string, 0, sizeof(m_string));

		return m_rendering;
	}

	return false;
}

namespace simple_font
{
	HOOK_DECLARE_CLASS_MEMBER(0x00A76B50, s_font_data, install);
	HOOK_DECLARE(0x00A76C60, install);

	REFERENCE_DECLARE_ARRAY(0x0191BFD0, s_font_data*, g_fonts, 2);
	REFERENCE_DECLARE(0x0191C000, s_font_data, g_font_6x10);
	REFERENCE_DECLARE(0x0191C03C, s_font_data, g_font_4x6);
	REFERENCE_DECLARE(0x0524B6B4, s_simple_font_globals, g_simple_font_globals);
	REFERENCE_DECLARE(0x0524B6B8, s_font_data*, g_activeFont);
	REFERENCE_DECLARE(0x0524B6BC, bool, __unknown8);
	REFERENCE_DECLARE(0x0524B6C0, vector2d, __vectorC);
	REFERENCE_DECLARE(0x0524B6C8, vector2d, __vector14);

	bool __cdecl begin_rendering(real scale, bool a2)
	{
		return INVOKE(0x00A76690, simple_font::begin_rendering, scale, a2);

		//if (simple_font::g_activeFont->installed)
		//{
		//	bool rendering = c_rasterizer_draw_string::begin_rendering();
		//	c_rasterizer::set_sampler_texture(0, simple_font::g_activeFont->texture_ref);
		//
		//	real v4 = 0.1f;
		//	if (scale <= 0.1f || (v4 = 10.0f, scale >= 10.0f))
		//		scale = v4;
		//
		//	simple_font::g_activeFont->scale = scale;
		//	simple_font::g_simple_font_globals.__unknown0 = a2;
		//	return rendering;
		//}
		//
		//return false;
	}

	//.text:00A769F0 ; void __cdecl simple_font::draw_quads(rasterizer_vertex_screen*, long, bool)

	void __cdecl end_rendering()
	{
		INVOKE(0x00A76AE0, simple_font::end_rendering);

		//if (simple_font::g_activeFont->installed)
		//	c_rasterizer_draw_string::end_rendering();
	}

	long __cdecl get_height()
	{
		return INVOKE(0x00A76AF0, simple_font::get_height);

		//return long(real(simple_font::g_activeFont->font_character_height) * simple_font::g_activeFont->scale);
	}

	long __cdecl get_width()
	{
		return INVOKE(0x00A76B30, simple_font::get_width);

		//return long(real(simple_font::g_activeFont->font_character_width) * simple_font::g_activeFont->scale);
	}

	void s_font_data::install()
	{
		//INVOKE_CLASS_MEMBER(0x00A76B50, simple_font::s_font_data, install);

		if (!installed && c_rasterizer::g_device)
		{
			if (texture_bitmap = bitmap_2d_new((short)texture_width, (short)texture_height, 0, _bitmap_format_a8y8, 0))
			{
				c_rasterizer_texture_ref internal_hardware_format;

				// c_rasterizer_texture_ref::allocate?
				DECLFUNC(0x00A6DFF0, void, __cdecl, c_rasterizer_texture_ref&, bitmap_data*, char const*, bool)(
					internal_hardware_format, texture_bitmap, "simple_font", true);

				texture_ref = internal_hardware_format;
				if (internal_hardware_format.valid())
				{
					texture_bitmap->internal_hardware_format = internal_hardware_format;
					texture_bitmap->flags.set(_bitmap_flag_bit8, true);

					__unknown2C = texture_width;

					short_rectangle2d rect{};
					rect.x1 = (short)font_width;
					rect.y1 = (short)font_height;

					long buffer_offset = 0;
					for (short y = 0; y < rect.y1; y++)
					{
						for (short x = 0; x < rect.x1; x++)
						{
							char font_char = font_buffer[buffer_offset++];
							byte* char_address = (byte*)bitmap_2d_address(texture_bitmap, x, y, 0);
							byte char_value = (font_char != '#') - 1;
							char_address[0] = char_value;
							char_address[1] = char_value;
						}
					}

					rasterizer_bitmap_2d_changed_rect(texture_bitmap, texture_ref, &rect, true);
				}
			}
		}

		installed = true;
	}

	void __cdecl install()
	{
		//INVOKE(0x00A76C60, simple_font::install);

		for (s_font_data* font_data : g_fonts)
			font_data->install();

		g_simple_font_globals.__unknown0 = false;
	}

	//void __cdecl render_text(s_font_data* a1, long a2, long a3, long a4, long a5, dword a6, char const* string, unsigned int string_length, bool a9)
	//{
	//	INVOKE(0x00A77480, render_text, a1, a2, a3, a4, a5, a6, string, string_length, a9);
	//}

	void __cdecl print(long a1, long a2, dword a3, char const* string, long string_length, bool a6)
	{
		INVOKE(0x00A770E0, print, a1, a2, a3, string, string_length, a6);

		//render_text(g_activeFont, a1, a2, 1024, 0, a3, string, string_length, a6);
	}
}
