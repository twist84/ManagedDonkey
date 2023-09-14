#pragma once

#include "interface/debug_menu/debug_menu_item.hpp"

struct c_font_cache_base;
union int16_point2d;
class c_debug_menu_item_numbered :
	public c_debug_menu_item
{
public:
	virtual ~c_debug_menu_item_numbered();
	virtual void render(c_font_cache_base* font_cache, int16_point2d const& point) override;

	c_debug_menu_item_numbered(c_debug_menu* menu, const char* name, c_debug_menu* child);

protected:
	void render_number(c_font_cache_base* font_cache, int16_point2d const& point);

	short get_indent();
};

struct c_debug_menu_item_type :
	public c_debug_menu_item_numbered
{
public:
	virtual void render(c_font_cache_base* font_cache, int16_point2d const& point) override;
	virtual void to_string(char* buffer, long buffer_size);
	virtual void render_value(c_font_cache_base* font_cache, int16_point2d const& point);

	c_debug_menu_item_type(c_debug_menu* menu, const char* name, bool readonly);

	bool get_readonly();

protected:
	bool m_readonly;
};

template<typename t_type>
class c_debug_menu_value_hs_global_external
{
public:
	c_debug_menu_value_hs_global_external(char const* hs_global_name);
	t_type get();

protected:
	short m_hs_global_external_index;
};

