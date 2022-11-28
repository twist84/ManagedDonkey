#pragma once

#include "cseries/cseries.hpp"

enum e_font_id;
enum e_text_style;
enum e_text_justification;
enum e_text_drop_shadow_style;
enum e_utf32;
struct c_font_cache_base;
struct s_font_header;
struct s_font_character;
struct c_draw_string
{
	enum e_text_flags
	{
		_text_flag_wrap_horizontally_bit = 0,
		_text_flag_wrap_vertically_bit,
		_text_flag_center_vertically_bit,
		_text_flag_align_bottom_vertically_bit,
		_text_flag_draw_utf32_bit,
		_text_flag_precache_required_bit,
		_text_flag_draw_more_bit,

		k_text_flag_count
	};

	struct s_parse_string_state
	{
		byte __data[0x4C];
	};
	static_assert(sizeof(s_parse_string_state) == 0x4C);

public:
	virtual ~c_draw_string();
	virtual bool draw(c_font_cache_base* font_cache, e_utf32 const* string);
	virtual bool draw(c_font_cache_base* font_cache, wchar_t const* string);
	virtual bool draw(c_font_cache_base* font_cache, char const* string);
	virtual bool predict_character(c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character);
	virtual bool precache_character(c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, s_font_character const** font_character);
	virtual void draw_character(c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, dword a4, dword a5, real a6, real a7, real a8, real a9, real a10, real a11);

	void __cdecl set_color(real_argb_color const* color);
	void __cdecl set_shadow_color(real_argb_color const* shadow_color);
	void __cdecl set_scale(real scale);
	bool __cdecl draw_more(c_font_cache_base* font_cache, char const* s);

protected:
	c_flags<e_text_flags, dword, k_text_flag_count> m_flags;

	// e_font_id
	long m_font_id;

	s_font_header* m_font;

	// e_text_style
	long m_style;

	// e_text_justification
	long m_justification;

	// e_text_drop_shadow_style
	long m_drop_shadow_style;

	real_argb_color m_color;
	real_argb_color m_shadow_color;
	real_bounds m_scale;

	short __unknown44;

	short m_tab_stop_count;
	short m_tab_stops[16];
	real_rectangle2d m_bounds[3];
	real_point2d m_cursor;

	real __unknownA0;
	real __unknownA4;
	byte __unknownA8[4];

	c_draw_string::s_parse_string_state m_parse_string_state;

	word __unknownF8[4];
};
static_assert(sizeof(c_draw_string) == 0x100);
