#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

struct c_font_cache_base;
struct c_user_interface_text
{
public:
	virtual ~c_user_interface_text();
	virtual void set_string(wchar_t const* _string, bool parse_xml, long);
	virtual wchar_t const* get_string();
	virtual void update(long);

	void set_flags(dword_flags flags)
	{
		m_flags = flags;
	}

	void set_unknown8(long unknown8)
	{
		__unknown8 = unknown8;
	}

	void set_font(long font)
	{
		m_font = font;
	}

	void set_argb_color(argb_color color)
	{
		m_argb_color = color;
	}

	void set_drop_shadow_style(long drop_shadow_style)
	{
		m_drop_shadow_style = drop_shadow_style;
	}

	void set_style(long style)
	{
		m_style = style;
	}

	void set_justification(long justification)
	{
		m_justification = justification;
	}

	void set_rotation_origin(real x, real y)
	{
		m_rotation_origin.x = x;
		m_rotation_origin.y = y;
	}

	void set_rotation(real rotation)
	{
		m_rotation = rotation;
	}

	void set_scroll_amount(real i, real j)
	{
		m_scroll_amount.i = i;
		m_scroll_amount.j = j;
	}

	void set_scale(real scale)
	{
		m_scale = scale;
	}

protected:
	// 1:  render_uppercase
	// 2:  align_vertically
	// 4:  wrap_horizontally
	// 8:  has_overflowed
	// 16: ?
	// 32: get_string_was_set
	dword_flags m_flags;

	long __unknown8;
	long m_font;
	argb_color m_argb_color;
	long m_drop_shadow_style;
	long m_style;
	long m_justification;
	short m_tab_stops[16];
	long m_tab_stop_count;
	real_point2d m_rotation_origin;
	real m_rotation;
	vector2d m_scroll_amount;
	real m_scale;
};
static_assert(sizeof(c_user_interface_text) == 0x5C);

extern void wchar_string_sanitize_for_game(wchar_t* string, long maximum_character_count);

enum e_controller_index;
enum e_utf32;

extern void __cdecl parse_build_number_string(e_controller_index controller_index, e_utf32 utf32, c_static_wchar_string<1024>* out_string);
