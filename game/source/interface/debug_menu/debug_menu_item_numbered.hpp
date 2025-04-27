#pragma once

#include "cseries/cseries.hpp"
#include "interface/debug_menu/debug_menu_item.hpp"
#include "interface/debug_menu/debug_menu_value_hs_global_external.hpp"

struct c_font_cache_base;
class c_debug_menu_item_numbered :
	public c_debug_menu_item
{
public:
	virtual ~c_debug_menu_item_numbered();
	virtual void render(c_font_cache_base* font_cache, point2d const& position) override;

	c_debug_menu_item_numbered(c_debug_menu* menu, char const* name, c_debug_menu* child);

protected:
	void render_number(c_font_cache_base* font_cache, point2d const& position);

	int16 get_indent();
};

