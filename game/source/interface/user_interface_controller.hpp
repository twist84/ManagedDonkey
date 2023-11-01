#pragma once

#include "interface/c_controller.hpp"

enum e_bungienet_user_flags
{
	_bungienet_user_flag_bit0 = 0,
	_bungienet_user_flag_bit1,
	_bungienet_user_flag_bungie_bit,
	_bungienet_user_flag_community_bit,
	_bungienet_user_flag_community2_bit,
	_bungienet_user_flag_community3_bit,

	// are there more?

	k_bungienet_user_flags
};

struct s_user_interface_controller
{
	c_flags<e_bungienet_user_flags, dword, k_bungienet_user_flags> bungienet_user;
	byte hopper_access;
	bool extras_portal_debug;

	byte __unknown6;

	bool desires_veto;
	long armor_loadout_index;
	long weapon_loadout_index;
	bool desires_rematch;
	bool griefer;
	bool __unknown12;

	byte __data13[0x19];
};
static_assert(sizeof(s_user_interface_controller) == 0x2C);

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

	_controller_component_any_stick,

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
static_assert(sizeof(s_event_record) == 0x10);

struct s_user_interface_controller_globals
{
	s_user_interface_controller controller[k_number_of_controllers];
	s_event_record record[k_number_of_controllers];
	bool controller_detached[k_number_of_controllers];
	bool event_manager_suppress;
};
static_assert(sizeof(s_user_interface_controller_globals) == 0xF8);

extern s_user_interface_controller_globals& g_user_interface_controller_globals;

extern void __cdecl event_manager_button_pressed(e_controller_index controller_index, char gamepad_button);
extern void __cdecl event_manager_tab(long gamepad_stick, e_controller_index controller_index, int16_point2d const* a3, dword a4, e_controller_component controller_component);
extern void __cdecl user_interface_controller_input_event_submit(s_event_record* event_record);

