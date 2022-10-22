#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"

#include "interface/c_controller.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS(0x00B21A20, c_gui_screen_pregame_lobby, handle_controller_input_message);

bool __fastcall c_gui_screen_pregame_lobby::handle_controller_input_message(void* _this, void* unused, c_controller_input_message* input_message)
{
	if (input_message->get_event_type() == _event_type_controller_component && input_message->get_component() == _controller_component_button_x)
	{
		e_controller_index controller_index = input_message->get_controller();

		// `c_gui_screen_pregame_lobby_multiplayer::handle_list_item_chosen`
		// `c_gui_screen_pregame_lobby_mapeditor::handle_list_item_chosen`
		// STRING_ID(gui, advanced_options)
		DECLFUNC(0x00B225B0, void, __thiscall, void*, e_controller_index)(_this, controller_index);
	}

	bool result = false;
	HOOK_INVOKE_CLASS(result =, c_gui_screen_pregame_lobby, handle_controller_input_message, bool(__thiscall*)(void*, c_controller_input_message*), _this, input_message);
	return result;
}
