#include "interface/user_interface_text.hpp"

#include "config/version.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "text/draw_string.hpp"

#include <string.h>

REFERENCE_DECLARE_ARRAY(0x01942C48, s_parse_text_entry, g_parse_text_table, 131);
REFERENCE_DECLARE(0x052697B1, bool, use_keyboard_hints) = true; // Press <E> to pick up
REFERENCE_DECLARE(0x052697B8, s_last_known_session_state, g_last_known_session_state);

HOOK_DECLARE(0x00ABC070, parse_build_number_string);

bool g_render_text_as_font_index = false;

void wchar_string_sanitize_for_game(wchar_t* string, int32 maximum_character_count)
{
	ASSERT(string != NULL);

	wchar_t v1[3][2]
	{
		{ '<', '[' },
		{ '>', ']' },
		{ '|', '-' }
	};

	bool v2 = 0;
	do
	{
		for (int32 i = 0; i < maximum_character_count && string[i]; ++i)
		{
			for (int32 j = 0; j < 3; ++j)
			{
				if (string[i] == v1[j][0])
				{
					string[i] = v1[j][1];
					v2 = 1;
					break;
				}
			}
		}
	} while (v2);
}

void utf32_to_string(e_utf32 utf32, wchar_t(&out_string)[2])
{
	union
	{
		e_utf32 value;
		wchar_t str[2];
	} out = { .value = utf32 };

	ustrnzcpy(out_string, out.str, 2);
}

bool __cdecl parse_xml_ui_screen_party_privacy(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00AA4B50, parse_xml_ui_screen_party_privacy, this_ptr, buffer, buffer_length);
}

//.text:00ABCE90 ; public: c_user_interface_text::c_user_interface_text()
//.text:00ABCF40 ; public: virtual c_user_interface_text::~c_user_interface_text()
//.text:00ABCF70 ; public: virtual void* c_user_interface_text::`scalar deleting destructor'(unsigned int)
//.text:00ABCFA0 ; protected: bool c_user_interface_text::compute_bounds_internal(c_font_cache_base*, int32, const rectangle2d*, real32, const rectangle2d*, int16, rectangle2d*, rectangle2d*)
//.text:00ABD170 ; public: void c_user_interface_text::compute_caret_bounds(c_font_cache_base*, int32, const rectangle2d*, real32, const rectangle2d*, int16, rectangle2d*)
//.text:00ABD1B0 ; public: bool c_user_interface_text::compute_text_bounds(c_font_cache_base*, int32, const rectangle2d*, real32, const rectangle2d*, rectangle2d*)
//.text:00ABD200 ; 
//.text:00ABD210 ; public: bool c_user_interface_text::get_align_vertically() const
//.text:00ABD220 ; public: real_argb_color c_user_interface_text::get_argb_color() const
//.text:00ABD240 ; 
//.text:00ABD3A0 ; 
//.text:00ABD3D0 ; public: e_text_drop_shadow_style c_user_interface_text::get_drop_shadow_style() const
//.text:00ABD3E0 ; public: e_text_justification c_user_interface_text::get_justification() const
//.text:00ABD3F0 ; public: bool c_user_interface_text::get_render_uppercase() const
//.text:00ABD400 ; public: real64 c_user_interface_text::get_rotation() const
//.text:00ABD410 ; public: const real_point2d* c_user_interface_text::get_rotation_origin() const
//.text:00ABD420 ; public: real64 c_user_interface_text::get_scale() const
//.text:00ABD430 ; public: e_text_style c_user_interface_text::get_style() const
//.text:00ABD440 ; public: void c_user_interface_text::get_tab_stops(int16*, int16*)
//.text:00ABD480 ; 
//.text:00ABD490 ; public: bool c_user_interface_text::get_wrap_horizontally() const
//.text:00ABD4A0 ; public: void c_user_interface_text::initialize(const wchar_t*, int16, e_font_id, const real_rgb_color*, int32, e_text_style, e_text_justification, e_controller_index)
//.text:00ABD510 ; 

void __cdecl c_user_interface_text::render(s_user_interface_text_render_data* render_data, const rectangle2d* window_bounds)
{
	//INVOKE(0x00ABD530, c_user_interface_text::render, render_data, window_bounds);

	ASSERT(render_data != NULL);

	wchar_t* render_data_text = ((s_gui_text_widget_extra_large_render_data*)render_data)->text;
	if (g_render_text_as_font_index)
	{
		// making this static to fix stack corruption
		static wchar_t string_with_font_prepended[1024]{};

		usnzprintf(string_with_font_prepended, sizeof(string_with_font_prepended), L"(%d):%s", render_data->font, render_data_text);
		render_data_text = string_with_font_prepended;
	}

	c_rasterizer_draw_string draw_string;
	c_font_cache_mt_safe font_cache;

	real_rectangle2d bounds_rect = render_data->bounds_rect;
	real_rectangle2d clip_rect = render_data->clip_rect;

	offset_real_rectangle2d(&bounds_rect, (real32)window_bounds->x0, (real32)window_bounds->y0);
	offset_real_rectangle2d(&clip_rect, (real32)window_bounds->x0, (real32)window_bounds->y0);

	draw_string.set_font(render_data->font);
	draw_string.set_style(render_data->style);
	draw_string.set_justification(render_data->justification);
	draw_string.set_drop_shadow_style(render_data->drop_shadow_style);
	draw_string.set_color(render_data->color);
	draw_string.set_shadow_color(render_data->shadow_color);
	draw_string.set_wrap_horizontally(render_data->wrap_horizontally);
	draw_string.set_align_bottom_vertically(render_data->align_vertically);
	draw_string.set_tab_stops(render_data->tab_stops, render_data->tab_stop_count);
	draw_string.set_rotation_origin(&render_data->rotation_origin);
	draw_string.set_rotation(render_data->rotation_angle_radians);
	draw_string.set_scale(render_data->glyph_scale);
	draw_string.set_bounds(&bounds_rect, &clip_rect);
	draw_string.set_precache_required(true);
	draw_string.draw(&font_cache, render_data_text);
}

//.text:00ABD750 ; public: void c_user_interface_text::render_halox(int32, const real_rectangle2d*, const real_rectangle2d*, real32, real32, const rectangle2d*)

void c_user_interface_text::set_argb_color(real_argb_color* color)
{
	//INVOKE_CLASS_MEMBER(0x00ABDA90, c_user_interface_text, set_argb_color, color);

	m_argb_color.value = real_argb_color_to_pixel32(color);
}

void c_user_interface_text::set_font(e_font_id font)
{
	//INVOKE_CLASS_MEMBER(0x00ABDAB0, c_user_interface_text, set_font, font);

	m_font = font;
}

void c_user_interface_text::set_justification(e_text_justification justification)
{
	//INVOKE_CLASS_MEMBER(0x00ABDAC0, c_user_interface_text, set_justification, justification);

	m_justification = justification;
}

void c_user_interface_text::set_style(e_text_style new_style)
{
	//INVOKE_CLASS_MEMBER(0x00ABDAD0, c_user_interface_text, set_style, new_style);

	m_text_style = new_style;
}

//.text:00ABDAE0 ; 
//.text:00ABDB00 ; bool __cdecl string_contains_special_characters(const wchar_t*)
//.text:00ABDB60 ; 
//.text:00ABDBA0 ; void __cdecl user_interface_parse_interface_string(e_controller_index, c_static_wchar_string<1024>*)
//.text:00ABDBF0 ; bool __cdecl user_interface_parse_string(int32, wchar_t*, int32)
//.text:00ABDD90 ; 
//.text:00ABDDA0 ; 
//.text:00ABDDE0 ; 
//.text:00ABDE40 ; bool __cdecl wchar_parse_private_use_characters(wchar_t*)
//.text:00ABE000 ; 
//.text:00ABE030 ; 

// this hook never gets hit
void __cdecl parse_build_number_string(int32 user_index, e_utf32 character, c_static_wchar_string<1024>* text)
{
	const char* build_name = version_get_build_name();
	const char* build_string = version_get_build_string();

	if (strlen_debug(build_name) > 1)
	{
		text->append_print(L"%hs (%hs)", build_name, build_string);
	}
	else
	{
		text->append_print(L"%hs", build_string);
	}
}

bool __cdecl parse_xml_lobby_coop_max_players(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B226F0, parse_xml_lobby_coop_max_players, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_countdown_remaining(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B22710, parse_xml_lobby_countdown_remaining, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_current_players(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B22750, parse_xml_lobby_current_players, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_delaying_player(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B22790, parse_xml_lobby_delaying_player, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_film_max_players(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B227E0, parse_xml_lobby_film_max_players, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_film_party_leader_requirement(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B22800, parse_xml_lobby_film_party_leader_requirement, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_header(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B22850, parse_xml_lobby_header, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_max_players(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B228C0, parse_xml_lobby_max_players, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_network(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B22910, parse_xml_lobby_network, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_party_leader(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B22930, parse_xml_lobby_party_leader, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_percent_loaded(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B22980, parse_xml_lobby_percent_loaded, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_start_button_name(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B22A10, parse_xml_lobby_start_button_name, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_title(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B22A80, parse_xml_lobby_title, this_ptr, buffer, buffer_length);
}

void c_user_interface_text::set_flags(uns32 flags)
{
	m_flags = flags;
}

void c_user_interface_text::set_controller_index(e_controller_index controller_index)
{
	m_controller_index = controller_index;
}

void c_user_interface_text::set_drop_shadow_style(e_text_drop_shadow_style drop_shadow_style)
{
	m_drop_shadow_style = drop_shadow_style;
}

void c_user_interface_text::set_rotation_origin(real32 x, real32 y)
{
	m_rotation_origin.x = x;
	m_rotation_origin.y = y;
}

void c_user_interface_text::set_rotation(real32 rotation)
{
	m_rotation = rotation;
}

void c_user_interface_text::set_scroll_amount(real32 i, real32 j)
{
	m_scroll_amount.i = i;
	m_scroll_amount.j = j;
}

void c_user_interface_text::set_scale(real32 scale)
{
	m_scale = scale;
}

void user_interface_text_debug_display_font_index(bool display_font_index_in_place_of_text)
{
	g_render_text_as_font_index = display_font_index_in_place_of_text;
}

