#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_selection.hpp"

#include "cseries/async_helpers.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/gui_selected_items.hpp"
#include "interface/gui_selected_items_game_variant.hpp"
#include "interface/gui_selected_items_map.hpp"
#include "interface/gui_selected_items_saved_film.hpp"
#include "interface/gui_selected_items_saved_screenshot.hpp"
#include "interface/user_interface_data.hpp"
#include "memory/module.hpp"
#include "networking/online/online_guide_pc.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B03280, c_gui_screen_pregame_selection, begin_deletion_of_selected_item_);
HOOK_DECLARE_CLASS_MEMBER(0x00B033D0, c_gui_screen_pregame_selection, begin_saved_game_file_save_changes_);
HOOK_DECLARE_CLASS_MEMBER(0x00B03530, c_gui_screen_pregame_selection, begin_selected_item_text_edit_);
HOOK_DECLARE_CLASS_MEMBER(0x00B03950, c_gui_screen_pregame_selection, handle_controller_input_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00B04190, c_gui_screen_pregame_selection, handle_x_button_press_on_autosave_queue_file_);

void __thiscall c_gui_screen_pregame_selection::begin_deletion_of_selected_item_(e_controller_index controller_index)
{
	c_gui_screen_pregame_selection::begin_deletion_of_selected_item(controller_index);
}

void __thiscall c_gui_screen_pregame_selection::begin_saved_game_file_save_changes_(e_controller_index controller_index, const wchar_t* new_title, const wchar_t* new_description)
{
	c_gui_screen_pregame_selection::begin_saved_game_file_save_changes(controller_index, new_title, new_description);
}

void __thiscall c_gui_screen_pregame_selection::begin_selected_item_text_edit_(e_controller_index controller_index, bool edit_title, bool edit_description)
{
	c_gui_screen_pregame_selection::begin_selected_item_text_edit(controller_index, edit_title, edit_description);
}

bool __thiscall c_gui_screen_pregame_selection::handle_controller_input_message_(const c_controller_input_message* message)
{
	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_gui_screen_pregame_selection, handle_controller_input_message_, message);
	return result;
}

void __thiscall c_gui_screen_pregame_selection::handle_x_button_press_on_autosave_queue_file_(e_controller_index controller_index, const c_gui_selected_item* selected_item)
{
	HOOK_INVOKE_CLASS_MEMBER(, c_gui_screen_pregame_selection, handle_x_button_press_on_autosave_queue_file_, controller_index, selected_item);

	c_gui_screen_pregame_selection::handle_x_button_press_on_autosave_queue_file(controller_index, selected_item);
}

//.text:00B030B0 ; public: c_gui_screen_pregame_selection::c_gui_screen_pregame_selection(int32)
//.text:00B03200 ; public: virtual c_gui_screen_pregame_selection::~c_gui_screen_pregame_selection()
//.text:00B03210 ; 
//.text:00B03220 ; public: virtual void* c_gui_screen_pregame_selection::`scalar deleting destructor'(unsigned int)

bool c_gui_screen_pregame_selection::asynchronous_operation_in_progress() const
{
	//return INVOKE_CLASS_MEMBER(0x00B03250, c_gui_screen_pregame_selection, asynchronous_operation_in_progress);

	return m_delete_file_async_task_id != NONE || m_save_file_async_task_id != NONE || m_edit_title_in_progress || m_edit_description_in_progress;
}

void c_gui_screen_pregame_selection::begin_deletion_of_selected_item(e_controller_index controller_index)
{
	//INVOKE_CLASS_MEMBER(0x00B03280, c_gui_screen_pregame_selection, begin_deletion_of_selected_item, controller_index);

	c_gui_list_widget* subitem = c_gui_widget::get_child_list_widget(STRING_ID(gui, subitem));
	if (!subitem)
	{
		event(_event_warning, "ui: failed to locate 'subitem' list widget");
		return;
	}

	c_gui_data* data = subitem->get_data();
	if (!data)
	{
		event(_event_warning, "ui: failed to locate a datasource for the active list");
		return;
	}

	const c_gui_selected_item* selected_item = data->get_gui_selected_item(subitem->get_focused_element_handle());
	if (!selected_item || selected_item->get_location() != _gui_stored_item_location_saved_game_file)
	{
		event(_event_warning, "ui: this is not an item which can be deleted!");
		return;
	}

	//if (!controller_get(controller_index)->storage_device_valid())
	//{
	//	user_interface_offer_opportunity_to_select_storage_device(
	//		controller_index,
	//		m_name,
	//		m_screen_index);
	//	return;
	//}

	wchar_t filename_buffer[256]{};
	if (!gui_selected_item_get_file_path(selected_item, filename_buffer, NUMBEROF(filename_buffer)))
	{
		event(_event_warning, "ui: failed to get path to saved game file for deletion");
		return;
	}

	m_async_task_signal.set(0);
	m_async_task_success.set(0);
	m_async_task_result.set(0);

	char filename[256]{};
	wchar_string_to_ascii_string(filename_buffer, filename, NUMBEROF(filename), nullptr);

	m_delete_file_async_task_id = async_delete_file(filename,
		false,
		_async_category_saved_games,
		_async_priority_important_non_blocking,
		&m_async_task_success,
		&m_async_task_signal);

	if (m_delete_file_async_task_id != NONE)
	{
		c_gui_screen_pregame_selection::load_progress_ui(
			controller_index,
			STRING_ID(gui, pregame_selection_deleting_title),
			STRING_ID(gui, pregame_selection_deleting_message));
	}
}

void c_gui_screen_pregame_selection::begin_saved_game_file_save_changes(e_controller_index controller_index, const wchar_t* new_title, const wchar_t* new_description)
{
	//INVOKE_CLASS_MEMBER(0x00B033D0, c_gui_screen_pregame_selection, begin_saved_game_file_save_changes, controller_index, new_title, new_description);

	return;

	if (m_save_file_async_task_id != NONE)
	{
		event(_event_warning, "ui: already saving changes to an item");
		return;
	}

	//if (!controller_get(controller_index)->storage_device_valid())
	//{
	//	user_interface_offer_opportunity_to_select_storage_device(controller_index, m_name, m_screen_index);
	//	event(_event_warning, "ui: unable to save selected item - no storage device selected yet");
	//	return;
	//}

	c_gui_list_widget* subitem = c_gui_widget::get_child_list_widget(STRING_ID(gui, subitem));
	c_gui_data* data = nullptr;
	const c_gui_selected_item* selected_item = nullptr;
	if (!subitem ||
		!(data = subitem->get_data()) ||
		!(selected_item = data->get_gui_selected_item(subitem->get_focused_element_handle())) ||
		selected_item->get_location() != _gui_stored_item_location_saved_game_file)
	{
		event(_event_warning, "ui: unable to determine selected item for updating");
		return;
	}

	wchar_t filename_buffer[256]{};
	if (!gui_selected_item_get_file_path(selected_item, filename_buffer, NUMBEROF(filename_buffer)))
	{
		event(_event_warning, "ui: unable to retrieve filepath of selected item for updating");
		return;
	}

	char filename[256]{};
	wchar_string_to_ascii_string(filename_buffer, filename, NUMBEROF(filename), nullptr);

	//m_async_task_signal.set(0);
	//m_async_task_success.set(0);
	//m_async_task_result.set(0);
	//
	//m_save_file_async_task_id = saved_game_files_update_metadata(
	//	controller_index,
	//	content_item_index,
	//	new_title,
	//	new_description,
	//	&m_async_task_success,
	//	&m_async_task_signal);
	//
	//if (m_save_file_async_task_id != NONE)
	//{
	//	c_gui_screen_pregame_selection::load_progress_ui(
	//		controller_index,
	//		STRING_ID(gui, updating_metadata_title),
	//		STRING_ID(gui, updating_metadata_message));
	//}
}

void c_gui_screen_pregame_selection::begin_selected_item_text_edit(e_controller_index controller_index, bool edit_title, bool edit_description)
{
	//INVOKE_CLASS_MEMBER(0x00B03530, c_gui_screen_pregame_selection, begin_selected_item_text_edit, controller_index, edit_title, edit_description);

	c_gui_list_widget* subitem = c_gui_widget::get_child_list_widget(STRING_ID(gui, subitem));
	c_gui_data* data = nullptr;
	const c_gui_selected_item* selected_item = nullptr;
	if (!subitem ||
		!(data = subitem->get_data()) ||
		!(selected_item = data->get_gui_selected_item(subitem->get_focused_element_handle())) ||
		selected_item->get_location() != _gui_stored_item_location_saved_game_file)
	{
		event(_event_warning, "ui: no selected item to be edited");
		return;
	}

	if (selected_item->get_corrupt())
	{
		event(_event_warning, "ui: selected item is corrupt");
		return;
	}

	if (selected_item->get_location() != _gui_stored_item_location_saved_game_file)
	{
		event(_event_warning, "ui: selected item cannot be edited");
		return;
	}

	if (c_gui_screen_pregame_selection::asynchronous_operation_in_progress() || !m_keyboard_task || m_keyboard_task->busy())
	{
		event(_event_warning, "ui: unable to initiate virtual keyboard for editing text");
		return;
	}

	//if (!controller_get(controller_index)->storage_device_valid())
	//{
	//	user_interface_offer_opportunity_to_select_storage_device(
	//		controller_index,
	//		m_name,
	//		m_screen_index);
	//	return;
	//}

	int32 edit_id = 0;
	int32 edit_message_id = 0;
	c_static_wchar_string<1024> title_text;
	c_static_wchar_string<1024> description_text;
	wchar_t default_text[128]{};
	int32 maximum_input_characters = 0;

	if (edit_title)
	{
		edit_id = STRING_ID(gui, edit_title);
		edit_message_id = STRING_ID(gui, edit_title_message);
		ustrnzcpy(default_text, selected_item->c_gui_selected_item::get_title(), NUMBEROF(default_text));
		maximum_input_characters = 16;
	}
	else if (edit_description)
	{
		edit_id = STRING_ID(gui, edit_description);
		edit_message_id = STRING_ID(gui, edit_description_message);
		ustrnzcpy(default_text, selected_item->c_gui_selected_item::get_description(), NUMBEROF(default_text));
		maximum_input_characters = 128;
	}

	c_gui_screen_pregame_selection::get_string_by_string_id(edit_id, &title_text);
	c_gui_screen_pregame_selection::get_string_by_string_id(edit_message_id, &description_text);

	m_keyboard_task->set_controller_index(controller_index);
	m_keyboard_task->set_default_text(default_text);
	m_keyboard_task->set_title_text(title_text.get_string());
	m_keyboard_task->set_description_text(description_text.get_string());
	m_keyboard_task->set_maximum_input_characters(maximum_input_characters);
	m_keyboard_task->set_character_flags(0x4001);
	m_virtual_keyboard_results_buffer[0] = 0;

	if (!overlapped_task_start_internal(m_keyboard_task, __FILE__, __LINE__))
	{
		event(_event_error, "ui: failed to start virtual keyboard task for editing saved game file name or description!");
		m_keyboard_task->reset();
		return;
	}

	if (edit_title)
	{
		m_edit_title_in_progress = true;
	}
	else if (edit_description)
	{
		m_edit_description_in_progress = true;
	}
}

//.text:00B03790 ; public: virtual c_gui_bitmap_widget* c_gui_screen_pregame_selection::create_bitmap_widget(const s_runtime_bitmap_widget_definition*)
//.text:00B03810 ; public: virtual void c_gui_screen_pregame_selection::dispose()
//.text:00B03880 ; private: void c_gui_screen_pregame_selection::dispose_progress_ui(bool)
//.text:00B038F0 ; private: int32 c_gui_screen_pregame_selection::get_current_category_element_handle()
//.text:00B03910 ; 
//.text:00B03920 ; 
//.text:00B03930 ; 
//.text:00B03940 ; 
//.text:00B03950 ; public: virtual bool c_gui_screen_pregame_selection::handle_controller_input_message(const c_controller_input_message*)
//.text:00B03D90 ; public: virtual bool c_gui_screen_pregame_selection::handle_dialog_result(const c_dialog_result_message*)
//.text:00B03EE0 ; public: virtual bool c_gui_screen_pregame_selection::handle_list_item_chosen(const c_controller_input_message*, int32, c_gui_list_item_widget*, c_gui_data*)

void c_gui_screen_pregame_selection::handle_x_button_press_on_autosave_queue_file(e_controller_index controller_index, const c_gui_selected_item* selected_item)
{
	//INVOKE_CLASS_MEMBER(0x00B04190, c_gui_screen_pregame_selection, handle_x_button_press_on_autosave_queue_file, controller_index, selected_item);

	switch (m_selection_type)
	{
	case _gui_selection_type_film:
	{
		const c_gui_saved_film_selected_item* saved_film_selected_item = (const c_gui_saved_film_selected_item*)selected_item;

		printf("");
	}
	break;
	}
}

//.text:00B04330 ; private: void c_gui_screen_pregame_selection::handle_x_button_press_on_network_mode(e_controller_index, c_gui_data*, int32, int32)
//.text:00B04420 ; private: void c_gui_screen_pregame_selection::handle_x_button_press_on_saved_game_file(e_controller_index, int32)
//.text:00B044B0 ; public: virtual void c_gui_screen_pregame_selection::initialize()
//.text:00B045A0 ; public: virtual void c_gui_screen_pregame_selection::initialize_datasource()
//.text:00B048B0 ; private: bool c_gui_screen_pregame_selection::is_current_category_recent_items_list()

void c_gui_screen_pregame_selection::load_progress_ui(e_controller_index controller_index, int32 title, int32 message)
{
	INVOKE_CLASS_MEMBER(0x00B048F0, c_gui_screen_pregame_selection, load_progress_ui, controller_index, title, message);
}

//.text:00B04930 ; private: void c_gui_screen_pregame_selection::offset_widget_recursive(c_gui_widget*, real32, real32)
//.text:00B04A00 ; 
//.text:00B04A60 ; 
//.text:00B04A80 ; 
//.text:00B04AB0 ; private: void c_gui_screen_pregame_selection::post_error_for_save_game_item_failure()
//.text:00B04B30 ; public: virtual void c_gui_screen_pregame_selection::post_initialize()
//.text:00B04BB0 ; public: virtual void c_gui_screen_pregame_selection::reconstruct()
//.text:00B04BD0 ; public: void c_gui_screen_pregame_selection::restore_focus_to_selected_category()
//.text:00B04C20 ; private: void c_gui_screen_pregame_selection::set_current_category(c_gui_data*, int32)
//.text:00B04FC0 ; private: void c_gui_screen_pregame_selection::set_initial_focused_category()
//.text:00B05070 ; private: void c_gui_screen_pregame_selection::shift_subitem_menu()
//.text:00B051A0 ; private: bool c_gui_screen_pregame_selection::should_lock_users_out_until_enumeration_is_complete()
//.text:00B051D0 ; public: virtual void c_gui_screen_pregame_selection::update(uns32)
//.text:00B053D0 ; private: void c_gui_screen_pregame_selection::update_async_tasks()
//.text:00B057F0 ; private: void c_gui_screen_pregame_selection::update_map_image()
//.text:00B058C0 ; private: void c_gui_screen_pregame_selection::update_network_mode()
//.text:00B05A40 ; private: void c_gui_screen_pregame_selection::update_progress_ui()
//.text:00B05B10 ; public: virtual void c_gui_screen_pregame_selection::update_render_state(uns32)
//.text:00B05B40 ; private: void c_gui_screen_pregame_selection::update_subitem()

