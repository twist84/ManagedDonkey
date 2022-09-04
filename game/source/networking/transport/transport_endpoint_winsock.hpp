#pragma once

#include "cseries/cseries.hpp"

#include <winsock.h>

enum e_transport_type
{
	_transport_type_udp = 0,
	_transport_type_vdp,
	_transport_type_tcp,
	_transport_type_unix,

	k_transport_type_count
};

struct transport_endpoint
{
	SOCKET socket;
	c_enum<e_transport_type, long, k_transport_type_count> type;
	dword_flags flags;
};
