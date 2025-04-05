#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_campaign.hpp"

#include "interface/c_gui_list_item_widget.hpp"
#include "interface/gui_selected_items.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/module.hpp"
#include "shell/shell.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AFFAE0, c_gui_screen_pregame_lobby_campaign, handle_list_item_chosen_);

bool __thiscall c_gui_screen_pregame_lobby_campaign::handle_list_item_chosen_(c_controller_input_message const* message, long list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	if (list_name == STRING_ID(gui, lobby_list))
	{
		long target_name = _string_id_invalid;
		long element_handle = list_item_widget->get_element_handle();
		if (datasource->get_string_id_value(element_handle, STRING_ID(gui, target), &target_name))
		{
			switch (target_name)
			{
			case STRING_ID(global, level):
			{
				long campaign_id = 1;
				long map_id = 3005;
				short campaign_insertion_point = 0;
				user_interface_session_get_map(&campaign_id, &map_id);
				if (c_load_campaign_select_level_screen_message* screen_message = new c_load_campaign_select_level_screen_message(
					STRING_ID(gui, campaign_select_level),
					message->get_controller(),
					get_render_window(),
					m_name.get_value(),
					_campaign_level_setup_mode_lobby,
					campaign_id,
					map_id,
					campaign_insertion_point))
				{
					screen_message->set_parent_screen_index(m_screen_index);
					user_interface_messaging_post(screen_message);
				}
			}
			break;
			case STRING_ID(global, difficulty):
			{
				if (c_load_campaign_select_difficulty_screen_message* screen_message = new c_load_campaign_select_difficulty_screen_message(
					STRING_ID(gui, campaign_select_difficulty),
					message->get_controller(),
					get_render_window(),
					m_name.get_value(),
					_campaign_difficulty_setup_mode_lobby,
					1,
					NONE,
					user_interface_game_settings_get_campaign_difficulty()))
				{
					screen_message->set_parent_screen_index(m_screen_index);
					user_interface_messaging_post(screen_message);
				}
			}
			break;
			}
			return true;
		}
	}

	return c_gui_screen_pregame_lobby::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

