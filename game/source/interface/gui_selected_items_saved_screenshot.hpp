#pragma once

#include "interface/gui_selected_items.hpp"
#include "tag_files/files.hpp"

class c_gui_saved_screenshot_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_controller_index m_controller_index;
	s_file_reference m_file_reference;
	char m_author[16];
	uns64 m_date;
	uns64 m_size_in_bytes;
	int32 m_list_item_index;
};
COMPILE_ASSERT(sizeof(c_gui_saved_screenshot_selected_item) == sizeof(c_gui_selected_item) + 0x140);
COMPILE_ASSERT(0x190 == OFFSETOF(c_gui_saved_screenshot_selected_item, m_controller_index));
COMPILE_ASSERT(0x194 == OFFSETOF(c_gui_saved_screenshot_selected_item, m_file_reference));
COMPILE_ASSERT(0x2A4 == OFFSETOF(c_gui_saved_screenshot_selected_item, m_author));
COMPILE_ASSERT(0x2B8 == OFFSETOF(c_gui_saved_screenshot_selected_item, m_date));
COMPILE_ASSERT(0x2C0 == OFFSETOF(c_gui_saved_screenshot_selected_item, m_size_in_bytes));
COMPILE_ASSERT(0x2C8 == OFFSETOF(c_gui_saved_screenshot_selected_item, m_list_item_index));

