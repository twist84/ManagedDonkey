#pragma once

#include "cseries/cseries.hpp"
#include "interface/user_interface_text.hpp"
#include "text/font_cache.hpp"

struct s_font_header;
struct s_font_character;

struct dynamic_screen_vertex;
struct s_widget_geometry;

// 0165FC44
class c_draw_string
{
public:
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
		int32 string_index;
		const e_utf32* string;
		int32 result;
		int32 previous_result;
		utf32 character;
		utf32 previous_character;
		e_font_id current_font;
		e_font_id previous_font;
		real_argb_color base_color;
		real_argb_color modifier_color;
		e_text_justification current_justification;
		e_text_justification previous_justification;
		bool can_line_break_before_current_character;
		bool pad[0x3];
	};
	COMPILE_ASSERT(sizeof(s_parse_string_state) == 0x4C);

	// HACK: so we don't have to manually construct the class
	struct
	{
		void(__thiscall* dtor)(c_draw_string* _this);
		bool(__thiscall* draw_utf32)(c_draw_string* _this, c_font_cache_base* font_cache, const e_utf32* string);
		bool(__thiscall* draw_wchar)(c_draw_string* _this, c_font_cache_base* font_cache, const wchar_t* string);
		bool(__thiscall* draw)(c_draw_string* _this, c_font_cache_base* font_cache, const char* string);
		bool(__thiscall* predict_character)(c_draw_string* _this, c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character);
		bool(__thiscall* precache_character)(c_draw_string* _this, c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, const s_font_character** font_character);
		void(__thiscall* draw_character)(c_draw_string* _this, c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, uns32 a4, uns32 a5, real32 a6, real32 a7, real32 a8, real32 a9, real32 a10, real32 a11);
	}*__vftable;

public:
	c_draw_string();
	~c_draw_string();

	bool draw(c_font_cache_base* font_cache, const e_utf32* string)
	{
		return __vftable->draw_utf32(this, font_cache, string);
	}
	bool draw(c_font_cache_base* font_cache, const wchar_t* string)
	{
		return __vftable->draw_wchar(this, font_cache, string);
	}
	bool draw(c_font_cache_base* font_cache, const char* string)
	{
		return __vftable->draw(this, font_cache, string);
	}
	bool predict_character(c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character)
	{
		return __vftable->predict_character(this, font_cache, font_id, character);
	}
	bool precache_character(c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, const s_font_character** font_character)
	{
		return __vftable->precache_character(this, font_cache, font_id, character, font_character);
	}
	void draw_character(c_font_cache_base* font_cache, e_font_id font_id, e_utf32 character, uns32 a4, uns32 a5, real32 a6, real32 a7, real32 a8, real32 a9, real32 a10, real32 a11)
	{
		return __vftable->draw_character(this, font_cache, font_id, character, a4, a5, a6, a7, a8, a9, a10, a11);
	}

	bool draw_more(c_font_cache_base* font_cache, const char* string);
	//draw_partial

	void get_cursor(point2d* cursor) const;
	void get_cursor(real_point2d* cursor) const;
	e_text_justification get_justification() const;
	int16 get_line_height() const;

	void set_align_bottom_vertically(bool align_bottom);
	void set_bounds(const real_rectangle2d* bounds, const real_rectangle2d* clip);
	void set_bounds(const real_rectangle2d* bounds);
	void set_bounds(const rectangle2d* bounds);
	void set_center_vertically(bool center_vertically);
	void set_color(uns32 color);
	void set_color(argb_color color);
	void set_color(const real_argb_color* color);
	void set_display_resolution_scale_adjustment(real32 scale);
	void set_drop_shadow_style(e_text_drop_shadow_style drop_shadow_style);
	void set_font(e_font_id font);
	void set_height_adjust(int16 height_adjust);
	void set_justification(e_text_justification justification);
	void set_paragraph_indent(int16 indent);
	void set_permutation_proc(bool(__cdecl* proc)(dynamic_screen_vertex*, void*), void* permutation_context);
	void set_precache_required(bool precache);
	void set_scale(real32 scale);
	void set_shadow_color(uns32 color);
	void set_shadow_color(const real_argb_color* color);
	void set_style(e_text_style style);
	void set_tab_stops(const int16* tab_stops, int16 count);
	void set_wrap_horizontally(bool wrap_horizontally);
	void text_bounds_draw_character(real32 screen_left, real32 screen_top, real32 bitmap_widthm, real32 bitmap_height);

protected:
	c_flags<e_text_flags, uns32, k_text_flags> m_flags;
	e_font_id m_font_id;
	const s_font_header* m_styled_font_header;
	e_text_style m_style;
	e_text_justification m_justification;
	e_text_drop_shadow_style m_drop_shadow_style;
	real_argb_color m_color;
	real_argb_color m_shadow_color;
	real32 m_scale;
	real32 m_display_resolution_scale_adjustment;
	int16 m_height_adjust;
	int16 m_tab_stop_count;
	int16 m_tab_stops[16];
	real_rectangle2d m_bounds;
	real_rectangle2d m_text_bounds;
	real_rectangle2d m_clip;
	real_point2d m_cursor;
	bool(__cdecl* m_permutation_proc)(dynamic_screen_vertex*, void*);
	void* m_permutation_context;
	int16 m_initial_indent;
	int16 m_paragraph_indent;
	c_draw_string::s_parse_string_state m_saved_parse_state;
	int16 m_saved_tab_stop_index;
	int16 m_saved_line_count;
	int16 m_saved_tab_stop_line_count;
	int16 m_saved_maximum_tab_stop_line_count;
};
COMPILE_ASSERT(sizeof(c_draw_string) == 0x100);

// 01692AD0
class c_hardware_draw_string :
	public c_draw_string
{
public:
	c_hardware_draw_string();

protected:
	c_font_cache_mt_safe m_font_cache;
	int32 __unknown108;
	int32 __unknown10C;
	int32 __unknown110;
};
COMPILE_ASSERT(sizeof(c_hardware_draw_string) == sizeof(c_draw_string) + 0x14);

// 01697104
class c_simple_font_draw_string :
	public c_draw_string
{
public:
	struct s_character_group_render_data
	{
		s_character_group_render_data();
		bool is_full();
		void reset();

	protected:
		uns32 color;
		int32 screen_x;
		int32 screen_y;
		int32 count;
		char characters[256];
	};

public:
	c_simple_font_draw_string();

protected:
	s_character_group_render_data m_character_cache;
};
COMPILE_ASSERT(sizeof(c_simple_font_draw_string) == sizeof(c_draw_string) + 0x110);

// 01692AF0
class c_rasterizer_draw_string :
	public c_draw_string
{
public:
	struct s_glyph_group_render_data
	{
		struct s_glyph_render_data
		{
			e_utf32 character;
			real32 screen_left;
			real32 screen_top;
			real32 bitmap_xoffset;
			real32 bitmap_yoffset;
			real32 bitmap_width;
			real32 bitmap_height;
		};
		COMPILE_ASSERT(sizeof(s_glyph_render_data) == 0x1C);

		e_font_id font_id;
		uns32 color;
		uns32 shadow_color;
		s_glyph_render_data glyphs[64];
		int32 count;
	};
	COMPILE_ASSERT(sizeof(s_glyph_group_render_data) == 0x710);

public:
	c_rasterizer_draw_string();
	void set_rotation(real32 angle_radians);
	void set_rotation_origin(const real_point2d* origin);

protected:
	real_point2d m_rotation_origin;
	real32 m_rotation;
	real32 m_sine_rotation;
	real32 m_cosine_rotation;
	bool m_use_shader_system;
	bool pad[0x3];
	s_glyph_group_render_data m_render_data;
};
COMPILE_ASSERT(sizeof(c_rasterizer_draw_string) == sizeof(c_draw_string) + 0x728);

// 01692AF0
class c_chud_draw_string :
	public c_draw_string
{
public:
	c_chud_draw_string();

protected:
	s_widget_geometry* m_geometry;
	real32 m_character_scale;
	real_rectangle2d m_pixel_clip;
};
COMPILE_ASSERT(sizeof(c_chud_draw_string) == sizeof(c_draw_string) + 0x18);

extern real32 __cdecl draw_string_get_glyph_scaling_for_display_settings();

