#include "interface/debug_menu/debug_menu_item.hpp"

#include "cseries/cseries.hpp"
#include "interface/debug_menu/debug_menu.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/interface_constants.hpp"
#include "text/draw_string.hpp"

#include <string.h>

c_debug_menu_item::c_debug_menu_item(c_debug_menu* menu, const char* name, c_debug_menu* child, bool active) :
	m_name(),
	m_menu_ref(menu),
	m_child_ref(child)
{
	set_name(name ? name : "Item???");
	set_active(active);
	set_data(NONE);
}

c_debug_menu_item::~c_debug_menu_item()
{
	if (m_child_ref)
	{
		m_child_ref->~c_debug_menu();
	}
	m_child_ref = nullptr;
}

bool c_debug_menu_item::get_active()
{
    return m_active;
}

const real_argb_color* c_debug_menu_item::get_background_color()
{
	if (!get_is_selection())
	{
		return debug_real_argb_tv_white;
	}

	real32 v4 = (debug_menu_get_time() % 80) / 79.0f;
	real32 v5 = v4 + v4;
	if (v4 >= 0.5f)
	{
		v5 = 1.0f - ((v4 - 0.5f) + (v4 - 0.5f));
	}

	static real_argb_color background_color{};
	background_color.alpha = 1.0f;
	for (int32 i = 0; i < NUMBEROF(background_color.rgb.n); i++)
	{
		real32 v6 = (v5 * 0.4f) + 0.05f;
		background_color.rgb.n[i] = (debug_real_argb_tv_white->rgb.n[i] * v6) + (global_real_argb_black->rgb.n[i] * (1.0f - v6));
	}

	return &background_color;
}

c_debug_menu* c_debug_menu_item::get_child()
{
	return m_child_ref;
}

bool c_debug_menu_item::get_data()
{
	return m_data;
}

const real_argb_color* c_debug_menu_item::get_enabled_color()
{
	if (get_child())
	{
		return debug_real_argb_tv_green;
	}

	return debug_real_argb_grey;
}

int16 c_debug_menu_item::get_index()
{
	for (int16 item_index = 0; item_index < get_menu()->get_num_items(); item_index++)
	{
		if (get_menu()->get_item(item_index) == this)
		{
			return item_index;
		}
	}

	UNREACHABLE();
	return 0;
}

bool c_debug_menu_item::get_is_selection()
{
	return get_index() == get_menu()->get_selection();
}

c_debug_menu* c_debug_menu_item::get_menu()
{
	return m_menu_ref;
}

const char* c_debug_menu_item::get_name()
{
	return m_name ? m_name : "";
}

void c_debug_menu_item::notify_left()
{
}

void c_debug_menu_item::notify_right()
{
}

void c_debug_menu_item::notify_selected()
{
	if (get_child())
	{
		debug_menu_set_active_menu(get_child(), false);
	}
}

void c_debug_menu_item::open()
{
	set_active(true);
}

void c_debug_menu_item::render(c_font_cache_base* font_cache, const point2d& position)
{
	const real_argb_color* color = global_real_argb_black;
	if (get_active() && get_menu()->get_enabled())
	{
		color = get_enabled_color();
	}

	c_rasterizer_draw_string draw_string{};

	rectangle2d bounds{};
	interface_get_current_display_settings(nullptr, nullptr, nullptr, &bounds);
	set_rectangle2d(&bounds, position.x, position.y, bounds.x1, bounds.y1);

	draw_string.set_bounds(&bounds);
	draw_string.set_color(color);
	draw_string.draw(font_cache, get_name());
}

void c_debug_menu_item::set_active(bool active)
{
	m_active = active;
}

void c_debug_menu_item::set_data(int32 data)
{
	m_data = data;
}

void c_debug_menu_item::set_name(const char* name)
{
	ASSERT(name);

	int32 name_size = strlen_debug(name) + 1;

	ASSERT(name != nullptr && m_name == nullptr);
	m_name = static_cast<char*>(debug_menu_malloc(name_size));
	csstrnzcpy(m_name, name, name_size);
}

void c_debug_menu_item::update()
{
}

