#pragma once

#include "interface/debug_menu/debug_menu.hpp"

class c_debug_menu_scroll :
	public c_debug_menu
{
public:
	virtual ~c_debug_menu_scroll() override;
	virtual void update() override;
	virtual void render(c_font_cache_base* font_cache, int16_point2d* point) override;
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

