#pragma once

#include "interface/gui_selected_items.hpp"

enum e_network_mode_category;
enum e_network_mode_choice;
struct c_gui_network_mode_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_network_mode_category m_network_mode;
	e_network_mode_choice m_network_choice;
};
static_assert(sizeof(c_gui_network_mode_selected_item) == sizeof(c_gui_selected_item) + 0x8);
static_assert(0x190 == OFFSETOF(c_gui_network_mode_selected_item, m_network_mode));
static_assert(0x194 == OFFSETOF(c_gui_network_mode_selected_item, m_network_choice));

