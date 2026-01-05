#include "interface/gui_screens/start_menu/panes/game_editor/start_menu_game_editor.hpp"

#include "cseries/async_helpers.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game_engine_team.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/module.hpp"
#include "saved_games/scenario_map_variant.hpp"

struct controller_interface_class // c_controller_interface
{
	bool __thiscall storage_device_valid()
	{
		return true;
	}
};
static void* storage_device_valid_for_save_map_as = member_to_static_function(&controller_interface_class::storage_device_valid);
HOOK_DECLARE_CALL(0x00AEAAC8, storage_device_valid_for_save_map_as);

HOOK_DECLARE_CLASS_MEMBER(0x00AEB490, c_start_menu_game_editor, update_save_as_new_operation);

void __thiscall c_start_menu_game_editor::update_save_as_new_operation_()
{
	c_start_menu_game_editor::update_save_as_new_operation();
}

//.text:00AE9D70 ; 
//.text:00AE9DA0 ; 
//.text:00AE9DD0 ; 
//.text:00AE9DF0 ; public: c_load_legal_screen_message::c_load_legal_screen_message(e_controller_index, e_window_index, int32, c_start_menu_game_editor*, bool)
//.text:00AE9E30 ; public: c_start_menu_game_editor::c_start_menu_game_editor(int32)
//.text:00AE9F90 ; 
//.text:00AE9FA0 ; 
//.text:00AE9FB0 ; public: virtual c_start_menu_game_editor::~c_start_menu_game_editor()
//.text:00AEA010 ; 
//.text:00AEA020 ; public: virtual void* c_gui_gametype_category_datasource::`scalar deleting destructor'(unsigned int)
//.text:00AEA050 ; public: virtual void* c_load_legal_screen_message::`vector deleting destructor'(unsigned int)
//.text:00AEA080 ; public: virtual void* c_start_menu_game_editor::`scalar deleting destructor'(unsigned int)
//.text:00AEA0F0 ; public: virtual bool c_start_menu_game_editor::allow_pane_tab_change()
//.text:00AEA100 ; public: virtual void c_load_legal_screen_message::apply_initial_state(c_gui_screen_widget*) const
//.text:00AEA120 ; protected: void c_start_menu_game_editor::begin_save_as_current_edited_variant(e_controller_index)
//.text:00AEA380 ; protected: void c_start_menu_game_editor::begin_save_current_edited_variant(e_controller_index)
//.text:00AEA510 ; public: virtual void c_gui_gametype_category_datasource::get_column_names(int32* const, int32*)
//.text:00AEA520 ; protected: virtual int32 c_gui_gametype_category_datasource::get_current_item_count_internal()
//.text:00AEA530 ; public: virtual bool c_gui_gametype_category_datasource::get_element(int32, void*, int32)
//.text:00AEA550 ; public: virtual bool c_gui_gametype_category_datasource::get_integer_value(int32, int32, int32*)
//.text:00AEA570 ; public: virtual bool c_gui_gametype_category_datasource::get_text_value(int32, int32, c_static_wchar_string<1024>*)
//.text:00AEA5E0 ; public: virtual bool c_start_menu_game_editor::handle_dialog_result(const c_dialog_result_message*)
//.text:00AEA7D0 ; public: virtual bool c_start_menu_game_editor::handle_list_item_chosen(const c_controller_input_message*, int32, c_gui_list_item_widget*, c_gui_data*)
//.text:00AEACE0 ; public: virtual bool c_gui_gametype_category_datasource::initialize(int32)
//.text:00AEAD30 ; public: virtual void c_start_menu_game_editor::initialize()
//.text:00AEADA0 ; public: virtual void c_start_menu_game_editor::initialize_datasource()

void c_start_menu_game_editor::load_progress_ui(e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x00AEB050, c_start_menu_game_editor, load_progress_ui, controller_index);
}

//.text:00AEB0F0 ; public: void c_start_menu_game_editor::post_save_map(const c_controller_input_message*)
//.text:00AEB1D0 ; public: void c_start_menu_game_editor::post_save_map_as(const c_controller_input_message*)
//.text:00AEB200 ; protected: static bool __cdecl c_start_menu_game_editor::saved_game_variant_file_already_exists(e_controller_index, const wchar_t*)
//.text:00AEB220 ; 
//.text:00AEB230 ; 
//.text:00AEB240 ; 

void c_start_menu_game_editor::setup_map_variant_for_write(s_blffile_map_variant* variant_on_disk, const wchar_t* map_name, const wchar_t* map_description, e_map_id map_id, const c_map_variant* map_variant)
{
	INVOKE_CLASS_MEMBER(0x00AEB250, c_start_menu_game_editor, setup_map_variant_for_write, variant_on_disk, map_name, map_description, map_id, map_variant);
}

//.text:00AEB2D0 ; public: virtual void c_start_menu_game_editor::submenu_invoked(c_gui_list_widget*)
//.text:00AEB320 ; public: virtual void c_start_menu_game_editor::update(uns32)

void c_start_menu_game_editor::update_save_as_new_operation()
{
	//INVOKE_CLASS_MEMBER(0x00AEB490, c_start_menu_game_editor, update_save_as_new_operation);

	const c_map_variant* map_variant = game_engine_get_runtime_map_variant();

	if (m_create_new_variant_task == INVALID_ASYNC_TASK_ID)
	{
		wchar_t map_description[128]{};
		//utf8_string_to_wchar_string(map_variant->m_metadata.description, map_description, NUMBEROF(map_description), NULL);
		ascii_string_to_wchar_string(map_variant->m_metadata.description, map_description, NUMBEROF(map_description), NULL);

		//wchar_t map_name[256]{};
		const wchar_t* map_name = L"Test Map"; // $TODO
		c_start_menu_game_editor::setup_map_variant_for_write(
			&m_variant_on_disk,
			map_name,
			map_description,
			map_variant->m_map_id,
			map_variant);

		m_variant_on_disk.content_header.metadata.unique_id = system_seconds();

		//wchar_t filename[256]{};
		const wchar_t* filename = L"bin\\test_map.map"; // $TODO

		m_create_new_variant_task = async_write_buffer_to_file(
			filename,
			&m_variant_on_disk,
			sizeof(m_variant_on_disk),
			0,
			_async_category_saved_games,
			_async_priority_important_non_blocking,
			&m_async_success,
			&m_async_complete);
		if (m_create_new_variant_task == INVALID_ASYNC_TASK_ID)
		{
			event(_event_error, "ui: failed to initiate async saved game file creation for map variant!");
		}
		else
		{
			c_start_menu_game_editor::load_progress_ui(m_asynchronous_operation_controller);
		}
	}
	else
	{
		if (m_create_new_variant_task != INVALID_ASYNC_TASK_ID)
		{
			c_start_menu_game_editor::load_progress_ui(m_asynchronous_operation_controller);
		}

		internal_async_yield_until_done(&m_async_complete, false, false, __FILE__, __LINE__);
		if (m_async_complete.peek() == 1)
		{
			m_asynchronous_operation = _operation_none;
			m_confirmation_action = _invalid_result;
			m_confirmation_result = _result_indeterminate;

			m_create_new_variant_task = INVALID_ASYNC_TASK_ID;
		}
	}
}

void c_start_menu_game_editor::update_save_operation()
{
	INVOKE_CLASS_MEMBER(0x00AEB9D0, c_start_menu_game_editor, update_save_operation);
}

