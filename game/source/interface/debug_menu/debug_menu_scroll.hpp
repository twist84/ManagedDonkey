#pragma once

#include "interface/debug_menu/debug_menu.hpp"

class c_debug_menu_scroll :
	public c_debug_menu
{
public:
	virtual ~c_debug_menu_scroll();
	virtual void update() override;
	virtual void render(c_font_cache_base* font_cache, int16_point2d const& point) override;
	virtual void open() override;

protected:
	virtual long get_num_items_to_render() override;

public:
	c_debug_menu_scroll(c_debug_menu* parent, short num_visible, char const* name);

protected:
	short get_num_visible();
	short get_first();

private:
	void set_num_visible(short num_visible);
	void set_first(short first);

protected:
	short m_num_visible;
	short m_first;
};

class c_debug_menu_zone_sets :
	public c_debug_menu_scroll
{
public:
	virtual ~c_debug_menu_zone_sets();
	virtual void notify_selected(short selected_value) override;
	virtual void open() override;

protected:
	virtual void notify_up() override;
	virtual void notify_down() override;

public:
	c_debug_menu_zone_sets(c_debug_menu* parent, short num_visible, char const* name);

private:
	void update_caption();

protected:
	char m_some_string[128];
};

class c_debug_menu_global_preferences :
	public c_debug_menu_scroll
{
public:
	virtual ~c_debug_menu_global_preferences();
	virtual void notify_selected(short selected_value) override;
	virtual void open() override;

protected:
	virtual void notify_up() override;
	virtual void notify_down() override;

public:
	c_debug_menu_global_preferences(c_debug_menu* parent, short num_visible, char const* name);

private:
	void update_caption();

protected:
	char m_some_string[128];
};

