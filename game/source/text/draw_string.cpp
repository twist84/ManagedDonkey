#include "text/draw_string.hpp"

#include "interface/interface_constants.hpp"
#include "math/color_math.hpp"
#include "text/font_group.hpp"
#include "text/font_loading.hpp"

bool c_draw_string::draw_more(c_font_cache_base* font_cache, const char* string)
{
	return INVOKE_CLASS_MEMBER(0x00657DD0, c_draw_string, draw_more, font_cache, string);
}

void c_draw_string::get_cursor(point2d* cursor) const
{
	//DECLFUNC(0x006583E0, void, __thiscall, const c_draw_string*, point2d*)(this, cursor);

	ASSERT(cursor != nullptr);
	cursor->x = (int16)m_cursor.x;
	cursor->y = (int16)m_cursor.y;
}

void c_draw_string::get_cursor(real_point2d* cursor) const
{
	//DECLFUNC(0x00658410, void, __thiscall, const c_draw_string*, real_point2d*)(this, cursor);

	ASSERT(cursor != nullptr);
	*cursor = m_cursor;
}

e_text_justification c_draw_string::get_justification() const
{
	//return INVOKE_CLASS_MEMBER(0x00658430, c_draw_string, get_justification);

	return m_justification;
}

int16 c_draw_string::get_line_height() const
{
	//return INVOKE_CLASS_MEMBER(0x00658440, c_draw_string, get_line_height);

	const s_font_header* header = font_get_header(m_font_id);
	real32 scale = m_display_resolution_scale_adjustment * m_scale;
	return (int16)((real32)font_get_line_height(header) * scale);
}

//.text:00658AA0 ; private: bool c_draw_string::recache_font_header()

void c_draw_string::set_align_bottom_vertically(bool align_bottom)
{
	m_flags.set(_text_flag_align_bottom_vertically_bit, align_bottom);
}

void c_draw_string::set_bounds(const real_rectangle2d* bounds, const real_rectangle2d* clip)
{
	DECLFUNC(0x00658B10, void, __thiscall, c_draw_string*, const real_rectangle2d*, const real_rectangle2d*)(this, bounds, clip);

	// $IMPLEMENT
}

void c_draw_string::set_bounds(const real_rectangle2d* bounds)
{
	//DECLFUNC(0x00658BC0, void, __thiscall, c_draw_string*, const real_rectangle2d*)(this, bounds);

	real32 x0 = 0.0f;
	real32 x1 = 0.0f;
	real32 y0 = 0.0f;
	real32 y1 = 0.0f;
	if (bounds)
	{
		if ((bounds->x1 - bounds->x0) >= 0.0f && (bounds->y1 - bounds->y0) >= 0.0f)
		{
			x0 = bounds->x0;
			x1 = bounds->x1;
			y0 = bounds->y0;
			y1 = bounds->y1;
		}
	}
	set_real_rectangle2d(&m_bounds, x0, x1, y0, y1);
}

void c_draw_string::set_bounds(const rectangle2d* bounds)
{
	//DECLFUNC(0x00658D20, void, __thiscall, c_draw_string*, const rectangle2d*)(this, bounds);

	real32 x0 = 0.0f;
	real32 x1 = 0.0f;
	real32 y0 = 0.0f;
	real32 y1 = 0.0f;
	if (bounds)
	{
		if (rectangle2d_width(bounds) >= 0 && rectangle2d_height(bounds) >= 0)
		{
			x0 = (real32)bounds->x0;
			x1 = (real32)bounds->x1;
			y0 = (real32)bounds->y0;
			y1 = (real32)bounds->y1;
		}
	}
	set_real_rectangle2d(&m_bounds, x0, x1, y0, y1);
}

void c_draw_string::set_center_vertically(bool center_vertically)
{
	//INVOKE_CLASS_MEMBER(0x00658DA0, c_draw_string, set_center_vertically, center_vertically);

	m_flags.set(_text_flag_center_vertically_bit, center_vertically);
}

void c_draw_string::set_color(uns32 color)
{
	real_argb_color real_color{};
	set_color(pixel32_to_real_argb_color({ .value = color }, &real_color));
}

void c_draw_string::set_color(argb_color color)
{
	real_argb_color real_color{};
	set_color(pixel32_to_real_argb_color(color, &real_color));
}

void c_draw_string::set_color(const real_argb_color* color)
{
	m_color = *color;
}

void c_draw_string::set_display_resolution_scale_adjustment(real32 scale)
{
	m_display_resolution_scale_adjustment = scale;
}

void c_draw_string::set_drop_shadow_style(e_text_drop_shadow_style drop_shadow_style)
{
	m_drop_shadow_style = drop_shadow_style;
}

void c_draw_string::set_font(e_font_id font)
{
	if (font == _font_id_fallback)
	{
		m_font_id = _font_id_fallback;
		m_styled_font_header = font_get_header(_font_id_fallback);
	}
	else
	{
		if (font < _terminal_font)
		{
			font = _terminal_font;
		}
		if (font > _main_menu_font)
		{
			font = _main_menu_font;
		}
		m_font_id = font;
		m_styled_font_header = font_get_header(font);
	}
}

void c_draw_string::set_height_adjust(int16 height_adjust)
{
	m_height_adjust = height_adjust;
}

void c_draw_string::set_justification(e_text_justification justification)
{
	m_justification = justification;
}

void c_draw_string::set_paragraph_indent(int16 indent)
{
	m_paragraph_indent = indent;
}

void c_draw_string::set_permutation_proc(bool(__cdecl* proc)(dynamic_screen_vertex*, void*), void* permutation_context)
{
	m_permutation_proc = proc;
	m_permutation_context = permutation_context;
}

void c_draw_string::set_precache_required(bool precache)
{
	m_flags.set(_text_flag_precache_required_bit, precache);
}

void c_draw_string::set_scale(real32 scale)
{
	m_scale = scale;
}

void c_draw_string::set_shadow_color(uns32 color)
{
	real_argb_color real_color{};
	set_shadow_color(pixel32_to_real_argb_color({ .value = color }, &real_color));
}

void c_draw_string::set_shadow_color(const real_argb_color* color)
{
	ASSERT(color != nullptr);

	real_argb_color use_color = *color;
	use_color.alpha = PIN(color->alpha, 0.0f, 1.0f);
	use_color.red = PIN(color->red, 0.0f, 1.0f);
	use_color.green = PIN(color->green, 0.0f, 1.0f);
	use_color.blue = PIN(color->blue, 0.0f, 1.0f);

	m_shadow_color = use_color;
}

void c_draw_string::set_style(e_text_style style)
{
	m_style = style;
}

void c_draw_string::set_tab_stops(const int16* tab_stops, int16 count)
{
	ASSERT((tab_stops != nullptr && count >= 0) || (tab_stops == nullptr && count == 0));

	if (tab_stops)
	{
		int16 tab_stop_count = count;
		if (count > NUMBEROF(m_tab_stops))
		{
			tab_stop_count = NUMBEROF(m_tab_stops);
		}

		m_tab_stop_count = tab_stop_count;
		if (tab_stop_count > 0)
		{
			csmemcpy(m_tab_stops, tab_stops, sizeof(*m_tab_stops) * tab_stop_count);
		}
	}
	else
	{
		m_tab_stop_count = 0;
	}
}

void c_draw_string::set_wrap_horizontally(bool wrap_horizontally)
{
	m_flags.set(_text_flag_wrap_horizontally_bit, wrap_horizontally);
}

void c_draw_string::text_bounds_draw_character(real32 screen_left, real32 screen_top, real32 bitmap_widthm, real32 bitmap_height)
{
	INVOKE_CLASS_MEMBER(0x00659340, c_draw_string, text_bounds_draw_character, screen_left, screen_top, bitmap_widthm, bitmap_height);
}

c_draw_string::c_draw_string() :
	__vftable(reinterpret_cast<decltype(__vftable)>(0x0165FC44)),
	m_flags(),
	m_font_id(_terminal_font),
	m_styled_font_header(font_get_header(_terminal_font)),
	m_style(_text_style_plain),
	m_justification(_text_justification_left),
	m_drop_shadow_style(_shadow_drop),
	m_color(*global_real_argb_white),
	m_shadow_color(*global_real_argb_black),
	m_scale(1.0f),
	m_display_resolution_scale_adjustment(draw_string_get_glyph_scaling_for_display_settings()),
	m_height_adjust(0),
	m_tab_stop_count(0),
	m_tab_stops(),
	m_bounds(),
	m_text_bounds(),
	m_clip(),
	m_cursor(),
	m_permutation_proc(nullptr),
	m_permutation_context(nullptr),
	m_initial_indent(0),
	m_paragraph_indent(0),
	m_saved_parse_state(),
	m_saved_tab_stop_index(0),
	m_saved_line_count(0),
	m_saved_tab_stop_line_count(0),
	m_saved_maximum_tab_stop_line_count(0)
{
	//DECLFUNC(0x00657010, void, __thiscall, c_draw_string*)(this);

	rectangle2d display_bounds{};
	rectangle2d display_title_safe_pixel_bounds{};
	interface_get_current_display_settings(nullptr, nullptr, &display_bounds, &display_title_safe_pixel_bounds);
	c_draw_string::set_bounds(&display_title_safe_pixel_bounds);
	csmemset(&m_saved_parse_state, 0, sizeof(m_saved_parse_state));
}

c_draw_string::~c_draw_string()
{
	//DECLFUNC(0x006571D0, void, __thiscall, c_draw_string*)(this);
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

bool c_simple_font_draw_string::s_character_group_render_data::is_full()
{
	return count == 255;
}

void c_simple_font_draw_string::s_character_group_render_data::reset()
{
	color = 0;
	screen_x = 0;
	screen_y = 0;
	count = 0;
}

c_rasterizer_draw_string::c_rasterizer_draw_string() :
	c_draw_string(),
	m_rotation_origin(),
	m_rotation(),
	m_sine_rotation(),
	m_cosine_rotation(),
	m_use_shader_system(),
	pad(),
	m_render_data()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(0x01692AF0);
	DECLFUNC(0x00A25F00, void, __thiscall, c_rasterizer_draw_string*)(this);
}

void c_rasterizer_draw_string::set_rotation(real32 angle_radians)
{
	INVOKE_CLASS_MEMBER(0x00A28160, c_rasterizer_draw_string, set_rotation, angle_radians);
}

void c_rasterizer_draw_string::set_rotation_origin(const real_point2d* origin)
{
	INVOKE_CLASS_MEMBER(0x00A28210, c_rasterizer_draw_string, set_rotation_origin, origin);

	//if (origin)
	//{
	//	m_rotation_origin = *origin;
	//}
	//else
	//{
	//	m_rotation_origin = {};
	//}
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

real32 __cdecl draw_string_get_glyph_scaling_for_display_settings()
{
	return INVOKE(0x006582E0, draw_string_get_glyph_scaling_for_display_settings);
}

