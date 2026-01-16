#pragma once

#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"
#include "main/global_preferences.hpp"
#include "multithreading/synchronized_value.hpp"
#include "networking/tools/network_blf.hpp"

class c_virtual_keyboard_task;

class c_start_menu_game_editor :
	public c_start_menu_pane_screen_widget
{
public:
	void __thiscall update_save_as_new_operation_();

public:
	enum e_asynchronous_operation
	{
		_operation_none = 0,
		_operation_save,
		_operation_save_as_new,

		k_number_of_variant_desired_operations,
	};

	enum e_destructive_action_confirmation_result
	{
		_result_indeterminate = 0,
		_result_confirmation,
		_result_cancellation,
	};

	enum e_variant_save_options_dialog_result
	{
		_save = 0,
		_save_as_new,
		_cancel,

		k_number_of_variant_save_options_dialog_results,

		_invalid_result = NONE,
	};

public:
	virtual ~c_start_menu_game_editor() = default;
	virtual void initialize() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual void initialize_datasource();
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual bool handle_dialog_result(const c_dialog_result_message* message) override;
	virtual void submenu_invoked(c_gui_list_widget* submenu_widget) override;
	virtual bool allow_pane_tab_change() override;

public:
	c_start_menu_game_editor(int32 name);

protected:
	void load_progress_ui(e_controller_index controller_index);

private:
	void setup_map_variant_for_write(s_blffile_map_variant* variant_on_disk, const wchar_t* map_name, const wchar_t* map_description, e_map_id map_id, const c_map_variant* map_variant);

public:
	void update_save_as_new_operation();
	void update_save_operation();

public:
	e_asynchronous_operation m_asynchronous_operation;
	e_variant_save_options_dialog_result m_confirmation_action;
	e_destructive_action_confirmation_result m_confirmation_result;
	__declspec(align(8)) s_blffile_map_variant m_variant_on_disk;
	e_controller_index m_asynchronous_operation_controller;
	c_virtual_keyboard_task* m_keyboard_task;
	wchar_t m_virtual_keyboard_results_buffer[256];
	s_gui_game_setup_storage::s_mapeditor_settings m_multiplayer_setup;
	int32 m_create_new_variant_task;
	int32 m_write_new_variant_task;
	int32 m_write_variant_task;
	c_synchronized_long m_async_success;
	c_synchronized_long m_async_complete;
	c_synchronized_long m_content_item_index;
	uns32 m_progress_dialog_start_time_milliseconds;
};
COMPILE_ASSERT(sizeof(c_start_menu_game_editor) == 0x1E838);
COMPILE_ASSERT(sizeof(c_start_menu_game_editor) == sizeof(c_start_menu_pane_screen_widget) + 0x1C6D0);

