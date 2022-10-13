#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_parameters.hpp"

struct c_network_session
{
	c_network_session_parameters* get_session_parameters();
	bool established();

	byte __data0[0x20];
	byte m_session_membership[0x1A3F20];
	c_network_session_parameters m_session_parameters;
	long m_local_state;
	byte __data25B874[0x3CC];
};
static_assert(sizeof(c_network_session) == 0x25BC40);
