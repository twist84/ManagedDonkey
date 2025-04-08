#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

enum e_gui_selected_item_type;
struct c_gui_data;
struct c_virtual_keyboard_task;
struct c_gui_screen_pregame_selection :
	public c_gui_screen_widget
{
public:
	c_gui_screen_pregame_selection(long name) :
		c_gui_screen_widget(name)
	{
		DECLFUNC(0x00B030B0, c_gui_screen_pregame_selection*, __thiscall, c_gui_screen_pregame_selection*, long)(this, name);
	}

//protected:
	e_gui_selected_item_type m_selection_type;
	long m_selected_category_element_handle;
	c_gui_data* m_subitem_datasource;
	long m_asynchronous_operation_controller;
	long m_delete_file_async_task_id;
	long m_save_file_async_task_id;
	long m_copy_to_autosave_task;
	long m_autosave_queue_metadata_file_type;
	c_synchronized_long m_async_task_signal;
	c_synchronized_long m_async_task_success;
	c_synchronized_long m_async_task_result;
	c_synchronized_long m_created_content_item_index;
	c_synchronized_long m_async_saved_game_file_operation_result;
	dword m_progress_dialog_start_time_milliseconds;
	dword m_progress_dialog_show_time_milliseconds;
	long m_progress_title;
	long m_progress_message;
	s_file_reference m_autosave_queue_source_file;
	c_virtual_keyboard_task* m_keyboard_task;
	wchar_t m_virtual_keyboard_results_buffer[256];
	bool m_edit_title_in_progress;
	bool m_edit_description_in_progress;
	long m_edit_title_game_content_type;
	bool m_last_was_enumerating;
	long __unknown1E04;
};
static_assert(sizeof(c_gui_screen_pregame_selection) == sizeof(c_gui_screen_widget) + 0x368);

