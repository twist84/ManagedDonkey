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
#include "networking/network_memory.hpp"
#include "networking/network_time.hpp"
#include "networking/session/network_session_manager.hpp"
#include "networking/session/network_session.hpp"

void __cdecl c_network_message_handler::handle_out_of_band_message(transport_address const* address, e_network_message_type message_type, long message_storage_size, void const* message_storage)
{
	ASSERT(m_initialized);

#define RECEIVED_LOG c_console::write_line("networking:messages:handler: %d/%s received out-of-band from '%s'", \
		message_type, \
		m_message_types->get_message_type_name(message_type), \
		transport_address_get_string(address));

	network_message_converter_t converter = { .message_storage = message_storage };

	bool message_type_handled = false;
	switch (message_type)
	{
	case _network_message_ping:
	{
		RECEIVED_LOG;
		ASSERT(message_storage_size == sizeof(s_network_message_ping));
		handle_ping(address, converter.message_ping);
		return;
	}
	break;
	case _network_message_pong:
	{
		RECEIVED_LOG;
		ASSERT(message_storage_size == sizeof(s_network_message_pong));
		handle_pong(address, converter.message_pong);
		return;
	}
	break;
	case _network_message_broadcast_search:
	{
		RECEIVED_LOG;
		ASSERT(message_storage_size == sizeof(s_network_message_broadcast_search));
		handle_broadcast_search(address, converter.message_broadcast_search);
		return;
	}
	break;
	case _network_message_broadcast_reply:
	{
		RECEIVED_LOG;
		ASSERT(message_storage_size == sizeof(s_network_message_broadcast_reply));
		handle_broadcast_reply(address, converter.message_broadcast_reply);
		return;
	}
	break;
	//case _network_message_connect_request:
	//{
	//	RECEIVED_LOG;
	//	ASSERT(message_storage_size == sizeof(s_network_message_connect_request));
	//	handle_connect_request(address, converter.message_connect_request);
	//	return;
	//}
	//break;
	//case _network_message_connect_refuse:
	//{
	//	RECEIVED_LOG;
	//	c_network_channel* associated_channel = m_link->get_associated_channel(address);
	//	if (associated_channel)
	//	{
	//		ASSERT(message_storage_size == sizeof(s_network_message_connect_refuse));
	//		handle_connect_refuse(associated_channel, converter.message_connect_refuse);
	//	return;
	//	}
	//}
	//break;
	//case _network_message_connect_establish:
	//{
	//	RECEIVED_LOG;
	//	c_network_channel* associated_channel = m_link->get_associated_channel(address);
	//	if (associated_channel)
	//	{
	//		ASSERT(message_storage_size == sizeof(s_network_message_connect_establish));
	//		handle_connect_establish(associated_channel, converter.message_connect_establish);
	//	return;
	//	}
	//}
	//break;
	//case _network_message_connect_closed:
	//{
	//	RECEIVED_LOG;
	//	c_network_channel* associated_channel = m_link->get_associated_channel(address);
	//	if (associated_channel)
	//	{
	//		ASSERT(message_storage_size == sizeof(s_network_message_connect_closed));
	//		handle_connect_closed(associated_channel, converter.message_connect_closed);
	//	return;
	//	}
	//}
	//break;
	//case _network_message_join_request:
	//{
	//	RECEIVED_LOG;
	//	ASSERT(message_storage_size == sizeof(s_network_message_join_request));
	//	handle_join_request(address, converter.message_join_request);
	//	return;
	//}
	//break;
	//case _network_message_peer_connect:
	//{
	//	RECEIVED_LOG;
	//	ASSERT(message_storage_size == sizeof(s_network_message_peer_connect));
	//	handle_peer_connect(address, converter.message_peer_connect);
	//	return;
	//}
	//break;
	case _network_message_join_abort:
	{
		RECEIVED_LOG;
		ASSERT(message_storage_size == sizeof(s_network_message_join_abort));
		handle_join_abort(address, converter.message_join_abort);
		return;
	}
	break;
	//case _network_message_join_refuse:
	//{
	//	RECEIVED_LOG;
	//	ASSERT(message_storage_size == sizeof(s_network_message_join_refuse));
	//	handle_join_refuse(address, converter.message_join_refuse);
	//	return;
	//}
	//break;
	//case _network_message_leave_session:
	//{
	//	RECEIVED_LOG;
	//	ASSERT(message_storage_size == sizeof(s_network_message_leave_session));
	//	handle_leave_session(address, converter.message_leave_session);
	//	return;
	//}
	//break;
	//case _network_message_leave_acknowledge:
	//{
	//	RECEIVED_LOG;
	//	ASSERT(message_storage_size == sizeof(s_network_message_leave_acknowledge));
	//	handle_leave_acknowledge(address, converter.message_leave_acknowledge);
	//	return;
	//}
	//break;
	//case _network_message_session_disband:
	//{
	//	RECEIVED_LOG;
	//	ASSERT(message_storage_size == sizeof(s_network_message_session_disband));
	//	handle_session_disband(address, converter.message_session_disband);
	//	return;
	//}
	//break;
	//case _network_message_session_boot:
	//{
	//	RECEIVED_LOG;
	//	ASSERT(message_storage_size == sizeof(s_network_message_session_boot));
	//	handle_session_boot(address, converter.message_session_boot);
	//	return;
	//}
	//break;
	//case _network_message_time_synchronize:
	//{
	//	RECEIVED_LOG;
	//	ASSERT(message_storage_size == sizeof(s_network_message_time_synchronize));
	//	handle_time_synchronize(address, converter.message_time_synchronize);
	//	return;
	//}
	//break;
	//default:
	//{
	//	c_console::write_line("networking:messages:handler: %d/%s from '%s' cannot be handled out-of-band, discarding",
	//		message_type,
	//		m_message_types->get_message_type_name(message_type),
	//		transport_address_get_string(address));
	//}
	//break;
	}

	DECLFUNC(0x0049D2C0, void, __thiscall, c_network_message_handler*, transport_address const*, e_network_message_type, long, void const*)(this, address, message_type, message_storage_size, message_storage);
}

void __cdecl c_network_message_handler::handle_ping(transport_address const* address, s_network_message_ping const* message)
{
	//DECLFUNC(0x0049D720, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_ping const*)(this, address, message);

	c_console::write_line("networking:test:ping: ping #%d received from '%s' at local %dms", message->id, transport_address_get_string(address), network_time_get_exact());

	s_network_message_pong pong =
	{
		.id = message->id,
		.timestamp = message->timestamp,
		.qos_response = 2
	};
	m_message_gateway->send_message_directed(address, _network_message_pong, sizeof(s_network_message_pong), &pong);
}

void __cdecl c_network_message_handler::handle_pong(transport_address const* address, s_network_message_pong const* message)
{
	//DECLFUNC(0x0049D9B0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_pong const*)(this, address, message);

	c_console::write_line("networking:test:ping: ping #%d returned from '%s' at local %dms (latency %dms)", message->id, transport_address_get_string(address), network_time_get_exact(), network_time_since(message->timestamp));
}

void __cdecl c_network_message_handler::handle_broadcast_search(transport_address const* address, s_network_message_broadcast_search const* message)
{
	DECLFUNC(0x0049C310, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_broadcast_search const*)(this, address, message);
}

void __cdecl c_network_message_handler::handle_broadcast_reply(transport_address const* address, s_network_message_broadcast_reply const* message)
{
	DECLFUNC(0x0049C2B0, void, __cdecl, transport_address const*, s_network_message_broadcast_reply const*)(address, message);
}

void __cdecl c_network_message_handler::handle_connect_request(transport_address const* address, s_network_message_connect_request const* message)
{
	DECLFUNC(0x0049CDC0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_connect_request const*)(this, address, message);
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

void __cdecl c_network_message_handler::handle_join_request(transport_address const* address, s_network_message_join_request const* message)
{
	DECLFUNC(0x0049D0C0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_join_request const*)(this, address, message);
}

void __cdecl c_network_message_handler::handle_peer_connect(transport_address const* address, s_network_message_peer_connect const* message)
{
	DECLFUNC(0x0049D5C0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_peer_connect const*)(this, address, message);
}

void __cdecl c_network_message_handler::handle_join_abort(transport_address const* address, s_network_message_join_abort const* message)
{
	//DECLFUNC(0x0049CF00, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_join_abort const*)(this, address, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->established() && session->is_host())
	{
		e_network_join_refuse_reason reason = _network_join_refuse_reason_none;
		if (session->join_abort(address, message->join_nonce))
		{
			reason = _network_join_refuse_reason_aborted;
		}
		else
		{
			c_console::write_line("networking:messages:join-abort: ignoring unknown abort from '%s'", transport_address_get_string(address));
			reason = _network_join_refuse_reason_abort_ignored;
		}

		c_console::write_line("networking:messages:join-abort: received abort, sending join-refusal (%s) to '%s'", network_message_join_refuse_get_reason_string(reason), transport_address_get_string(address));

		s_network_message_join_refuse join_refuse =
		{
			.session_id = message->session_id,
			.reason = reason
		};
		m_message_gateway->send_message_directed(address, _network_message_join_refuse, sizeof(s_network_message_join_refuse), &join_refuse);
	}
	else
	{
		c_console::write_line("networking:messages:join-abort: no session, ignoring join abort from '%s'", transport_address_get_string(address));
	}
}

void __cdecl c_network_message_handler::handle_join_refuse(transport_address const* address, s_network_message_join_refuse const* message)
{
	DECLFUNC(0x0049CFF0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_join_refuse const*)(this, address, message);
}

void __cdecl c_network_message_handler::handle_leave_session(transport_address const* address, s_network_message_leave_session const* message)
{
	DECLFUNC(0x0049D1C0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_leave_session const*)(this, address, message);
}

void __cdecl c_network_message_handler::handle_leave_acknowledge(transport_address const* address, s_network_message_leave_acknowledge const* message)
{
	DECLFUNC(0x0049D150, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_leave_acknowledge const*)(this, address, message);
}

void __cdecl c_network_message_handler::handle_session_disband(transport_address const* address, s_network_message_session_disband const* message)
{
	DECLFUNC(0x0049DA80, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_session_disband const*)(this, address, message);
}

void __cdecl c_network_message_handler::handle_session_boot(transport_address const* address, s_network_message_session_boot const* message)
{
	DECLFUNC(0x0049DA00, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_session_boot const*)(this, address, message);
}

void __cdecl c_network_message_handler::handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message)
{
	DECLFUNC(0x0049CEA0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_host_decline const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message)
{
	DECLFUNC(0x0049D620, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_peer_establish const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_time_synchronize(transport_address const* address, s_network_message_time_synchronize const* message)
{
	DECLFUNC(0x0049DE40, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_time_synchronize const*)(this, address, message);
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
