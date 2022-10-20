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

enum e_transport_endpoint_option
{
	// SO_REUSEADDR
	_transport_endpoint_option_reuse_address,

	// SO_LINGER
	_transport_endpoint_option_linger,

	// SO_BROADCAST
	_transport_endpoint_option_broardcast,

	// SO_SNDBUF
	_transport_endpoint_option_send_buffer_size,

	// SO_RCVBUF
	_transport_endpoint_option_receive_buffer_size,

	// 0x4001
	_transport_endpoint_option_unknown5,

	k_transport_endpoint_option_count
};


struct transport_endpoint
{
	SOCKET socket;
	c_enum<e_transport_type, long, k_transport_type_count> type;
	dword_flags flags;
};
