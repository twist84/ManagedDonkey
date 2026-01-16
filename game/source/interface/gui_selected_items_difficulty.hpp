#pragma once

#include "interface/gui_selected_items.hpp"

class c_gui_difficulty_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_campaign_difficulty_level m_difficulty;
};
COMPILE_ASSERT(sizeof(c_gui_difficulty_selected_item) == sizeof(c_gui_selected_item) + 0x8);
COMPILE_ASSERT(0x190 == OFFSETOF(c_gui_difficulty_selected_item, m_difficulty));

