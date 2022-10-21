#include "interface/gui_screens/start_menu/gui_screen_start_menu.hpp"

#include "cseries/console.hpp"
#include "cutscene/cinematics.hpp"
#include "game/players.hpp"
#include "interface/c_controller.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"
#include "networking/online/online_service_record.hpp"
#include "simulation/simulation.hpp"

HOOK_DECLARE(0x00AE0720, handle_global_start_button_press);

bool __cdecl load_start_menu(e_controller_index controller_index, s_player_identifier* player_identifier, qword* player_xuid, s_service_record_identity* identity, s_start_menu_breadcrumb* breadcrumbs, long breadcrumb_count)
{
	return INVOKE(0x00AE0BE0, load_start_menu, controller_index, player_identifier, player_xuid, identity, breadcrumbs, breadcrumb_count);
}

bool __cdecl handle_global_start_button_press(c_controller_input_message* input_message)
{
    bool result = false;

	if (input_message->get_event_type() == _event_type_controller_component && input_message->get_component() == _controller_component_button_start)
	{
		e_controller_index controller_index = input_message->get_controller();
		c_controller_interface* controller = controller_get(controller_index);

		if (cinematic_in_progress())
		{
			c_console::write_line("ui:start_menu: Can't show start menu -- cinematic in progress.");
		}
		else if (simulation_starting_up())
		{
			c_console::write_line("ui:start_menu: Can't show start menu -- simulation starting up.");
		}
		else if (controller->in_use() && controller->get_user_index() != -1)
		{
			s_player_identifier player_identifier{};
			controller->get_player_identifier(&player_identifier);
			qword player_xuid = controller->get_player_xuid();

			result = load_start_menu(controller_index, &player_identifier, &player_xuid, NULL, NULL, 0);
		}
		else
		{
			c_console::write_line("ui:start_menu: controller not in use.");
		}
	}

    return result;
}
