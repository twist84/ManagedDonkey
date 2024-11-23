#pragma once

struct c_controller_input_message;
struct c_main_menu_screen_widget
{
	bool __thiscall handle_controller_input_message(c_controller_input_message* message);
};

