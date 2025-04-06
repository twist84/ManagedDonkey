#pragma once

#include "interface/gui_selected_items.hpp"
#include "tag_files/files.hpp"

enum e_controller_index;
struct c_gui_map_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_controller_index m_controller_index;
	s_file_reference m_file_reference;
	long m_sort_order;
};
static_assert(sizeof(c_gui_map_selected_item) == sizeof(c_gui_selected_item) + 0x118);
static_assert(0x190 == OFFSETOF(c_gui_map_selected_item, m_controller_index));
static_assert(0x194 == OFFSETOF(c_gui_map_selected_item, m_file_reference));
static_assert(0x2A4 == OFFSETOF(c_gui_map_selected_item, m_sort_order));

