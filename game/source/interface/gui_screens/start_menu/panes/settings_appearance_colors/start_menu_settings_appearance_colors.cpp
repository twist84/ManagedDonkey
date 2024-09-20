#include "interface/gui_screens/start_menu/panes/settings_appearance_colors/start_menu_settings_appearance_colors.hpp"

#include "cseries/cseries.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AF9C20, c_start_menu_settings_appearance_colors, initialize_);
HOOK_DECLARE_CLASS_MEMBER(0x00AF9F70, c_start_menu_settings_appearance_colors, set_color_values_from_profile);

bool __cdecl parse_xml_armor1(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	//return INVOKE(0x00AF9D90, parse_xml_armor1, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_armor1));
}

bool __cdecl parse_xml_armor2(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	//return INVOKE(0x00AF9DB0, parse_xml_armor2, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_armor2));
}

bool __cdecl parse_xml_armor3(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	//return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_armor3));

	long color_armor3 = string_id_retrieve("color_armor3");
	if (color_armor3 == _string_id_invalid)
		return false;

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, color_armor3);
}

bool __cdecl parse_xml_color(c_gui_screen_widget* screen_widget, wchar_t* buffer, long buffer_length, long name)
{
	return INVOKE(0x00AF9DD0, parse_xml_color, screen_widget, buffer, buffer_length, name);
}

bool __cdecl parse_xml_emblem1(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	//return INVOKE(0x00AF9EE0, parse_xml_emblem1, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_emblem1));
}

bool __cdecl parse_xml_emblem2(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	//return INVOKE(0x00AF9F00, parse_xml_emblem2, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_emblem2));
}

bool __cdecl parse_xml_emblem3(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	//return INVOKE(0x00AF9F20, parse_xml_emblem3, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_emblem3));
}

void __thiscall c_start_menu_settings_appearance_colors::initialize_()
{
	//DECLFUNC(0x00AF9C20, void, __thiscall, c_start_menu_settings_appearance_colors*)(this);

	DECLFUNC(0x00AB14D0, void, __thiscall, c_gui_screen_widget*)(this);
	//c_gui_screen_widget::initialize();

	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<color-armor1", this, parse_xml_armor1));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<color-armor2", this, parse_xml_armor2));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<color-armor3", this, parse_xml_armor3));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<color-emblem1", this, parse_xml_emblem1));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<color-emblem2", this, parse_xml_emblem2));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<color-emblem3", this, parse_xml_emblem3));
}

void c_start_menu_settings_appearance_colors::set_color_focused_list_item(long name, e_player_color_index player_color_index)
{
	DECLFUNC(0x00AF9F50, void, __thiscall, c_start_menu_settings_appearance_colors*, long, e_player_color_index)(this, name, player_color_index);

	//if (c_gui_list_widget* child_list_widget = get_child_list_widget(name))
	//	child_list_widget->set_focused_item_index(player_color_index, false);
}

void __thiscall c_start_menu_settings_appearance_colors::set_color_values_from_profile()
{
	//DECLFUNC(0x00AF9F70, void, __thiscall, c_start_menu_settings_appearance_colors*)(this);

	e_controller_index controller_index = get_any_responding_controller();
	if (controller_get(controller_index)->in_use())
	{
		c_player_profile_interface* player_profile = controller_get(controller_index)->get_player_profile_interface();
		set_color_focused_list_item(STRING_ID(gui, color_armor1), player_profile->get_primary_change_color());
		set_color_focused_list_item(STRING_ID(gui, color_armor2), player_profile->get_secondary_change_color());
		//set_color_focused_list_item(STRING_ID(gui, color_armor3), player_profile->get_tertiary_change_color());
		//set_color_focused_list_item(STRING_ID(gui, color_emblem1), player_profile->get_emblem_info().emblem_primary_color);
		//set_color_focused_list_item(STRING_ID(gui, color_emblem2), player_profile->get_emblem_info().emblem_secondary_color);
		//set_color_focused_list_item(STRING_ID(gui, color_emblem3), player_profile->get_emblem_info().emblem_background_color);
	}
}

