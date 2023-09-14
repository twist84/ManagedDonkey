#include "interface/debug_menu/debug_menu_item_numbered.hpp"

#include "cseries/cseries.hpp"
#include "interface/interface_constants.hpp"
#include "main/console.hpp"
#include "text/draw_string.hpp"

c_debug_menu_item_numbered::~c_debug_menu_item_numbered()
{
}

void c_debug_menu_item_numbered::render(c_font_cache_base* font_cache, int16_point2d const& point)
{
	render_number(font_cache, point);

	int16_point2d next_point{};
	set_point2d(&next_point, point.x + get_indent(), point.y);
	c_debug_menu_item::render(font_cache, next_point);
}

c_debug_menu_item_numbered::c_debug_menu_item_numbered(c_debug_menu* menu, const char* name, struct c_debug_menu* child) :
	c_debug_menu_item(menu, name, child, true)
{
}

void c_debug_menu_item_numbered::render_number(c_font_cache_base* font_cache, int16_point2d const& point)
{
	char string[64]{};

	short index = get_index();
	bool is_number = index < 10;
	bool is_letter = index >= 10 && index < 36;

	c_rasterizer_draw_string draw_string{};

	short_rectangle2d bounds{};
	interface_get_current_display_settings(nullptr, nullptr, nullptr, &bounds);

	ASSERT(!(is_number && is_letter));

	if (is_number)
	{
		csnzprintf(string, sizeof(string), "   %s%d", index < 9 ? " " : "", get_index() + 1);
	}
	else if (is_letter)
	{
		csnzprintf(string, sizeof(string), "(%c)%d", index + 87, get_index() + 1);
	}
	else
	{
		csnzprintf(string, sizeof(string), "   %d", index + 1);
	}

	set_rectangle2d(&bounds, point.x - 3, point.y, bounds.x1, bounds.y1);

	real_argb_color const* color = global_real_argb_black;
	if (get_active())
		color = get_enabled_color();

	draw_string.set_color(color);
	draw_string.set_bounds(&bounds);
	draw_string.draw(font_cache, string);
}

short c_debug_menu_item_numbered::get_indent()
{
	return 45;
}

