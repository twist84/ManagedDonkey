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
	virtual void update(c_font_cache_base* font_cache, long);

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
	word m_tab_stops[16];
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
