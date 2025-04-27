#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_matchmaking.hpp"

#include "interface/c_gui_list_item_widget.hpp"
#include "interface/gui_selected_items.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_matchmaking.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_networking.hpp"
#include "memory/module.hpp"

//HOOK_DECLARE_CLASS_MEMBER(0x00B01920, c_gui_screen_pregame_lobby_matchmaking, handle_list_item_chosen_);
//
//bool __thiscall c_gui_screen_pregame_lobby_matchmaking::handle_list_item_chosen_(c_controller_input_message const* message, long list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
//{
//	if (list_name == STRING_ID(gui, lobby_list))
//	{
//		long target_name = _string_id_invalid;
//		long element_handle = list_item_widget->get_element_handle();
//		if (datasource->get_string_id_value(element_handle, STRING_ID(gui, target), &target_name))
//		{
//
//			return true;
//		}
//		else if (datasource->get_string_id_value(element_handle, STRING_ID(global, name), &target_name))
//		{
//			switch (target_name)
//			{
//			case STRING_ID(gui, advanced_options):
//			{
//				if (c_load_screen_message* screen_message = new c_load_screen_message(
//					STRING_ID(gui, pregame_advanced_options_matchmaking),
//					message->get_controller(),
//					get_render_window(),
//					m_name.get_value()))
//				{
//					screen_message->set_parent_screen_index(m_screen_index);
//					user_interface_messaging_post(screen_message);
//				}
//			}
//			break;
//			case STRING_ID(global, hopper):
//			{
//				e_gui_network_session_advertisement_mode session_advertisement = user_interface_networking_get_session_advertisement();
//				if (session_advertisement != _gui_network_session_advertisement_mode_open_to_public &&
//					session_advertisement != _gui_network_session_advertisement_mode_open_to_friends &&
//					session_advertisement != _gui_network_session_advertisement_mode_invite_only)
//				{
//					if (c_load_dialog_screen_message* dialog_screen_message = new c_load_dialog_screen_message(
//						message->get_controller(),
//						get_render_window(),
//						m_name.get_value(),
//						STRING_ID(gui_dialog, matchmaking_lobby_switch_to_live),
//						m_name.get_value()))
//					{
//						user_interface_messaging_post(dialog_screen_message);
//					}
//				}
//				else
//				{
//					if (user_interface_matchmaking_hopper_catalog_load_status() == _network_files_load_complete)
//					{
//						if (c_load_pregame_selection_screen_message* pregame_selection_screen_message = new c_load_pregame_selection_screen_message(
//							message->get_controller(),
//							get_render_window(),
//							m_name.get_value(),
//							_gui_selection_type_hopper))
//						{
//							pregame_selection_screen_message->set_parent_screen_index(m_screen_index);
//							user_interface_messaging_post(pregame_selection_screen_message);
//						}
//					}
//				}
//			}
//			break;
//			}
//			return true;
//		}
//	}
//
//	return c_gui_screen_pregame_lobby::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
//}

