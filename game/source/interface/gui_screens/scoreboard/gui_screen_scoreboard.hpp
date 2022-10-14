#pragma once

#include "interface/c_gui_screen_widget.hpp"

struct c_gui_screen_scoreboard : c_gui_screen_widget
{
	enum e_scoreboard_mode;

protected:
	long m_current_scoreboard_mode;
	bool m_is_interactive;
};
