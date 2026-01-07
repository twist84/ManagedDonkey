#include "interface/gui_screens/start_menu/panes/settings/start_menu_settings.hpp"

#include "game/game.hpp"
#include "interface/c_gui_group_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/gui_screens/start_menu/gui_screen_start_menu.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"

// $TODO find out why `top_menu_cap` string id isn't assigned to a widget
//#define TOP_MENU_CAP

c_start_menu_settings::c_start_menu_settings(int32 name) :
	c_start_menu_pane_screen_widget(name)
{
	//DECLFUNC(0x00AF7340, void, __thiscall, c_start_menu_settings*, int32)(this, name);
}

//.text:00AF7360 ; public: c_start_menu_settings::~c_start_menu_settings()
//.text:00AF7370 ; public: virtual void* c_start_menu_settings::`vector deleting destructor'(unsigned int)

bool c_start_menu_settings::allow_pane_tab_change()
{
	//return INVOKE_CLASS_MEMBER(0x00AF73A0, c_start_menu_settings, allow_pane_tab_change);

	return true;
}

bool c_start_menu_settings::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00AF73B0, c_start_menu_settings, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	bool handled = false;

	bool game_is_in_ui_shell = game_in_progress() && game_is_ui_shell();
	if (list_name == STRING_ID(gui, sidebar))
	{
		string_id item_name = NONE;
		if (datasource->get_string_id_value(list_item_widget->get_element_handle(), STRING_ID(global, name), &item_name)
			&& item_name == STRING_ID(gui, appearance)
			&& !game_is_in_ui_shell)
		{
			user_interface_error_manager_get()->post_error(STRING_ID(gui_alert, can_not_view_in_game), message->get_controller(), false);
			handled = true;
		}
	}

	if (!handled)
	{
		handled = c_start_menu_pane_screen_widget::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
	}
	return handled;
}

void c_start_menu_settings::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AF7460, c_start_menu_settings, initialize);

	c_gui_screen_widget::initialize();

#if defined(TOP_MENU_CAP)
	bool ui_shell = game_is_ui_shell();
	c_gui_widget::set_child_visible(_gui_group, STRING_ID(gui, top_menu), ui_shell);
	c_gui_widget::set_child_visible(_gui_group, STRING_ID(gui, top_menu_cap), !ui_shell);
#endif
}

void c_start_menu_settings::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00AF7470, c_start_menu_settings, update, current_milliseconds);

	string_id header_string_id = c_start_menu_screen_widget::get_current_game_pane_name();
	if (header_string_id == NONE)
	{
		header_string_id = STRING_ID(gui, top_menu_display_item0);
	}

	c_gui_text_widget* header = c_gui_widget::get_child_text_widget(header_string_id);
	c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, top_menu_display_item0), false);
	c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, start_menu_game_campaign), false);
	c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, start_menu_game_saved_film), false);
	c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, start_menu_game_editor), false);
	c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, start_menu_game_multiplayer), false);

#if defined(TOP_MENU_CAP)
	c_gui_group_widget* top_menu_cap = c_gui_widget::get_child_group_widget(STRING_ID(gui, top_menu_cap));
	if (top_menu_cap && top_menu_cap->get_visible())
	{
		header = top_menu_cap->get_child_text_widget(header_string_id);
		top_menu_cap->set_child_visible(_gui_text, STRING_ID(gui, top_menu_display_item0), false);
		top_menu_cap->set_child_visible(_gui_text, STRING_ID(gui, start_menu_game_campaign), false);
		top_menu_cap->set_child_visible(_gui_text, STRING_ID(gui, start_menu_game_saved_film), false);
		top_menu_cap->set_child_visible(_gui_text, STRING_ID(gui, start_menu_game_editor), false);
		top_menu_cap->set_child_visible(_gui_text, STRING_ID(gui, start_menu_game_multiplayer), false);
	}
#endif

	if (header)
	{
		header->set_visible(true);
	}

	c_start_menu_pane_screen_widget::update(current_milliseconds);
}


