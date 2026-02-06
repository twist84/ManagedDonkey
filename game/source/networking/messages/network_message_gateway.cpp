#include "networking/messages/network_message_gateway.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/delivery/network_link.hpp"
#include "networking/messages/network_message_handler.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/messages/network_messages_session_protocol.hpp"

char const k_network_message_packet_header[] = "blam";
char const k_network_message_experimental_packet_header[] = "bexp";
bool net_experimental = false;

void c_network_message_gateway::attach_handler(c_network_message_handler* message_handler)
{
	m_message_handler = message_handler;
}

void c_network_message_gateway::destroy_gateway()
{
	if (m_initialized)
	{
		if (m_link)
		{
			m_link->attach_out_of_band(nullptr);
			m_link = nullptr;
		}

		m_message_handler = nullptr;
		m_message_types = nullptr;
		m_initialized = false;

		//c_console::write_line("MP/NET/MESSAGE,CTRL: c_network_message_gateway::destroy_gateway: Message gateway destoyed");
	}
}

bool c_network_message_gateway::initialize_gateway(c_network_link* link, const c_network_message_type_collection* message_types)
{
	ASSERT(link);
	ASSERT(message_types);

	m_link = link;
	link->attach_out_of_band(this);

	m_message_types = message_types;
	message_types->check_message_types();

	m_message_handler = nullptr;
	m_outgoing_packet_pending = false;
	m_initialized = true;

	//c_console::write_line("MP/NET/MESSAGE,CTRL: c_network_message_gateway::initialize_gateway: Message gateway inited");

	return true;
}

bool __cdecl c_network_message_gateway::read_packet_header(c_bitstream* packet)
{
	//return INVOKE(0x00483E20, c_network_message_gateway::read_packet_header, packet);

	ASSERT(packet);

	const char* header_chars = k_network_message_packet_header;
	if (net_experimental)
		header_chars = k_network_message_experimental_packet_header;

	bool invalid_header = false;
	for (int32 i = 0; i < header_chars[i]; i++)
	{
		if (static_cast<uns8>(packet->read_integer("header_char", 8)) != static_cast<uns8>(header_chars[i]))
			invalid_header = true;
	}

	if (packet->read_integer("protocol", 32) != k_network_protocol_version)
		invalid_header = true;

	return !invalid_header && !packet->error_occurred();
}

c_network_message_gateway::c_network_message_gateway() :
	c_network_out_of_band_consumer(),
	m_outgoing_packet_pending(false),
	m_outgoing_packet(m_outgoing_packet_storage, sizeof(m_outgoing_packet_storage)),
	m_link(nullptr),
	m_message_types(nullptr),
	m_message_handler(nullptr),
	m_initialized(false)
{
}

c_network_message_gateway::~c_network_message_gateway()
{
	destroy_gateway();
	m_outgoing_packet.~c_bitstream();
}

bool c_network_message_gateway::receive_out_of_band_packet(const transport_address* incoming_address, c_bitstream* packet)
{
	//return INVOKE_CLASS_MEMBER(0x00483E80, c_network_message_gateway, receive_out_of_band_packet, incoming_address, packet);

	bool result = true;

	ASSERT(transport_address_valid(incoming_address));
	ASSERT(packet);

	e_network_message_type message_type;
	int32 message_storage_size = 0;
	byte message_storage[0x40000]{};

	packet->data_is_untrusted(true);
	packet->begin_reading();

	if (read_packet_header(packet))
	{
		packet->data_is_untrusted(false);
		while (packet->read_bool("has_message"))
		{
			result = m_message_types->decode_message(packet, &message_type, &message_storage_size, message_storage);
			if (!result)
			{
				event(_event_verbose, "networking:messages:gateway:anomaly: failed to parse %d/%s %d-byte message from '%s'",
					message_type,
					m_message_types->get_message_type_name(message_type),
					message_storage_size,
					transport_address_get_string(incoming_address));

				break;
			}

			event(_event_verbose, "networking:messages:gateway:receive: received %d/%s %d-byte message out-of-band from '%s'",
				message_type,
				m_message_types->get_message_type_name(message_type),
				message_storage_size,
				transport_address_get_string(incoming_address));

			if (m_message_handler)
				m_message_handler->handle_out_of_band_message(incoming_address, message_type, message_storage_size, message_storage);
		}

		if (result)
			packet->finish_reading();
	}
	else
	{
		result = false;

		int32 data_length = 0;
		const uns8* data = packet->get_data(&data_length);
		c_static_string<256> data_string;

		// added by us
		data_string.append("{ ");

		for (int32 offset = 0;; offset++)
		{
			int32 length = data_length <= 8 ? data_length : 8;
			if (offset >= length)
				break;

			data_string.append_print("%02X", data[offset]);

			// added by us
			data_string.append(" ");
		}

		// added by us
		data_string.append("}");

		static char address_string[256]{};
		transport_address_to_string(incoming_address, nullptr, address_string, 256, true, true);

		event(_event_verbose, "networking:messages:gateway:anomaly: foreign packet (%d bytes %s%s) from '%s'",
			data_length,
			data_length <= 8 ? "" : "beginning ",
			data_string.get_string(),
			address_string);
	}

	packet->data_is_untrusted(false);

	return result;
}

void c_network_message_gateway::send_all_pending_messages()
{
	//INVOKE_CLASS_MEMBER(0x00483F50, c_network_message_gateway, send_all_pending_messages);

	ASSERT(m_initialized);

	if (m_outgoing_packet_pending)
	{
		bool result = true;

		ASSERT(m_outgoing_packet.was_writing());
		ASSERT(!m_outgoing_packet.would_overflow(1));

		m_outgoing_packet.write_bool("has_message", false);
		m_outgoing_packet.finish_writing(nullptr);

		if (m_outgoing_packet.begin_consistency_check())
		{
			result = read_packet_header(&m_outgoing_packet);
			if (!result)
				event(_event_critical, "networking:messages:gateway:send_all_pending_messages: an outgoing message header is corrupt!");

			e_network_message_type message_type;
			int32 message_storage_size = 0;
			byte message_storage[0x40000]{};

			while (m_outgoing_packet.read_bool("has_message"))
			{
				result = m_message_types->decode_message(&m_outgoing_packet, &message_type, &message_storage_size, message_storage);
				if (!result)
				{
					event(_event_critical, "networking:messages:gateway:send_all_pending_messages: an outgoing message payload is corrupt!");
					break;
				}

				m_message_types->dispose_message(message_type, message_storage_size, message_storage);
			}

			m_outgoing_packet.finish_reading();
		}

		if (result)
			m_link->send_out_of_band(&m_outgoing_packet, &m_outgoing_packet_address, nullptr);

		m_outgoing_packet_pending = false;
	}

	ASSERT(!m_outgoing_packet_pending);
	ASSERT(!m_outgoing_packet.reading() && !m_outgoing_packet.writing());
}

bool c_network_message_gateway::send_message_broadcast(e_network_message_type message_type, int32 data_size, const void* data, uns16 port)
{
	//return INVOKE_CLASS_MEMBER(0x00484080, c_network_message_gateway, send_message_broadcast, message_type, data_size, data, port);

	transport_address outgoing_address{};
	transport_get_broadcast_address(&outgoing_address, port);
	return send_message_directed(&outgoing_address, message_type, data_size, data);
}

bool c_network_message_gateway::send_message_directed(const transport_address* outgoing_address, e_network_message_type message_type, int32 data_size, const void* data)
{
	//return INVOKE_CLASS_MEMBER(0x004840C0, c_network_message_gateway, send_message_directed, outgoing_address, message_type, data_size, data);

	ASSERT(outgoing_address);
	ASSERT(transport_address_valid(outgoing_address));

	if (m_outgoing_packet_pending && !transport_address_equivalent(&m_outgoing_packet_address, outgoing_address))
		send_all_pending_messages();

	int32 encoded_bits = 0;
	while (true)
	{
		bool v12 = false;
		if (!m_outgoing_packet_pending)
		{
			int32 packet_size = sizeof(m_outgoing_packet_storage);
			if (!m_link->adjust_packet_size(true, 0, &packet_size))
			{
				event(_event_error, "networking:messages:gateway:send_message: unable to get packet size to send [tried %d bytes]", packet_size);
				break;
			}

			ASSERT(!m_outgoing_packet.reading() && !m_outgoing_packet.writing());
			ASSERT(packet_size <= sizeof(m_outgoing_packet_storage));

			m_outgoing_packet.set_data(m_outgoing_packet_storage, packet_size);
			m_outgoing_packet.begin_writing(1);

			m_outgoing_packet_address = *outgoing_address;
			m_outgoing_packet_pending = true;

			v12 = true;

			write_packet_header();
		}

		ASSERT(m_outgoing_packet_pending);
		ASSERT(m_outgoing_packet.was_writing());
		ASSERT(!m_outgoing_packet.would_overflow(1));

		m_outgoing_packet.push_position();
		m_outgoing_packet.write_bool("has_message", true);

		int32 space_used_in_bits = m_outgoing_packet.get_space_used_in_bits();
		m_message_types->encode_message(&m_outgoing_packet, message_type, data_size, data);
		encoded_bits = m_outgoing_packet.get_space_used_in_bits() - space_used_in_bits;

		if (m_outgoing_packet.would_overflow(1))
		{
			if (v12)
			{
				m_outgoing_packet.pop_position(true);
				break;
			}

			m_outgoing_packet.pop_position(true);
			send_all_pending_messages();
		}
		else
		{
			m_outgoing_packet.pop_position(false);
			return true;
		}
	}

	event(_event_critical, "networking:messages:gateway:send_message: message type '%s' size raw-bytes/enc-bits %d/%d doesn't fit in a packet and cannot be sent out-of-band",
		m_message_types->get_message_type_name(message_type),
		data_size,
		encoded_bits);

	return false;
}

void c_network_message_gateway::send_pending_messages_to_address(const transport_address* address)
{
	ASSERT(m_initialized);

	if (m_outgoing_packet_pending && transport_address_equivalent(&m_outgoing_packet_address, address))
		send_all_pending_messages();
}

void c_network_message_gateway::write_packet_header()
{
	//INVOKE_CLASS_MEMBER(0x004842D0, c_network_message_gateway, write_packet_header);

	ASSERT(m_outgoing_packet_pending);

	const char* header_chars = k_network_message_packet_header;
	if (net_experimental)
		header_chars = k_network_message_experimental_packet_header;

	for (int32 i = 0; i < header_chars[i]; i++)
		m_outgoing_packet.write_integer("header_char", header_chars[i], 8);

	m_outgoing_packet.write_integer("protocol", k_network_protocol_version, 32);
}

