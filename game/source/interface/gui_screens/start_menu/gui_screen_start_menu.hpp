#pragma once

struct s_start_menu_breadcrumb
{
	long count;
	long __unknown4;
};
static_assert(sizeof(s_start_menu_breadcrumb) == 0x8);

struct c_controller_input_message;

extern bool __cdecl handle_global_start_button_press(c_controller_input_message* input_message);