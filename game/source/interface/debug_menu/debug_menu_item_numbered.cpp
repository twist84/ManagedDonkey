#include "interface/debug_menu/debug_menu_item_numbered.hpp"

#include "interface/debug_menu/debug_menu.hpp"
#include "interface/interface_constants.hpp"
#include "text/draw_string.hpp"

c_debug_menu_item_numbered::~c_debug_menu_item_numbered()
{
}

void c_debug_menu_item_numbered::render(c_font_cache_base* font_cache, point2d const& position)
{
	render_number(font_cache, position);

	point2d super_class_render_position{};
	set_point2d(&super_class_render_position, position.x + get_indent(), position.y);
	c_debug_menu_item::render(font_cache, super_class_render_position);
}

c_debug_menu_item_numbered::c_debug_menu_item_numbered(c_debug_menu* menu, char const* name, struct c_debug_menu* child) :
	c_debug_menu_item(menu, name, child, true)
{
}

void c_debug_menu_item_numbered::render_number(c_font_cache_base* font_cache, point2d const& position)
{
	char buffer[64]{};

	short index = get_index();
	bool is_number = index < 10;
	bool is_letter = index >= 10 && index < 36;

	c_rasterizer_draw_string draw_string{};

	rectangle2d bounds{};
	interface_get_current_display_settings(NULL, NULL, NULL, &bounds);

	ASSERT(!(is_number && is_letter));

	if (is_number)
	{
		csnzprintf(buffer, sizeof(buffer), "   %s%d", index < 9 ? " " : "", get_index() + 1);
	}
	else if (is_letter)
	{
		csnzprintf(buffer, sizeof(buffer), "(%c)%d", index + 87, get_index() + 1);
	}
	else
	{
		csnzprintf(buffer, sizeof(buffer), "   %d", index + 1);
	}

	set_rectangle2d(&bounds, position.x - 3, position.y, bounds.x1, bounds.y1);

	real_argb_color const* color = global_real_argb_black;
	if (get_active())
		color = get_enabled_color();

	draw_string.set_color(color);
	draw_string.set_bounds(&bounds);
	draw_string.draw(font_cache, buffer);
}

short c_debug_menu_item_numbered::get_indent()
{
	return 45;
}
