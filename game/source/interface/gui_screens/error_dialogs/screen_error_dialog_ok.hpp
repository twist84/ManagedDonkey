#pragma once

#include "interface/gui_screens/error_dialogs/screen_error_dialog.hpp"

class c_error_dialog_ok_screen_widget :
	public c_error_dialog_screen_widget
{
public:
	c_error_dialog_ok_screen_widget(int32 name);

//protected:
};
COMPILE_ASSERT(sizeof(c_error_dialog_screen_widget) == sizeof(c_error_dialog_screen_widget));

