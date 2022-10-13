#pragma once

#include "networking/session/network_session_parameters_game.hpp"

struct c_network_session;
struct c_network_observer;
struct c_network_session_parameters
{
	struct s_network_session_parameters_internal
	{
		byte __data0[0xB5808];
		c_network_session_parameter_countdown_timer countdown_timer;
		byte __data[0x2010];
	};
	static_assert(sizeof(s_network_session_parameters_internal) == 0xB7858);

	s_network_session_parameters_internal m_parameters_internal;
	c_network_session* m_session;
	c_network_observer* m_observer;
	c_network_session_parameter_base* m_parameter_interfaces[49];
	dword m_flags;
	dword m_initial_parameters_update_mask;
	long : 32;
};
static_assert(sizeof(c_network_session_parameters) == 0xB7930);
