#include "interface/gui_screens/start_menu/panes/settings_appearance_colors/start_menu_settings_appearance_colors.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/gui_screens/start_menu/panes/settings_appearance_emblem/start_menu_settings_appearance_emblem.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AF9A30, c_start_menu_settings_appearance_colors, handle_controller_input_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00AF9C20, c_start_menu_settings_appearance_colors, initialize_);
HOOK_DECLARE_CLASS_MEMBER(0x00AF9F70, c_start_menu_settings_appearance_colors, set_color_values_from_profile);

bool __cdecl parse_xml_armor1(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	//return INVOKE(0x00AF9D90, parse_xml_armor1, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_armor1));
}

bool __cdecl parse_xml_armor2(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	//return INVOKE(0x00AF9DB0, parse_xml_armor2, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_armor2));
}

bool __cdecl parse_xml_armor3(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	//return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_armor3));

	int32 color_armor3 = string_id_retrieve("color_armor3");
	if (color_armor3 == _string_id_invalid)
		return false;

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, color_armor3);
}

bool __cdecl parse_xml_color(c_gui_screen_widget* screen_widget, wchar_t* buffer, int32 buffer_length, int32 name)
{
	return INVOKE(0x00AF9DD0, parse_xml_color, screen_widget, buffer, buffer_length, name);
}

bool __cdecl parse_xml_emblem1(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	//return INVOKE(0x00AF9EE0, parse_xml_emblem1, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_emblem1));
}

bool __cdecl parse_xml_emblem2(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	//return INVOKE(0x00AF9F00, parse_xml_emblem2, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_emblem2));
}

bool __cdecl parse_xml_emblem3(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	//return INVOKE(0x00AF9F20, parse_xml_emblem3, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_emblem3));
}

int32 c_gui_list_item_widget::get_list_item_index()
{
	//return INVOKE_CLASS_MEMBER(0x00AF9A20, c_gui_list_item_widget, get_list_item_index);
	return m_list_item_index;
}

bool __thiscall c_start_menu_settings_appearance_colors::handle_controller_input_message_(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00AF9A30, c_start_menu_settings_appearance_colors*, handle_controller_input_message, message);

	bool result = false;

	e_event_type event_type = message->get_event_type();
	e_controller_component component = message->get_component();

	c_gui_list_widget* focused_parent_list_widget = NULL;
	if (c_gui_widget* focused_widget = get_focused_widget())
		focused_parent_list_widget = focused_widget->get_parent_list();

	c_gui_list_widget* group_selector_child_list = get_child_list_widget(STRING_ID(gui, group_selector));
	if (focused_parent_list_widget && group_selector_child_list)
	{
		if (focused_parent_list_widget == group_selector_child_list)
		{
			if ((event_type == _event_type_button_press && component == _controller_component_button_a) || event_type == _event_type_tab_right)
			{
				c_gui_list_item_widget* parent_list_item = NULL;
				if (c_gui_widget* focused_widget = get_focused_widget())
					parent_list_item = focused_widget->get_parent_list_item();

				if (c_gui_list_widget* child_list = get_child_list_widget(parent_list_item->m_name))
				{
					int32 focused_item_index = child_list->get_focused_item_index();
					for (c_gui_list_item_widget* child_list_item = (c_gui_list_item_widget*)child_list->get_first_child_widget_by_type(_gui_list_item);
						child_list_item;
						child_list_item = get_next_list_item_widget(true))
					{
						if (focused_item_index == child_list_item->get_list_item_index())
						{
							c_gui_screen_widget::transfer_focus(child_list_item);
							result = true;
						}
					}
				}

			}
			else if (event_type == _event_type_button_press && component == _controller_component_button_b)
			{
				close_current_subpane();
				return true;
			}
		}
		else if (event_type == _event_type_button_press && (component == _controller_component_button_a || component == _controller_component_button_b))
		{
			int32 parent_list_name = focused_parent_list_widget->m_name;
			for (c_gui_list_item_widget* child_list_item = (c_gui_list_item_widget*)group_selector_child_list->get_first_child_widget_by_type(_gui_list_item);
				child_list_item;
				child_list_item = get_next_list_item_widget(true))
			{
				if (child_list_item->m_name == parent_list_name)
				{
					group_selector_child_list->set_focused_item_index(child_list_item->get_list_item_index(), false);
					c_gui_screen_widget::transfer_focus(child_list_item);
					return true;
				}
			}

			if (component != _controller_component_button_a)
			{
				set_color_values_from_profile();
			}
			else
			{
				e_controller_index controller_index = get_arbitrary_responding_controller();
				c_player_profile_interface* player_profile = controller_get(controller_index)->get_player_profile_interface();
				int32 focused_item_index = focused_parent_list_widget->get_focused_item_index();

				s_emblem_info emblem_info = player_profile->get_emblem_info();
				switch (parent_list_name)
				{
				case STRING_ID(gui, color_armor1):
				{
					player_profile->set_primary_change_color((e_player_color_index)focused_item_index, true);
				}
				return true;
				case STRING_ID(gui, color_armor2):
				{
					player_profile->set_secondary_change_color((e_player_color_index)focused_item_index, true);
				}
				return true;
				//case STRING_ID(gui, color_armor3):
				//{
				//	//player_profile->set_tertiary_change_color(focused_item_index, true);
				//}
				//return true;
				case STRING_ID(gui, color_emblem1):
				{
					emblem_info.primary_color_index = focused_item_index;
					player_profile->set_emblem_info(&emblem_info, true);
				}
				return true;
				case STRING_ID(gui, color_emblem2):
				{
					emblem_info.secondary_color_index = focused_item_index;
					player_profile->set_emblem_info(&emblem_info, true);
				}
				return true;
				case STRING_ID(gui, color_emblem3):
				{
					emblem_info.background_color_index = focused_item_index;
					player_profile->set_emblem_info(&emblem_info, true);
				}
				return true;
				default:
				{
					if (parent_list_name == string_id_retrieve("color_armor3"))
					{
						//player_profile->set_tertiary_change_color(focused_item_index, true);
						return true;
					}

					return true;
				}
				break;
				}

				return true;
			}
		}
	}

	if (event_type == _event_type_button_press)
	{
		e_controller_index controller_index = message->get_controller();
		switch (component)
		{
		case _controller_component_button_x:
		{
			player_emblem_toggle_alternate_emblem_foreground(controller_index);
		}
		return true;
		case _controller_component_button_left_thumb:
		{
			player_emblem_toggle_flip_emblem_foreground(controller_index);
		}
		return true;
		case _controller_component_button_right_thumb:
		{
			player_emblem_toggle_flip_emblem_background(controller_index);
		}
		return true;
		}
	}

	if (!result)
	{
		//result = c_start_menu_pane_screen_widget::handle_controller_input_message(message);
		result = INVOKE_CLASS_MEMBER(0x00B1F620, c_start_menu_pane_screen_widget, handle_controller_input_message, message);
	}

	return result;
}

void __thiscall c_start_menu_settings_appearance_colors::initialize_()
{
	//INVOKE_CLASS_MEMBER(0x00AF9C20, c_start_menu_settings_appearance_colors, initialize);

	INVOKE_CLASS_MEMBER(0x00AB14D0, c_gui_screen_widget, initialize);

	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<color-armor1", this, parse_xml_armor1));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<color-armor2", this, parse_xml_armor2));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<color-armor3", this, parse_xml_armor3));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<color-emblem1", this, parse_xml_emblem1));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<color-emblem2", this, parse_xml_emblem2));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<color-emblem3", this, parse_xml_emblem3));
}

void c_start_menu_settings_appearance_colors::set_color_focused_list_item(int32 name, e_player_color_index player_color_index)
{
	INVOKE_CLASS_MEMBER(0x00AF9F50, c_start_menu_settings_appearance_colors, set_color_focused_list_item, name, player_color_index);

	//if (c_gui_list_widget* child_list_widget = get_child_list_widget(name))
	//	child_list_widget->set_focused_item_index(player_color_index, false);
}

void __thiscall c_start_menu_settings_appearance_colors::set_color_values_from_profile()
{
	//INVOKE_CLASS_MEMBER(0x00AF9F70, c_start_menu_settings_appearance_colors, set_color_values_from_profile);

	e_controller_index controller_index = get_arbitrary_responding_controller();
	if (controller_get(controller_index)->in_use())
	{
		c_player_profile_interface* player_profile = controller_get(controller_index)->get_player_profile_interface();
		set_color_focused_list_item(STRING_ID(gui, color_armor1), player_profile->get_primary_change_color());
		set_color_focused_list_item(STRING_ID(gui, color_armor2), player_profile->get_secondary_change_color());
		//set_color_focused_list_item(STRING_ID(gui, color_armor3), player_profile->get_tertiary_change_color());
		//set_color_focused_list_item(string_id_retrieve("color_armor3"), player_profile->get_tertiary_change_color());
		
		//s_emblem_info emblem_info = player_profile->get_emblem_info();
		s_emblem_info emblem_info = player_profile->m_appearance.emblem;
		set_color_focused_list_item(STRING_ID(gui, color_emblem1), emblem_info.primary_color_index);
		set_color_focused_list_item(STRING_ID(gui, color_emblem2), emblem_info.secondary_color_index);
		set_color_focused_list_item(STRING_ID(gui, color_emblem3), emblem_info.background_color_index);
	}
}

