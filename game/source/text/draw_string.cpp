#include "text/draw_string.hpp"

#include "text/font_loading.hpp"

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

void __cdecl c_draw_string::set_font(long font_id)
{
	if (font_id < 0)
		font_id = 0;
	if (font_id > 10)
		font_id = 10;

	m_font_id = font_id;
	m_font = font_get_header(font_id);
}

bool __cdecl c_draw_string::draw_more(c_font_cache_base* font_cache, char const* s)
{
	return DECLFUNC(0x00657DD0, bool, __thiscall, c_draw_string*, c_font_cache_base*, char const*)(this, font_cache, s);
}

c_font_cache_base::c_font_cache_base() :
	__vftable(reinterpret_cast<decltype(__vftable)>(0x0165FCB0))
{
}

c_font_cache_mt_safe::c_font_cache_mt_safe() :
	c_font_cache_base(),
	m_locked()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(0x0165FCCC);
	DECLFUNC(0x00659650, void, __thiscall, c_font_cache_mt_safe*)(this);
}

c_font_cache_mt_safe::~c_font_cache_mt_safe()
{
	DECLFUNC(0x00659720, void, __thiscall, c_font_cache_mt_safe*)(this);
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
	__unknown44(),
	m_tab_stop_count(),
	m_tab_stops(),
	m_bounds(),
	m_cursor(),
	__unknownA0(),
	__unknownA4(),
	__unknownA8(),
	m_parse_string_state(),
	__unknownF8()
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
	__unknown104(),
	__unknown108(),
	__unknown10C(),
	__unknown110()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(0x01692AD0);
}

c_simple_font_draw_string::c_simple_font_draw_string() :
	c_draw_string(),
	__unknown100(),
	__unknown104(),
	__unknown108(),
	string_length(),
	m_string()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(0x01697104);
	DECLFUNC(0x00A76540, void, __thiscall, c_simple_font_draw_string*)(this);
}

c_rasterizer_draw_string::c_rasterizer_draw_string() :
	c_draw_string(),
	m_rotation_origin(),
	m_rotation(),
	m_sine_rotation(),
	m_cosine_rotation(),
	__unknown114(),
	render_data()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(0x01692AF0);
	DECLFUNC(0x00A25F00, void, __thiscall, c_rasterizer_draw_string*)(this);
}

c_chud_draw_string::c_chud_draw_string() :
	c_draw_string(),
	__unknown100(),
	__unknown104(),
	__unknown108(),
	__unknown10C(),
	__unknown110(),
	__unknown114()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(0x0169A144);
	DECLFUNC(0x00A95570, void, __thiscall, c_chud_draw_string*)(this);
}

