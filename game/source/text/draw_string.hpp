#pragma once

#include "cseries/cseries.hpp"

enum e_font_id;
enum e_text_style;
enum e_text_justification;
enum e_text_drop_shadow_style;
enum e_utf32;
struct s_font_header;
struct s_font_character;

struct dynamic_screen_vertex;
struct s_widget_geometry;

// 0165FCB0
struct c_font_cache_base
{
	struct c_font_cache_base_vtbl* __vftable /*VFT*/;

public:
	c_font_cache_base();
};
static_assert(sizeof(c_font_cache_base) == 0x4);

struct c_font_cache_scope_lock
{
	volatile bool m_value;
};
static_assert(sizeof(c_font_cache_scope_lock) == 0x1);

// 0165FCCC
struct c_font_cache_mt_safe : // thread safe
	c_font_cache_base
{
public:
	c_font_cache_mt_safe();
	~c_font_cache_mt_safe();

protected:
	c_font_cache_scope_lock m_locked;
};
static_assert(sizeof(c_font_cache_mt_safe) == sizeof(c_font_cache_base) + 0x4);

// 0165FCE8
//c_null_font_cache

// 0165FC44
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

		k_text_flags
	};

	struct s_parse_string_state
	{
		byte __data[0x4C];
	};
	static_assert(sizeof(s_parse_string_state) == 0x4C);

	// HACK: so we don't have to manually construct the class
	struct
	{
		void(__thiscall* dtor)(c_draw_string*);
		bool(__thiscall* draw_utf32)(c_draw_string*, c_font_cache_base* font_cache, e_utf32 const* string);
		bool(__thiscall* draw_wchar)(c_draw_string*, c_font_cache_base* font_cache, wchar_t const* string);
		bool(__thiscall* draw)(c_draw_string*, c_font_cache_base* font_cache, char const* string);
		bool(__thiscall* predict_character)(c_draw_string*, c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character);
		bool(__thiscall* precache_character)(c_draw_string*, c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, s_font_character const** font_character);
		void(__thiscall* draw_character)(c_draw_string*, c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, dword a4, dword a5, real a6, real a7, real a8, real a9, real a10, real a11);
	}*__vftable;

public:
	c_draw_string();
	~c_draw_string();

	bool draw(c_font_cache_base* font_cache, e_utf32 const* string)
	{
		return __vftable->draw_utf32(this, font_cache, string);
	}
	bool draw(c_font_cache_base* font_cache, wchar_t const* string)
	{
		return __vftable->draw_wchar(this, font_cache, string);
	}
	bool draw(c_font_cache_base* font_cache, char const* string)
	{
		return __vftable->draw(this, font_cache, string);
	}
	bool predict_character(c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character)
	{
		return __vftable->predict_character(this, font_cache, font_id, character);
	}
	bool precache_character(c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, s_font_character const** font_character)
	{
		return __vftable->precache_character(this, font_cache, font_id, character, font_character);
	}
	void draw_character(c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, dword a4, dword a5, real a6, real a7, real a8, real a9, real a10, real a11)
	{
		return __vftable->draw_character(this, font_cache, font_id, character, a4, a5, a6, a7, a8, a9, a10, a11);
	}

	void __cdecl set_bounds(short_rectangle2d const* bounds);
	void __cdecl set_color(real_argb_color const* color);
	void __cdecl set_shadow_color(real_argb_color const* shadow_color);
	void __cdecl set_style(long style);
	void __cdecl set_tab_stops(short const* tab_stops, short count);
	void __cdecl set_wrap_horizontally(bool wrap_horizontally);
	void __cdecl text_bounds_draw_character(real a1, real a2, real a3, real a4);
	void __cdecl set_scale(real scale);
	void __cdecl set_font(long font_id);
	void __cdecl set_justification(long justification);
	bool __cdecl draw_more(c_font_cache_base* font_cache, char const* s);
	//draw_partial
	//get_cursor
	short __cdecl get_line_height() const;

protected:
	c_flags<e_text_flags, dword, k_text_flags> m_flags;

	// e_font_id
	long m_font_id;

	s_font_header const* m_font;

	// e_text_style
	long m_style;

	// e_text_justification
	long m_justification;

	// e_text_drop_shadow_style
	long m_drop_shadow_style;

	real_argb_color m_color;
	real_argb_color m_shadow_color;

	real m_scale;
	real m_display_resolution_scale_adjustment;

	short m_height_adjust;

	short m_tab_stop_count;
	short m_tab_stops[16];

	real_rectangle2d m_bounds[3];
	real_point2d m_cursor;

	bool(__cdecl* m_permutation_proc)(dynamic_screen_vertex*, void*);
	void* m_permutation_proc_data;

	short m_initial_indent;
	short m_paragraph_indent;

	c_draw_string::s_parse_string_state m_parse_string_state;

	word __unknownF8;
	word __unknownFA;
	word __unknownFC;
	word __unknownFE;
};
static_assert(sizeof(c_draw_string) == 0x100);

// 01692AD0
struct c_hardware_draw_string :
	c_draw_string
{
public:
	c_hardware_draw_string();

protected:
	c_font_cache_mt_safe m_font_cache;
	long __unknown108;
	long __unknown10C;
	long __unknown110;
};
static_assert(sizeof(c_hardware_draw_string) == sizeof(c_draw_string) + 0x14);

// 01697104
struct c_simple_font_draw_string :
	c_draw_string
{
	struct s_character_group_render_data
	{
		s_character_group_render_data();
		bool __cdecl is_full();
		void __cdecl reset();

	protected:
		long __unknown0;
		long __unknown4;
		long __unknown8;

		long count;
		char characters[256];
	};

public:
	c_simple_font_draw_string();

protected:
	s_character_group_render_data m_character_cache;
};
static_assert(sizeof(c_simple_font_draw_string) == sizeof(c_draw_string) + 0x110);

// 01692AF0
struct c_rasterizer_draw_string :
	c_draw_string
{
	struct s_glyph_group_render_data
	{
		long __unknown0;
		long __unknown4;
		long __unknown8;
		byte __dataC[0x700];
		long __unknown70C;
	};
	static_assert(sizeof(s_glyph_group_render_data) == 0x710);

public:
	c_rasterizer_draw_string();

protected:
	real_point2d m_rotation_origin;
	real m_rotation;
	real m_sine_rotation;
	real m_cosine_rotation;
	bool __unknown114;
	s_glyph_group_render_data m_render_data;
};
static_assert(sizeof(c_rasterizer_draw_string) == sizeof(c_draw_string) + 0x728);

// 01692AF0
struct c_chud_draw_string :
	c_draw_string
{
public:
	c_chud_draw_string();

protected:
	s_widget_geometry* m_geometry;
	real m_character_scale;
	real_rectangle2d m_pixel_clip;
};
static_assert(sizeof(c_chud_draw_string) == sizeof(c_draw_string) + 0x18);

extern real __cdecl draw_string_get_glyph_scaling_for_display_settings();

