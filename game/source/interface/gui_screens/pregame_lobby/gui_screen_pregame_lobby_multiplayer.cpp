#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_multiplayer.hpp"

#include "interface/c_gui_list_item_widget.hpp"
#include "interface/gui_selected_items.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B01250, c_gui_screen_pregame_lobby_multiplayer, handle_list_item_chosen_);

bool __thiscall c_gui_screen_pregame_lobby_multiplayer::handle_list_item_chosen_(c_controller_input_message const* message, long list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	if (list_name == STRING_ID(gui, lobby_list))
	{
		long target_name = _string_id_invalid;
		long element_handle = list_item_widget->get_element_handle();
		if (datasource->get_string_id_value(element_handle, STRING_ID(gui, target), &target_name))
		{
			e_gui_selected_item_type selection_type = _gui_selection_type_none;
			switch (target_name)
			{
			case STRING_ID(global, variant):
				selection_type = _gui_selection_type_game_variant;
				break;
			case STRING_ID(global, map):
				selection_type = _gui_selection_type_map;
				break;
			}

			if (selection_type != _gui_selection_type_none)
			{
				if (c_load_pregame_selection_screen_message* pregame_selection_screen_message = new c_load_pregame_selection_screen_message(
					message->get_controller(),
					get_render_window(),
					m_name.get_value(),
					selection_type))
				{
					pregame_selection_screen_message->set_parent_screen_index(m_screen_index);
					user_interface_messaging_post(pregame_selection_screen_message);
				}
				return true;
			}
		}
		else if (datasource->get_string_id_value(element_handle, STRING_ID(global, name), &target_name) && target_name == STRING_ID(gui, advanced_options))
		{
			load_game_variant_editing_screen(message->get_controller());
			return true;
		}
	}

	return c_gui_screen_pregame_lobby::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

