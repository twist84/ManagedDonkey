#pragma once

#include "networking/network_statistics.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"

struct c_network_out_of_band_consumer;
struct c_network_link
{
	bool m_initialized;
	long __unknown4;
	long __unknown8;
	transport_endpoint* m_endpoint;
	c_network_out_of_band_consumer* m_out_of_band;
	long __unknown14;
	c_network_time_statistics m_time_statistics[4];
};
static_assert(sizeof(c_network_link) == 0x378);