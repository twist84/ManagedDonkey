#pragma once

#include "cseries/cseries.hpp"
#include "networking/network_statistics.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"

int32 const k_network_link_maximum_game_data_size = 0x5BE;
int32 const k_network_link_maximum_voice_data_size = 0x200;

enum
{
	_network_packet_mode_channel = 0,
	_network_packet_mode_out_of_band,
	_network_packet_mode_broadcast,

	k_network_packet_mode_count,
};

class c_network_out_of_band_consumer;
class c_network_channel;
class c_bitstream;
class c_network_link
{
public:
	bool __thiscall read_data_immediate_(transport_address* address, int32* packet_data_length, byte* packet_buffer, int32 packet_buffer_size);

public:
	static const int32 k_network_link_maximum_channels = 16;
	static const int32 k_network_link_maximum_encoded_packet_size = 4096;
	static const int32 k_network_link_statistics_period = 2000;

public:
	struct s_link_packet
	{
		int32 packet_mode;
		bool simulate_packet;
		transport_address address;
		int32 game_data_length;
		byte game_data_buffer[k_network_link_maximum_game_data_size];
		int32 voice_data_length;
		byte voice_data_buffer[k_network_link_maximum_voice_data_size];
	};
	COMPILE_ASSERT(sizeof(s_link_packet) == 0x7E4);

public:
	c_network_link();
	~c_network_link();

	static bool __cdecl adjust_packet_size(bool out_of_band, int32 voice_data_length, int32* game_data_length);
	void attach_out_of_band(c_network_out_of_band_consumer* out_of_band);
	int32 compute_size_on_wire(const s_link_packet* packet) const;
	bool create_endpoint(e_transport_type endpoint_type, uns16 port, bool broadcast, transport_endpoint** out_endpoint);
	bool create_endpoints();
	bool decode_packet(int32 data_length, const byte* data_buffer, s_link_packet* packet) const;
	void destroy_endpoints();
	void destroy_link();
	void encode_packet(const s_link_packet* packet, int32* data_length, byte* data_buffer, int32 data_buffer_size) const;
	uns32 generate_channel_identifier();
	c_network_channel* get_associated_channel(const transport_address* address) const;
	static const char* get_packet_mode_string(int32 packet_mode);
	bool initialize_link();
	static void __cdecl initialize_packet(s_link_packet* packet);
	static bool __cdecl packet_has_voice(int32 packet_mode);
	static bool __cdecl physical_link_available();
	void process_all_channels();
	void process_incoming_packets();
	void process_packet_internal(const s_link_packet* packet);
	bool read_data_immediate(int32 packet_mode, transport_address* address, int32* packet_data_length, byte* packet_buffer, int32 packet_buffer_size);
	bool read_packet_internal(s_link_packet* packet);
	void send_broadcast(const c_bitstream* game_data, int32* out_size_on_wire);
	void send_channel(const c_bitstream* game_data, int32 voice_data_length, const byte* voice_data, const transport_address* address, int32* out_bytes_on_wire);
	void send_data_immediate(int32 packet_mode, const transport_address* address, int32 packet_data_length, const void* packet_data);
	void send_out_of_band(const c_bitstream* game_data, const transport_address* address, int32* out_size_on_wire);
	void send_packet_internal(const s_link_packet* packet);

//private:
	bool m_initialized;
	uns32 m_next_channel_identifier;
	int32 m_next_first_channel_index;

	// $TODO add this back
	//bool m_endpoints_created;

	transport_endpoint* m_endpoints[1/*k_network_packet_mode_count*/];
	c_network_out_of_band_consumer* m_out_of_band_consumer;
	c_network_time_statistics m_packets_transmitted;
	c_network_time_statistics m_packets_received;
	c_network_time_statistics m_upstream_bandwidth;
	c_network_time_statistics m_downstream_bandwidth;

	// $TODO add these back
	//s_network_simulation_parameters m_simulation_parameters;
	//c_network_simulation_state m_simulation_state;
	//c_network_simulation_queue m_incoming_queue;
	//c_network_simulation_queue m_outgoing_queue;
};
//COMPILE_ASSERT(sizeof(c_network_link) == 0x380);
COMPILE_ASSERT(sizeof(c_network_link) == 0x378);
COMPILE_ASSERT(0x000 == OFFSETOF(c_network_link, m_initialized));
COMPILE_ASSERT(0x004 == OFFSETOF(c_network_link, m_next_channel_identifier));
COMPILE_ASSERT(0x008 == OFFSETOF(c_network_link, m_next_first_channel_index));
//COMPILE_ASSERT(0x00C == OFFSETOF(c_network_link, m_endpoints_created));
COMPILE_ASSERT(0x00C/*0x010*/ == OFFSETOF(c_network_link, m_endpoints));
COMPILE_ASSERT(0x010/*0x01C*/ == OFFSETOF(c_network_link, m_out_of_band_consumer));
COMPILE_ASSERT(0x018/*0x020*/ == OFFSETOF(c_network_link, m_packets_transmitted));
COMPILE_ASSERT(0x0F0/*0x0F8*/ == OFFSETOF(c_network_link, m_packets_received));
COMPILE_ASSERT(0x1C8/*0x1D0*/ == OFFSETOF(c_network_link, m_upstream_bandwidth));
COMPILE_ASSERT(0x2A0/*0x2A8*/ == OFFSETOF(c_network_link, m_downstream_bandwidth));

