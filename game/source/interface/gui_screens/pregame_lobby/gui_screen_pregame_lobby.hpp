#pragma once

struct c_controller_input_message;

struct c_gui_screen_pregame_lobby
{
	static bool __fastcall handle_controller_input_message(void* _this, void* unused, c_controller_input_message* input_message);
};