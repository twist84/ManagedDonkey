#include "interface/user_interface_controller.hpp"

#include "input/input_abstraction.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00A935C0, event_manager_button_pressed);
HOOK_DECLARE(0x00A94B40, user_interface_controller_input_event_submit);

void __cdecl event_manager_button_pressed(e_controller_index controller_index, char gamepad_button)
{
    s_event_record event_record = {};

    event_record.type = _event_type_controller_component;
    event_record.controller = controller_index;
	switch (gamepad_button)
	{
    case _gamepad_button_left_trigger:
        event_record.component = _controller_component_button_left_trigger;
        break;
    case _gamepad_button_right_trigger:
        event_record.component = _controller_component_button_right_trigger;
        break;
    case _gamepad_button_dpad_up:
    case _gamepad_button_dpad_down:
    case _gamepad_button_dpad_left:
    case _gamepad_button_dpad_right:
        break;
    case _gamepad_button_start:
        event_record.component = _controller_component_button_start;
        break;
    case _gamepad_button_back:
        event_record.component = _controller_component_button_back;
        break;
    case _gamepad_button_left_stick:
        event_record.component = _controller_component_button_left_stick;
        break;
    case _gamepad_button_right_stick:
        event_record.component = _controller_component_button_right_stick;
        break;
    case _gamepad_button_a:
        event_record.component = _controller_component_button_a;
        break;
    case _gamepad_button_b:
        event_record.component = _controller_component_button_b;
        break;
    case _gamepad_button_x:
        event_record.component = _controller_component_button_x;
        break;
    case _gamepad_button_y:
        event_record.component = _controller_component_button_y;
        break;
    case _gamepad_button_right_bumper:
        event_record.component = _controller_component_button_left_bumper;
        break;
    case _gamepad_button_left_bumper:
        event_record.component = _controller_component_button_right_bumper;
        break;
	}
    event_record.value = 0xFF;

    user_interface_controller_input_event_submit(&event_record);
}

void user_interface_controller_input_event_submit(s_event_record* event_record)
{
    c_window_manager* window_manager = window_manager_get();
    window_manager->handle_global_controller_event(event_record);
}

