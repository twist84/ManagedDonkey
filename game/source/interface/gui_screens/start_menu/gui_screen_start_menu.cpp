#include "interface/gui_screens/start_menu/gui_screen_start_menu.hpp"

#include "cutscene/cinematics.hpp"
#include "game/players.hpp"
#include "interface/c_controller.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"
#include "networking/online/online_service_record.hpp"
#include "simulation/simulation.hpp"

HOOK_DECLARE_CLASS(0x00AE05B0, c_start_menu_screen_widget, handle_controller_input_message);
HOOK_DECLARE_CLASS(0x00AE0720, c_start_menu_screen_widget, handle_global_start_button_press);

bool __fastcall c_start_menu_screen_widget::handle_controller_input_message(void* _this, void* unused, c_controller_input_message* input_message)
{
	if (input_message->get_event_type() == _event_type_controller_component)
	{
		if (input_message->get_component() == _controller_component_button_b || input_message->get_component() == _controller_component_button_start)
		{
			// c_static_stack<s_start_menu_breadcrumb, 8>::count != 0
			if (reinterpret_cast<long>(static_cast<byte*>(_this) + 0x215C) != 1)
			{
				// back_out_current_pane
				DECLFUNC(0x00AE01D0, void, __thiscall, void*)(_this);
				return true;
			}
		}
	}

	bool result = false;
	HOOK_INVOKE_CLASS(result =, c_start_menu_screen_widget, handle_controller_input_message, bool(__thiscall*)(void*, c_controller_input_message*), _this, input_message);
	return result;
}

bool __cdecl c_start_menu_screen_widget::handle_global_start_button_press(c_controller_input_message* input_message)
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

bool __cdecl c_start_menu_screen_widget::load_start_menu(e_controller_index controller_index, s_player_identifier* player_identifier, qword* player_xuid, s_service_record_identity* identity, s_start_menu_breadcrumb* breadcrumbs, long breadcrumb_count)
{
	return INVOKE(0x00AE0BE0, load_start_menu, controller_index, player_identifier, player_xuid, identity, breadcrumbs, breadcrumb_count);
}
