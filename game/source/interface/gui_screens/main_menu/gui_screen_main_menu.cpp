#include "interface/gui_screens/main_menu/gui_screen_main_menu.hpp"

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "interface/c_controller.hpp"
#include "interface/gui_screens/start_menu/gui_screen_start_menu.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AE7660, c_main_menu_screen_widget, handle_controller_input_message);

bool __thiscall c_main_menu_screen_widget::handle_controller_input_message(c_controller_input_message* input_message)
{
	if (input_message->get_event_type() == _event_type_controller_component && input_message->get_component() == _controller_component_button_start)
	{
		e_controller_index controller_index = input_message->get_controller();
		c_controller_interface const* controller = controller_get(controller_index);

		s_player_identifier player_identifier{};
		qword const player_xuid = controller->get_player_xuid();
		c_start_menu_screen_widget::load_start_menu(controller_index, controller->get_player_identifier(&player_identifier), &player_xuid, NULL, NULL, 0);
	}

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_main_menu_screen_widget, handle_controller_input_message, input_message);
	return result;
}

