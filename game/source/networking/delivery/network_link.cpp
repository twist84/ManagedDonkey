#include "networking/delivery/network_link.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/bitstream.hpp"
#include "memory/module.hpp"
#include "networking/network_memory.hpp"
#include "networking/transport/transport.hpp"
#include "networking/transport/transport_address.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x0043B7C0, c_network_link, create_endpoints);

bool c_network_link::adjust_packet_size(bool game_data, int32 voice_data_length, int32* game_data_length) const
{
	//return DECLFUNC(0x0043B5E0, bool, __cdecl, bool, int32, int32*)(game_data, voice_data_length, game_data_length);

	ASSERT(game_data_length);
	ASSERT(voice_data_length > 0 || *game_data_length > 0);

	if (game_data)
	{
		int32 maximum_payload = transport_get_packet_maximum_payload(0);

		ASSERT(voice_data_length == 0);
		ASSERT((maximum_payload % 8) == 0);

		if (*game_data_length > 8)
			*game_data_length -= *game_data_length % 8;

		if (*game_data_length > maximum_payload)
			*game_data_length = maximum_payload;
	}
	else
	{
		int32 maximum_payload = transport_get_packet_maximum_payload(1);
		int32 payload_length = maximum_payload - (voice_data_length + 2) - (maximum_payload - (voice_data_length + 2)) % 8;

		if (payload_length < 0)
		{
			event(_event_warning, "networking:link: requested in-channel VDP packet (%d bytes voice, %d bytes data) exceeds maximum payload (%d bytes), impossible to send", voice_data_length, *game_data_length, maximum_payload);
			return false;
		}
		else
		{
			if (*game_data_length > 8)
				*game_data_length -= *game_data_length % 8;

			if (*game_data_length > payload_length)
				*game_data_length = payload_length;
		}
	}

	return true;
}

void c_network_link::attach_out_of_band(c_network_out_of_band_consumer* out_of_band)
{
	m_out_of_band_consumer = out_of_band;
}

int32 c_network_link::compute_size_on_wire(const s_link_packet* packet) const
{
	return INVOKE_CLASS_MEMBER(0x0043B6A0, c_network_link, compute_size_on_wire, packet);
}

bool c_network_link::create_endpoint(e_transport_type endpoint_type, uns16 port, bool broadcast, transport_endpoint** out_endpoint)
{
	//return DECLFUNC(0x0043B6F0, bool, __cdecl, e_transport_type, uns16, bool, transport_endpoint**)(endpoint_type, port, broadcast, out_endpoint);
	//return INVOKE_CLASS_MEMBER(0x0043B6F0, c_network_link, create_endpoint, endpoint_type, port, broadcast, out_endpoint);

	bool success = false;
	if (transport_endpoint* endpoint = transport_endpoint_create(endpoint_type))
	{
		transport_address listen_address{};
		transport_get_listen_address(&listen_address, port);
		// $TODO transport_get_listen_address_ipv6(&listen_address, port);

		success = transport_endpoint_bind(endpoint, &listen_address) && transport_endpoint_set_blocking(endpoint, false);
		if (broadcast)
		{
			success = success && transport_endpoint_set_option_value(endpoint, _transport_endpoint_option_broadcast, true);
			if (success)
			{
				*out_endpoint = endpoint;
			}
			else
			{
				event(_event_error, "networking:link: create_endpoint(%d) - unable to setup endpoint!",
					endpoint_type);
				transport_endpoint_delete(endpoint);
			}
		}
	}
	else
	{
		event(_event_error, "networking:link: create_endpoint[%d] - unable to create endpoint!",
			endpoint_type);
	}
	return success;
}

bool c_network_link::create_endpoints()
{
	//return INVOKE_CLASS_MEMBER(0x0043B7C0, c_network_link, create_endpoints);

	bool result = false;

	uns16 broadcast_port = k_broadcast_port;
	while (true)
	{
		g_broadcast_port = broadcast_port;

		result = c_network_link::create_endpoint(_transport_type_udp, broadcast_port, true, &m_endpoint);
		if (result)
			break;

		if (m_endpoint)
		{
			destroy_endpoints();
			m_endpoint = nullptr;
		}

		if (++broadcast_port - k_broadcast_port >= k_broadcast_port_alt_ammount)
		{
			g_broadcast_port = k_broadcast_port;
			return result;
		}
	}

	return result;
}

bool c_network_link::decode_packet(int32 data_buffer_size, const byte* data_buffer, s_link_packet* packet) const
{
	return INVOKE_CLASS_MEMBER(0x0043B820, c_network_link, decode_packet, data_buffer_size, data_buffer, packet);
}

void c_network_link::destroy_endpoints()
{
	//INVOKE_CLASS_MEMBER(0x0043B940, c_network_link, destroy_endpoints);

	if (m_endpoint)
	{
		transport_endpoint_delete(m_endpoint);
		m_endpoint = 0;
	}
}

void c_network_link::destroy_link()
{
	INVOKE_CLASS_MEMBER(0x0043B960, c_network_link, destroy_link);
}

void c_network_link::encode_packet(const s_link_packet* packet, int32* data_length, byte* data_buffer, int32 data_buffer_size) const
{
	INVOKE_CLASS_MEMBER(0x0043B990, c_network_link, encode_packet, packet, data_length, data_buffer, data_buffer_size);
}

uns32 c_network_link::generate_channel_identifier()
{
	return INVOKE_CLASS_MEMBER(0x0043BA20, c_network_link, generate_channel_identifier);
}

c_network_channel* c_network_link::get_associated_channel(const transport_address* address) const
{
	return DECLFUNC(0x0043BA40, c_network_channel*, __cdecl, const transport_address*)(address);

	//c_network_channel* associated_channel = NULL;
	//do
	//	associated_channel = network_channel_iterate(associated_channel);
	//while (associated_channel && !associated_channel->is_channel_remote_address(address));
	//
	//return associated_channel;
}

bool c_network_link::initialize_link()
{
	return INVOKE_CLASS_MEMBER(0x0043BAB0, c_network_link, initialize_link);

	//m_packets_transmitted.initialize(2000);
	//m_packets_received.initialize(2000);
	//m_upstream_bandwidth.initialize(2000);
	//m_downstream_bandwidth.initialize(2000);
	//bool result = m_simulation_queues[0].initialize_queue() && m_simulation_queues[1].initialize_queue();
	//ASSERT(transport_get_packet_overhead(_transport_type_udp) + k_network_link_maximum_game_data_size <= k_network_link_maximum_encoded_packet_size);
	//ASSERT(transport_get_packet_overhead(_transport_type_vdp) + k_network_link_maximum_game_data_size + k_network_link_maximum_voice_data_size <= k_network_link_maximum_encoded_packet_size);
	//if (result && transport_available() && !create_endpoints())
	//	event(_event_error, "networking:link: initialize() failed to create packet endpoints!");
	//if (result)
	//	m_initialized = true;
	//else
	//	destroy_link();
	//return result;
}

void __cdecl c_network_link::initialize_packet(s_link_packet* packet)
{
	INVOKE(0x0043BB20, initialize_packet, packet);

	//ASSERT(packet);
	//packet->packet_mode = NONE;
	//packet->simulate_packet = false;
	//packet->address = {};
	//packet->game_data_length = 0;
	//packet->voice_data_length = 0;
}

bool __cdecl c_network_link::physical_link_available()
{
	//INVOKE(0x0043BBB0, c_network_link::physical_link_available);

	return transport_available();
}

void c_network_link::process_all_channels()
{
	INVOKE_CLASS_MEMBER(0x0043BBC0, c_network_link, process_all_channels);
}

void c_network_link::process_incoming_packets()
{
	INVOKE_CLASS_MEMBER(0x0043BC70, c_network_link, process_incoming_packets);
}

void c_network_link::process_packet_internal(const s_link_packet* packet)
{
	INVOKE_CLASS_MEMBER(0x0043BDA0, c_network_link, process_packet_internal, packet);
}

bool c_network_link::read_data_immediate(transport_address* address, int32* packet_data_length, byte* packet_buffer, int32 packet_buffer_size)
{
	return INVOKE_CLASS_MEMBER(0x0043BEC0, c_network_link, read_data_immediate, address, packet_data_length, packet_buffer, packet_buffer_size);
}

bool c_network_link::read_packet_internal(s_link_packet* packet)
{
	return INVOKE_CLASS_MEMBER(0x0043BF50, c_network_link, read_packet_internal, packet);
}

void c_network_link::send_broadcast(const c_bitstream* game_data, int32* out_size_on_wire)
{
	INVOKE_CLASS_MEMBER(0x0043BFD0, c_network_link, send_broadcast, game_data, out_size_on_wire);
}

void c_network_link::send_channel(const c_bitstream* packet, int32 voice_data_length, const byte* voice_data, const transport_address* remote_address, int32* out_size_on_wire)
{
	INVOKE_CLASS_MEMBER(0x0043C020, c_network_link, send_channel, packet, voice_data_length, voice_data, remote_address, out_size_on_wire);
}

void c_network_link::send_data_immediate(int32 packet_mode, const transport_address* address, int32 packet_data_length, const void* packet_data)
{
	INVOKE_CLASS_MEMBER(0x0043C150, c_network_link, send_data_immediate, packet_mode, address, packet_data_length, packet_data);
}

void c_network_link::send_out_of_band(const c_bitstream* game_data, const transport_address* address, int32* out_size_on_wire)
{
	ASSERT(game_data);
	ASSERT(address);

	INVOKE_CLASS_MEMBER(0x0043C250, c_network_link, send_out_of_band, game_data, address, out_size_on_wire);
}

void c_network_link::send_packet_internal(const s_link_packet* packet)
{
	INVOKE_CLASS_MEMBER(0x0043C370, c_network_link, send_packet_internal, packet);
}

