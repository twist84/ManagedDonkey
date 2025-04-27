#include "interface/debug_menu/debug_menu_item_type.hpp"

#include "interface/debug_menu/debug_menu.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "text/draw_string.hpp"

c_debug_menu_item_type::~c_debug_menu_item_type()
{
}

void c_debug_menu_item_type::render(c_font_cache_base* font_cache, point2d const& position)
{
	point2d value_point{};
	point2d number_point{};
	point2d next_point{};

	set_point2d(&value_point, position.x - 66, position.y);
	set_point2d(&number_point, position.x, position.y);
	set_point2d(&next_point, position.x + get_indent(), position.y);

	render_value(font_cache, value_point);
	render_number(font_cache, number_point);
	c_debug_menu_item::render(font_cache, next_point);
}

void c_debug_menu_item_type::to_string(char* buffer, long buffer_size)
{
	csstrnzcpy(buffer, "overload toString", buffer_size);
}

void c_debug_menu_item_type::render_value(c_font_cache_base* font_cache, point2d const& position)
{
	c_rasterizer_draw_string draw_string{};

	char buffer[1024]{};
	to_string(buffer, sizeof(buffer));

	rectangle2d bounds{};
	set_rectangle2d(&bounds, position.x + 2, position.y, position.x + 60, position.y + get_menu()->get_item_height());

	if (get_active())
	{
		real32 alpha = get_menu()->get_enabled() ? 0.7f : 0.1f;

		short x0 = position.x;
		short y0 = short(position.y + debug_menu_get_item_indent_y());
		short x1 = short(position.x + 60);
		short y1 = short((position.y + get_menu()->get_item_height()) - (2.0f * debug_menu_get_item_indent_y()));

		debug_menu_draw_rect(x0, y0, x1, y1, alpha, get_background_color());
	}

	real_argb_color const* color = debug_real_argb_grey;
	if (!get_active() || get_readonly())
		color = global_real_argb_black;

	draw_string.set_color(color);
	draw_string.set_bounds(&bounds);
	draw_string.draw(font_cache, buffer);
}

c_debug_menu_item_type::c_debug_menu_item_type(c_debug_menu* menu, char const* name, bool readonly) :
	c_debug_menu_item_numbered(menu, name, NULL),
	m_readonly(readonly)
{
}

bool c_debug_menu_item_type::get_readonly()
{
	return m_readonly;
}

c_debug_menu_item_type_bool::~c_debug_menu_item_type_bool()
{
}

void c_debug_menu_item_type_bool::notify_left()
{
	c_debug_menu_item::notify_left();

	if (!get_readonly())
		m_value.set(!m_value.get());
}

void c_debug_menu_item_type_bool::notify_right()
{
	c_debug_menu_item::notify_right();

	if (!get_readonly())
		m_value.set(!m_value.get());
}

void c_debug_menu_item_type_bool::to_string(char* buffer, long buffer_size)
{
	csnzprintf(buffer, buffer_size, "%s", m_value.get() ? "True" : "False");
}

c_debug_menu_item_type_bool::c_debug_menu_item_type_bool(c_debug_menu* menu, char const* name, bool readonly, char const* hs_global_name) :
	c_debug_menu_item_type(menu, name, readonly),
	m_value(hs_global_name)
{
}

c_debug_menu_item_type_real::~c_debug_menu_item_type_real()
{
}

void c_debug_menu_item_type_real::notify_left()
{
	c_debug_menu_item::notify_left();

	if (!get_readonly())
		m_value.set(MIN(m_max, MAX(m_min, m_value.get() - m_inc)));
}

void c_debug_menu_item_type_real::notify_right()
{
	c_debug_menu_item::notify_right();

	if (!get_readonly())
		m_value.set(MIN(m_max, MAX(m_min, m_value.get() + m_inc)));
}

void c_debug_menu_item_type_real::to_string(char* buffer, long buffer_size)
{
	csnzprintf(buffer, buffer_size, "%f", m_value.get());
}

c_debug_menu_item_type_real::c_debug_menu_item_type_real(c_debug_menu* menu, char const* name, bool readonly, char const* variable, real32 min, real32 max, real32 inc) :
	c_debug_menu_item_type(menu, name, readonly),
	m_value(variable),
	m_min(min),
	m_max(max),
	m_inc(inc)
{
}

c_debug_menu_item_type_short::~c_debug_menu_item_type_short()
{
}

void c_debug_menu_item_type_short::notify_left()
{
	c_debug_menu_item::notify_left();

	if (!get_readonly())
		m_value.set(MIN(m_max, MAX(m_min, m_value.get() - m_inc)));
}

void c_debug_menu_item_type_short::notify_right()
{
	c_debug_menu_item::notify_right();

	if (!get_readonly())
		m_value.set(MIN(m_max, MAX(m_min, m_value.get() + m_inc)));
}

void c_debug_menu_item_type_short::to_string(char* buffer, long buffer_size)
{
	csnzprintf(buffer, buffer_size, "%d", m_value.get());
}

c_debug_menu_item_type_short::c_debug_menu_item_type_short(c_debug_menu* menu, char const* name, bool readonly, char const* variable, short min, short max, short inc) :
	c_debug_menu_item_type(menu, name, readonly),
	m_value(variable),
	m_min(min),
	m_max(min),
	m_inc(inc)
{
}

c_debug_menu_item_type_long::~c_debug_menu_item_type_long()
{
}

void c_debug_menu_item_type_long::notify_left()
{
	c_debug_menu_item::notify_left();

	if (!get_readonly())
		m_value.set(MIN(m_max, MAX(m_min, m_value.get() - m_inc)));
}

void c_debug_menu_item_type_long::notify_right()
{
	c_debug_menu_item::notify_right();

	if (!get_readonly())
		m_value.set(MIN(m_max, MAX(m_min, m_value.get() + m_inc)));
}

void c_debug_menu_item_type_long::to_string(char* buffer, long buffer_size)
{
	csnzprintf(buffer, buffer_size, "%d", m_value.get());
}

c_debug_menu_item_type_long::c_debug_menu_item_type_long(c_debug_menu* menu, char const* name, bool readonly, char const* variable, long min, long max, long inc) :
	c_debug_menu_item_type(menu, name, readonly),
	m_value(variable),
	m_min(min),
	m_max(max),
	m_inc(inc)
{
}

