#pragma once

#include "networking/session/network_session_parameters_base.hpp"

struct s_network_session_parameter_countdown_timer
{
	long delayed_reason;
	long countdown_timer;
};

struct c_network_session_parameter_countdown_timer : c_network_session_parameter_base
{
	long get_countdown_timer();

	s_network_session_parameter_countdown_timer m_data;
	s_network_session_parameter_countdown_timer m_change_request_data;
};