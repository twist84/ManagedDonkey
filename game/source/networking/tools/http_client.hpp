#pragma once

#include "cseries/cseries.hpp"
#include "networking/tools/http_stream.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"

struct c_http_client
{
	transport_address m_address;
	char m_ip_address_string[16];
	transport_endpoint m_endpoint;
	transport_endpoint* m_endpoint_ptr;
	c_http_stream* m_http_stream;
	char m_response_buffer[4096];
	long m_response_buffer_count;
	byte __data38[0x4];
	long m_socket_count;
	byte __data1044[0x18];
	long m_current_state;
};
static_assert(sizeof(c_http_client) == 0x1060);

