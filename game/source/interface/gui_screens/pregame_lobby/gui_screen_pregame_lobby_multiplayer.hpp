#include "cseries/cseries.hpp"

#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"

struct c_gui_screen_pregame_lobby_multiplayer :
	public c_gui_screen_pregame_lobby
{
	bool __thiscall handle_list_item_chosen_(c_controller_input_message const* message, long list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);
};
static_assert(sizeof(c_gui_screen_pregame_lobby_multiplayer) == sizeof(c_gui_screen_pregame_lobby));

