#include "networking/delivery/network_channel.hpp"

#include "cseries/cseries_console.hpp"
#include "networking/messages/network_message_gateway.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/messages/network_messages_connect.hpp"
#include "networking/network_time.hpp"

long const c_network_channel::k_network_channel_name_size = 256;

char const* c_network_channel::get_short_name() const
{
	//return m_channel_name;
	return "";
}

char const* c_network_channel::get_name() const
{
	//return m_channel_description;
	return "";
}

char const* __cdecl c_network_channel::get_closure_reason_string(e_network_channel_closure_reason closure_reason)
{
	switch (closure_reason)
	{
	case _network_channel_reason_none:
		return "no-reason-given";
	case _network_channel_reason_link_destroyed:
		return "link-destroyed";
	case _network_channel_reason_link_refused_listen:
		return "link-refused-listen";
	case _network_channel_reason_channel_deleted:
		return "channel-deleted";
	case _network_channel_reason_connect_timeout:
		return "connect-timeout";
	case _network_channel_reason_connect_refused:
		return "connect-refused";
	case _network_channel_reason_connect_reinitiate:
		return "connect-reinitiate";
	case _network_channel_reason_establish_timeout:
		return "establish-timeout";
	case _network_channel_reason_address_change:
		return "address-change";
	case _network_channel_reason_destination_unreachable:
		return "destination-unreachable";
	case _network_channel_reason_remote_closure:
		return "remote-closure";
	case _network_channel_reason_connection_overflow:
		return "connection-overflow";
	case _network_channel_reason_message_overflow:
		return "message-overflow";
	case _network_channel_reason_security_lost:
		return "security-lost";
	case _network_channel_reason_observer_released:
		return "observer-released";
	case _network_channel_reason_observer_refused:
		return "observer-refused";
	case _network_channel_reason_observer_timeout:
		return "observer-timeout";
	case _network_channel_reason_observer_reset:
		return "observer-reset";
	case _network_channel_reason_observer_reset_security:
		return "observer-reset-security";
	case _network_channel_reason_test:
		return "test";
	}

	return "<unknown>";
}

char const* __cdecl c_network_channel::get_state_string(e_network_channel_state state)
{
	switch (state)
	{
	case _network_channel_state_none:
		return "none";
	case _network_channel_state_empty:
		return "empty";
	case _network_channel_state_closed:
		return "closed";
	case _network_channel_state_connecting:
		return "connecting";
	case _network_channel_state_established:
		return "established";
	case _network_channel_state_connected:
		return "connected";
	}

	return "<unknown>";
}

dword __cdecl c_network_channel::get_remote_identifier() const
{
	return m_remote_channel_identifier;
}

dword __cdecl c_network_channel::get_identifier() const
{
	return m_local_channel_identifier;
}

bool __cdecl c_network_channel::get_remote_address(transport_address* address) const
{
	ASSERT(address);

	if (get_state() == _network_channel_state_none && get_state() != _network_channel_state_empty)
	{
		*address = m_remote_address;
		return true;
	}

	return false;
}

c_network_message_queue const* __cdecl c_network_channel::network_message_queue_get() const
{
	return &m_message_queue;
}

c_network_connection const* __cdecl c_network_channel::network_connection_get() const
{
	return &m_connection;
}

bool __cdecl c_network_channel::allocated() const
{
	return get_state() != _network_channel_state_none;
}

e_network_channel_state __cdecl c_network_channel::get_state() const
{
	return m_channel_state;
}

bool __cdecl c_network_channel::closed() const
{
	return get_state() <= _network_channel_state_closed;
}

bool __cdecl c_network_channel::established() const
{
	return get_state() >= _network_channel_state_established;
}

bool __cdecl c_network_channel::connected() const
{
	return get_state() == _network_channel_state_connected;
}

void __cdecl c_network_channel::close(e_network_channel_closure_reason closure_reason)
{
	ASSERT(closure_reason > _network_channel_reason_none && closure_reason < k_network_channel_reason_count);
	ASSERT(allocated());
	ASSERT(!closed());

	// Halo Online log
	c_console::write_line("MP/NET/CHANNEL,CTRL: c_network_channel::close: Closing channel. Reason %s.",
		get_closure_reason_string(closure_reason));

	if (connected() && closure_reason != _network_channel_reason_connect_reinitiate)
	{
		s_network_message_connect_closed connect_closed =
		{
			.remote_identifier = m_remote_channel_identifier,
			.identifier = m_local_channel_identifier,
			.reason = closure_reason
		};

		m_message_gateway->send_message_directed(&m_remote_address, _network_message_connect_closed, sizeof(s_network_message_connect_closed), &connect_closed);
	}

	if (m_simulation_interface)
		m_simulation_interface->notify_closed();

	ASSERT(m_simulation_interface == NULL);

	m_channel_state = _network_channel_state_closed;
	m_channel_closure_reason = closure_reason;
	m_local_address = m_remote_address;
	m_local_channel_identifier = NONE;
	m_remote_channel_identifier = NONE;
}

void __cdecl c_network_channel::open(transport_address const* remote_address, bool send_connect_packets, long channel_identifier)
{
	DECLFUNC(0x004603B0, void, __thiscall, c_network_channel*, transport_address const*, bool, long)(this, remote_address, send_connect_packets, channel_identifier);

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

