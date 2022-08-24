#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_search.hpp"

struct s_broadcast_search_globals
{
	bool initialized;
	byte : 8;
	byte : 8;
	byte : 8;

	struct c_network_link* link;
	struct c_network_message_gateway* message_gateway;

	bool search_active;

	// new status data
	bool __unknownD;
	byte : 8;
	byte : 8;

	dword search_time;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;

	qword search_nonce;
	long maximum_session_count;
	s_available_session* available_sessions;
};
static_assert(sizeof(s_broadcast_search_globals) == 0x28);

extern s_broadcast_search_globals& g_broadcast_search_globals;
