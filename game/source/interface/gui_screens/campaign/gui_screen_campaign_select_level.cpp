#include "interface/gui_screens/campaign/gui_screen_campaign_select_level.hpp"

#include "input/controllers.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/gui_custom_bitmap_widget.hpp"
#include "interface/gui_pregame_setup_manager.hpp"
#include "interface/gui_selected_items_level.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "main/levels.hpp"
#include "memory/module.hpp"
#include "saved_games/saved_game_files.hpp"
#include "text/text_group.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AFD9B0, c_gui_screen_campaign_select_level, post_initialize_);
HOOK_DECLARE_CLASS_MEMBER(0x00AFD600, c_gui_screen_campaign_select_level, initialize_datasource_);
HOOK_DECLARE_CLASS_MEMBER(0x00AFCF90, c_gui_screen_campaign_select_level, handle_list_item_chosen_);

void __thiscall c_gui_screen_campaign_select_level::post_initialize_()
{
	c_gui_screen_campaign_select_level::post_initialize();
}

void __thiscall c_gui_screen_campaign_select_level::initialize_datasource_()
{
	c_gui_screen_campaign_select_level::initialize_datasource();
}

bool __thiscall c_gui_screen_campaign_select_level::handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return c_gui_screen_campaign_select_level::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

c_gui_screen_campaign_select_level::c_gui_screen_campaign_select_level(int32 name) :
	c_gui_screen_widget(name),
	m_campaign_setup_mode(_campaign_level_setup_mode_replay_level),
	m_campaign_id(_campaign_id_none),
	m_map_id(_map_id_none),
	m_campaign_insertion_point(0),
	m_controller_index(k_no_controller),
	m_saved_game(false),
	m_coop_saved_game(false)
{
	//DECLFUNC(0x00AFCA00, void, __thiscall, c_gui_screen_campaign_select_level*, int32)(this, name);
}

//.text:00AFCAE0 ; public: virtual c_gui_screen_campaign_select_level::~c_gui_screen_campaign_select_level()
//.text:00AFCBA0 ; public: virtual void* c_gui_screen_campaign_select_level::`vector deleting destructor'(unsigned int)

c_gui_bitmap_widget* c_gui_screen_campaign_select_level::create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00AFCC00, c_gui_screen_campaign_select_level, create_bitmap_widget, definition);

	c_gui_bitmap_widget* bitmap_widget = nullptr;
	if (definition->widget_identifier == STRING_ID(gui, level_image))
	{
		bitmap_widget = new (_ui_allocation_marker_dummy) c_gui_custom_bitmap_widget();
	}
	else
	{
		bitmap_widget = c_gui_widget::create_bitmap_widget(definition);
	}
	return bitmap_widget;
}

bool c_gui_screen_campaign_select_level::handle_controller_input_message(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AFCE00, c_gui_screen_campaign_select_level, handle_controller_input_message, message);

	return c_gui_screen_widget::handle_controller_input_message(message);
}

bool c_gui_screen_campaign_select_level::handle_dialog_result(const c_dialog_result_message* dialog_result_message)
{
	//return INVOKE_CLASS_MEMBER(0x00AFCF20, c_gui_screen_campaign_select_level, handle_dialog_result, dialog_result_message);

	bool handled = false;

	if (dialog_result_message->get_dialog_name() == STRING_ID(gui_dialog, campaign_changing_setting_will_delete_saved_game))
	{
		if (dialog_result_message->get_dialog_result() == k_gui_dialog_choice_ok)
		{
			saved_game_files_delete_saved_game_state_blocking(dialog_result_message->get_controller());
			c_gui_screen_campaign_select_level::level_chosen_immediate(dialog_result_message->get_controller());
		}

		handled = true;
	}

	if (!handled)
	{
		handled = c_gui_screen_widget::handle_dialog_result(dialog_result_message);
	}
	return handled;
}

bool c_gui_screen_campaign_select_level::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00AFCF90, c_gui_screen_campaign_select_level, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	switch (list_name)
	{
	case STRING_ID(global, level):
	{
		c_gui_data* level_data = c_gui_screen_widget::get_data(STRING_ID(global, level), nullptr);
		c_gui_level_selected_item* level_selected_item = (c_gui_level_selected_item*)level_data->get_gui_selected_item(list_item_widget->get_element_handle());
		if (level_selected_item->m_map_id == _map_id_none)
		{
			return true;
		}

		c_gui_insertion_point_data* insertion_point_data = (c_gui_insertion_point_data*)c_gui_screen_widget::get_data(STRING_ID(gui, insertion_point), nullptr);
		if (insertion_point_data->set_campaign_level_id(level_selected_item->m_campaign_id, level_selected_item->m_map_id) <= 0)
		{
			c_gui_screen_campaign_select_level::level_chosen_immediate(message->get_controller());
			return true;
		}

		if (list_item_widget->get_parent_list()->invoke_submenu(STRING_ID(gui, insertion_point), STRING_ID(gui, insertion_point), false))
		{
			c_gui_screen_widget::transfer_focus_to_list(c_gui_widget::get_child_list_widget(STRING_ID(gui, insertion_point)), false, false, false);
		}

		return true;
	}
	break;
	case STRING_ID(gui, insertion_point):
	{
		e_controller_index controller_index = message->get_controller();
		ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));

		bool setting_up_lobby = m_campaign_setup_mode == _campaign_level_setup_mode_lobby && user_interface_squad_get_player_count() == 1;
		if (!setting_up_lobby || !saved_game_files_controller_has_saved_game_state_blocking(message->get_controller()))
		{
			c_gui_screen_campaign_select_level::level_chosen_immediate(message->get_controller());
			return true;
		}

		c_load_dialog_screen_message* screen_message = new (_ui_allocation_marker_dummy) c_load_dialog_screen_message(
			message->get_controller(),
			c_gui_screen_widget::get_render_window(),
			m_name,
			STRING_ID(gui_dialog, campaign_changing_setting_will_delete_saved_game),
			m_name);
		if (screen_message)
		{
			screen_message->set_parent_screen_index(m_screen_index);
			user_interface_messaging_post(screen_message);
		}
		return true;
	}
	break;
	}

	return c_gui_screen_widget::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

void c_gui_screen_campaign_select_level::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AFD180, c_gui_screen_campaign_select_level, initialize);

	c_gui_screen_widget::initialize();
}

void c_gui_screen_campaign_select_level::initialize_datasource()
{
	//INVOKE_CLASS_MEMBER(0x00AFD600, c_gui_screen_campaign_select_level, initialize_datasource);

	c_gui_screen_widget::initialize_datasource();

	c_gui_level_subitem_selectable_item_datasource* level = new (_ui_allocation_marker_dummy) c_gui_level_subitem_selectable_item_datasource();
	if (level && level->initialize(c_gui_widget::get_single_responding_controller(), STRING_ID(global, level), m_campaign_id))
	{
		c_gui_screen_widget::add_datasource(level);
	}

	c_gui_insertion_point_data* insertion_point = new (_ui_allocation_marker_dummy) c_gui_insertion_point_data(
		c_gui_screen_widget::get_multilingual_unicode_string_list_index());
	if (insertion_point && insertion_point->initialize(STRING_ID(gui, insertion_point)))
	{
		c_gui_screen_widget::add_datasource(insertion_point);
	}
}

void c_gui_screen_campaign_select_level::level_chosen_immediate(e_controller_index controller_index)
{
	//INVOKE_CLASS_MEMBER(0x00AFD6F0, c_gui_screen_campaign_select_level, level_chosen_immediate, controller_index);

	c_gui_data* level_data = c_gui_screen_widget::get_data(STRING_ID(global, level), nullptr);
	c_gui_list_widget* level_list_widget = get_child_list_widget(STRING_ID(global, level));
	if (!level_data || !level_list_widget)
	{
		return;
	}

	c_gui_level_selected_item* item = (c_gui_level_selected_item*)level_data->get_gui_selected_item(level_list_widget->get_focused_element_handle());
	if (!item)
	{
		return;
	}

	c_gui_level_selected_item level_item = *item;
	//ASSERT(item->get_selection_type() == _gui_selection_type_level);
	ASSERT(item->m_selection_type == _gui_selection_type_level);

	c_gui_list_widget* insertion_point_list_widget = get_child_list_widget(STRING_ID(gui, insertion_point));
	if (insertion_point_list_widget)
	{
		level_item.m_insertion_point = (int16)insertion_point_list_widget->get_focused_element_handle();
	}
	else
	{
		level_item.m_insertion_point = 0;
	}

	switch (m_campaign_setup_mode)
	{
	case _campaign_level_setup_mode_replay_level:
	{
		c_controller_interface* controller = controller_get(controller_index);
		if (controller)
		{
			return;
		}
		c_player_profile_interface* player_profile = controller->get_player_profile_interface();
		if (!player_profile)
		{
			return;
		}

		c_load_campaign_select_difficulty_screen_message* difficulty_screen_message = new (_ui_allocation_marker_dummy) c_load_campaign_select_difficulty_screen_message(
			STRING_ID(gui, campaign_select_difficulty),
			controller_index,
			c_gui_screen_widget::get_render_window(),
			m_name,
			_campaign_difficulty_setup_mode_replay_level,
			level_item.m_campaign_id,
			level_item.m_map_id,
			player_profile->get_campaign_difficulty());
		if (difficulty_screen_message)
		{
			difficulty_screen_message->set_parent_screen_index(m_screen_index);
			user_interface_messaging_post(difficulty_screen_message);
		}
	}
	break;
	case _campaign_level_setup_mode_lobby:
	{
		c_gui_pregame_setup_manager::get()->set_selected_item(controller_index, &level_item, false, NONE);
		c_gui_screen_widget::transition_out(_transition_out_normal);
	}
	break;
	}
}

//.text:00AFD850 ; bool __cdecl parse_xml_last_played_time(void*, wchar_t*, int32)

void c_gui_screen_campaign_select_level::post_initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AFD9B0, c_gui_screen_campaign_select_level, post_initialize);

	c_gui_screen_widget::post_initialize();

	c_gui_data* level_data = c_gui_screen_widget::get_data(STRING_ID(global, level), nullptr);
	c_gui_list_widget* level_list_widget = get_child_list_widget(STRING_ID(global, level));
	if (!level_list_widget || !level_data)
	{
		return;
	}

	int32 level_element_handle = NONE;
	for (int32 element_handle = level_data->get_first_element_handle();
		element_handle != NONE;
		element_handle = level_data->get_next_element_handle(element_handle))
	{
		const c_gui_selected_item* selected_item = level_data->get_gui_selected_item(element_handle);
		if (!selected_item || selected_item->m_selection_type != _gui_selection_type_level)
		{
			continue;
		}

		c_gui_level_selected_item* level_selected_item = (c_gui_level_selected_item*)selected_item;
		if (level_selected_item->m_campaign_id == m_campaign_id &&
			level_selected_item->m_map_id == m_map_id)
		{
			level_element_handle = element_handle;
			break;
		}
	}

	if (level_element_handle == NONE)
	{
		return;
	}

	c_gui_screen_widget::transfer_focus_to_list(level_list_widget, level_element_handle, false, false);
}


//.text:00AFDA80 ; 
//.text:00AFDB80 ; 

void c_gui_screen_campaign_select_level::setup(e_gui_campaign_level_setup_mode campaign_setup_mode, e_campaign_id campaign_id, e_map_id map_id, int16 campaign_insertion_point)
{
	//INVOKE_CLASS_MEMBER(0x00AFDBA0, c_gui_screen_campaign_select_level, setup, campaign_setup_mode, campaign_id, map_id, campaign_insertion_point);

	m_campaign_setup_mode = campaign_setup_mode;
	m_campaign_id = campaign_id;
	m_map_id = map_id;
	m_campaign_insertion_point = campaign_insertion_point;
}

//.text:00AFDBD0 ; 

void c_gui_screen_campaign_select_level::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00AFDC50, c_gui_screen_campaign_select_level, update, current_milliseconds);
}

//.text:00AFDED0 ; 
//.text:00AFE0A0 ; 
//.text:00AFE160 ; 
//.text:00AFE1E0 ; 

c_gui_insertion_point_data::c_gui_insertion_point_data(int32 string_list_index) :
	c_gui_ordered_data(),
	m_insertion_point_names(),
	m_insertion_point_descriptions(),
	m_insertion_point_count(0),
	m_insertion_point_unlocked(1),
	m_string_list_index(string_list_index)
{
}

void c_gui_insertion_point_data::get_column_names(string_id* const column_names, int32* column_count)
{
	*column_count = 0;
	column_names[(*column_count)++] = STRING_ID(gui, insertion_point);
}

int32 c_gui_insertion_point_data::get_current_item_count_internal()
{
	return m_insertion_point_count;
}

bool c_gui_insertion_point_data::get_text_value(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* buffer)
{
	if (!VALID_INDEX(element_handle, m_insertion_point_count))
	{
		return false;
	}

	switch (value_name)
	{
	case STRING_ID(gui, insertion_point):
	{
		if (!TEST_BIT(m_insertion_point_unlocked, element_handle))
		{
			return string_list_get_interface_string(m_string_list_index, STRING_ID(gui, insertion_point_locked), buffer);
		}

		buffer->set(m_insertion_point_names[element_handle]);
		return true;
	}
	break;
	case STRING_ID(gui, insertion_point_description):
	{
		if (TEST_BIT(m_insertion_point_unlocked, element_handle))
		{
			buffer->set(m_insertion_point_descriptions[element_handle]);
		}
		else
		{
			string_list_get_interface_string(m_string_list_index, STRING_ID(gui, insertion_point_description_locked), buffer);
		}
		return true;
	}
	break;
	}

	return false;
}

bool c_gui_insertion_point_data::initialize(string_id name)
{
	m_insertion_point_count = 0;
	m_insertion_point_unlocked = 1;
	return c_gui_data::initialize(name);
}

int16 c_gui_insertion_point_data::set_campaign_level_id(e_campaign_id campaign_id, e_map_id map_id)
{
	int16 insertion_point_count = 0;

	s_level_insertion_datum insertion{};
	if (levels_try_and_get_campaign_insertion(map_id, &insertion) && map_id != _map_id_none)
	{
		m_insertion_point_unlocked = 0;
		m_insertion_point_count = insertion.insertion_point_count;
		for (int32 insertion_point_index = 0; insertion_point_index < m_insertion_point_count; insertion_point_index++)
		{
			ustrnzcpy(m_insertion_point_names[insertion_point_index], insertion.insertion_point_names[insertion_point_index], NUMBEROF(m_insertion_point_names[0]));
			ustrnzcpy(m_insertion_point_descriptions[insertion_point_index], insertion.insertion_point_descriptions[insertion_point_index], NUMBEROF(m_insertion_point_descriptions[0]));
		}

		int16 campaign_level_index = levels_get_campaign_level_index(campaign_id, map_id);

		for (e_controller_index controller_index = first_controller(); controller_index != k_no_controller; controller_index = next_controller(controller_index))
		{
			c_controller_interface* controller = controller_get(controller_index);
			if (controller->in_use())
			{
				c_player_profile_interface* player_profile = controller->get_player_profile_interface();
				for (int16 insertion_point_index = 0; insertion_point_index < m_insertion_point_count; insertion_point_index++)
				{
					//if (!TEST_BIT(m_insertion_point_unlocked, insertion_point_index) &&
					//	(player_profile->player_campaign_progress_test_insertion_point(campaign_level_index, insertion_point_index) || !insertion_point_index))
					//{
						SET_BIT(m_insertion_point_unlocked, insertion_point_index, true);
						insertion_point_count++;
					//}
				}
			}
		}
	}

	return insertion_point_count;
}

void c_gui_insertion_point_data::update()
{
}

