#include "interface/gui_screens/campaign/gui_screen_campaign_settings.hpp"

#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/gui_screens/campaign/gui_screen_campaign_select_skulls.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "saved_games/saved_game_files.hpp"

template<>
void ui_track_delete<c_gui_primary_skulls_data>(const c_gui_primary_skulls_data* object)
{
	ASSERT(object != NULL);

	object->~c_gui_primary_skulls_data();
	user_interface_free(object);
}

template<>
void ui_track_delete<c_gui_secondary_skulls_data>(const c_gui_secondary_skulls_data* object)
{
	ASSERT(object != NULL);

	object->~c_gui_secondary_skulls_data();
	user_interface_free(object);
}

c_gui_screen_campaign_settings::c_gui_screen_campaign_settings(int32 name) :
	c_gui_screen_widget(name)
{
}

c_gui_screen_campaign_settings::~c_gui_screen_campaign_settings()
{
}

bool c_gui_screen_campaign_settings::handle_controller_input_message(const c_controller_input_message* message)
{
	c_gui_list_widget* primary_skulls_list_widget = c_gui_widget::get_child_list_widget(STRING_ID(gui, primary_skulls));
	c_gui_list_widget* secondary_skulls_list_widget = c_gui_widget::get_child_list_widget(STRING_ID(gui, secondary_skulls));

	if (!c_gui_widget::get_child_list_widget(STRING_ID(gui, spinner)))
	{
		switch (message->get_event_type())
		{
		case _event_type_tab_up:
		{
			if (!primary_skulls_list_widget || !secondary_skulls_list_widget)
			{
				break;
			}

			if (secondary_skulls_list_widget->get_item_count(false) <= 0 || !primary_skulls_list_widget->within_focus_chain())
			{
				break;
			}

			int32 focused_item_index = primary_skulls_list_widget->get_focused_item_index();
			if (focused_item_index < k_campaign_skull_secondary_count)
			{
				break;
			}

			int16 new_item_index = focused_item_index % k_campaign_skull_secondary_count;
			VALID_INDEX(new_item_index, k_campaign_skull_secondary_count);

			c_gui_screen_campaign_settings::set_focus(_focus_state_secondary, new_item_index);

			return true;
		}
		break;
		case _event_type_tab_down:
		{
			if (!primary_skulls_list_widget || primary_skulls_list_widget->get_item_count(false) <= 5)
			{
				break;
			}

			if (!secondary_skulls_list_widget || !secondary_skulls_list_widget->within_focus_chain())
			{
				break;
			}

			int16 new_item_index = (int16)secondary_skulls_list_widget->get_focused_item_index();
			if (new_item_index >= k_campaign_skull_primary_count)
			{
				new_item_index -= 5;
			}

			ASSERT(VALID_INDEX(new_item_index, k_campaign_skull_primary_count));

			c_gui_screen_campaign_settings::set_focus(_focus_state_primary, new_item_index);

			return true;
		}
		break;
		}
	}

	return c_gui_screen_widget::handle_controller_input_message(message);
}

bool c_gui_screen_campaign_settings::handle_dialog_result(const c_dialog_result_message* message)
{
	if (message->get_dialog_name() == STRING_ID(gui_dialog, campaign_changing_setting_will_delete_saved_game))
	{
		if (message->get_dialog_result() == _gui_dialog_choice_first)
		{
			saved_game_files_delete_saved_game_state_blocking(message->get_controller());
		}

		return true;
	}

	return c_gui_screen_widget::handle_dialog_result(message);
}

bool c_gui_screen_campaign_settings::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return false;
}

void c_gui_screen_campaign_settings::initialize()
{
	c_gui_screen_widget::initialize();
	c_gui_screen_campaign_settings::set_focus(_focus_state_primary, _metagame_on);
}

void c_gui_screen_campaign_settings::initialize_datasource()
{
	c_gui_screen_widget::initialize_datasource();

	c_gui_primary_skulls_data* primary_skulls = UI_MALLOC(c_gui_primary_skulls_data);
	if (primary_skulls && primary_skulls->initialize(STRING_ID(gui, primary_skulls)))
	{
		c_gui_screen_widget::add_datasource(primary_skulls);
	}
	else
	{
		ui_track_delete<c_gui_primary_skulls_data>(primary_skulls);
	}
	
	c_gui_secondary_skulls_data* secondary_skulls = UI_MALLOC(c_gui_secondary_skulls_data);
	if (secondary_skulls && secondary_skulls->initialize(STRING_ID(gui, secondary_skulls)))
	{
		c_gui_screen_widget::add_datasource(secondary_skulls);
	}
	else
	{
		ui_track_delete<c_gui_secondary_skulls_data>(secondary_skulls);
	}
}

void c_gui_screen_campaign_settings::set_focus(e_focus_state new_state, int16 new_index)
{
	c_gui_text_widget* primary_status_text_widget = c_gui_widget::get_child_text_widget(STRING_ID(gui, primary_status));
	c_gui_text_widget* secondary_status_text_widget = c_gui_widget::get_child_text_widget(STRING_ID(gui, secondary_status));
	
	c_gui_text_widget* primary_skull_description_text_widget = c_gui_widget::get_child_text_widget(STRING_ID(gui, primary_skull_description));
	c_gui_text_widget* primary_skull_name_text_widget = c_gui_widget::get_child_text_widget(STRING_ID(gui, primary_skull_name));
	c_gui_bitmap_widget* primary_skulls_bitmap_widget = c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, primary_skulls));

	c_gui_text_widget* secondary_skull_description_text_widget = c_gui_widget::get_child_text_widget(STRING_ID(gui, secondary_skull_description));
	c_gui_text_widget* secondary_skull_name_text_widget = c_gui_widget::get_child_text_widget(STRING_ID(gui, secondary_skull_name));
	c_gui_bitmap_widget* secondary_skulls_bitmap_widget = c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, secondary_skulls));
	
	c_gui_bitmap_widget* main_skull_bitmap_widget = c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, main_skull));

	bool primary_visible = false;
	bool secondary_visible = false;

	c_gui_list_widget* chosen_list = NULL;

	switch (new_state)
	{
	case _focus_state_primary:
	{
		chosen_list = c_gui_widget::get_child_list_widget(STRING_ID(gui, primary_skulls));
		primary_visible = true;
		secondary_visible = false;
	}
	break;
	case _focus_state_secondary:
	{
		chosen_list = c_gui_widget::get_child_list_widget(STRING_ID(gui, secondary_skulls));
		secondary_visible = true;
	}
	break;
	}

	if (chosen_list)
	{
		VALID_INDEX(new_index, chosen_list->get_item_count(false));
		c_gui_screen_widget::transfer_focus_to_list(chosen_list, new_index, false, false);
	}

	if (primary_status_text_widget)
	{
		primary_status_text_widget->set_visible(primary_visible);
	}
	if (primary_skull_description_text_widget)
	{
		primary_skull_description_text_widget->set_visible(primary_visible);
	}
	if (primary_skull_name_text_widget)
	{
		primary_skull_name_text_widget->set_visible(primary_visible);
	}
	if (primary_skulls_bitmap_widget)
	{
		primary_skulls_bitmap_widget->set_visible(primary_visible);
	}

	if (secondary_status_text_widget)
	{
		secondary_status_text_widget->set_visible(secondary_visible);
	}
	if (secondary_skull_description_text_widget)
	{
		secondary_skull_description_text_widget->set_visible(secondary_visible);
	}
	if (secondary_skull_name_text_widget)
	{
		secondary_skull_name_text_widget->set_visible(secondary_visible);
	}
	if (secondary_skulls_bitmap_widget)
	{
		secondary_skulls_bitmap_widget->set_visible(secondary_visible);
	}

	if (main_skull_bitmap_widget)
	{
		main_skull_bitmap_widget->set_visible(primary_visible || secondary_visible);
	}
}

void c_gui_screen_campaign_settings::update(uns32 current_milliseconds)
{
	c_gui_screen_widget::update(current_milliseconds);

	c_gui_list_widget* primary_skulls_list_widget = c_gui_widget::get_child_list_widget(STRING_ID(gui, primary_skulls));
	c_gui_data* primary_skulls_list_data = NULL;
	if (primary_skulls_list_widget)
	{
		primary_skulls_list_data = primary_skulls_list_widget->get_data();
	}

	if (primary_skulls_list_widget && primary_skulls_list_data)
	{
		for (c_gui_list_item_widget* child_widget = (c_gui_list_item_widget*)primary_skulls_list_widget->get_first_child_widget_by_type(_gui_list_item);
			child_widget;
			child_widget = child_widget->get_next_list_item_widget(true))
		{
			int32 state = 0;
			bool use_alternate_ambient_state = !primary_skulls_list_data->get_integer_value(child_widget->get_element_handle(), STRING_ID(gui, primary_skull_state), &state) || (state & ~FLAG(_skull_enabled)) == 0;
			child_widget->set_use_alternate_ambient_state(true);
		}
	}

	c_gui_list_widget* secondary_skulls_list_widget = c_gui_widget::get_child_list_widget(STRING_ID(gui, secondary_skulls));
	c_gui_data* secondary_skulls_list_data = NULL;
	if (secondary_skulls_list_widget)
	{
		secondary_skulls_list_data = secondary_skulls_list_widget->get_data();
	}

	if (secondary_skulls_list_widget && secondary_skulls_list_data)
	{
		for (c_gui_list_item_widget* child_widget = (c_gui_list_item_widget*)secondary_skulls_list_widget->get_first_child_widget_by_type(_gui_list_item);
			child_widget;
			child_widget = child_widget->get_next_list_item_widget(true))
		{
			int32 state = 0;
			bool use_alternate_ambient_state = !secondary_skulls_list_data->get_integer_value(child_widget->get_element_handle(), STRING_ID(gui, secondary_skull_state), &state) || (state & ~FLAG(_skull_enabled)) == 0;
			child_widget->set_use_alternate_ambient_state(true);
		}
	}

	c_gui_data* scoring_option_list_data = NULL;
	c_gui_list_widget* scoring_option_list_widget = c_gui_widget::get_child_list_widget(STRING_ID(gui, scoring_option));
	if (scoring_option_list_widget)
	{
		scoring_option_list_data = scoring_option_list_widget->get_data();
	}

	c_gui_list_widget* spinner_list_widget = c_gui_widget::get_child_list_widget(STRING_ID(gui, spinner));
	if (spinner_list_widget && spinner_list_widget->get_visible())
	{
		scoring_option_list_widget->set_focused_element_handle(spinner_list_widget->get_focused_element_handle(), false);
		return;
	}

	e_campaign_metagame_scoring metagame_scoring = user_interface_game_settings_get_campaign_metagame_scoring();
	if (scoring_option_list_widget && scoring_option_list_data &&
		VALID_INDEX(metagame_scoring, scoring_option_list_data->get_current_item_count()))
	{
		scoring_option_list_widget->set_focused_element_handle(metagame_scoring, false);
	}
}

bool c_gui_screen_campaign_settings::warn_about_loss_of_saved_game_progress(e_controller_index controller_index)
{
	if (!saved_game_files_controller_has_saved_game_state_blocking(controller_index) || user_interface_squad_get_player_count() != 1)
	{
		return false;
	}

	c_load_dialog_screen_message* screen_message = UI_MALLOC(c_load_dialog_screen_message,
		controller_index,
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

