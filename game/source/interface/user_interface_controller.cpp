#include "interface/user_interface_controller.hpp"

#include "input/input_abstraction.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x05260268, s_user_interface_controller_globals, g_user_interface_controller_globals);

HOOK_DECLARE(0x00A935C0, event_manager_button_pressed);
//HOOK_DECLARE(0x00A936B0, event_manager_tab);
HOOK_DECLARE_CALL(0x00A941B9, event_manager_tab);
HOOK_DECLARE(0x00A94B40, user_interface_controller_input_event_submit);

void __cdecl event_manager_button_pressed(e_controller_index controller_index, char gamepad_button)
{
	s_event_record event_record = {};

	event_record.type = _event_type_controller_component;
	event_record.controller = controller_index;
	switch (gamepad_button)
	{
	case _button_action_left_trigger:
		event_record.component = _controller_component_button_left_trigger;
		break;
	case _button_action_right_trigger:
		event_record.component = _controller_component_button_right_trigger;
		break;
	case _button_action_dpad_up:
	case _button_action_dpad_down:
	case _button_action_dpad_left:
	case _button_action_dpad_right:
		break;
	case _button_action_start:
		event_record.component = _controller_component_button_start;
		break;
	case _button_action_back:
		event_record.component = _controller_component_button_back;
		break;
	case _button_action_left_stick:
		event_record.component = _controller_component_button_left_stick;
		break;
	case _button_action_right_stick:
		event_record.component = _controller_component_button_right_stick;
		break;
	case _button_action_a:
		event_record.component = _controller_component_button_a;
		break;
	case _button_action_b:
		event_record.component = _controller_component_button_b;
		break;
	case _button_action_x:
		event_record.component = _controller_component_button_x;
		break;
	case _button_action_y:
		event_record.component = _controller_component_button_y;
		break;
	case _button_action_right_bumper:
		event_record.component = _controller_component_button_left_bumper;
		break;
	case _button_action_left_bumper:
		event_record.component = _controller_component_button_right_bumper;
		break;
	}
	event_record.value = 0xFF;

	user_interface_controller_input_event_submit(&event_record);
}

void __cdecl event_manager_tab(long gamepad_stick, e_controller_index controller_index, int16_point2d const* a3, dword a4, e_controller_component controller_component)
{
	// skip extra button processing in `event_manager_update`,
	// fixes double button pressing when no controller is plugged in
	//if (controller_component == _controller_component_any_stick)
	//	return;

	//INVOKE(0x00A936B0, event_manager_tab, gamepad_stick, controller_index, a3, a4, controller_component);
	//HOOK_INVOKE(, event_manager_tab, gamepad_stick, controller_index, a3, a4, controller_component);
}

void __cdecl user_interface_controller_input_event_submit(s_event_record* event_record)
{
	if (input_globals.suppressed)
		return;

	c_window_manager* window_manager = window_manager_get();
	window_manager->handle_global_controller_event(event_record);
}

