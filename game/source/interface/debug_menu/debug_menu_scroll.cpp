#include "interface/debug_menu/debug_menu_scroll.hpp"

#include "cseries/cseries.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/interface_constants.hpp"
#include "text/draw_string.hpp"

void c_debug_menu_scroll::update()
{
	c_debug_menu::update();

	if (get_selection() + 1 >= get_first() + get_num_visible())
	{
		int16 v3 = get_num_items();
		if (v3 <= get_num_visible())
			v3 = get_num_visible();

		if (get_first() + 1 >= v3 - get_num_visible())
		{
			int16 v5 = get_num_items();
			if (v5 <= get_num_visible())
				v5 = get_num_visible();

			set_first(v5 - get_num_visible());
		}
		else
			set_first(get_first() + 1);
	}

	if (get_selection() - 1 < get_first())
	{
		if (get_first() - 1 <= 0)
			set_first(0);
		else
			set_first(get_first() - 1);
	}
}

void c_debug_menu_scroll::render(c_font_cache_base* font_cache, const point2d& position)
{
	render_background(font_cache, position);
	render_title(font_cache, position);
	render_caption(font_cache, position);
	render_global_caption(font_cache, position);
	if (get_num_visible() > 0)
		render_items(font_cache, position, get_first(), get_first() + get_num_visible() - 1);

	c_rasterizer_draw_string draw_string{};

	rectangle2d bounds{};
	interface_get_current_display_settings(nullptr, nullptr, nullptr, &bounds);

	draw_string.set_color(debug_real_argb_tv_magenta);
	if (get_first() > 0)
	{
		set_rectangle2d(&bounds, position.x, position.y + get_title_height(), bounds.x1, bounds.y1);

		draw_string.set_bounds(&bounds);
		draw_string.draw(font_cache, "^");

		bounds.y0 += get_item_height();
		draw_string.set_bounds(&bounds);
		draw_string.draw(font_cache, "|");
	}

	if (get_num_items() - get_first() > get_num_visible())
	{
		set_rectangle2d(&bounds, position.x, (position.y + get_title_height()) + (get_num_visible() - 2) * get_item_height(), bounds.x1, bounds.y1);
		draw_string.set_bounds(&bounds);
		draw_string.draw(font_cache, "|");

		bounds.y0 += get_item_height();
		draw_string.set_bounds(&bounds);
		draw_string.draw(font_cache, "v");
	}
}

void c_debug_menu_scroll::open()
{
	c_debug_menu::open();
	set_first(0);
}

int32 c_debug_menu_scroll::get_num_items_to_render()
{
	if (get_num_items() <= get_num_visible())
		return get_num_items();

	return get_num_visible();
}

c_debug_menu_scroll::c_debug_menu_scroll(c_debug_menu* parent, int16 num_visible, const char* name) :
	c_debug_menu(parent, name)
{
	set_num_visible(num_visible);
	set_first(0);
}

int16 c_debug_menu_scroll::get_num_visible()
{
	return m_num_visible;
}

int16 c_debug_menu_scroll::get_first()
{
	return m_first;
}

void c_debug_menu_scroll::set_num_visible(int16 num_visible)
{
	ASSERT(num_visible >= 0);

	m_num_visible = num_visible;
}

void c_debug_menu_scroll::set_first(int16 first)
{
	ASSERT(first >= 0 && first <= FLOOR(get_num_items(), get_num_visible()) - get_num_visible());

	m_first = first;
}

