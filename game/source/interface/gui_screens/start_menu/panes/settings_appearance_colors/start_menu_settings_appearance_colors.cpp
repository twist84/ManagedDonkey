#include "interface/gui_screens/start_menu/panes/settings_appearance_colors/start_menu_settings_appearance_colors.hpp"

#include "cseries/cseries.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AF9F70, c_start_menu_settings_appearance_colors, set_color_values_from_profile);

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

