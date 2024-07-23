#include "interface/user_interface_text.hpp"

#include "config/version.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"

#include <string.h>

REFERENCE_DECLARE_ARRAY(0x01942C48, s_parse_text_entry, g_parse_text_table, 131);

HOOK_DECLARE(0x00ABC070, parse_build_number_string);

void wchar_string_sanitize_for_game(wchar_t* string, long maximum_character_count)
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
		for (long i = 0; i < maximum_character_count && string[i]; ++i)
		{
			for (long j = 0; j < 3; ++j)
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

bool __cdecl parse_lobby_privacy(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00AA4B50, parse_lobby_privacy, this_ptr, buffer, buffer_length);
}

//.text:00ABCE90 ; public: __cdecl c_user_interface_text::c_user_interface_text()
//.text:00ABCF40 ; public: virtual __cdecl c_user_interface_text::~c_user_interface_text()
//.text:00ABCF70 ; public: virtual void* __cdecl c_user_interface_text::`scalar deleting destructor'(unsigned int)
//.text:00ABCFA0 ; protected: bool __cdecl c_user_interface_text::compute_bounds_internal(c_font_cache_base*, long, short_rectangle2d const*, real, short_rectangle2d const*, short, short_rectangle2d*, short_rectangle2d*)
//.text:00ABD170 ; public: void __cdecl c_user_interface_text::compute_caret_bounds(c_font_cache_base*, long, short_rectangle2d const*, real, short_rectangle2d const*, short, short_rectangle2d*)
//.text:00ABD1B0 ; public: bool __cdecl c_user_interface_text::compute_text_bounds(c_font_cache_base*, long, short_rectangle2d const*, real, short_rectangle2d const*, short_rectangle2d*)
//.text:00ABD200 ; 
//.text:00ABD210 ; public: bool __cdecl c_user_interface_text::get_align_vertically() const
//.text:00ABD220 ; public: real_argb_color __cdecl c_user_interface_text::get_argb_color() const
//.text:00ABD240 ; 
//.text:00ABD3A0 ; 
//.text:00ABD3D0 ; public: e_text_drop_shadow_style __cdecl c_user_interface_text::get_drop_shadow_style() const
//.text:00ABD3E0 ; public: e_text_justification __cdecl c_user_interface_text::get_justification() const
//.text:00ABD3F0 ; public: bool __cdecl c_user_interface_text::get_render_uppercase() const
//.text:00ABD400 ; public: double __cdecl c_user_interface_text::get_rotation() const
//.text:00ABD410 ; public: real_point2d const * __cdecl c_user_interface_text::get_rotation_origin() const
//.text:00ABD420 ; public: double __cdecl c_user_interface_text::get_scale() const
//.text:00ABD430 ; public: e_text_style __cdecl c_user_interface_text::get_style() const
//.text:00ABD440 ; public: void __cdecl c_user_interface_text::get_tab_stops(short*, short*)
//.text:00ABD480 ; 
//.text:00ABD490 ; public: bool __cdecl c_user_interface_text::get_wrap_horizontally() const
//.text:00ABD4A0 ; public: void __cdecl c_user_interface_text::initialize(wchar_t const*, short, e_font_id, real_rgb_color const*, long, e_text_style, e_text_justification, e_controller_index)
//.text:00ABD510 ; 
//.text:00ABD530 ; public: static void __cdecl c_user_interface_text::render(s_user_interface_text_render_data*, short_rectangle2d*)
//.text:00ABD750 ; public: void __cdecl c_user_interface_text::render_halox(long, real_rectangle2d const*, real_rectangle2d const*, real, real, short_rectangle2d const*)

void c_user_interface_text::set_argb_color(real_argb_color* color)
{
	//DECLFUNC(0x00ABDA90, void, __thiscall, c_user_interface_text*, real_argb_color*)(this, color);

	m_argb_color.value = real_argb_color_to_pixel32(color);
}

void c_user_interface_text::set_font(long font)
{
	//DECLFUNC(0x00ABDAB0, void, __thiscall, c_user_interface_text*, long)(this, font);

	m_font = font;
}

void c_user_interface_text::set_justification(long justification)
{
	//DECLFUNC(0x00ABDAC0, void, __thiscall, c_user_interface_text*, long)(this, justification);

	m_justification = justification;
}

void c_user_interface_text::set_style(long style)
{
	//DECLFUNC(0x00ABDAD0, void, __thiscall, c_user_interface_text*, long)(this, style);

	m_style = style;
}

//.text:00ABDAE0 ; 
//.text:00ABDB00 ; bool __cdecl string_contains_special_characters(wchar_t const*)
//.text:00ABDB60 ; 
//.text:00ABDBA0 ; void __cdecl user_interface_parse_interface_string(e_controller_index, c_static_wchar_string<1024>*)
//.text:00ABDBF0 ; bool __cdecl user_interface_parse_string(long, wchar_t*, long)
//.text:00ABDD90 ; 
//.text:00ABDDA0 ; 
//.text:00ABDDE0 ; 
//.text:00ABDE40 ; bool __cdecl wchar_parse_private_use_characters(wchar_t*)
//.text:00ABE000 ; 
//.text:00ABE030 ; 

bool __cdecl parse_lobby_coop_max_players(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B226F0, parse_lobby_coop_max_players, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_countdown_remaining(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22710, parse_lobby_countdown_remaining, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_current_players(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22750, parse_lobby_current_players, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_delaying_player(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22790, parse_lobby_delaying_player, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_film_max_players(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B227E0, parse_lobby_film_max_players, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_film_party_leader_requirement(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22800, parse_lobby_film_party_leader_requirement, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_header(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22850, parse_lobby_header, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_max_players(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B228C0, parse_lobby_max_players, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_network(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22910, parse_lobby_network, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_party_leader(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22930, parse_lobby_party_leader, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_percent_loaded(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22980, parse_lobby_percent_loaded, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_start_button_name(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22A10, parse_lobby_start_button_name, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_title(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22A80, parse_lobby_title, this_ptr, buffer, buffer_length);
}

// this hook never gets hit
void __cdecl parse_build_number_string(e_controller_index controller_index, e_utf32 utf32, c_static_wchar_string<1024>* out_string)
{
	char const* build_name = version_get_build_name();
	char const* build_string = version_get_build_string();

	out_string->set(L"");

	if (strlen(build_name) > 1)
	{
		out_string->append_print(L"%hs (%hs)", build_name, build_string);
	}
	else
	{
		out_string->append_print(L"%hs", build_string);
	}
}

void c_user_interface_text::set_flags(dword_flags flags)
{
	m_flags = flags;
}

void c_user_interface_text::set_controller_index(e_controller_index controller_index)
{
	m_controller_index = controller_index;
}

void c_user_interface_text::set_drop_shadow_style(long drop_shadow_style)
{
	m_drop_shadow_style = drop_shadow_style;
}

void c_user_interface_text::set_rotation_origin(real x, real y)
{
	m_rotation_origin.x = x;
	m_rotation_origin.y = y;
}

void c_user_interface_text::set_rotation(real rotation)
{
	m_rotation = rotation;
}

void c_user_interface_text::set_scroll_amount(real i, real j)
{
	m_scroll_amount.i = i;
	m_scroll_amount.j = j;
}

void c_user_interface_text::set_scale(real scale)
{
	m_scale = scale;
}

