#pragma once

#include "networking/session/network_session_parameters_generic.hpp"
#include "shell/shell.hpp"

struct c_network_session_parameter_ui_game_mode
	: public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<e_gui_game_mode>
{
};
static_assert(sizeof(c_network_session_parameter_ui_game_mode) == 0x38);
