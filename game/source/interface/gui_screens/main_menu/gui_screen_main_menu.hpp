#pragma once

struct c_controller_input_message;

struct c_main_menu_screen_widget
{
	static bool __fastcall handle_controller_input_message(void* _this, void* unused, c_controller_input_message* input_message);
};
