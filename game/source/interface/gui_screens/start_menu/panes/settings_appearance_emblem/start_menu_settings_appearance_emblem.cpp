#include "interface/gui_screens/start_menu/panes/settings_appearance_emblem/start_menu_settings_appearance_emblem.hpp"

#include "cseries/cseries_events.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_utilities.hpp"

c_start_menu_settings_appearance_emblem::c_start_menu_settings_appearance_emblem(int32 name) :
	c_start_menu_pane_screen_widget(name)
{
}

bool c_start_menu_settings_appearance_emblem::handle_controller_input_message(const c_controller_input_message* message)
{
	e_event_type event_type = message->get_event_type();
	e_controller_component component = message->get_component();
	e_controller_index controller_index = message->get_controller();

	switch (event_type)
	{
	case _event_type_button_press:
	{
		switch (component)
		{
		case _controller_component_button_b:
		{
			c_start_menu_pane_screen_widget::close_current_subpane();
			return true;
		}
		break;
		case _controller_component_button_x:
		{
			player_emblem_toggle_alternate_emblem_foreground(controller_index);
			return true;
		}
		break;
		case _controller_component_button_left_thumb:
		case _controller_component_button_right_thumb:
			return true;
		}
	}
	break;
	case _event_type_tab_left:
	case _event_type_tab_right:
	{
		c_gui_widget* focused_widget = c_gui_screen_widget::get_focused_widget();
		if (!focused_widget)
		{
			break;
		}

		c_gui_list_widget* parent_list = focused_widget->get_parent_list();
		if (!parent_list)
		{
			break;
		}

		c_gui_list_widget* list = event_type == _event_type_tab_left ? parent_list->get_previous_list_widget() : parent_list->get_next_list_widget();
		if (!list)
		{
			break;
		}

		c_gui_list_item_widget* child_list_item = list->try_and_get_focused_child_list_item_widget();
		if (!child_list_item)
		{
			break;
		}

		c_gui_screen_widget::transfer_focus(child_list_item);
		return true;
	}
	}


	return c_start_menu_pane_screen_widget::handle_controller_input_message(message);
}

bool c_start_menu_settings_appearance_emblem::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	ASSERT(message != NULL);
	ASSERT(list_item_widget != NULL);
	ASSERT(datasource != NULL);

	switch (list_name)
	{
	case STRING_ID(gui, foreground_emblem):
	case STRING_ID(gui, background_emblem):
	{
		int32 emblem_index = NONE;
		if (!datasource->get_integer_value(list_item_widget->get_element_handle(), STRING_ID(gui, image_index), &emblem_index))
		{
			event(_event_warning, "ui: datasource doesn't export integer 'image_index', unable to set emblem choice!");
			return true;
		}

		if (emblem_index < 0)
		{
			event(_event_warning, "ui: datasource exported invalid integer value for 'image_index', unable to set emblem choice!");
			return true;
		}

		e_controller_index controller_index = c_gui_widget::get_single_responding_controller();
		c_controller_interface* controller = controller_get(controller_index);
		c_player_profile_interface* player_profile = controller->get_player_profile_interface();
		s_emblem_info emblem_info = player_profile->get_emblem_info();

		if (list_name == STRING_ID(gui, foreground_emblem))
		{
			emblem_info.foreground_emblem_index = (uns8)emblem_index;
		}
		else
		{
			emblem_info.background_emblem_index = (uns8)emblem_index;
		}

		player_profile->set_emblem_info(&emblem_info, true);

		return true;
	}
	break;
	}

	return c_start_menu_pane_screen_widget::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

void c_start_menu_settings_appearance_emblem::post_initialize()
{
	c_gui_screen_widget::post_initialize();

	c_gui_list_widget* foreground_emblem_list = c_gui_widget::get_child_list_widget(STRING_ID(gui, foreground_emblem));
	if (!foreground_emblem_list)
	{
		event(_event_warning, "ui: failed to find list named 'foreground_emblem', cannot set initial foreground emblem value");
		return;
	}

	c_gui_list_widget* background_emblem_list = c_gui_widget::get_child_list_widget(STRING_ID(gui, background_emblem));
	if (!background_emblem_list)
	{
		event(_event_warning, "ui: failed to find list named 'background_emblem', cannot set initial background emblem value");
		return;
	}

	e_controller_index controller_index = c_gui_widget::get_single_responding_controller();
	c_controller_interface* controller = controller_get(controller_index);
	c_player_profile_interface* player_profile = controller->get_player_profile_interface();
	s_emblem_info emblem_info = player_profile->get_emblem_info();

	c_start_menu_pane_screen_widget::set_focused_datasource_element_from_value(foreground_emblem_list, STRING_ID(gui, image_index), emblem_info.foreground_emblem_index, true);
	c_start_menu_pane_screen_widget::set_focused_datasource_element_from_value(background_emblem_list, STRING_ID(gui, image_index), emblem_info.background_emblem_index, true);
}

void c_start_menu_settings_appearance_emblem::update(uns32 current_milliseconds)
{
	c_gui_widget* focused_widget = c_gui_screen_widget::get_focused_widget();
	if (focused_widget)
	{
		c_gui_list_widget* parent_list = focused_widget->get_parent_list();
		if (parent_list)
		{
			c_gui_data* data = parent_list->get_data();
			if (data)
			{
				int32 display_group_to_show_name = NONE;
				if (data->get_string_id_value(0, STRING_ID(gui, display_group), &display_group_to_show_name))
				{
					c_gui_screen_widget::load_display_group_by_name(_display_group_type_default, display_group_to_show_name);
				}
			}
		}
	}

	c_start_menu_pane_screen_widget::update(current_milliseconds);
}

void c_start_menu_settings_appearance_emblem::update_render_state(uns32 current_milliseconds)
{
	c_gui_widget::update_render_state(current_milliseconds);

	e_controller_index controller_index = c_gui_widget::get_single_responding_controller();
	c_controller_interface* controller = controller_get(controller_index);
	c_player_profile_interface* player_profile = controller->get_player_profile_interface();
	e_player_color_index primary_change_color = player_profile->get_primary_change_color();

	for (c_gui_bitmap_widget* child_widget = c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, base_color));
		child_widget != NULL;
		child_widget = child_widget->get_next_bitmap_widget())
	{
		if (child_widget->get_name() == STRING_ID(gui, base_color))
		{
			tint_widget_to_change_color(child_widget, NONE, false);
		}
	}
}

void player_emblem_toggle_alternate_emblem_foreground(e_controller_index controller_index)
{
	c_controller_interface* controller = controller_get(controller_index);
	if (TEST_BIT(controller->m_player_profile.m_flags, 8))
	{
		event(_event_message, "player_profile: settings change request ignored because player profile is busy with i/o");
		return;
	}

	s_emblem_info emblem_info = controller->m_player_profile.get_emblem_info();
	emblem_info.emblem_info_flags.set(_emblem_info_flag_alternate_foreground_channel_off, !emblem_info.emblem_info_flags.test(_emblem_info_flag_alternate_foreground_channel_off));
	controller->m_player_profile.set_emblem_info(&emblem_info, true);
}

void player_emblem_toggle_flip_emblem_foreground(e_controller_index controller_index)
{
	c_controller_interface* controller = controller_get(controller_index);
	if (TEST_BIT(controller->m_player_profile.m_flags, 8))
	{
		event(_event_message, "player_profile: settings change request ignored because player profile is busy with i/o");
		return;
	}

	s_emblem_info emblem_info = controller->m_player_profile.get_emblem_info();
	emblem_info.emblem_info_flags.set(_emblem_info_flag_flip_foreground, !emblem_info.emblem_info_flags.test(_emblem_info_flag_flip_foreground));
	controller->m_player_profile.set_emblem_info(&emblem_info, true);
}

void player_emblem_toggle_flip_emblem_background(e_controller_index controller_index)
{
	c_controller_interface* controller = controller_get(controller_index);
	if (TEST_BIT(controller->m_player_profile.m_flags, 8))
	{
		event(_event_message, "player_profile: settings change request ignored because player profile is busy with i/o");
		return;
	}

	s_emblem_info emblem_info = controller->m_player_profile.get_emblem_info();
	emblem_info.emblem_info_flags.set(_emblem_info_flag_flip_background, !emblem_info.emblem_info_flags.test(_emblem_info_flag_flip_background));
	controller->m_player_profile.set_emblem_info(&emblem_info, true);
}

