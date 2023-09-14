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

