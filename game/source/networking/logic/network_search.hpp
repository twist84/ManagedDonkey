#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_session_interface.hpp"

struct s_available_session
{
	bool initialized;
	byte : 8;
	byte : 8;
	byte : 8;
	dword time;
	bool has_time;
	byte : 8;
	byte : 8;
	byte : 8;
	char __dataC[0x2C];
	s_network_session_status_data status_data;
};
static_assert(sizeof(s_available_session) == 0x164F8);

struct s_network_search_globals
{
	long search_category;
	long reference_count;
	long maximum_sessions;
	dword_flags squad_search_flags;
	struct c_allocation_base* allocation;
	long available_session_count;
	s_available_session* available_sessions;
};
static_assert(sizeof(s_network_search_globals) == 0x1C);

extern s_network_search_globals& g_network_search_globals;