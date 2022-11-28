#include "text/draw_string.hpp"

void __cdecl c_draw_string::set_color(real_argb_color const* color)
{
	m_color = *color;
}

void __cdecl c_draw_string::set_shadow_color(real_argb_color const* shadow_color)
{
	m_shadow_color = *shadow_color;
}

void __cdecl c_draw_string::set_scale(real scale)
{
	m_scale.lower = scale;
}

bool __cdecl c_draw_string::draw_more(c_font_cache_base* font_cache, char const* s)
{
	return DECLFUNC(0x00657DD0, bool, __thiscall, c_draw_string*, c_font_cache_base*, char const*)(this, font_cache, s);
}

