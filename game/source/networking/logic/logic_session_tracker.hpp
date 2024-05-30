#pragma once

#include "networking/messages/network_messages_out_of_band.hpp"

struct s_network_session_tracker_session_data
{
	char name[16];
	s_transport_session_description description;
	byte __data40[0x20];
	s_network_session_status_data status_data;
};
static_assert(sizeof(s_network_session_tracker_session_data) == 0x16520);

extern bool __cdecl network_session_tracker_initialize();

