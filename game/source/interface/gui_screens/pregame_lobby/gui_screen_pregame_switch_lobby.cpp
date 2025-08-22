#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_switch_lobby.hpp"

#include "interface/user_interface.hpp"
#include "interface/user_interface_data.hpp"

c_gui_screen_pregame_switch_lobby::c_gui_screen_pregame_switch_lobby(int32 name) :
	c_gui_screen_widget(name)
{
	//DECLFUNC(0x00B02DF0, void, __thiscall, c_gui_screen_pregame_switch_lobby*, int32)(this, name);
}

//.text:00B02E10 ; public: virtual c_gui_screen_pregame_switch_lobby::~c_gui_screen_pregame_switch_lobby()
//.text:00B02E20 ; public: virtual void* c_gui_screen_pregame_switch_lobby::`vector deleting destructor'(unsigned int)

bool c_gui_screen_pregame_switch_lobby::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return INVOKE_CLASS_MEMBER(0x00B02E50, c_gui_screen_pregame_switch_lobby, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	// $TODO: implemenent me
}

void c_gui_screen_pregame_switch_lobby::post_initialize()
{
	//INVOKE_CLASS_MEMBER(0x00B02F60, c_gui_screen_pregame_switch_lobby, post_initialize);

	c_gui_data* switch_lobby_data = c_gui_screen_widget::get_data(STRING_ID(gui, switch_lobby), NULL);
	if (switch_lobby_data)
	{
		if (get_alpha_is_internal_beta())
		{
			switch_lobby_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, campaign));
			switch_lobby_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, mapeditor));
		}

		if (get_alpha_custom_games_disabled())
		{
			switch_lobby_data->set_disabled_element(STRING_ID(global, name), STRING_ID(global, multiplayer));
		}

		if (!get_alpha_is_locked_down())
		{
			switch_lobby_data->set_disabled_element(STRING_ID(global, name), STRING_ID(global, locked));
		}
		else
		{
			switch_lobby_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, matchmaking));
			switch_lobby_data->set_disabled_element(STRING_ID(global, name), STRING_ID(gui, theater));
		}
	}

	c_gui_screen_widget::post_initialize();
}

