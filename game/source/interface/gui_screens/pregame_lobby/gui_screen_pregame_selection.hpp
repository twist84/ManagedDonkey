#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

class c_gui_data;
class c_gui_selected_item;
class c_virtual_keyboard_task;

class c_gui_screen_pregame_selection :
	public c_gui_screen_widget
{
public:
	void __thiscall begin_deletion_of_selected_item_(e_controller_index controller_index);
	bool __thiscall handle_controller_input_message_(const c_controller_input_message* message);
	void __thiscall handle_x_button_press_on_autosave_queue_file_(e_controller_index controller_index, const c_gui_selected_item* selected_item);

public:
	c_gui_screen_pregame_selection(int32 name) :
		c_gui_screen_widget(name)
	{
		DECLFUNC(0x00B030B0, void, __thiscall, c_gui_screen_pregame_selection*, int32)(this, name);
	}

private:
	void begin_deletion_of_selected_item(e_controller_index controller_index);
	void handle_x_button_press_on_autosave_queue_file(e_controller_index controller_index, const c_gui_selected_item* selected_item);
	void load_progress_ui(e_controller_index controller_index, int32 title, int32 message);

public:

//protected:
	e_gui_selected_item_type m_selection_type;
	int32 m_selected_category_element_handle;
	c_gui_data* m_subitem_datasource;
	int32 m_asynchronous_operation_controller;
	int32 m_delete_file_async_task_id;
	int32 m_save_file_async_task_id;
	int32 m_copy_to_autosave_task;
	int32 m_autosave_queue_metadata_file_type;
	c_synchronized_long m_async_task_signal;
	c_synchronized_long m_async_task_success;
	c_synchronized_long m_async_task_result;
	c_synchronized_long m_created_content_item_index;
	c_synchronized_long m_async_saved_game_file_operation_result;
	uns32 m_progress_dialog_start_time_milliseconds;
	uns32 m_progress_dialog_show_time_milliseconds;
	int32 m_progress_title;
	int32 m_progress_message;
	s_file_reference m_autosave_queue_source_file;
	c_virtual_keyboard_task* m_keyboard_task;
	wchar_t m_virtual_keyboard_results_buffer[256];
	bool m_edit_title_in_progress;
	bool m_edit_description_in_progress;
	int32 m_edit_title_game_content_type;
	bool m_last_was_enumerating;
	int32 __unknown1E04;
};
static_assert(sizeof(c_gui_screen_pregame_selection) == sizeof(c_gui_screen_widget) + 0x368);

