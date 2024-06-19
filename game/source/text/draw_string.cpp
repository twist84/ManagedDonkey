#include "text/draw_string.hpp"

#include "math/color_math.hpp"
#include "text/font_loading.hpp"

void __cdecl c_draw_string::set_bounds(real_rectangle2d const* bounds_a, real_rectangle2d const* bounds_b)
{
	DECLFUNC(0x00658B10, void, __thiscall, c_draw_string*, real_rectangle2d const*, real_rectangle2d const*)(this, bounds_a, bounds_b);
}

void __cdecl c_draw_string::set_bounds(real_rectangle2d const* bounds)
{
	DECLFUNC(0x00658BC0, void, __thiscall, c_draw_string*, real_rectangle2d const*)(this, bounds);
}

void __cdecl c_draw_string::set_bounds(short_rectangle2d const* bounds)
{
	DECLFUNC(0x00658D20, void, __thiscall, c_draw_string*, short_rectangle2d const*)(this, bounds);
}

void __cdecl c_draw_string::set_color(dword color)
{
	real_argb_color real_color{};
	set_color(pixel32_to_real_argb_color({ .value = color }, &real_color));
}

void __cdecl c_draw_string::set_color(argb_color color)
{
	real_argb_color real_color{};
	set_color(pixel32_to_real_argb_color(color, &real_color));
}

void __cdecl c_draw_string::set_color(real_argb_color const* color)
{
	m_color = *color;
}

void __cdecl c_draw_string::set_shadow_color(real_argb_color const* shadow_color)
{
	m_shadow_color = *shadow_color;
}

void __cdecl c_draw_string::set_style(long style)
{
	m_style = style;
}

void __cdecl c_draw_string::set_tab_stops(short const* tab_stops, short count)
{
	ASSERT((tab_stops != NULL && count >= 0) || (tab_stops == NULL && count == 0));

	if (tab_stops)
	{
		short tab_stop_count = count;
		if (count > NUMBEROF(m_tab_stops))
			tab_stop_count = NUMBEROF(m_tab_stops);

		m_tab_stop_count = tab_stop_count;
		if (tab_stop_count > 0)
			csmemcpy(m_tab_stops, tab_stops, sizeof(*m_tab_stops) * tab_stop_count);
	}
	else
	{
		m_tab_stop_count = 0;
	}
}

void __cdecl c_draw_string::set_wrap_horizontally(bool wrap_horizontally)
{
	m_flags.set(_text_flag_wrap_horizontally_bit, wrap_horizontally);
}

void __cdecl c_draw_string::text_bounds_draw_character(real a1, real a2, real a3, real a4)
{
	DECLFUNC(0x00659340, void, __thiscall, c_draw_string*, real, real, real, real)(this, a1, a2, a3, a4);
}

void __cdecl c_draw_string::set_scale(real scale)
{
	m_scale = scale;
}

void __cdecl c_draw_string::set_font(long font_id)
{
	if (font_id < 0)
		font_id = 0;

	if (font_id > 10)
		font_id = 10;

	m_font_id = font_id;
	m_font = font_get_header(font_id);
}

void __cdecl c_draw_string::set_justification(long justification)
{
	m_justification = justification;
}

bool __cdecl c_draw_string::draw_more(c_font_cache_base* font_cache, char const* s)
{
	return DECLFUNC(0x00657DD0, bool, __thiscall, c_draw_string*, c_font_cache_base*, char const*)(this, font_cache, s);
}

void __cdecl c_draw_string::get_cursor(int16_point2d* cursor) const
{
	return DECLFUNC(0x006583E0, void, __thiscall, c_draw_string const*, int16_point2d*)(this, cursor);
}

//.text:00658410 ; public: void c_draw_string::get_cursor(real_point2d*) const
//.text:00658430 ; public: e_text_justification c_draw_string::get_justification() const

short __cdecl c_draw_string::get_line_height() const
{
	return DECLFUNC(0x00658440, short, __thiscall, c_draw_string const*)(this);
}

c_draw_string::c_draw_string() :
	__vftable(reinterpret_cast<decltype(__vftable)>(0x0165DB98)),
	m_flags(),
	m_font_id(),
	m_font(),
	m_style(),
	m_justification(),
	m_drop_shadow_style(),
	m_color(),
	m_shadow_color(),
	m_scale(),
	m_display_resolution_scale_adjustment(),
	m_height_adjust(),
	m_tab_stop_count(),
	m_tab_stops(),
	m_bounds(),
	m_cursor(),
	m_permutation_proc(),
	m_permutation_proc_data(),
	m_initial_indent(),
	m_paragraph_indent(),
	m_parse_string_state(),
	__unknownF8(),
	__unknownFA(),
	__unknownFC(),
	__unknownFE()
{
	DECLFUNC(0x00657010, void, __thiscall, c_draw_string*)(this);
}

c_draw_string::~c_draw_string()
{
	DECLFUNC(0x006571D0, void, __thiscall, c_draw_string*)(this);
}

c_hardware_draw_string::c_hardware_draw_string() : 
	c_draw_string(),
	m_font_cache(),
	__unknown108(),
	__unknown10C(),
	__unknown110()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(0x01692AD0);
}

c_simple_font_draw_string::c_simple_font_draw_string() :
	c_draw_string(),
	m_character_cache()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(0x01697104);
	DECLFUNC(0x00A76540, void, __thiscall, c_simple_font_draw_string*)(this);
}

c_simple_font_draw_string::s_character_group_render_data::s_character_group_render_data()
{
	reset();
	csmemset(characters, 0, sizeof(characters));
}

bool __cdecl c_simple_font_draw_string::s_character_group_render_data::is_full()
{
	return count == 255;
}

void __cdecl c_simple_font_draw_string::s_character_group_render_data::reset()
{
	__unknown0 = 0;
	__unknown4 = 0;
	__unknown8 = 0;
	count = 0;
}

c_rasterizer_draw_string::c_rasterizer_draw_string() :
	c_draw_string(),
	m_rotation_origin(),
	m_rotation(),
	m_sine_rotation(),
	m_cosine_rotation(),
	__unknown114(),
	m_render_data()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(0x01692AF0);
	DECLFUNC(0x00A25F00, void, __thiscall, c_rasterizer_draw_string*)(this);
}

c_chud_draw_string::c_chud_draw_string() :
	c_draw_string(),
	m_geometry(),
	m_character_scale(),
	m_pixel_clip()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(0x0169A144);
	DECLFUNC(0x00A95570, void, __thiscall, c_chud_draw_string*)(this);
}

real __cdecl draw_string_get_glyph_scaling_for_display_settings()
{
	return INVOKE(0x006582E0, draw_string_get_glyph_scaling_for_display_settings);
}

