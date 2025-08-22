#pragma once

#include "interface/debug_menu/debug_menu_scroll.hpp"

class c_debug_menu_zone_sets :
	public c_debug_menu_scroll
{
public:
	virtual ~c_debug_menu_zone_sets() = default;
	virtual void notify_selected(int16 selected_value) override;
	virtual void open() override;

protected:
	virtual void notify_up() override;
	virtual void notify_down() override;

public:
	c_debug_menu_zone_sets(c_debug_menu* parent, int16 num_visible, const char* name);

private:
	void update_caption();

protected:
	char m_caption[128];
};

