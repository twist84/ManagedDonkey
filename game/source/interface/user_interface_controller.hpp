#pragma once

#include "interface/c_controller.hpp"

enum e_event_type
{
	_event_type_none = 0,
	_event_type_vertical_navigation1,
	_event_type_horizontal_navigation2,
	_event_type_vertical_navigation3,
	_event_type_horizontal_navigation4,
	_event_type_vertical_navigation5,
	_event_type_horizontal_navigation6,
	_event_type_vertical_navigation7,
	_event_type_horizontal_navigation8,
	_event_type_vertical_navigation9,
	_event_type_horizontal_navigation10,
	_event_type_vertical_navigation11,
	_event_type_horizontal_navigation12,
	_event_type_controller_component,

	k_event_type_count
};

enum e_controller_component
{
	_controller_component_button_a = 0,
	_controller_component_button_b,
	_controller_component_button_x,
	_controller_component_button_y,

	_controller_component_button_right_bumper,
	_controller_component_button_left_bumper,

	_controller_component_button_left_trigger,
	_controller_component_button_right_trigger,

	_controller_component_button_dpad_up,
	_controller_component_button_dpad_down,
	_controller_component_button_dpad_left,
	_controller_component_button_dpad_right,

	_controller_component_button_start,
	_controller_component_button_back,

	_controller_component_button_left_stick,
	_controller_component_button_right_stick,

	_controller_component_unknown16,
	_controller_component_unknown17,

	_controller_component_left_stick_left,
	_controller_component_left_stick_right,
	_controller_component_left_stick_up,
	_controller_component_left_stick_down,

	_controller_component_right_stick_left,
	_controller_component_right_stick_right,
	_controller_component_right_stick_up,
	_controller_component_right_stick_down,

	_controller_component_left_stick_x,
	_controller_component_left_stick_y,

	_controller_component_right_stick_x,
	_controller_component_right_stick_y,
};

struct s_event_record
{
	e_event_type type;
	e_controller_index controller;
	e_controller_component component;
	short value;
};

extern void __cdecl event_manager_button_pressed(e_controller_index controller_index, char gamepad_button);
extern void user_interface_controller_input_event_submit(s_event_record* event_record);

