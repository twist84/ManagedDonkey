#include "networking/delivery/network_link.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/bitstream.hpp"
#include "memory/module.hpp"
#include "networking/network_memory.hpp"
#include "networking/network_time.hpp"
#include "networking/transport/transport.hpp"
#include "networking/transport/transport_address.hpp"
#include "profiler/profiler_tracedump.hpp"

#if 0
HOOK_DECLARE_CLASS(0x0043B5E0, c_network_link, adjust_packet_size);
HOOK_DECLARE_CLASS_MEMBER(0x0043B6A0, c_network_link, compute_size_on_wire);
HOOK_DECLARE_CLASS_MEMBER(0x0043B6F0, c_network_link, create_endpoint);
HOOK_DECLARE_CLASS_MEMBER(0x0043B7C0, c_network_link, create_endpoints);
HOOK_DECLARE_CLASS_MEMBER(0x0043B820, c_network_link, decode_packet);
HOOK_DECLARE_CLASS_MEMBER(0x0043B940, c_network_link, destroy_endpoints);
HOOK_DECLARE_CLASS_MEMBER(0x0043B960, c_network_link, destroy_link);
HOOK_DECLARE_CLASS_MEMBER(0x0043B990, c_network_link, encode_packet);
HOOK_DECLARE_CLASS_MEMBER(0x0043BA20, c_network_link, generate_channel_identifier);
HOOK_DECLARE_CLASS_MEMBER(0x0043BA40, c_network_link, get_associated_channel);
HOOK_DECLARE_CLASS_MEMBER(0x0043BAB0, c_network_link, initialize_link);
HOOK_DECLARE_CLASS(0x0043BB20, c_network_link, initialize_packet);
HOOK_DECLARE_CLASS(0x0043BBA0, c_network_link, packet_has_voice);
HOOK_DECLARE_CLASS(0x0043BBB0, c_network_link, physical_link_available);
HOOK_DECLARE_CLASS_MEMBER(0x0043BBC0, c_network_link, process_all_channels);
HOOK_DECLARE_CLASS_MEMBER(0x0043BC70, c_network_link, process_incoming_packets);
HOOK_DECLARE_CLASS_MEMBER(0x0043BDA0, c_network_link, process_packet_internal);
HOOK_DECLARE_CLASS_MEMBER(0x0043BEC0, c_network_link, read_data_immediate_);
HOOK_DECLARE_CLASS_MEMBER(0x0043BF50, c_network_link, read_packet_internal);
HOOK_DECLARE_CLASS_MEMBER(0x0043BFD0, c_network_link, send_broadcast);
HOOK_DECLARE_CLASS_MEMBER(0x0043C020, c_network_link, send_channel);
HOOK_DECLARE_CLASS_MEMBER(0x0043C150, c_network_link, send_data_immediate);
HOOK_DECLARE_CLASS_MEMBER(0x0043C250, c_network_link, send_out_of_band);
HOOK_DECLARE_CLASS_MEMBER(0x0043C370, c_network_link, send_packet_internal);
#endif

bool __thiscall c_network_link::read_data_immediate_(transport_address* address, int32* packet_data_length, byte* packet_buffer, int32 packet_buffer_size)
{
	return c_network_link::read_data_immediate(_network_packet_mode_channel, address, packet_data_length, packet_buffer, packet_buffer_size);
}

c_network_link::c_network_link() :
	m_initialized(false),
	m_next_channel_identifier(0),
	m_next_first_channel_index(0),
	//m_endpoints_created(false),
	m_endpoints({ NULL }),
	//m_simulation_parameters(),
	m_out_of_band_consumer(NULL)
{
	//c_network_link::reset_simulation_state();
}

c_network_link::~c_network_link()
{
	c_network_link::destroy_link();
}

bool __cdecl c_network_link::adjust_packet_size(bool out_of_band, int32 voice_data_length, int32* game_data_length)// const
{
	return INVOKE(0x0043B5E0, c_network_link::adjust_packet_size, out_of_band, voice_data_length, game_data_length);

#if 0
	ASSERT(game_data_length);
	ASSERT(voice_data_length > 0 || *game_data_length > 0);

	bool success = false;
	if (out_of_band)
	{
		int32 maximum_payload = transport_get_packet_maximum_payload(0);

		ASSERT(voice_data_length == 0);
		ASSERT((maximum_payload % 8) == 0);

		if (*game_data_length > 8)
		{
			*game_data_length -= *game_data_length % 8;
		}

		if (*game_data_length > maximum_payload)
		{
			*game_data_length = maximum_payload;
		}

		success = true;
	}
	else
	{
		int32 maximum_payload = transport_get_packet_maximum_payload(1);
		int32 maximum_game_data_length = maximum_payload - (voice_data_length + 2) - (maximum_payload - (voice_data_length + 2)) % 8;

		if (maximum_game_data_length < 0)
		{
			event(_event_warning, "networking:link: requested in-channel VDP packet (%d bytes voice, %d bytes data) exceeds maximum payload (%d bytes), impossible to send",
				voice_data_length,
				*game_data_length,
				maximum_payload);
		}
		else
		{
			if (*game_data_length > 8)
			{
				*game_data_length -= *game_data_length % 8;
			}

			if (*game_data_length > maximum_game_data_length)
			{
				*game_data_length = maximum_game_data_length;
			}

			success = true;
		}
	}

	return true;
#endif
}

void c_network_link::attach_out_of_band(c_network_out_of_band_consumer* out_of_band)
{
	m_out_of_band_consumer = out_of_band;
}

int32 c_network_link::compute_size_on_wire(const s_link_packet* packet)// const
{
	return INVOKE_CLASS_MEMBER(0x0043B6A0, c_network_link, compute_size_on_wire, packet);

#if 0
	int32 transport_type = c_network_link::packet_has_voice(packet->packet_mode);
	int32 padded_game_data = packet->game_data_length;
	if (padded_game_data % 8 > 0)
	{
		padded_game_data += 8 - padded_game_data % 8;
	}
	padded_game_data += packet->voice_data_length + transport_get_packet_overhead(transport_type);
	return padded_game_data;
#endif
}

bool c_network_link::create_endpoint(e_transport_type endpoint_type, uns16 port, bool broadcast, transport_endpoint** out_endpoint)
{
	return INVOKE_CLASS_MEMBER(0x0043B6F0, c_network_link, create_endpoint, endpoint_type, port, broadcast, out_endpoint);

#if 0
	bool success = false;
	if (transport_endpoint* endpoint = transport_endpoint_create(endpoint_type))
	{
		transport_address listen_address{};
		transport_get_listen_address(&listen_address, port);

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
#endif
}

bool c_network_link::create_endpoints()
{
	//return INVOKE_CLASS_MEMBER(0x0043B7C0, c_network_link, create_endpoints);

	bool success = false;

#if 0
	if (c_network_link::create_endpoint(_transport_type_vdp, 1000, false, &m_endpoints[_network_packet_mode_channel]) &&
		c_network_link::create_endpoint(_transport_type_udp, 1002, false, &m_endpoints[_network_packet_mode_out_of_band]) &&
		c_network_link::create_endpoint(_transport_type_vdp, 1003, true, &m_endpoints[_network_packet_mode_broadcast]))
	{
		m_endpoints_created = true;
		success = true;
	}
	else
	{
		c_network_link::destroy_endpoints();
		m_endpoints_created = false;
		success = false;
	}
#else
	uns16 broadcast_port = k_broadcast_port;
	while (true)
	{
		g_broadcast_port = broadcast_port;

		success = c_network_link::create_endpoint(_transport_type_udp, broadcast_port, true, &m_endpoints[0]);
		if (success)
		{
			break;
		}

		c_network_link::destroy_endpoints();

		if (++broadcast_port - k_broadcast_port >= k_broadcast_port_alt_ammount)
		{
			g_broadcast_port = k_broadcast_port;
			return success;
		}
	}
#endif

	return success;
}

bool c_network_link::decode_packet(int32 data_buffer_size, const byte* data_buffer, s_link_packet* packet)// const
{
	return INVOKE_CLASS_MEMBER(0x0043B820, c_network_link, decode_packet, data_buffer_size, data_buffer, packet);

#if 0
	ASSERT(data_buffer);
	ASSERT(packet);

	bool packet_valid = true;
	if (c_network_link::packet_has_voice(packet->packet_mode))
	{
		if (data_buffer_size < 2)
		{
			event(_event_verbose, "networking:link:receive:decode_packet: received a VDP packet that was too small (%d < 2 bytes)",
				data_buffer_size);

			packet_valid = false;
		}
		else
		{
			packet->game_data_length = *(uns16*)data_buffer;
			packet->voice_data_length = data_buffer_size - (packet->game_data_length + 2);
			
			if (!IN_RANGE_INCLUSIVE(packet->game_data_length, 0, k_network_link_maximum_game_data_size) ||
				!IN_RANGE_INCLUSIVE(packet->voice_data_length, 0, k_network_link_maximum_voice_data_size))
			{
				event(_event_verbose, "networking:link:receive:decode_packet: VDP packet (game %d voice %d) is invalid (max sizes %d/%d)",
					packet->game_data_length,
					packet->voice_data_length,
					k_network_link_maximum_game_data_size,
					k_network_link_maximum_voice_data_size);

				packet_valid = false;
			}
			else
			{
				csmemcpy(
					packet->game_data_buffer,
					&data_buffer[2],
					packet->game_data_length);
				csmemcpy(
					packet->voice_data_buffer,
					&data_buffer[packet->game_data_length + 2],
					packet->voice_data_length);
			}
		}
	}
	else if (data_buffer_size > k_network_link_maximum_game_data_size)
	{
		event(_event_verbose, "networking:link:receive:decode_packet: received a game-data-only packet that was too big (%d > %d bytes)",
			data_buffer_size,
			k_network_link_maximum_game_data_size);

		packet_valid = false;
	}
	else
	{
		packet->game_data_length = data_buffer_size;
		csmemcpy(packet->game_data_buffer, data_buffer, data_buffer_size);
	}
	return packet_valid;
#endif
}

void c_network_link::destroy_endpoints()
{
	INVOKE_CLASS_MEMBER(0x0043B940, c_network_link, destroy_endpoints);

#if 0
	for (int32 packet_mode = 0; packet_mode < NUMBEROF(m_endpoints); packet_mode++)
	{
		if (m_endpoints[packet_mode])
		{
			transport_endpoint_delete(m_endpoints[packet_mode]);
			m_endpoints[packet_mode] = NULL;
		}
	}
#endif
}

void c_network_link::destroy_link()
{
	INVOKE_CLASS_MEMBER(0x0043B960, c_network_link, destroy_link);

#if 0
	//m_incoming_queue.destroy_queue();
	//m_outgoing_queue.destroy_queue();

	for (const c_network_channel* channel = network_channel_iterate(NULL);
		channel != NULL;
		channel = network_channel_iterate(channel))
	{
		ASSERT(channel->allocated() == false);
	}

	c_network_link::destroy_endpoints();
	m_out_of_band_consumer = NULL;
	m_initialized = false;
#endif
}

void c_network_link::encode_packet(const s_link_packet* packet, int32* data_length, byte* data_buffer, int32 data_buffer_size)// const
{
	INVOKE_CLASS_MEMBER(0x0043B990, c_network_link, encode_packet, packet, data_length, data_buffer, data_buffer_size);

#if 0
	ASSERT(packet);
	ASSERT(data_length);
	ASSERT(data_buffer);

	if (c_network_link::packet_has_voice(packet->packet_mode))
	{
		ASSERT(data_buffer_size >= 2 + packet->game_data_length + packet->voice_data_length);

		*(uns16*)data_buffer = (uns16)packet->game_data_length;
		csmemcpy(&data_buffer[2], packet->game_data_buffer, packet->game_data_length);
		csmemcpy(&data_buffer[packet->game_data_length + 2], packet->voice_data_buffer, packet->voice_data_length);
		*data_length = packet->game_data_length + 2 + packet->voice_data_length;
	}
	else
	{
		ASSERT(data_buffer_size >= packet->game_data_length);
		csmemcpy(data_buffer, packet->game_data_buffer, packet->game_data_length);

		ASSERT(packet->voice_data_length == 0);
		*data_length = packet->game_data_length;
	}
#endif
}

uns32 c_network_link::generate_channel_identifier()
{
	return INVOKE_CLASS_MEMBER(0x0043BA20, c_network_link, generate_channel_identifier);

#if 0
	uns32 identifier = m_next_channel_identifier;
	m_next_channel_identifier = identifier + 1;
	if (m_next_channel_identifier == NONE)
	{
		m_next_channel_identifier++;
	}
	return identifier;
#endif
}

c_network_channel* c_network_link::get_associated_channel(const transport_address* address)// const
{
	return INVOKE_CLASS_MEMBER(0x0043BA40, c_network_link, get_associated_channel, address);

#if 0
	c_network_channel* channel = NULL;
	while (channel = network_channel_iterate(channel))
	{
		if (channel->is_channel_remote_address(address))
		{
			break;
		}
	}
	return channel;
#endif
}

const char* c_network_link::get_packet_mode_string(int32 packet_mode)
{
	const char* mode_string = "<unknown>";
	switch (packet_mode)
	{
	case _network_packet_mode_channel:
		mode_string = "channel";
		break;
	case _network_packet_mode_out_of_band:
		mode_string = "out-of-band";
		break;
	case _network_packet_mode_broadcast:
		mode_string = "broadcast";
		break;
	}
	return mode_string;
}

bool c_network_link::initialize_link()
{
	return INVOKE_CLASS_MEMBER(0x0043BAB0, c_network_link, initialize_link);

#if 0
	m_packets_transmitted.initialize(k_network_link_statistics_period);
	m_packets_received.initialize(k_network_link_statistics_period);
	m_upstream_bandwidth.initialize(k_network_link_statistics_period);
	m_downstream_bandwidth.initialize(k_network_link_statistics_period);
	
	bool success = true;// m_incoming_queue.initialize_queue() && m_outgoing_queue.initialize_queue();

	ASSERT(transport_get_packet_overhead(_transport_type_udp) + k_network_link_maximum_game_data_size <= k_network_link_maximum_encoded_packet_size);
	ASSERT(transport_get_packet_overhead(_transport_type_vdp) + k_network_link_maximum_game_data_size + k_network_link_maximum_voice_data_size <= k_network_link_maximum_encoded_packet_size);
	
	if (success)
	{
		if (transport_available() && !c_network_link::create_endpoints())
		{
			event(_event_error, "networking:link: initialize() failed to create packet endpoints!");
		}
		m_initialized = true;
	}
	else
	{
		c_network_link::destroy_link();
	}
	
	return success;
#endif
}

void __cdecl c_network_link::initialize_packet(s_link_packet* packet)
{
	INVOKE(0x0043BB20, c_network_link::initialize_packet, packet);

#if 0
	ASSERT(packet);
	packet->packet_mode = NONE;
	packet->simulate_packet = false;
	packet->address = {};
	packet->game_data_length = 0;
	packet->voice_data_length = 0;
#endif
}

bool c_network_channel::is_channel_remote_address(const transport_address* address) const
{
	return INVOKE_CLASS_MEMBER(0x0043BB60, c_network_channel, is_channel_remote_address, address);

#if 0
	bool is_address = false;
	if (c_network_channel::get_state() && c_network_channel::get_state() != _network_channel_state_empty)
	{
		is_address = transport_address_equivalent(&m_remote_address, address);
	}
	return is_address;
#endif
}

bool __cdecl c_network_link::packet_has_voice(int32 packet_mode)
{
	return INVOKE(0x0043BBA0, c_network_link::packet_has_voice, packet_mode);

#if 0
	return packet_mode == _network_packet_mode_channel;
#endif
}

bool __cdecl c_network_link::physical_link_available()
{
	return INVOKE(0x0043BBB0, c_network_link::physical_link_available);

#if 0
	return transport_available();
#endif
}

void c_network_link::process_all_channels()
{
	INVOKE_CLASS_MEMBER(0x0043BBC0, c_network_link, process_all_channels);

#if 0
	c_network_channel* channels[k_maximum_machines * 2] = {};
	int32 channel_count = 0;

	for (c_network_channel* channel = network_channel_iterate(NULL);
		channel != NULL;
		channel = network_channel_iterate(channel))
	{
		ASSERT(channel_count < NUMBEROF(channels));
		channels[channel_count++] = channel;
	}

	if (channel_count)
	{
		bool sent_packet = false;

		int32 channel_start = m_next_first_channel_index % channel_count;
		for (int32 channel_index = channel_start;
			channel_index != channel_start;
			channel_index = (channel_index + 1) % channel_count)
		{
			c_network_channel* channel = channels[channel_index];
			ASSERT(channels[channel_index] != nullptr);

			if (channel->idle() && channel_index == channel_start)
			{
				sent_packet = true;
			}
		}

		if (sent_packet)
		{
			m_next_first_channel_index++;
		}
	}

	//if (m_simulation_parameters.enabled)
	//{
	//	uns32 current_timestamp = network_time_get();
	//	c_network_simulation_queue::update_simulation_state(&m_simulation_parameters, &m_simulation_state, current_timestamp);
	//	send_pending_simulated_packets(current_timestamp);
	//}
#endif
}

void c_network_link::process_incoming_packets()
{
	INVOKE_CLASS_MEMBER(0x0043BC70, c_network_link, process_incoming_packets);

#if 0
	// $TODO simulate packet logic

	ASSERT(m_initialized);

	c_network_channel* processed_channels[16]{};
	bool tracing_enabled = trace_dump_is_trace_enabled();

	bool packet_received = false;
	do
	{
		transport_address source_address{};
		int32 source_packet_mode = NONE;
		int32 packet_length = 0;
		byte packet_buffer[4096]{};

		packet_received = false;
		for (int32 read_packet_mode = 0; read_packet_mode < NUMBEROF(m_endpoints); read_packet_mode++)
		{
			if (packet_received = c_network_link::read_data_immediate(read_packet_mode, &source_address, &packet_length, packet_buffer, sizeof(packet_buffer)))
			{
				source_packet_mode = read_packet_mode;
				break;
			}
		}

		s_link_packet packet{};

		bool packet_valid = false;
		if (packet_received)
		{
			c_network_link::initialize_packet(&packet);
			packet.packet_mode = source_packet_mode;
			packet.address = source_address;
			packet_valid = c_network_link::decode_packet(packet_length, packet_buffer, &packet);
		}

		if (packet_valid)
		{
			bool process_packet = true;
			if (tracing_enabled)
			{
				if (packet.packet_mode == _network_packet_mode_out_of_band || packet.packet_mode == _network_packet_mode_broadcast)
				{
					process_packet = false;
				}
				else if (packet.packet_mode == _network_packet_mode_channel)
				{
					c_network_channel* channel = c_network_link::get_associated_channel(&packet.address);
					for (int32 index = 0; index < NUMBEROF(processed_channels); index++)
					{
						if (channel == processed_channels[index])
						{
							process_packet = false;
							break;
						}

						if (!processed_channels[index] == NULL)
						{
							processed_channels[index] = channel;
							break;
						}
					}
				}
			}

			if (process_packet)
			{
				c_network_link::process_packet_internal(&packet);
			}
		}
	}
	while (packet_received);
#endif
}

void c_network_link::process_packet_internal(const s_link_packet* packet)
{
	INVOKE_CLASS_MEMBER(0x0043BDA0, c_network_link, process_packet_internal, packet);

#if 0
	ASSERT(packet);

	int32 packet_size_on_wire = c_network_link::compute_size_on_wire(packet);

	m_packets_received.add_event(packet->game_data_length);
	m_downstream_bandwidth.add_event(packet_size_on_wire);

	switch (packet->packet_mode)
	{
	case _network_packet_mode_channel:
	{
		c_network_channel* channel = c_network_link::get_associated_channel(&packet->address);
		if (channel != NULL && !channel->closed())
		{
			if (packet->game_data_length > 0)
			{
				// I don't like this
				c_bitstream game_data((uns8*)packet->game_data_buffer, packet->game_data_length);

				channel->receive_packet(&game_data, packet_size_on_wire, false);
			}

			if (packet->voice_data_length > 0)
			{
				//network_process_voice_data(packet->voice_data_buffer, packet->voice_data_length);
				channel->update_activity(_network_channel_activity_receive_voice);
			}
		}
	}
	break;
	case _network_packet_mode_out_of_band:
	{
		// I don't like this
		c_bitstream game_data((uns8*)packet->game_data_buffer, packet->game_data_length);

		if (m_out_of_band_consumer)
		{
			m_out_of_band_consumer->receive_out_of_band_packet(&packet->address, &game_data);
		}
	}
	break;
	case _network_packet_mode_broadcast:
	{
		// I don't like this
		c_bitstream game_data((uns8*)packet->game_data_buffer, packet->game_data_length);

		printf("");
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}
#endif
}

bool c_network_link::read_data_immediate(int32 packet_mode, transport_address* address, int32* packet_data_length, byte* packet_buffer, int32 packet_buffer_size)
{
	return INVOKE_CLASS_MEMBER(0x0043BEC0, c_network_link, read_data_immediate_, address, packet_data_length, packet_buffer, packet_buffer_size);

#if 0
	ASSERT(VALID_INDEX(packet_mode, k_network_packet_mode_count));
	ASSERT(address);
	ASSERT(packet_data_length);
	ASSERT(packet_buffer);
	ASSERT(packet_buffer_size > 0);

	bool packet_received = false;
	if (m_endpoints[packet_mode])
	{
		int32 bytes_read = transport_endpoint_read_from(m_endpoints[packet_mode], packet_buffer, (int16)packet_buffer_size, address);
		if (bytes_read > 0)
		{
			event(_event_verbose, "networking:link:receive: received %d-byte packet on mode %s from '%s'",
				bytes_read,
				c_network_link::get_packet_mode_string(packet_mode),
				transport_address_get_string(address));

			if (transport_address_valid(address))
			{
				*packet_data_length = bytes_read;
				packet_received = true;
			}
			else
			{
				event(_event_verbose, "networking:link:receive: read_data_immediate: read %d-byte packet on mode %s from invalid address '%s'",
					bytes_read,
					c_network_link::get_packet_mode_string(packet_mode),
					transport_address_get_string(address));
			}
		}
		else if (bytes_read < 0 && bytes_read != _transport_readwrite_error_would_block)
		{
			event(_event_verbose, "networking:link:receive: read_data_immediate: transport error #%d reading on packet mode %s",
				bytes_read,
				c_network_link::get_packet_mode_string(packet_mode));
		}
	}
	return packet_received;
#endif
}

bool c_network_link::read_packet_internal(s_link_packet* packet)
{
	return INVOKE_CLASS_MEMBER(0x0043BF50, c_network_link, read_packet_internal, packet);

#if 0
	ASSERT(packet);

	transport_address source_address{};
	int32 data_length = 0;
	byte data_buffer[4096]{};

	bool simulated_packets = false;
	bool data_received = false;
	int32 source_packet_mode;

#if 0
	if (m_simulation_parameters.enabled)
	{
		data_received = m_incoming_queue.dequeue_packet(
			network_time_get(),
			false,
			&source_packet_mode,
			&source_address,
			&data_length,
			data_buffer,
			sizeof(data_buffer));
		simulated_packets = true;
	}
#endif
	
	if (!data_received)
	{
		for (int32 packet_mode = 0; packet_mode < NUMBEROF(m_endpoints); packet_mode++)
		{
			bool allow_direct_reception = !simulated_packets;
			if (allow_direct_reception && c_network_link::read_data_immediate(packet_mode, &source_address, &data_length, data_buffer, sizeof(data_buffer)))
			{
				source_packet_mode = packet_mode;
				data_received = true;
				break;
			}
		}
	}
	
	if (data_received)
	{
		packet->packet_mode = source_packet_mode;
		packet->address = source_address;
		data_received = c_network_link::decode_packet(data_length, data_buffer, packet);
	}

	return data_received;
#endif
}

void c_network_link::send_broadcast(const c_bitstream* game_data, int32* out_size_on_wire)
{
	INVOKE_CLASS_MEMBER(0x0043BFD0, c_network_link, send_broadcast, game_data, out_size_on_wire);

#if 0
	transport_address broadcast_address{};
	transport_get_broadcast_address(&broadcast_address, 11774 /*1003*/);
	
	int32 bytes_transmitted_on_wire = 0;
	c_network_link::send_out_of_band(game_data, &broadcast_address, &bytes_transmitted_on_wire);
	
	if (out_size_on_wire)
	{
		*out_size_on_wire = bytes_transmitted_on_wire;
	}
#endif
}

void c_network_link::send_channel(const c_bitstream* game_data, int32 voice_data_length, const byte* voice_data, const transport_address* address, int32* out_bytes_on_wire)
{
	INVOKE_CLASS_MEMBER(0x0043C020, c_network_link, send_channel, game_data, voice_data_length, voice_data, address, out_bytes_on_wire);

#if 0
	ASSERT(voice_data_length == 0 || voice_data != NULL);

	int32 bytes_transmitted_on_wire = 0;
	bool packet_valid = true;
	bool simulate_packets = false; //simulate_packets = m_simulation_parameters.enabled;

	c_network_link::s_link_packet packet{};
	c_network_link::initialize_packet(&packet);
	packet.packet_mode = _network_packet_mode_channel;
	packet.simulate_packet = simulate_packets;
	packet.address = *address;

	if (game_data)
	{
		const byte* game_data_payload = game_data->get_data(&packet.game_data_length);
		if (packet.game_data_length > k_network_link_maximum_game_data_size)
		{
			event(_event_error, "networking:link:send_channel: game data too large [%d > %d bytes]",
				packet.game_data_length,
				k_network_link_maximum_game_data_size);

			packet_valid = false;
		}
		else
		{
			csmemcpy(packet.game_data_buffer, game_data_payload, packet.game_data_length);
		}
	}

	if (packet_valid && voice_data_length > 0)
	{
		ASSERT(voice_data);

		if (voice_data_length > k_network_link_maximum_voice_data_size)
		{
			event(_event_error, "networking:link:send_channel: voice data too large [%d > %d bytes]",
				voice_data_length,
				k_network_link_maximum_voice_data_size);

			packet_valid = false;
		}
		else
		{
			csmemcpy(packet.voice_data_buffer, voice_data, voice_data_length);
		}
	}

	if (packet_valid)
	{
		c_network_link::send_packet_internal(&packet);
		bytes_transmitted_on_wire = c_network_link::compute_size_on_wire(&packet);
	}

	if (out_bytes_on_wire)
	{
		*out_bytes_on_wire = bytes_transmitted_on_wire;
	}
#endif
}

void c_network_link::send_data_immediate(int32 packet_mode, const transport_address* address, int32 packet_data_length, const void* packet_data)
{
	INVOKE_CLASS_MEMBER(0x0043C150, c_network_link, send_data_immediate, packet_mode, address, packet_data_length, packet_data);

#if 0
	ASSERT(VALID_INDEX(packet_mode, k_network_packet_mode_count));
	ASSERT(transport_address_valid(address));
	ASSERT(packet_data_length > 0);
	ASSERT(packet_data);

	transport_address full_address = *address;

#if 0
	uns16 destination_port = 0;
	switch (packet_mode)
	{
	case _network_packet_mode_channel:
		destination_port = 1000;
		break;
	case _network_packet_mode_out_of_band:
		destination_port = 1002;
		break;
	case _network_packet_mode_broadcast:
		destination_port = 1003;
		break;
	default:
		UNREACHABLE();
		break;
	}
	
	if (destination_port != 0)
	{
		full_address.port = destination_port;
	}
#endif

	if (m_endpoints[packet_mode])
	{
		int32 bytes_written = transport_endpoint_write_to(m_endpoints[packet_mode], packet_data, (int16)packet_data_length, &full_address);
		if (bytes_written != packet_data_length)
		{
			if (bytes_written == _transport_write_error_unreachable)
			{
				event(_event_verbose, "networking:link:send: send_data_immediate: destination unreachable writing %d bytes on packet mode %s to '%s'",
					packet_data_length,
					c_network_link::get_packet_mode_string(packet_mode),
					transport_address_get_string(&full_address));

				if (packet_mode == _network_packet_mode_channel)
				{
					c_network_channel* channel = c_network_link::get_associated_channel(address);
					if (channel)
					{
						channel->notify_destination_unreachable();
					}
				}
			}
			else if (bytes_written == _transport_readwrite_error_would_block)
			{
				event(_event_warning, "networking:link:send: send_data_immediate: send endpoint would block (buffer full?) writing %d bytes on packet mode %s to '%s'",
					packet_data_length,
					c_network_link::get_packet_mode_string(packet_mode),
					transport_address_get_string(&full_address));

			}
			else if (bytes_written >= 0)
			{
				event(_event_warning, "networking:link:send: send_data_immediate: write underflow %d<%d bytes on packet mode %s to '%s'",
					packet_data_length,
					c_network_link::get_packet_mode_string(packet_mode),
					transport_address_get_string(&full_address));
			}
			else
			{
				event(_event_warning, "networking:link:send: send_data_immediate: unknown transport error #%d writing %d bytes on packet mode %s to '%s'",
					bytes_written,
					packet_data_length,
					c_network_link::get_packet_mode_string(packet_mode),
					transport_address_get_string(&full_address));
			}
		}
	}
	else
	{
		event(_event_warning, "networking:link:send: send_data_immediate: no endpoint available writing %d bytes on packet mode %s to '%s'",
			packet_data_length,
			c_network_link::get_packet_mode_string(packet_mode),
			transport_address_get_string(&full_address));
	}
#endif
}

void c_network_link::send_out_of_band(const c_bitstream* game_data, const transport_address* address, int32* out_size_on_wire)
{
	INVOKE_CLASS_MEMBER(0x0043C250, c_network_link, send_out_of_band, game_data, address, out_size_on_wire);

#if 0
	ASSERT(game_data);
	ASSERT(address);

	c_network_link::s_link_packet packet{};
	c_network_link::initialize_packet(&packet);
	packet.packet_mode = _network_packet_mode_out_of_band;
	//packet.simulate_packet = m_simulation_parameters.enabled;
	packet.address = *address;

	int32 game_data_length;
	const byte* game_data_payload = game_data->get_data(&game_data_length);

	bool packet_valid = true;
	if (game_data_length > sizeof(packet.game_data_buffer))
	{
		event(_event_error, "networking:link:send_out_of_band: game data too large [%d > %d bytes]",
			packet.game_data_length,
			sizeof(packet.game_data_buffer));
		packet_valid = false;
	}
	else
	{
		packet.game_data_length = game_data_length;
		csmemcpy(packet.game_data_buffer, game_data_payload, packet.game_data_length);
	}

	int32 bytes_transmitted_on_wire = 0;
	if (packet_valid)
	{
		c_network_link::send_packet_internal(&packet);
		bytes_transmitted_on_wire = c_network_link::compute_size_on_wire(&packet);
	}

	if (out_size_on_wire)
	{
		*out_size_on_wire = bytes_transmitted_on_wire;
	}
#endif
}

void c_network_link::send_packet_internal(const s_link_packet* packet)
{
	INVOKE_CLASS_MEMBER(0x0043C370, c_network_link, send_packet_internal, packet);

#if 0
	ASSERT(packet);
	ASSERT(m_initialized);

	int32 data_length = 0;
	byte data_buffer[4096]{};
	c_network_link::encode_packet(packet, &data_length, data_buffer, sizeof(data_buffer));

	m_packets_transmitted.add_event(packet->game_data_length);
	m_upstream_bandwidth.add_event(c_network_link::compute_size_on_wire(packet));

	if (data_length <= transport_get_packet_maximum_payload(_transport_type_udp))
	{
		if (data_length > 0)
		{
			if (packet->simulate_packet)
			{
#if 0
				int32 packet_size_on_wire = c_network_link::compute_size_on_wire(packet);
				uns32 delivery_timestamp;
				if (c_network_simulation_queue::get_packet_delivery(&m_simulation_parameters, &m_simulation_state, network_time_get(), packet_size_on_wire, false, &delivery_timestamp))
				{
					c_network_link::add_packet_to_simulation_queue(false, delivery_timestamp, packet->packet_mode, &packet->address, data_length, data_buffer);
				}
#endif
			}
			else
			{
				c_network_link::send_data_immediate(packet->packet_mode, &packet->address, data_length, data_buffer);
			}
		}
	}
	else
	{
		event(_event_error, "networking:link:send_packet_internal: packet exceeds maximum size [%d > %d bytes]",
			data_length,
			transport_get_packet_maximum_payload(_transport_type_udp));
	}
#endif
}

