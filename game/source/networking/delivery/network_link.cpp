#include "networking/delivery/network_link.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/bitstream.hpp"
#include "memory/module.hpp"
#include "networking/network_memory.hpp"
#include "networking/transport/transport.hpp"
#include "networking/transport/transport_address.hpp"

bool c_network_link::adjust_packet_size(bool game_data, long voice_data_length, long* game_data_length) const
{
	//return DECLFUNC(0x0043B5E0, bool, __cdecl, bool, long, long*)(game_data, voice_data_length, game_data_length);

	ASSERT(game_data_length);
	ASSERT(voice_data_length > 0 || *game_data_length > 0);

	if (game_data)
	{
		long maximum_payload = transport_get_packet_maximum_payload(0);

		ASSERT(voice_data_length == 0);
		ASSERT((maximum_payload % 8) == 0);

		if (*game_data_length > 8)
			*game_data_length -= *game_data_length % 8;

		if (*game_data_length > maximum_payload)
			*game_data_length = maximum_payload;
	}
	else
	{
		long maximum_payload = transport_get_packet_maximum_payload(1);
		long payload_length = maximum_payload - (voice_data_length + 2) - (maximum_payload - (voice_data_length + 2)) % 8;

		if (payload_length < 0)
		{
			generate_event(_event_warning, "networking:link: requested in-channel VDP packet (%d bytes voice, %d bytes data) exceeds maximum payload (%d bytes), impossible to send", voice_data_length, *game_data_length, maximum_payload);
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
	m_out_of_band = out_of_band;
}

long c_network_link::compute_size_on_wire(s_link_packet const* packet) const
{
	return INVOKE_CLASS_MEMBER(0x0043B6A0, c_network_link, compute_size_on_wire, packet);
}

bool c_network_link::create_endpoint(e_transport_type type, word port, bool set_broadcast_option, transport_endpoint** out_endpoint)
{
	return DECLFUNC(0x0043B6F0, bool, __cdecl, e_transport_type, word, bool, transport_endpoint**)(type, port, set_broadcast_option, out_endpoint);

	//transport_endpoint* endpoint = transport_endpoint_create(type);
	//if (!endpoint)
	//{
	//	generate_event(_event_error, "networking:link: create_endpoint[%d] - unable to create endpoint!", type);
	//	return false;
	//}
	//
	//transport_address address;
	//transport_get_listen_address(&address, port);
	//
	//if (!transport_endpoint_bind(endpoint, &address) || !transport_endpoint_set_blocking(endpoint, false) || (set_broadcast_option && !transport_endpoint_set_option_value(endpoint, _transport_endpoint_option_broadcast, true)))
	//{
	//	generate_event(_event_error, "networking:link: create_endpoint(%d) - unable to setup endpoint!", type);
	//	transport_endpoint_delete(endpoint);
	//	return false;
	//}
	//
	//*out_endpoint = endpoint;
	//return true;
}

bool c_network_link::create_endpoints()
{
	//return INVOKE_CLASS_MEMBER(0x0043B7C0, c_network_link, create_endpoints);

	bool result = false;

	word broadcast_port = k_broadcast_port;
	while (true)
	{
		g_broadcast_port = broadcast_port;

		result = create_endpoint(_transport_type_udp, broadcast_port, true, &m_endpoint);
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

bool c_network_link::decode_packet(long data_buffer_size, byte const* data_buffer, s_link_packet* packet) const
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

void c_network_link::encode_packet(s_link_packet const* packet, long* data_length, byte* data_buffer, long data_buffer_size) const
{
	INVOKE_CLASS_MEMBER(0x0043B990, c_network_link, encode_packet, packet, data_length, data_buffer, data_buffer_size);
}

dword c_network_link::generate_channel_identifier()
{
	return INVOKE_CLASS_MEMBER(0x0043BA20, c_network_link, generate_channel_identifier);
}

c_network_channel* c_network_link::get_associated_channel(transport_address const* address) const
{
	return DECLFUNC(0x0043BA40, c_network_channel*, __cdecl, transport_address const*)(address);

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

	//m_time_statistics[0].initialize(2000);
	//m_time_statistics[1].initialize(2000);
	//m_time_statistics[2].initialize(2000);
	//m_time_statistics[0].initialize(2000);
	//bool result = m_simulation_queues[0].initialize_queue() && m_simulation_queues[1].initialize_queue();
	//ASSERT(transport_get_packet_overhead(_transport_type_udp) + k_network_link_maximum_game_data_size <= k_network_link_maximum_encoded_packet_size);
	//ASSERT(transport_get_packet_overhead(_transport_type_vdp) + k_network_link_maximum_game_data_size + k_network_link_maximum_voice_data_size <= k_network_link_maximum_encoded_packet_size);
	//if (result && transport_available() && !create_endpoints())
	//	generate_event(_event_error, "networking:link: initialize() failed to create packet endpoints!");
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
	//packet->mode = _network_packet_mode_none;
	//packet->__unknown4 = false;
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

void c_network_link::process_packet_internal(s_link_packet const* packet)
{
	INVOKE_CLASS_MEMBER(0x0043BDA0, c_network_link, process_packet_internal, packet);
}

bool c_network_link::read_data_immediate(transport_address* address, long* packet_data_length, byte* packet_buffer, long packet_buffer_size)
{
	return INVOKE_CLASS_MEMBER(0x0043BEC0, c_network_link, read_data_immediate, address, packet_data_length, packet_buffer, packet_buffer_size);
}

bool c_network_link::read_packet_internal(s_link_packet* packet)
{
	return INVOKE_CLASS_MEMBER(0x0043BF50, c_network_link, read_packet_internal, packet);
}

void c_network_link::send_broadcast(c_bitstream const* game_data, long* out_size_on_wire)
{
	INVOKE_CLASS_MEMBER(0x0043BFD0, c_network_link, send_broadcast, game_data, out_size_on_wire);
}

void c_network_link::send_channel(c_bitstream const* packet, long voice_data_length, byte const* voice_data, transport_address const* remote_address, long* out_size_on_wire)
{
	INVOKE_CLASS_MEMBER(0x0043C020, c_network_link, send_channel, packet, voice_data_length, voice_data, remote_address, out_size_on_wire);
}

void c_network_link::send_data_immediate(long packet_mode, transport_address const* address, long packet_data_length, void const* packet_data)
{
	INVOKE_CLASS_MEMBER(0x0043C150, c_network_link, send_data_immediate, packet_mode, address, packet_data_length, packet_data);
}

void c_network_link::send_out_of_band(c_bitstream const* game_data, transport_address const* address, long* out_size_on_wire)
{
	ASSERT(game_data);
	ASSERT(address);

	INVOKE_CLASS_MEMBER(0x0043C250, c_network_link, send_out_of_band, game_data, address, out_size_on_wire);
}

void c_network_link::send_packet_internal(s_link_packet const* packet)
{
	INVOKE_CLASS_MEMBER(0x0043C370, c_network_link, send_packet_internal, packet);
}

