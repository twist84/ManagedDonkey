#include "networking/messages/network_message_handler.hpp"

#include "cseries/cseries_console.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/messages/network_messages_connect.hpp"
#include "networking/messages/network_messages_session_protocol.hpp"
#include "networking/messages/network_messages_session_membership.hpp"
#include "networking/messages/network_messages_session_parameters.hpp"
#include "networking/messages/network_messages_simulation.hpp"
#include "networking/messages/network_messages_simulation_synchronous.hpp"
#include "networking/messages/network_messages_test.hpp"
#include "networking/messages/network_messages_simulation_distributed.hpp"

void __cdecl c_network_message_handler::handle_out_of_band_message(transport_address const* incoming_address, e_network_message_type message_type, long message_storage_size, void const* message_storage)
{
	ASSERT(m_initialized);

	#define RECEIVED_LOG c_console::write_line("networking:messages:handler: %d/%s received out-of-band from '%s'", \
		message_type, \
		m_message_types->get_message_type_name(message_type), \
		transport_address_get_string(incoming_address));
	
	network_message_converter_t converter = { .message_storage = message_storage };

	bool message_type_handled = false;
	switch (message_type)
	{
	//case _network_message_ping:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_ping));
	//	handle_ping(incoming_address, converter.message_ping);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_pong:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_pong));
	//	handle_pong(incoming_address, converter.message_pong);
	//	message_type_handled = true;
	//}
	//break;
	case _network_message_broadcast_search:
	{
		RECEIVED_LOG;
		ASSERT(message_storage_size == sizeof(s_network_message_broadcast_search));
		handle_broadcast_search(incoming_address, converter.message_broadcast_search);
		message_type_handled = true;
	}
	break;
	case _network_message_broadcast_reply:
	{
		RECEIVED_LOG;
		ASSERT(message_storage_size == sizeof(s_network_message_broadcast_reply));
		handle_broadcast_reply(incoming_address, converter.message_broadcast_reply);
		message_type_handled = true;
	}
	break;
	//case _network_message_connect_request:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_connect_request));
	//	handle_connect_request(incoming_address, converter.message_connect_request);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_connect_refuse:
	//{
	//	c_network_channel* associated_channel = m_link->get_associated_channel(incoming_address);
	//	if (associated_channel)
	//	{
	//		ASSERT(message_storage_size == sizeof(s_network_message_connect_refuse));
	//		handle_connect_refuse(associated_channel, converter.message_connect_refuse);
	//		message_type_handled = true;
	//	}
	//}
	//break;
	//case _network_message_connect_establish:
	//{
	//	c_network_channel* associated_channel = m_link->get_associated_channel(incoming_address);
	//	if (associated_channel)
	//	{
	//		ASSERT(message_storage_size == sizeof(s_network_message_connect_establish));
	//		handle_connect_establish(associated_channel, converter.message_connect_establish);
	//		message_type_handled = true;
	//	}
	//}
	//break;
	//case _network_message_connect_closed:
	//{
	//	c_network_channel* associated_channel = m_link->get_associated_channel(incoming_address);
	//	if (associated_channel)
	//	{
	//		ASSERT(message_storage_size == sizeof(s_network_message_connect_closed));
	//		handle_connect_closed(associated_channel, converter.message_connect_closed);
	//		message_type_handled = true;
	//	}
	//}
	//break;
	//case _network_message_join_request:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_join_request));
	//	handle_join_request(incoming_address, converter.message_join_request);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_peer_connect:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_peer_connect));
	//	handle_peer_connect(incoming_address, converter.message_peer_connect);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_join_abort:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_join_abort));
	//	handle_join_abort(incoming_address, converter.message_join_abort);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_join_refuse:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_join_refuse));
	//	handle_join_refuse(incoming_address, converter.message_join_refuse);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_leave_session:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_leave_session));
	//	handle_leave_session(incoming_address, converter.message_leave_session);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_leave_acknowledge:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_leave_acknowledge));
	//	handle_leave_acknowledge(incoming_address, converter.message_leave_acknowledge);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_session_disband:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_session_disband));
	//	handle_session_disband(incoming_address, converter.message_session_disband);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_session_boot:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_session_boot));
	//	handle_session_boot(incoming_address, converter.message_session_boot);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_time_synchronize:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_time_synchronize));
	//	handle_time_synchronize(incoming_address, converter.message_time_synchronize);
	//	message_type_handled = true;
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
	}

	if (!message_type_handled)
		DECLFUNC(0x0049D2C0, void, __thiscall, c_network_message_handler*, transport_address const*, e_network_message_type, long, void const*)(this, incoming_address, message_type, message_storage_size, message_storage);
}

void __cdecl c_network_message_handler::handle_ping(transport_address const* incoming_address, s_network_message_ping const* message)
{
	DECLFUNC(0x0049D720, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_ping const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_pong(transport_address const* incoming_address, s_network_message_pong const* message)
{
	DECLFUNC(0x0049D9B0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_pong const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_broadcast_search(transport_address const* incoming_address, s_network_message_broadcast_search const* message)
{
	DECLFUNC(0x0049C310, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_broadcast_search const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_broadcast_reply(transport_address const* incoming_address, s_network_message_broadcast_reply const* message)
{
	DECLFUNC(0x0049C2B0, void, __cdecl, transport_address const*, s_network_message_broadcast_reply const*)(incoming_address, message);
}

void __cdecl c_network_message_handler::handle_connect_request(transport_address const* incoming_address, s_network_message_connect_request const* message)
{
	DECLFUNC(0x0049CDC0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_connect_request const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_connect_refuse(c_network_channel* channel, s_network_message_connect_refuse const* message)
{
	DECLFUNC(0x0049CD10, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_connect_refuse const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_connect_establish(c_network_channel* channel, s_network_message_connect_establish const* message)
{
	DECLFUNC(0x0049CC10, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_connect_establish const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_connect_closed(c_network_channel* channel, s_network_message_connect_closed const* message)
{
	DECLFUNC(0x0049CBA0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_connect_closed const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_join_request(transport_address const* incoming_address, s_network_message_join_request const* message)
{
	DECLFUNC(0x0049D0C0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_join_request const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_peer_connect(transport_address const* incoming_address, s_network_message_peer_connect const* message)
{
	DECLFUNC(0x0049D5C0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_peer_connect const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_join_abort(transport_address const* incoming_address, s_network_message_join_abort const* message)
{
	DECLFUNC(0x0049CF00, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_join_abort const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_join_refuse(transport_address const* incoming_address, s_network_message_join_refuse const* message)
{
	DECLFUNC(0x0049CFF0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_join_refuse const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_leave_session(transport_address const* incoming_address, s_network_message_leave_session const* message)
{
	DECLFUNC(0x0049D1C0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_leave_session const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_leave_acknowledge(transport_address const* incoming_address, s_network_message_leave_acknowledge const* message)
{
	DECLFUNC(0x0049D150, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_leave_acknowledge const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_session_disband(transport_address const* incoming_address, s_network_message_session_disband const* message)
{
	DECLFUNC(0x0049DA80, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_session_disband const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_session_boot(transport_address const* incoming_address, s_network_message_session_boot const* message)
{
	DECLFUNC(0x0049DA00, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_session_boot const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message)
{
	DECLFUNC(0x0049CEA0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_host_decline const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message)
{
	DECLFUNC(0x0049D620, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_peer_establish const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_time_synchronize(transport_address const* incoming_address, s_network_message_time_synchronize const* message)
{
	DECLFUNC(0x0049DE40, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_time_synchronize const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_membership_update(c_network_channel* channel, s_network_message_membership_update const* message)
{
	DECLFUNC(0x0049D250, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_membership_update const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message)
{
	DECLFUNC(0x0049D6B0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_peer_properties const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_delegate_leadership(c_network_channel* channel, s_network_message_delegate_leadership const* message)
{
	DECLFUNC(0x0049CDD0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_delegate_leadership const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_boot_machine(c_network_channel* channel, s_network_message_boot_machine const* message)
{
	DECLFUNC(0x0049C240, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_boot_machine const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_player_add(c_network_channel* channel, s_network_message_player_add const* message)
{
	DECLFUNC(0x0049D7F0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_player_add const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message)
{
	DECLFUNC(0x0049D8D0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_player_refuse const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_player_remove(c_network_channel* channel, s_network_message_player_remove const* message)
{
	DECLFUNC(0x0049D940, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_player_remove const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message)
{
	DECLFUNC(0x0049D860, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_player_properties const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_parameters_update(c_network_channel* channel, s_network_message_parameters_update const* message)
{
	DECLFUNC(0x0049D550, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_parameters_update const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message)
{
	DECLFUNC(0x0049D4E0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_parameters_request const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_view_establishment(c_network_channel* channel, dword a2, s_network_message_view_establishment const* message)
{
	DECLFUNC(0x0049DEC0, void, __thiscall, c_network_message_handler*, c_network_channel*, dword, s_network_message_view_establishment const*)(this, channel, a2, message);
}

void __cdecl c_network_message_handler::handle_player_acknowledge(c_network_channel* channel, s_network_message_player_acknowledge const* message)
{
	DECLFUNC(0x0049D780, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_player_acknowledge const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_synchronous_update(c_network_channel* channel, s_network_message_synchronous_update const* message)
{
	DECLFUNC(0x0049DDA0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_synchronous_update const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_synchronous_playback_control(c_network_channel* channel, dword a2, s_network_message_synchronous_playback_control const* message)
{
	DECLFUNC(0x0049DD10, void, __thiscall, c_network_message_handler*, c_network_channel*, dword, s_network_message_synchronous_playback_control const*)(this, channel, a2, message);
}

void __cdecl c_network_message_handler::handle_synchronous_actions(c_network_channel* channel, s_network_message_synchronous_actions const* message)
{
	DECLFUNC(0x0049DB80, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_synchronous_actions const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_synchronous_acknowledge(c_network_channel* channel, dword a2, s_network_message_synchronous_acknowledge const* message)
{
	DECLFUNC(0x0049DB00, void, __thiscall, c_network_message_handler*, c_network_channel*, dword, s_network_message_synchronous_acknowledge const*)(this, channel, a2, message);
}

void __cdecl c_network_message_handler::handle_synchronous_gamestate(c_network_channel* channel, s_network_message_synchronous_gamestate const* message, long a3, void const* a4)
{
	DECLFUNC(0x0049DC70, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_synchronous_gamestate const*, long, void const*)(this, channel, message, a3, a4);
}

void __cdecl c_network_message_handler::handle_distributed_game_results(c_network_channel* channel, s_network_message_distributed_game_results const* message)
{
	DECLFUNC(0x0049CE40, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_distributed_game_results const*)(this, channel, message);
}
