#include "interface/gui_screens/start_menu/panes/game_editor/start_menu_game_editor.hpp"

#include "cseries/async_helpers.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game_engine_team.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/module.hpp"
#include "saved_games/scenario_map_variant.hpp"
#include "networking/online/online_guide_pc.hpp"
#include "interface/user_interface_controller.hpp"

// $TODO be better
constexpr const wchar_t* k_map_name = L"Test Map";
constexpr const wchar_t* k_map_filename = L"test_map";

struct controller_interface_class // c_controller_interface
{
	bool __thiscall storage_device_valid()
	{
		return true;
	}
};
static void* storage_device_valid_for_save_map_as = member_to_static_function(&controller_interface_class::storage_device_valid);
HOOK_DECLARE_CALL(0x00AEAAC8, storage_device_valid_for_save_map_as);

HOOK_DECLARE_CLASS_MEMBER(0x00AEB490, c_start_menu_game_editor, update_save_as_new_operation_);

void __thiscall c_start_menu_game_editor::update_save_as_new_operation_()
{
	c_start_menu_game_editor::update_save_as_new_operation();
}

//.text:00AE9D70 ; 
//.text:00AE9DA0 ; 
//.text:00AE9DD0 ; 
//.text:00AE9DF0 ; public: c_load_legal_screen_message::c_load_legal_screen_message(e_controller_index, e_window_index, int32, c_start_menu_game_editor*, bool)

c_start_menu_game_editor::c_start_menu_game_editor(int32 name) :
	c_start_menu_pane_screen_widget(name),
	m_asynchronous_operation(_operation_none),
	m_confirmation_action(_invalid_result),
	m_confirmation_result(_result_indeterminate),
	m_variant_on_disk(),
	m_asynchronous_operation_controller(k_no_controller),
	m_keyboard_task(c_virtual_keyboard_task::get_instance(__FILE__, __LINE__, k_no_controller, nullptr, nullptr, nullptr, NUMBEROF(m_virtual_keyboard_results_buffer), 0, true)),
	m_virtual_keyboard_results_buffer(),
	m_multiplayer_setup(),
	m_create_new_variant_task(INVALID_ASYNC_TASK_ID),
	m_write_new_variant_task(INVALID_ASYNC_TASK_ID),
	m_write_variant_task(INVALID_ASYNC_TASK_ID),
	m_async_success(),
	m_async_complete(),
	m_content_item_index(NONE),
	m_progress_dialog_start_time_milliseconds(0)

{
	//DECLFUNC(0x00AE9E30, void, __thiscall, c_start_menu_game_editor*, int32)(this, name);

	set_automatic_button_key(STRING_ID(gui, a_confirm_b_cancel));
}

//.text:00AE9F90 ; 
//.text:00AE9FA0 ; 
//.text:00AE9FB0 ; public: virtual c_start_menu_game_editor::~c_start_menu_game_editor()
//.text:00AEA010 ; 
//.text:00AEA020 ; public: virtual void* c_gui_gametype_category_datasource::`scalar deleting destructor'(unsigned int)
//.text:00AEA050 ; public: virtual void* c_load_legal_screen_message::`vector deleting destructor'(unsigned int)
//.text:00AEA080 ; public: virtual void* c_start_menu_game_editor::`scalar deleting destructor'(unsigned int)

bool c_start_menu_game_editor::allow_pane_tab_change()
{
	//return INVOKE_CLASS_MEMBER(0x00AEA0F0, c_start_menu_game_editor, allow_pane_tab_change);

	return true;
}

//.text:00AEA100 ; public: virtual void c_load_legal_screen_message::apply_initial_state(c_gui_screen_widget*) const
//.text:00AEA120 ; protected: void c_start_menu_game_editor::begin_save_as_current_edited_variant(e_controller_index)
//.text:00AEA380 ; protected: void c_start_menu_game_editor::begin_save_current_edited_variant(e_controller_index)
//.text:00AEA510 ; public: virtual void c_gui_gametype_category_datasource::get_column_names(int32* const, int32*)
//.text:00AEA520 ; protected: virtual int32 c_gui_gametype_category_datasource::get_current_item_count_internal()
//.text:00AEA530 ; public: virtual bool c_gui_gametype_category_datasource::get_element(int32, void*, int32)
//.text:00AEA550 ; public: virtual bool c_gui_gametype_category_datasource::get_integer_value(int32, int32, int32*)
//.text:00AEA570 ; public: virtual bool c_gui_gametype_category_datasource::get_text_value(int32, int32, c_static_wchar_string<1024>*)

bool c_start_menu_game_editor::handle_dialog_result(const c_dialog_result_message* dialog_result_message)
{
	return INVOKE_CLASS_MEMBER(0x00AEA5E0, c_start_menu_game_editor, handle_dialog_result, dialog_result_message);
}

bool c_start_menu_game_editor::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return INVOKE_CLASS_MEMBER(0x00AEA7D0, c_start_menu_game_editor, handle_list_item_chosen, message, list_name, list_item_widget, datasource);
}

//.text:00AEACE0 ; public: virtual bool c_gui_gametype_category_datasource::initialize(int32)

void c_start_menu_game_editor::initialize()
{
	INVOKE_CLASS_MEMBER(0x00AEAD30, c_start_menu_game_editor, initialize);
}

void c_start_menu_game_editor::initialize_datasource()
{
	INVOKE_CLASS_MEMBER(0x00AEADA0, c_start_menu_game_editor, initialize_datasource);
}

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
	//INVOKE_CLASS_MEMBER(0x00AEB250, c_start_menu_game_editor, setup_map_variant_for_write, variant_on_disk, map_name, map_description, map_id, map_variant);

	e_controller_index controller_index = c_gui_widget::get_single_responding_controller();
	uns64 unique_id = s_saved_game_item_metadata::generate_unique_id();

	variant_on_disk->content_header.metadata.initialize(controller_index, _saved_game_map_variant, map_name, map_description, unique_id, sizeof(s_blffile_map_variant));
	map_variant->save_to(&variant_on_disk->variant.map_variant);
	variant_on_disk->content_header.metadata.map_id = map_id;
	variant_on_disk->variant.map_variant.validate_for_all_engines();
	variant_on_disk->variant.map_variant.quantize();
	variant_on_disk->variant.map_variant.set_metadata(&variant_on_disk->content_header.metadata);
	variant_on_disk->variant.map_variant.m_map_id = map_id;
}

void c_start_menu_game_editor::submenu_invoked(c_gui_list_widget* submenu_widget)
{
	//INVOKE_CLASS_MEMBER(0x00AEB2D0, c_start_menu_game_editor, submenu_invoked, submenu_widget);

	ASSERT(VALID_INDEX(get_single_responding_controller(), k_number_of_controllers));

	c_gui_screen_widget::submenu_invoked(submenu_widget);

	int32 value_to_set = NONE;

	switch (submenu_widget->m_name)
	{
	case STRING_ID(global, change_teams):
		value_to_set = user_interface_controller_get_team_index(c_gui_widget::get_single_responding_controller());
		break;
	}

	if (value_to_set != NONE)
	{
		submenu_widget->set_focused_datasource_element_from_value(STRING_ID(global, value), value_to_set, false);
	}
}

void c_start_menu_game_editor::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00AEB320, c_start_menu_game_editor, update, current_milliseconds);

	c_gui_ordered_data* datasource = (c_gui_ordered_data*)c_gui_screen_widget::get_data(STRING_ID(gui, teams), nullptr);
	if (datasource)
	{
		const c_game_variant* variant = user_interface_game_settings_get_game_variant();

		e_map_id map_id = _map_id_none;
		e_campaign_id campaign_id = _campaign_id_none;
		if (user_interface_session_get_map(&campaign_id, &map_id) && variant)
		{
			ASSERT(campaign_id == _campaign_id_none);
			ASSERT(map_id != _map_id_none);

			datasource->clear_disabled_elements();

			uns16 teams_allowed = game_engine_get_available_teams();
			for (int32 element_handle = datasource->get_first_element_handle(); element_handle != NONE; element_handle = datasource->get_next_element_handle(element_handle))
			{
				e_game_team team_index;
				if (datasource->get_integer_value(element_handle, STRING_ID(global, value), (int32*)&team_index) && !TEST_BIT(teams_allowed, team_index))
				{
					int32 value = NONE;
					if (datasource->get_string_id_value(element_handle, STRING_ID(global, name), &value))
					{
						datasource->set_disabled_element(STRING_ID(global, name), value);
					}
				}
			}
		}
	}

	switch (m_asynchronous_operation)
	{
	case _operation_none:
	{
		c_gui_screen_widget* in_progress_screen = (c_gui_screen_widget*)c_gui_widget::get_child_widget(_gui_screen, STRING_ID(gui, in_progress));
		if (in_progress_screen && !in_progress_screen->transitioning_out())
		{
			uns32 milliseconds_elapsed = user_interface_milliseconds() - m_progress_dialog_start_time_milliseconds;
			if (milliseconds_elapsed >= 3000)
			{
				in_progress_screen->transition_out(_transition_out_normal);
			}
		}
	}
	break;
	case _operation_save:
	{
		c_start_menu_game_editor::update_save_operation();
	}
	break;
	case _operation_save_as_new:
	{
		c_start_menu_game_editor::update_save_as_new_operation();
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}

	c_start_menu_pane_screen_widget::update(current_milliseconds);
}

void c_start_menu_game_editor::update_save_as_new_operation()
{
	//INVOKE_CLASS_MEMBER(0x00AEB490, c_start_menu_game_editor, update_save_as_new_operation);

	const c_map_variant* map_variant = game_engine_get_runtime_map_variant();

	if (m_create_new_variant_task == INVALID_ASYNC_TASK_ID)
	{
		wchar_t map_description[128]{};
		//utf8_string_to_wchar_string(map_variant->m_metadata.description, map_description, NUMBEROF(map_description), nullptr);
		ascii_string_to_wchar_string(map_variant->m_metadata.description, map_description, NUMBEROF(map_description), nullptr);

		wchar_t map_name[256]{};
		ustrnzcpy(map_name, k_map_name, NUMBEROF(map_name));

		c_start_menu_game_editor::setup_map_variant_for_write(&m_variant_on_disk, map_name, map_description, map_variant->m_map_id, map_variant);

		wchar_t filename[256]{};
		usnzprintf(filename, NUMBEROF(filename), L"map_variants\\_saved\\%s\\", map_name, k_map_filename);
		{
			s_file_reference file{};
			file_reference_create_from_path_wide(&file, filename, true);
			file_create_parent_directories_if_not_present(&file);
		}
		ustrnzcat(filename, L"sandbox.map", NUMBEROF(filename));

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

		s_gui_game_setup_storage* last_game_setup = global_preferences_get_last_game_setup();
		s_gui_single_game_setup_storage single_game_setup{};
		single_game_setup.game_mode = _gui_game_setup_mode_mapeditor;
		single_game_setup.mapeditor = last_game_setup->map_editor_settings;
		ustrnzcpy(single_game_setup.mapeditor.map_variant_settings.only_local.file_path, filename, NUMBEROF(single_game_setup.mapeditor.map_variant_settings.only_local.file_path));
		single_game_setup.mapeditor.map_variant_settings.only_local.valid = true;
		single_game_setup.mapeditor.map_variant_settings.only_local.location = _gui_stored_item_location_saved_game_file;
		single_game_setup.mapeditor.dirtied_in_game = true;
		map_variant->save_to(&single_game_setup.mapeditor.map_variant_settings.variant);
		single_game_setup.mapeditor.map_variant_settings.variant.validate_for_all_engines();
		single_game_setup.mapeditor.map_variant_settings.variant.quantize();
		global_preferences_update_last_game_setup(&single_game_setup);
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

