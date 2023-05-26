#pragma once

#include "networking/network_statistics.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"

long const k_network_link_maximum_game_data_size = 0x5BE;
long const k_network_link_maximum_voice_data_size = 0x200;

enum e_network_packet_mode
{
	_network_packet_mode_none = -1,
	_network_packet_mode_channel,
	_network_packet_mode_out_of_band,
	_network_packet_mode_voice,

	k_network_packet_mode_count
};

struct c_network_out_of_band_consumer;
struct c_network_channel;
struct c_network_link
{
	struct s_link_packet
	{
		c_enum<e_network_packet_mode, long, _network_packet_mode_none, k_network_packet_mode_count> mode;
		bool __unknown4;
		transport_address address;

		long game_data_length;
		byte game_data[k_network_link_maximum_game_data_size];

		long voice_data_length;
		byte voice_data[k_network_link_maximum_voice_data_size];
	};
	static_assert(sizeof(s_link_packet) == 0x7E4);

	bool __cdecl create_endpoint(e_transport_type type, word port, bool a3, transport_endpoint** out_endpoint);
	bool __cdecl decode_packet(long data_buffer_size, byte const* data_buffer, s_link_packet* packet) const;
	void __cdecl destroy_endpoints();
	c_network_channel* __cdecl get_associated_channel(transport_address const* address) const;
	static bool __cdecl physical_link_available();

	bool __cdecl create_endpoints();

	bool m_initialized;
	long __unknown4;
	long __unknown8;
	transport_endpoint* m_endpoint;
	c_network_out_of_band_consumer* m_out_of_band;
	long __unknown14;
	c_network_time_statistics m_time_statistics[4];
};
static_assert(sizeof(c_network_link) == 0x378);

