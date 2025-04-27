#pragma once

#include "interface/gui_selected_items.hpp"
#include "tag_files/files.hpp"

struct c_gui_game_variant_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_controller_index m_controller_index;
	int32 m_variant_index;
	s_file_reference m_file_reference;
	int32 m_sort_order;
};
static_assert(sizeof(c_gui_game_variant_selected_item) == sizeof(c_gui_selected_item) + 0x120);
static_assert(0x190 == OFFSETOF(c_gui_game_variant_selected_item, m_controller_index));
static_assert(0x194 == OFFSETOF(c_gui_game_variant_selected_item, m_variant_index));
static_assert(0x198 == OFFSETOF(c_gui_game_variant_selected_item, m_file_reference));
static_assert(0x2A8 == OFFSETOF(c_gui_game_variant_selected_item, m_sort_order));

