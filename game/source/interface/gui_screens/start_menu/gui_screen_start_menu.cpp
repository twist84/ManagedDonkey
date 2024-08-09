#include "interface/gui_screens/start_menu/gui_screen_start_menu.hpp"

#include "cseries/cseries_events.hpp"
#include "cutscene/cinematics.hpp"
#include "game/players.hpp"
#include "interface/c_controller.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"
#include "networking/online/online_service_record.hpp"
#include "simulation/simulation.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AE05B0, c_start_menu_screen_widget, handle_controller_input_message);
HOOK_DECLARE_CLASS(0x00AE0720, c_start_menu_screen_widget, handle_global_start_button_press);

bool __thiscall c_start_menu_screen_widget::handle_controller_input_message(c_controller_input_message* input_message)
{
	if (input_message->get_event_type() == _event_type_controller_component)
	{
		if (input_message->get_component() == _controller_component_button_b || input_message->get_component() == _controller_component_button_start)
		{
			// c_static_stack<s_start_menu_breadcrumb, 8>::count != 0
			if (reinterpret_cast<long>(reinterpret_cast<byte*>(this) + 0x215C) != 1)
			{
				// back_out_current_pane
				DECLFUNC(0x00AE01D0, void, __thiscall, void*)(this);
				return true;
			}
		}
	}

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_start_menu_screen_widget, handle_controller_input_message, input_message);
	return result;
}

bool __cdecl c_start_menu_screen_widget::handle_global_start_button_press(c_controller_input_message* input_message)
{
	if (input_message->get_event_type() != _event_type_controller_component)
		return false;

	if (input_message->get_component() != _controller_component_button_start && input_message->get_component() != _controller_component_right_stick_y)
		return false;

	e_controller_index controller_index = input_message->get_controller();
	c_controller_interface const* controller = controller_get(controller_index);

	if (cinematic_in_progress())
	{
		generate_event(_event_level_status, "ui:start_menu: Can't show start menu -- cinematic in progress.");
		return false;
	}
	
	if (simulation_starting_up())
	{
		generate_event(_event_level_status, "ui:start_menu: Can't show start menu -- simulation starting up.");
		return false;
	}

	if (!controller->in_use() || controller->get_user_index() == NONE)
	{
		generate_event(_event_level_error, "ui:start_menu: controller not in use.");
		return false;
	}

	s_player_identifier player_identifier{};
	qword const player_xuid = controller->get_player_xuid();
	return load_start_menu(controller_index, controller->get_player_identifier(&player_identifier), &player_xuid, NULL, NULL, 0);
}

bool __cdecl c_start_menu_screen_widget::load_start_menu(e_controller_index controller_index, s_player_identifier const* player_identifier, qword const* player_xuid, s_service_record_identity const* identity, s_start_menu_breadcrumb const* breadcrumbs, long breadcrumb_count)
{
	return INVOKE(0x00AE0BE0, load_start_menu, controller_index, player_identifier, player_xuid, identity, breadcrumbs, breadcrumb_count);
}
