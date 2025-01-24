#pragma once

#include "cseries/cseries.hpp"
#include "networking/network_statistics.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"

long const k_network_link_maximum_game_data_size = 0x5BE;
long const k_network_link_maximum_voice_data_size = 0x200;

enum
{
	_network_packet_mode_channel = 0,
	_network_packet_mode_out_of_band
};

struct c_network_out_of_band_consumer;
struct c_network_channel;
struct c_bitstream;
struct c_network_link
{
public:
	struct s_link_packet
	{
		long packet_mode;
		bool simulate_packet;
		transport_address address;
		long game_data_length;
		byte game_data_buffer[k_network_link_maximum_game_data_size];
		long voice_data_length;
		byte voice_data_buffer[k_network_link_maximum_voice_data_size];
	};
	static_assert(sizeof(s_link_packet) == 0x7E4);

	bool adjust_packet_size(bool game_data, long voice_data_length, long* game_data_length) const;
	void attach_out_of_band(c_network_out_of_band_consumer* out_of_band);
	long compute_size_on_wire(s_link_packet const* packet) const;
	bool create_endpoint(e_transport_type type, word port, bool a3, transport_endpoint** out_endpoint);
	bool create_endpoints();
	bool decode_packet(long data_buffer_size, byte const* data_buffer, s_link_packet* packet) const;
	void destroy_endpoints();
	void destroy_link();
	void encode_packet(s_link_packet const* packet, long* data_length, byte* data_buffer, long data_buffer_size) const;
	dword generate_channel_identifier();
	c_network_channel* get_associated_channel(transport_address const* address) const;
	bool initialize_link();
	static void __cdecl initialize_packet(s_link_packet* packet);
	static bool __cdecl physical_link_available();
	void process_all_channels();
	void process_incoming_packets();
	void process_packet_internal(s_link_packet const* packet);
	bool read_data_immediate(transport_address* address, long* packet_data_length, byte* packet_buffer, long packet_buffer_size);
	bool read_packet_internal(s_link_packet* packet);
	void send_broadcast(c_bitstream const* game_data, long* out_size_on_wire);
	void send_channel(c_bitstream const* packet, long voice_data_length, byte const* voice_data, transport_address const* remote_address, long* out_size_on_wire);
	void send_data_immediate(long packet_mode, transport_address const* address, long packet_data_length, void const* packet_data);
	void send_out_of_band(c_bitstream const* game_data, transport_address const* address, long* out_size_on_wire);
	void send_packet_internal(s_link_packet const* packet);

//private:
	bool m_initialized;
	dword m_next_channel_identifier;
	long m_next_first_channel_index;
	transport_endpoint* m_endpoint;
	c_network_out_of_band_consumer* m_out_of_band_consumer;
	c_network_time_statistics m_packets_transmitted;
	c_network_time_statistics m_packets_received;
	c_network_time_statistics m_upstream_bandwidth;
	c_network_time_statistics m_downstream_bandwidth;
};
static_assert(sizeof(c_network_link) == 0x378);
static_assert(0x000 == OFFSETOF(c_network_link, m_initialized));
static_assert(0x004 == OFFSETOF(c_network_link, m_next_channel_identifier));
static_assert(0x008 == OFFSETOF(c_network_link, m_next_first_channel_index));
static_assert(0x00C == OFFSETOF(c_network_link, m_endpoint));
static_assert(0x010 == OFFSETOF(c_network_link, m_out_of_band_consumer));
static_assert(0x018 == OFFSETOF(c_network_link, m_packets_transmitted));
static_assert(0x0F0 == OFFSETOF(c_network_link, m_packets_received));
static_assert(0x1C8 == OFFSETOF(c_network_link, m_upstream_bandwidth));
static_assert(0x2A0 == OFFSETOF(c_network_link, m_downstream_bandwidth));

