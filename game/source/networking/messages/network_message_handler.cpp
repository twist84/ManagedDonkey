#include "networking/messages/network_message_handler.hpp"

#include "cseries/cseries_console.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/transport/transport_address.hpp"

void __cdecl c_network_message_handler::handle_out_of_band_message(transport_address const* incoming_address, e_network_message_type message_type, long message_storage_size, void const* message_storage)
{
	DECLFUNC(0x0049D2C0, void, __thiscall, c_network_message_handler*, transport_address const*, e_network_message_type, long, void const*)(this, incoming_address, message_type, message_storage_size, message_storage);
	return;

	ASSERT(m_initialized);

	//c_console::write_line("networking:messages:handler: %d/%s received out-of-band from '%s'",
	//	message_type,
	//	m_message_types->get_message_type_name(message_type),
	//	transport_address_get_string(incoming_address));
	//
	//union // quick type conversion union
	//{
	//	void const* pointer;
	//
	//	s_network_message_ping const* message_ping;
	//	s_network_message_pong const* message_pong;
	//	s_network_message_broadcast_search const* message_broadcast_search;
	//	s_network_message_broadcast_reply const* message_broadcast_reply;
	//	s_network_message_connect_request const* message_connect_request;
	//	s_network_message_connect_refuse const* message_connect_refuse;
	//	s_network_message_connect_establish const* message_connect_establish;
	//	s_network_message_connect_closed const* message_connect_closed;
	//	s_network_message_join_request const* message_join_request;
	//	s_network_message_peer_connect const* message_peer_connect;
	//	s_network_message_join_abort const* message_join_abort;
	//	s_network_message_join_refuse const* message_join_refuse;
	//	s_network_message_leave_session const* message_leave_session;
	//	s_network_message_leave_acknowledge const* message_leave_acknowledge;
	//	s_network_message_session_disband const* message_session_disband;
	//	s_network_message_session_boot const* message_session_boot;
	//	s_network_message_time_synchronize const* message_time_synchronize;
	//};
	//
	//pointer = message_storage;
	//
	//switch (message_type)
	//{
	//case _network_message_ping:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_ping));
	//	handle_ping(incoming_address, message_ping);
	//}
	//break;
	//case _network_message_pong:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_pong));
	//	handle_pong(incoming_address, message_pong);
	//}
	//break;
	//case _network_message_broadcast_search:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_broadcast_search));
	//	handle_broadcast_search(incoming_address, message_broadcast_search);
	//}
	//break;
	//case _network_message_broadcast_reply:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_broadcast_reply));
	//	handle_broadcast_reply(incoming_address, message_broadcast_reply);
	//}
	//break;
	//case _network_message_connect_request:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_connect_request));
	//	handle_connect_request(incoming_address, message_connect_request);
	//}
	//break;
	//case _network_message_connect_refuse:
	//{
	//	c_network_channel* associated_channel = m_link->get_associated_channel(incoming_address);
	//	if (associated_channel)
	//	{
	//		ASSERT(message_storage_size == sizeof(s_network_message_connect_refuse));
	//		handle_connect_refuse(associated_channel, message_connect_refuse);
	//	}
	//}
	//break;
	//case _network_message_connect_establish:
	//{
	//	c_network_channel* associated_channel = m_link->get_associated_channel(incoming_address);
	//	if (associated_channel)
	//	{
	//		ASSERT(message_storage_size == sizeof(s_network_message_connect_establish));
	//		handle_connect_establish(associated_channel, message_connect_establish);
	//	}
	//}
	//break;
	//case _network_message_connect_closed:
	//case _network_message_connect_establish:
	//{
	//	c_network_channel* associated_channel = m_link->get_associated_channel(incoming_address);
	//	if (associated_channel)
	//	{
	//		ASSERT(message_storage_size == sizeof(s_network_message_connect_closed));
	//		handle_connect_closed(associated_channel, message_connect_closed);
	//	}
	//}
	//break;
	//case _network_message_join_request:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_join_request));
	//	handle_join_request(incoming_address, message_join_request);
	//}
	//break;
	//case _network_message_peer_connect:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_peer_connect));
	//	handle_peer_connect(incoming_address, message_peer_connect);
	//}
	//break;
	//case _network_message_join_abort:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_join_abort));
	//	handle_join_abort(incoming_address, message_join_abort);
	//}
	//break;
	//case _network_message_join_refuse:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_join_refuse));
	//	handle_join_refuse(incoming_address, message_join_refuse);
	//}
	//break;
	//case _network_message_leave_session:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_leave_session));
	//	handle_leave_session(incoming_address, message_leave_session);
	//}
	//break;
	//case _network_message_leave_acknowledge:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_leave_acknowledge));
	//	handle_leave_acknowledge(incoming_address, message_leave_acknowledge);
	//}
	//break;
	//case _network_message_session_disband:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_session_disband));
	//	handle_session_disband(incoming_address, message_session_disband);
	//}
	//break;
	//case _network_message_session_boot:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_session_boot));
	//	handle_session_boot(incoming_address, message_session_boot);
	//}
	//break;
	//case _network_message_time_synchronize:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_time_synchronize));
	//	handle_time_synchronize(incoming_address, message_time_synchronize);
	//}
	//break;
	//default:
	//{
	//	c_console::write_line("networking:messages:handler: %d/%s from '%s' cannot be handled out-of-band, discarding",
	//		message_type,
	//		m_message_types->get_message_type_name(message_type),
	//		transport_address_get_string(incoming_address));
	//}
	//break;
	//}
}
