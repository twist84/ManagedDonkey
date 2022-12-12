#pragma once

#include "cseries/cseries.hpp"

struct c_network_session_manager;
struct s_network_time_globals
{
	bool locked;
	dword time;
	c_network_session_manager* session_manager;
};
static_assert(sizeof(s_network_time_globals) == 0xC);

extern s_network_time_globals& network_time_globals;

