#pragma once

#include "interface/debug_menu/debug_menu.hpp"

class c_debug_menu_scroll :
	public c_debug_menu
{
public:
	virtual ~c_debug_menu_scroll() = default;
	virtual void update() override;
	virtual void render(c_font_cache_base* font_cache, const point2d& point) override;
	virtual void open() override;

protected:
	virtual int32 get_num_items_to_render() override;

public:
	c_debug_menu_scroll(c_debug_menu* parent, int16 num_visible, const char* name);

protected:
	int16 get_num_visible();
	int16 get_first();

private:
	void set_num_visible(int16 num_visible);
	void set_first(int16 first);

protected:
	int16 m_num_visible;
	int16 m_first;
};

