#include "interface/debug_menu/debug_menu_item_numbered.hpp"

#include "interface/debug_menu/debug_menu.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/interface_constants.hpp"
#include "main/console.hpp"
#include "text/draw_string.hpp"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

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

c_debug_menu_item_type::~c_debug_menu_item_type()
{
}

void c_debug_menu_item_type::render(c_font_cache_base* font_cache, int16_point2d const& point)
{
	int16_point2d value_point{};
	int16_point2d number_point{};
	int16_point2d next_point{};

	set_point2d(&value_point, point.x - 66, point.y);
	set_point2d(&value_point, point.x, point.y);
	set_point2d(&next_point, point.x + get_indent(), point.y);

	render_value(font_cache, value_point);
	render_number(font_cache, number_point);
	c_debug_menu_item::render(font_cache, next_point);
}

void c_debug_menu_item_type::to_string(char* buffer, long buffer_size)
{
	csstrnzcpy(buffer, "overload toString", buffer_size);
}

void c_debug_menu_item_type::render_value(c_font_cache_base* font_cache, int16_point2d const& point)
{
	c_rasterizer_draw_string draw_string{};

	char buffer[1024]{};
	to_string(buffer, sizeof(buffer));

	short_rectangle2d bounds{};
	set_rectangle2d(&bounds, point.x + 2, point.y, point.x + 60, point.y + get_menu()->get_item_height());

	if (get_active())
	{
		real unused = get_menu()->get_enabled() ? 0.7f : 0.1f;

		short a1 = point.x;
		short a2 = short(point.y + debug_menu_get_item_indent_y());
		short a3 = short(point.x + 60);
		short a4 = short((point.y + get_menu()->get_item_height()) - (2.0f * debug_menu_get_item_indent_y()));

		debug_menu_draw_rect(a1, a2, a3, a4, unused, get_background_color());
	}

	real_argb_color const* color = debug_real_argb_grey;
	if (!get_active() || get_readonly())
		color = global_real_argb_black;

	draw_string.set_color(color);
	draw_string.set_bounds(&bounds);
	draw_string.draw(font_cache, buffer);
}

c_debug_menu_item_type::c_debug_menu_item_type(c_debug_menu* menu, const char* name, bool readonly) :
	c_debug_menu_item_numbered(menu, name, NULL),
	m_readonly(readonly)
{
}

bool c_debug_menu_item_type::get_readonly()
{
	return m_readonly;
}

template<typename t_type>
inline c_debug_menu_value_hs_global_external<t_type>::c_debug_menu_value_hs_global_external(char const* hs_global_name)
{
	ASSERT(hs_global_name);

	m_hs_global_external_index = NONE;
	for (short hs_global_external_index = 0; hs_global_external_index < k_console_global_count && m_hs_global_external_index == NONE; hs_global_external_index++)
	{
		if (csstricmp(hs_global_name, k_console_globals[hs_global_external_index]->name))
			continue;

		if (!k_console_globals[hs_global_external_index]->pointer)
			continue;

		e_hs_type type = k_console_globals[hs_global_external_index]->type;
		if (IN_RANGE_INCLUSIVE(type, _hs_type_boolean, _hs_type_long_integer))
			m_hs_global_external_index = hs_global_external_index;
	}
}

template<typename t_type>
inline t_type c_debug_menu_value_hs_global_external<t_type>::get()
{
	byte bytes[sizeof(t_type)];
	csmemset(bytes, 0, sizeof(t_type));

	if (m_hs_global_external_index != NONE)
	{
		ASSERT(k_console_globals[m_hs_global_external_index]->pointer != NULL);

		*reinterpret_cast<t_type*>(bytes) = *static_cast<t_type*>(k_console_globals[m_hs_global_external_index]->pointer);
	}

	return *reinterpret_cast<t_type*>(bytes);
}

template<typename t_type>
inline void c_debug_menu_value_hs_global_external<t_type>::set(t_type value)
{
	if (m_hs_global_external_index != NONE)
	{
		ASSERT(k_console_globals[m_hs_global_external_index]->pointer != NULL);

		*static_cast<t_type*>(k_console_globals[m_hs_global_external_index]->pointer) = value;
	}
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
		m_value.set(MIN(m_max_value, MAX(m_min_value, m_value.get() - m_inc_value)));
}

void c_debug_menu_item_type_real::notify_right()
{
	c_debug_menu_item::notify_right();

	if (!get_readonly())
		m_value.set(MIN(m_max_value, MAX(m_min_value, m_value.get() + m_inc_value)));
}

void c_debug_menu_item_type_real::to_string(char* buffer, long buffer_size)
{
	csnzprintf(buffer, buffer_size, "%f", m_value.get());
}

c_debug_menu_item_type_real::c_debug_menu_item_type_real(c_debug_menu* menu, char const* name, bool readonly, char const* hs_global_name, real min_value, real max_value, real inc_value) :
	c_debug_menu_item_type(menu, name, readonly),
	m_value(hs_global_name),
	m_min_value(min_value),
	m_max_value(max_value),
	m_inc_value(inc_value)
{
}

c_debug_menu_item_type_short::~c_debug_menu_item_type_short()
{
}

void c_debug_menu_item_type_short::notify_left()
{
	c_debug_menu_item::notify_left();

	if (!get_readonly())
		m_value.set(MIN(m_max_value, MAX(m_min_value, m_value.get() - m_inc_value)));
}

void c_debug_menu_item_type_short::notify_right()
{
	c_debug_menu_item::notify_right();

	if (!get_readonly())
		m_value.set(MIN(m_max_value, MAX(m_min_value, m_value.get() + m_inc_value)));
}

void c_debug_menu_item_type_short::to_string(char* buffer, long buffer_size)
{
	csnzprintf(buffer, buffer_size, "%d", m_value.get());
}

c_debug_menu_item_type_short::c_debug_menu_item_type_short(c_debug_menu* menu, char const* name, bool readonly, char const* hs_global_name, short min_value, short max_value, short inc_value) :
	c_debug_menu_item_type(menu, name, readonly),
	m_value(hs_global_name),
	m_min_value(min_value),
	m_max_value(max_value),
	m_inc_value(inc_value)
{
}

c_debug_menu_item_type_long::~c_debug_menu_item_type_long()
{
}

void c_debug_menu_item_type_long::notify_left()
{
	c_debug_menu_item::notify_left();

	if (!get_readonly())
		m_value.set(MIN(m_max_value, MAX(m_min_value, m_value.get() - m_inc_value)));
}

void c_debug_menu_item_type_long::notify_right()
{
	c_debug_menu_item::notify_right();

	if (!get_readonly())
		m_value.set(MIN(m_max_value, MAX(m_min_value, m_value.get() + m_inc_value)));
}

void c_debug_menu_item_type_long::to_string(char* buffer, long buffer_size)
{
	csnzprintf(buffer, buffer_size, "%d", m_value.get());
}

c_debug_menu_item_type_long::c_debug_menu_item_type_long(c_debug_menu* menu, char const* name, bool readonly, char const* hs_global_name, long min_value, long max_value, long inc_value) :
	c_debug_menu_item_type(menu, name, readonly),
	m_value(hs_global_name),
	m_min_value(min_value),
	m_max_value(max_value),
	m_inc_value(inc_value)
{
}
