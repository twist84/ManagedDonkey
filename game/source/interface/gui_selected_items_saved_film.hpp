#pragma once

#include "interface/gui_selected_items.hpp"
#include "tag_files/files.hpp"

enum e_saved_film_category;

struct c_gui_saved_film_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_controller_index m_controller_index;
	s_file_reference m_file_reference;
	uint64 m_sort_order;
	e_saved_film_category m_category;
};
static_assert(sizeof(c_gui_saved_film_selected_item) == sizeof(c_gui_selected_item) + 0x128);
static_assert(0x190 == OFFSETOF(c_gui_saved_film_selected_item, m_controller_index));
static_assert(0x194 == OFFSETOF(c_gui_saved_film_selected_item, m_file_reference));
static_assert(0x2A8 == OFFSETOF(c_gui_saved_film_selected_item, m_sort_order));
static_assert(0x2B0 == OFFSETOF(c_gui_saved_film_selected_item, m_category));

