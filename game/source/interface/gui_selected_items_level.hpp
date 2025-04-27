#pragma once

#include "interface/gui_selected_items.hpp"

struct c_gui_level_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_campaign_id m_campaign_id;
	e_map_id m_map_id;
	bool m_level_is_locked;
	int16 m_insertion_point;
};
static_assert(sizeof(c_gui_level_selected_item) == sizeof(c_gui_selected_item) + 0x10);
static_assert(0x190 == OFFSETOF(c_gui_level_selected_item, m_campaign_id));
static_assert(0x194 == OFFSETOF(c_gui_level_selected_item, m_map_id));
static_assert(0x198 == OFFSETOF(c_gui_level_selected_item, m_level_is_locked));
static_assert(0x19A == OFFSETOF(c_gui_level_selected_item, m_insertion_point));

