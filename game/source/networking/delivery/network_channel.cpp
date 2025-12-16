#include "networking/delivery/network_channel.hpp"

#include "cseries/cseries_events.hpp"
#include "networking/delivery/network_link.hpp"
#include "networking/messages/network_message_gateway.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/messages/network_messages_connect.hpp"
#include "networking/network_time.hpp"

int32 const c_network_channel::k_network_channel_name_size = 256;

const char* c_network_channel::get_short_name() const
{
	//return m_channel_name;
	return "";
}

const char* c_network_channel::get_name() const
{
	//return m_channel_description;
	return "";
}

const char* __cdecl c_network_channel::get_closure_reason_string(e_network_channel_closure_reason closure_reason)
{
	const char* reason_string = "<unknown>";
	switch (closure_reason)
	{
	case _network_channel_reason_none:
		reason_string = "no-reason-given";
		break;
	case _network_channel_reason_link_destroyed:
		reason_string = "link-destroyed";
		break;
	case _network_channel_reason_link_refused_listening:
		reason_string = "link-refused-listen";
		break;
	case _network_channel_reason_channel_deleted:
		reason_string = "channel-deleted";
		break;
	case _network_channel_reason_connect_request_timeout:
		reason_string = "connect-timeout";
		break;
	case _network_channel_reason_connect_refuse:
		reason_string = "connect-refused";
		break;
	case _network_channel_reason_connect_reinitiated:
		reason_string = "connect-reinitiate";
		break;
	case _network_channel_reason_establish_timeout:
		reason_string = "establish-timeout";
		break;
	case _network_channel_reason_address_change:
		reason_string = "address-change";
		break;
	case _network_channel_reason_destination_unreachable:
		reason_string = "destination-unreachable";
		break;
	case _network_channel_reason_remote_closure:
		reason_string = "remote-closure";
		break;
	case _network_channel_reason_connection_overflowed:
		reason_string = "connection-overflow";
		break;
	case _network_channel_reason_message_queue_overflowed:
		reason_string = "message-overflow";
		break;
	case _network_channel_reason_observer_security_lost:
		reason_string = "security-lost";
		break;
	case _network_channel_reason_observer_owners_released:
		reason_string = "observer-released";
		break;
	case _network_channel_reason_observer_fatal_refusal:
		reason_string = "observer-refused";
		break;
	case _network_channel_reason_observer_timeout:
		reason_string = "observer-timeout";
		break;
	case _network_channel_reason_observer_reset:
		reason_string = "observer-reset";
		break;
	case _network_channel_reason_observer_reset_security:
		reason_string = "observer-reset-security";
		break;
	case _network_channel_reason_test:
		reason_string = "test";
		break;
	}
	return reason_string;
}

const char* __cdecl c_network_channel::get_state_string(e_network_channel_state state)
{
	const char* state_string = "<unknown>";
	switch (state)
	{
	case _network_channel_state_none:
		state_string = "none";
		break;
	case _network_channel_state_empty:
		state_string = "empty";
		break;
	case _network_channel_state_closed:
		state_string = "closed";
		break;
	case _network_channel_state_connecting:
		state_string = "connecting";
		break;
	case _network_channel_state_established:
		state_string = "established";
		break;
	case _network_channel_state_connected:
		state_string = "connected";
		break;
	}
	return state_string;
}

bool c_network_channel::get_remote_address(transport_address* address) const
{
	//return INVOKE_CLASS_MEMBER(0x00446570, c_network_channel, get_remote_address, address);

	ASSERT(address);

	bool has_address = false;
	if (get_state() == _network_channel_state_none && get_state() != _network_channel_state_empty)
	{
		*address = m_remote_address;
		has_address = true;
	}
	return has_address;
}

uns32 c_network_channel::get_remote_identifier() const
{
	//return INVOKE_CLASS_MEMBER(0x004465C0, c_network_channel, get_remote_identifier);

	return m_remote_channel_identifier;
}

uns32 c_network_channel::get_identifier() const
{
	return m_channel_identifier;
}

const c_network_message_queue* c_network_channel::network_message_queue_get() const
{
	return &m_message_queue;
}

const c_network_connection* c_network_channel::network_connection_get() const
{
	return &m_connection;
}

bool c_network_channel::allocated() const
{
	return get_state() != _network_channel_state_none;
}

e_network_channel_state c_network_channel::get_state() const
{
	return m_channel_state;
}

bool c_network_channel::closed() const
{
	return get_state() <= _network_channel_state_closed;
}

bool c_network_channel::established() const
{
	return get_state() >= _network_channel_state_established;
}

bool c_network_channel::connected() const
{
	return get_state() == _network_channel_state_connected;
}

void c_network_channel::close(e_network_channel_closure_reason closure_reason)
{
	//INVOKE_CLASS_MEMBER(0x0045FA30, c_network_channel, close, closure_reason);

	ASSERT(closure_reason > _network_channel_reason_none && closure_reason < k_network_channel_reason_count);
	ASSERT(allocated());
	ASSERT(!closed());

	// Halo Online log
	c_console::write_line("MP/NET/CHANNEL,CTRL: c_network_channel::close: Closing channel. Reason %s.",
		get_closure_reason_string(closure_reason));

	if (c_network_channel::connected() && closure_reason != _network_channel_reason_connect_reinitiated)
	{
		s_network_message_connect_closed connect_closed =
		{
			.remote_identifier = m_remote_channel_identifier,
			.identifier = m_channel_identifier,
			.reason = closure_reason
		};

		m_message_gateway->send_message_directed(&m_remote_address, _network_message_connect_closed, sizeof(s_network_message_connect_closed), &connect_closed);
	}

	if (m_simulation_interface)
	{
		m_simulation_interface->notify_closed();
	}

	ASSERT(m_simulation_interface == NULL);

	m_channel_state = _network_channel_state_closed;
	m_channel_closure_reason = closure_reason;
	m_channel_closure_address = m_remote_address;
	m_channel_identifier = NONE;
	m_remote_channel_identifier = NONE;
}

//.text:0045FB00 ; void c_network_channel::complete_establishment()
//.text:0045FBB0 ; void c_network_channel::deallocate()
//.text:0045FC00 ; void c_network_channel::detach_simulation(c_network_channel_simulation_interface*)

void c_network_channel::establish(uns32 remote_channel_identifier)
{
	INVOKE_CLASS_MEMBER(0x0045FC20, c_network_channel, establish, remote_channel_identifier);
}

bool c_network_channel::idle()
{
	return INVOKE_CLASS_MEMBER(0x0045FE70, c_network_channel, idle);
}

void c_network_channel::notify_destination_unreachable()
{
	//INVOKE_CLASS_MEMBER(0x00460370, c_network_channel, notify_destination_unreachable);

	if (!c_network_channel::closed())
	{
		m_destination_unreachable = true;
	}
}

void c_network_channel::open(const transport_address* remote_address, bool send_connect_packets, int32 channel_identifier)
{
	INVOKE_CLASS_MEMBER(0x004603B0, c_network_channel, open, remote_address, send_connect_packets, channel_identifier);

	//ASSERT(remote_address && transport_address_valid(remote_address));
	//ASSERT(allocated());
	//ASSERT(closed());
	//ASSERT(m_simulation_interface == NULL);
	//
	//m_remote_address = *remote_address;
	//m_channel_state = _network_channel_state_connecting;
	//m_send_connect_packets = send_connect_packets;
	//
	//// m_channel_name, m_channel_description don't exist in in Halo Online
	////rebuild_channel_description();
	//
	//if (channel_identifier == NONE)
	//	m_local_channel_identifier = m_link->generate_channel_identifier();
	//m_remote_channel_identifier = NONE;
	//
	//// Halo Online log
	//c_console::write_line("MP/NET/CHANNEL,CTRL: c_network_channel::open: Network channel opened. Addr %s. Init connect %d. Id %d.", 
	//	transport_address_get_string(remote_address),
	//	send_connect_packets,
	//	channel_identifier);
	//
	//if (m_send_connect_packets)
	//{
	//	m_connect_identifier = network_time_get();
	//	m_connect_timestamp = network_time_get();
	//	m_connect_unknown = 0;
	//	check_to_send_connect_packet();
	//}
	//
	//reset_activity_times();
	//m_connection.reset();
	//m_message_queue.reset();
	//m_message_queue.reserve_first_fragment();
}

//.text:004604D0 ; public: virtual e_network_read_result c_network_channel_simulation_gatekeeper::read_from_packet(int32*, c_bitstream*)

bool c_network_channel::receive_packet(c_bitstream* packet, int32 packet_size_on_wire, bool consistency_check_only)
{
	return INVOKE_CLASS_MEMBER(0x00460520, c_network_channel, receive_packet, packet, packet_size_on_wire, consistency_check_only);
}

//.text:00460860 ; 
//.text:00460890 ; public: void c_network_channel::reset_activity_times()
//.text:004608C0 ; private: void c_network_channel::send_connection_established(bool)

void c_network_channel::send_message(e_network_message_type message_type, int32 raw_message_size, const void* raw_message_payload)
{
	//INVOKE_CLASS_MEMBER(0x00460930, c_network_channel, send_message, message_type, raw_message_size, raw_message_payload);

	m_message_queue.send_message(message_type, raw_message_size, raw_message_payload);
}

void c_network_channel::send_packet(c_bitstream* packet, bool sequenced_packet, bool force_fill_packet, int32 voice_data_size, const void* voice_data, int32* out_bytes_on_wire, int32* out_bytes_game_data, int32* out_bytes_voice_data)
{
	INVOKE_CLASS_MEMBER(0x00460940, c_network_channel, send_packet, packet, sequenced_packet, force_fill_packet, voice_data_size, voice_data, out_bytes_on_wire, out_bytes_game_data, out_bytes_voice_data);
}

//.text:00460C20 ; public: void c_network_channel_simulation_interface::set_established(bool)
//.text:00460C30 ; 
//.text:00460C40 ; 
//.text:00460C50 ; public: virtual int32 c_network_channel_simulation_gatekeeper::space_required_bits(int32, int32)

void c_network_channel::update_activity(e_network_channel_activity activity_type)
{
	//INVOKE_CLASS_MEMBER(0x00460C60, c_network_channel, update_activity, activity_type);

	ASSERT(VALID_INDEX(activity_type, k_network_channel_activity_count));

	m_activity_timers[activity_type].timestamp = network_time_get();
}

void c_network_channel::update_packet_delivery()
{
	INVOKE_CLASS_MEMBER(0x00460CA0, c_network_channel, update_packet_delivery);
}

//.text:00460E00 ; public: static bool __cdecl c_network_channel::valid_channel_flags(uns32)

