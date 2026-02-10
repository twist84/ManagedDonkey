#include "networking/messages/network_message_handler.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/messages/network_messages_connect.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/messages/network_messages_session_membership.hpp"
#include "networking/messages/network_messages_session_parameters.hpp"
#include "networking/messages/network_messages_session_protocol.hpp"
#include "networking/messages/network_messages_simulation.hpp"
#include "networking/messages/network_messages_simulation_distributed.hpp"
#include "networking/messages/network_messages_simulation_synchronous.hpp"
#include "networking/messages/network_messages_test.hpp"
#include "networking/messages/network_messages_text_chat.hpp"
#include "networking/network_memory.hpp"
#include "networking/network_time.hpp"
#include "networking/session/network_session.hpp"
#include "networking/session/network_session_manager.hpp"
#include "simulation/simulation_view.hpp"
#include "xbox/xnet.hpp"

void __fastcall network_message_handler_handle_channel_message(c_network_message_handler* _this, void* unused, c_network_channel* channel, e_network_message_type message_type, int32 message_storage_size, const void* message_storage)
{
	_this->handle_channel_message(channel, message_type, message_storage_size, message_storage);
}
HOOK_DECLARE_CALL(0x004607F4, network_message_handler_handle_channel_message);

void c_network_message_handler::destroy_handler()
{
	//INVOKE_CLASS_MEMBER(0x0049C200, c_network_message_handler, destroy_handler);

	if (m_initialized)
	{
		if (m_message_gateway)
		{
			m_message_gateway->attach_handler(nullptr);
			m_message_gateway = nullptr;
		}
		m_session_manager = nullptr;
		m_message_types = nullptr;
		m_link = nullptr;
		m_initialized = false;
	}
}

void c_network_message_handler::handle_boot_machine(c_network_channel* channel, const s_network_message_boot_machine* message)
{
	//INVOKE_CLASS_MEMBER(0x0049C240, c_network_message_handler, handle_boot_machine, channel, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->is_host())
	{
		if (!session->handle_boot_machine(channel, message))
		{
			event(_event_warning, "networking:messages:boot-machine: session failed to handle boot-machine (%s) from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:boot-machine: channel '%s' ignoring boot-machine (%s) (not host)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
	}
}

void c_network_message_handler::handle_broadcast_reply(const transport_address* address, const s_network_message_broadcast_reply* message)
{
	//DECLFUNC(0x0049C2B0, void, __cdecl, const transport_address*, const s_network_message_broadcast_reply*)(address, message);

	if (message->protocol_version == k_network_protocol_version)
	{
		uns64 search_nonce;
		if (network_broadcast_search_active(&search_nonce))
		{
			if (transport_secure_nonce_compare(message->search_nonce, search_nonce))
				network_broadcast_search_handle_reply(address, message);
		}
	}
	else
	{
		event(_event_error, "networking:messages:broadcast-reply: received message with incorrect protocol version [%d!=%d]",
			message->protocol_version,
			k_network_protocol_version);
	}
}

void c_network_message_handler::handle_broadcast_search(const transport_address* address, const s_network_message_broadcast_search* message)
{
	//INVOKE_CLASS_MEMBER(0x0049C310, c_network_message_handler, handle_broadcast_search, address, message);

	if (message->protocol_version != k_network_protocol_version)
	{
		event(_event_error, "networking:messages:broadcast-search: received message with incorrect protocol version [%d!=%d]",
			message->protocol_version,
			k_network_protocol_version);
		return;
	}

	uns64 search_nonce;
	if (network_broadcast_search_active(&search_nonce) && transport_secure_nonce_compare(message->nonce, search_nonce))
		return;

	c_network_session* in_system_link_advertisable_session = nullptr;
	if (!network_life_cycle_in_system_link_advertisable_session(&in_system_link_advertisable_session))
		return;

	s_network_squad_status_data game_status{};
	if (!network_squad_session_build_status(&game_status))
	{
		event(_event_warning, "networking:messages:broadcast-search: unable to reply, cannot build local game status");
		return;
	}

	s_transport_secure_address secure_local_address{};
	if (!transport_secure_address_get(&secure_local_address))
	{
		event(_event_warning, "networking:messages:broadcast-search: unable to reply, cannot get a secure local address to return");
		return;
	}

	game_status.update_host_player_identifier(&transport_security_globals.local_insecure_address);

	s_network_message_broadcast_reply broadcast_reply =
	{
		.protocol_version = k_network_protocol_version,
		.search_nonce = message->nonce,
		.status_data = game_status
	};

	m_message_gateway->send_message_broadcast(_network_message_broadcast_reply, sizeof(s_network_message_broadcast_reply), &broadcast_reply, address->port);
}

void c_network_message_handler::handle_channel_message(c_network_channel* channel, e_network_message_type message_type, int32 message_storage_size, const void* message_storage)
{
	//INVOKE_CLASS_MEMBER(0x0049C470, c_network_message_handler, handle_channel_message, channel, message_type, message_storage_size, message_storage);

	ASSERT(m_initialized);

	network_message_converter_t converter = { .message_storage = message_storage };

	switch (message_type)
	{
	case _network_message_connect_establish:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_connect_establish));
		handle_connect_establish(channel, converter.message_connect_establish);
	}
	break;
	case _network_message_leave_session:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_leave_session));

		transport_address remote_address{};
		if (channel->connected() && channel->get_remote_address(&remote_address))
		{
			handle_leave_session(&remote_address, converter.message_leave_session);
		}
		else
		{
			event(_event_warning, "networking:messages:handler: %d/%s received over CLOSED channel '%s'",
				_network_message_leave_session,
				m_message_types->get_message_type_name(_network_message_leave_session),
				channel->get_name());
		}
	}
	break;
	case _network_message_session_disband:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_session_disband));

		transport_address remote_address{};
		if (channel->connected() && channel->get_remote_address(&remote_address))
		{
			handle_session_disband(&remote_address, converter.message_session_disband);
		}
		else
		{
			event(_event_warning, "networking:messages:handler: %d/%s received over CLOSED channel '%s'",
				_network_message_session_disband,
				m_message_types->get_message_type_name(_network_message_session_disband),
				channel->get_name());
		}
	}
	break;
	case _network_message_session_boot:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_session_boot));

		transport_address remote_address{};
		if (channel->connected() && channel->get_remote_address(&remote_address))
		{
			handle_session_boot(&remote_address, converter.message_session_boot);
		}
		else
		{
			event(_event_warning, "networking:messages:handler: %d/%s received over CLOSED channel '%s'",
				_network_message_session_boot,
				m_message_types->get_message_type_name(_network_message_session_boot),
				channel->get_name());
		}
	}
	break;
	case _network_message_host_decline:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_host_decline));

		if (channel->connected())
		{
			handle_host_decline(channel, converter.message_host_decline);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_host_decline,
				m_message_types->get_message_type_name(_network_message_host_decline),
				channel->get_name());
		}
	}
	break;
	case _network_message_peer_establish:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_peer_establish));

		if (channel->connected())
		{
			handle_peer_establish(channel, converter.message_peer_establish);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_peer_establish,
				m_message_types->get_message_type_name(_network_message_peer_establish),
				channel->get_name());
		}
	}
	break;
	case _network_message_membership_update:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_membership_update));

		if (channel->connected())
		{
			handle_membership_update(channel, converter.message_membership_update);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_membership_update,
				m_message_types->get_message_type_name(_network_message_membership_update),
				channel->get_name());
		}
	}
	break;
	case _network_message_peer_properties:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_peer_properties));

		if (channel->connected())
		{
			handle_peer_properties(channel, converter.message_peer_properties);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_peer_properties,
				m_message_types->get_message_type_name(_network_message_peer_properties),
				channel->get_name());
		}
	}
	break;
	case _network_message_delegate_leadership:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_delegate_leadership));

		if (channel->connected())
		{
			handle_delegate_leadership(channel, converter.message_delegate_leadership);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_delegate_leadership,
				m_message_types->get_message_type_name(_network_message_delegate_leadership),
				channel->get_name());
		}
	}
	break;
	case _network_message_boot_machine:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_boot_machine));

		if (channel->connected())
		{
			handle_boot_machine(channel, converter.message_boot_machine);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_boot_machine,
				m_message_types->get_message_type_name(_network_message_boot_machine),
				channel->get_name());
		}
	}
	break;
	case _network_message_player_add:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_player_add));

		if (channel->connected())
		{
			handle_player_add(channel, converter.message_player_add);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_player_add,
				m_message_types->get_message_type_name(_network_message_player_add),
				channel->get_name());
		}
	}
	break;
	case _network_message_player_refuse:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_player_refuse));

		if (channel->connected())
		{
			handle_player_refuse(channel, converter.message_player_refuse);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_player_refuse,
				m_message_types->get_message_type_name(_network_message_player_refuse),
				channel->get_name());
		}
	}
	break;
	case _network_message_player_remove:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_player_remove));

		if (channel->connected())
		{
			handle_player_remove(channel, converter.message_player_remove);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_player_remove,
				m_message_types->get_message_type_name(_network_message_player_remove),
				channel->get_name());
		}
	}
	break;
	case _network_message_player_properties:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_player_properties));

		if (channel->connected())
		{
			handle_player_properties(channel, converter.message_player_properties);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_player_properties,
				m_message_types->get_message_type_name(_network_message_player_properties),
				channel->get_name());
		}
	}
	break;
	case _network_message_parameters_update:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_parameters_update));

		if (channel->connected())
		{
			handle_parameters_update(channel, converter.message_parameters_update);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_parameters_update,
				m_message_types->get_message_type_name(_network_message_parameters_update),
				channel->get_name());
		}
	}
	break;
	case _network_message_parameters_request:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_parameters_request));

		if (channel->connected())
		{
			handle_parameters_request(channel, converter.message_parameters_request);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_parameters_request,
				m_message_types->get_message_type_name(_network_message_parameters_request),
				channel->get_name());
		}
	}
	break;
	case _network_message_view_establishment:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_view_establishment));

		if (channel->connected())
		{
			handle_view_establishment(channel, converter.message_view_establishment);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_view_establishment,
				m_message_types->get_message_type_name(_network_message_view_establishment),
				channel->get_name());
		}
	}
	break;
	case _network_message_player_acknowledge:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_player_acknowledge));

		if (channel->connected())
		{
			handle_player_acknowledge(channel, converter.message_player_acknowledge);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_player_acknowledge,
				m_message_types->get_message_type_name(_network_message_player_acknowledge),
				channel->get_name());
		}
	}
	break;
	case _network_message_synchronous_update:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_synchronous_update));

		if (channel->connected())
		{
			handle_synchronous_update(channel, converter.message_synchronous_update);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_synchronous_update,
				m_message_types->get_message_type_name(_network_message_synchronous_update),
				channel->get_name());
		}
	}
	break;
	case _network_message_synchronous_playback_control:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_synchronous_playback_control));

		if (channel->connected())
		{
			handle_synchronous_playback_control(channel, converter.message_synchronous_playback_control);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_synchronous_playback_control,
				m_message_types->get_message_type_name(_network_message_synchronous_playback_control),
				channel->get_name());
		}
	}
	break;
	case _network_message_synchronous_actions:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_synchronous_actions));

		if (channel->connected())
		{
			handle_synchronous_actions(channel, converter.message_synchronous_actions);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_synchronous_actions,
				m_message_types->get_message_type_name(_network_message_synchronous_actions),
				channel->get_name());
		}
	}
	break;
	case _network_message_synchronous_acknowledge:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_synchronous_acknowledge));

		if (channel->connected())
		{
			handle_synchronous_acknowledge(channel, converter.message_synchronous_acknowledge);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_synchronous_acknowledge,
				m_message_types->get_message_type_name(_network_message_synchronous_acknowledge),
				channel->get_name());
		}
	}
	break;
	case _network_message_synchronous_gamestate:
	{
		ASSERT(message_storage_size >= sizeof(s_network_message_synchronous_gamestate));

		if (channel->connected())
		{
			handle_synchronous_gamestate(channel, converter.message_synchronous_gamestate, message_storage_size - sizeof(s_network_message_synchronous_gamestate), (byte*)converter.message_synchronous_gamestate + sizeof(s_network_message_synchronous_gamestate));
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_distributed_game_results,
				m_message_types->get_message_type_name(_network_message_distributed_game_results),
				channel->get_name());
		}
	}
	break;
	case _network_message_distributed_game_results:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_distributed_game_results));

		if (channel->connected())
		{
			handle_distributed_game_results(channel, converter.message_distributed_game_results);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_distributed_game_results,
				m_message_types->get_message_type_name(_network_message_distributed_game_results),
				channel->get_name());
		}
	}
	break;
	case _network_message_synchronous_client_ready:
	{
		handle_synchronous_client_ready(channel);
	}
	break;
	case _custom_network_message_text_chat:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_text_chat));

		if (channel->connected())
		{
			handle_text_chat(channel, converter.message_text_chat);
		}
	}
	break;
	case _network_message_test:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_test));

		if (channel->connected())
		{
			handle_test(channel, converter.message_test);
		}
		else
		{
			event(_event_message, "networking:message:handler: %d/%s received over a non-connected channel '%s', discarding",
				_network_message_distributed_game_results,
				m_message_types->get_message_type_name(_network_message_distributed_game_results),
				channel->get_name());
		}
	}
	break;
	default:
	{
		event(_event_warning, "networking:messages:handler: type %d/%s not allowed over channel '%s', discarding",
			message_type,
			m_message_types->get_message_type_name(message_type),
			channel->get_name());
	}
	break;
	}
}

void c_network_message_handler::handle_connect_closed(c_network_channel* channel, const s_network_message_connect_closed* message)
{
	//DECLFUNC(0x0049CBA0, void, __cdecl, c_network_channel*, const s_network_message_connect_closed*)(channel, message);

	if (channel->closed())
	{
		event(_event_message, "networking:channel:connect: '%s' ignoring remote closure (already closed)",
			channel->get_name());
	}
	else if (channel->get_identifier() == message->remote_identifier)
	{
		event(_event_message, "networking:channel:connect: '%s' remotely closed (reason #%d: '%s')",
			channel->get_name(),
			message->reason,
			channel->get_closure_reason_string(message->reason));

		channel->close(_network_channel_reason_remote_closure);
	}
	else
	{
		event(_event_message, "networking:channel:connect: '%s' ignoring remote closure (closed identifier %d != identifier %d)",
			channel->get_name(),
			message->remote_identifier,
			channel->get_identifier());
	}
}

void c_network_message_handler::handle_connect_establish(c_network_channel* channel, const s_network_message_connect_establish* message)
{
	//DECLFUNC(0x0049CC10, void, __cdecl, c_network_channel*, const s_network_message_connect_establish*)(channel, message);

	if (channel->closed())
	{
		event(_event_message, "networking:channel:connect: ignoring connect establish from '%s'/%d (currently closed)",
			channel->get_name(),
			message->remote_identifier);
	}
	else
	{
		if (channel->get_identifier() == message->identifier)
		{
			if (channel->established() && channel->get_remote_identifier() != message->remote_identifier)
			{
				event(_event_warning, "networking:channel:connect: received establishment from '%s'/%d but we are already established to %d",
					channel->get_name(),
					message->remote_identifier,
					channel->get_remote_identifier());

				int32 channel_identifier = NONE;
				if (!channel->network_message_queue_get()->has_channel_been_used())
					channel_identifier = channel->get_identifier();

				transport_address remote_address{};
				channel->get_remote_address(&remote_address);
				channel->close(_network_channel_reason_connect_reinitiated);
				channel->open(&remote_address, false, channel_identifier);
			}

			if (channel->closed())
			{
				event(_event_message, "networking:channel:connect: received establishment from '%s'/%d for local %d but the channel closed before we could establish",
					channel->get_name(),
					message->remote_identifier,
					channel->get_identifier());
			}
			else
			{
				event(_event_message, "networking:channel:connect: received establishment from '%s'/%d for local %d",
					channel->get_name(),
					message->remote_identifier,
					channel->get_identifier());

				channel->establish(message->remote_identifier);
			}
		}
		else
		{
			event(_event_message, "networking:channel:connect: ignoring connect establish from '%s'/%d (establishment identifier %d != local identifier %d)",
				channel->get_name(),
				message->remote_identifier,
				message->identifier,
				channel->get_identifier());
		}
	}
}

void c_network_message_handler::handle_connect_refuse(c_network_channel* channel, const s_network_message_connect_refuse* message)
{
	//DECLFUNC(0x0049CD10, void, __cdecl, c_network_channel*, const s_network_message_connect_refuse*)(channel, message);

	if (channel->closed() || channel->connected())
	{
		event(_event_message, "networking:channel:connect: '%s' ignoring connect refusal (%s)",
			channel->get_name(),
			channel->closed() ? "already closed" : "currently connected");
	}
	else if (channel->get_identifier() == message->remote_identifier)
	{
		switch (message->reason)
		{
		case _network_connect_refuse_reason_none:
		case _network_connect_refuse_reason_invalid_flags:
		case _network_connect_refuse_reason_old_identifier:
		case _network_connect_refuse_reason_too_many_channels:
		case _network_connect_refuse_reason_session_failed_to_add:
		{
			event(_event_message, "networking:channel:connect: '%s' connection refusal was fatal (%s)",
				channel->get_name(),
				network_message_connect_refuse_get_reason_string(message->reason));

			channel->close(_network_channel_reason_connect_refuse);
		}
		break;
		case _network_connect_refuse_reason_unknown_stranger:
		case _network_connect_refuse_reason_security_failed:
		case _network_connect_refuse_reason_invalid_topology:
		{
			event(_event_message, "networking:channel:connect: '%s' connection temporarily refused (%s)",
				channel->get_name(),
				network_message_connect_refuse_get_reason_string(message->reason));
		}
		break;
		default:
		{
			UNREACHABLE();
		}
		break;
		}
	}
	else
	{
		event(_event_message, "networking:channel:connect: '%s' ignoring connect refusal (refused identifier %d != identifier %d)",
			channel->get_name(),
			message->remote_identifier,
			channel->get_identifier());
	}
}

void c_network_message_handler::handle_connect_request(const transport_address* address, const s_network_message_connect_request* message)
{
	//INVOKE_CLASS_MEMBER(0x0049CDC0, c_network_message_handler, handle_connect_request, address, message);

	m_observer->handle_connect_request(address, message);
}

void c_network_message_handler::handle_delegate_leadership(c_network_channel* channel, const s_network_message_delegate_leadership* message)
{
	//INVOKE_CLASS_MEMBER(0x0049CDD0, c_network_message_handler, handle_delegate_leadership, channel, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->is_host())
	{
		if (!session->handle_delegate_leadership(channel, message))
		{
			event(_event_warning, "networking:messages:delegate-leadership: session failed to handle delegate-leadership (%s) from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:delegate-leadership: channel '%s' ignoring delegate-leadership (%s) (not host)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
	}
}

void c_network_message_handler::handle_distributed_game_results(c_network_channel* channel, const s_network_message_distributed_game_results* message)
{
	//DECLFUNC(0x0049CE40, void, __cdecl, c_network_channel*, const s_network_message_distributed_game_results*)(channel, message);

	c_simulation_view* remote_view = simulation_get_remote_view_by_channel(channel);
	if (remote_view && remote_view->view_type() == _simulation_view_type_distributed_to_remote_authority)
	{
		if (!remote_view->handle_distributed_game_results(message->establishment_identifier, message->update_number, &message->update))
		{
			event(_event_warning, "networking:messages:game-results-update: failed to handle game results update over channel '%s'",
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:game-results-update: received game-results-update over channel '%s' when not viewing remote authority",
			channel->get_name());
	}
}

void c_network_message_handler::handle_host_decline(c_network_channel* channel, const s_network_message_host_decline* message)
{
	//INVOKE_CLASS_MEMBER(0x0049CEA0, c_network_message_handler, handle_host_decline, channel, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session)
	{
		if (!session->handle_host_decline(channel, message))
		{
			event(_event_warning, "networking:messages:host-decline: session failed to handle host-decline (%s)",
				transport_secure_identifier_get_string(&message->session_id));
		}
	}
	else
	{
		event(_event_warning, "networking:messages:host-decline: channel '%s' ignoring host-decline (%s) (can not find session)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
	}
}

void c_network_message_handler::handle_join_abort(const transport_address* address, const s_network_message_join_abort* message)
{
	//INVOKE_CLASS_MEMBER(0x0049CF00, c_network_message_handler, handle_join_abort, address, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->established() && session->is_host())
	{
		e_network_join_refuse_reason reason = _network_join_refuse_reason_none;
		if (session->join_abort(address, message->join_nonce))
		{
			reason = _network_join_refuse_reason_abort_successful;
		}
		else
		{
			event(_event_warning, "networking:messages:join-abort: ignoring unknown abort from '%s'",
				transport_address_get_string(address));
			reason = _network_join_refuse_reason_abort_ignored;
		}

		event(_event_message, "networking:messages:join-abort: received abort, sending join-refusal (%s) to '%s'",
			network_message_join_refuse_get_reason_string(reason),
			transport_address_get_string(address));

		s_network_message_join_refuse join_refuse =
		{
			.session_id = message->session_id,
			.reason = reason
		};
		m_message_gateway->send_message_directed(address, _network_message_join_refuse, sizeof(s_network_message_join_refuse), &join_refuse);
	}
	else
	{
		event(_event_warning, "networking:messages:join-abort: no session, ignoring join abort from '%s'",
			transport_address_get_string(address));
	}
}

void c_network_message_handler::handle_join_refuse(const transport_address* address, const s_network_message_join_refuse* message)
{
	//INVOKE_CLASS_MEMBER(0x0049CFF0, c_network_message_handler, handle_join_refuse, address, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->waiting_for_host_connection(address))
	{
		event(_event_message, "networking:messages:join-refuse: host-connection received join refusal (%s) type %d/%s from '%s'",
			transport_secure_identifier_get_string(&message->session_id),
			message->reason,
			network_message_join_refuse_get_reason_string(message->reason),
			transport_address_get_string(address));

		session->host_connection_refused(address, message->reason);
	}
	else if (session && session->join_abort_in_progress(address))
	{
		event(_event_message, "networking:messages:join-refuse: join-abort received join refusal (%s) type %d/%s from '%s'",
			transport_secure_identifier_get_string(&message->session_id),
			message->reason,
			network_message_join_refuse_get_reason_string(message->reason),
			transport_address_get_string(address));

		session->join_abort_successful(address);
	}
	else
	{
		event(_event_warning, "networking:messages:join-refuse: can't handle join refusal (%s) type %d/%s from '%s'",
			transport_secure_identifier_get_string(&message->session_id),
			message->reason,
			network_message_join_refuse_get_reason_string(message->reason),
			transport_address_get_string(address));
	}
}

void c_network_message_handler::handle_join_request(const transport_address* address, const s_network_message_join_request* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D0C0, c_network_message_handler, handle_join_request, address, message);

	if (message->protocol == k_network_protocol_version)
	{
		c_network_session* session = m_session_manager->get_session(&message->session_id);
		if (session && session->is_host() && !session->handle_join_request(address, message))
		{
			event(_event_warning, "networking:messages:join-request: can't handle join-request for '%s' from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				transport_address_get_string(address));

			event(_event_error, "networking:messages:join-request: failed to handle incomming join request");
		}
	}
	else
	{
		event(_event_error, "networking:messages:join-request: received message with incorrect protocol version [%d!=%d]",
			message->protocol,
			k_network_protocol_version);
	}
}

void c_network_message_handler::handle_leave_acknowledge(const transport_address* address, const s_network_message_leave_acknowledge* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D150, c_network_message_handler, handle_leave_acknowledge, address, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session)
	{
		if (session->leaving_session())
			session->handle_leave_acknowledgement(address);
	}
	else
	{
		event(_event_warning, "networking:messages:leave-acknowledge: ignoring leave-session acknowledement (%s) from '%s' as we are not leaving (state=%s)",
			transport_secure_identifier_get_string(&message->session_id),
			transport_address_get_string(address),
			session ? session->get_state_string() : "no-session");
	}
}

void c_network_message_handler::handle_leave_session(const transport_address* address, const s_network_message_leave_session* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D1C0, c_network_message_handler, handle_leave_session, address, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session)
	{
		if (session->is_host())
		{
			if (!session->handle_leave_request(address))
			{
				event(_event_warning, "networking:messages:leave-session: can't handle leave-session request (%s) from '%s'",
					transport_secure_identifier_get_string(&message->session_id),
					transport_address_get_string(address));
			}
		}
		else
		{
			event(_event_warning, "networking:messages:leave-session: ignoring leave-session from '%s' (not hosting)",
				transport_address_get_string(address));
		}
	}
	else
	{
		event(_event_warning, "networking:messages:leave-session: ignoring leave-session from '%s' (session not found)",
			transport_address_get_string(address));
	}
}

void c_network_message_handler::handle_membership_update(c_network_channel* channel, const s_network_message_membership_update* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D250, c_network_message_handler, handle_membership_update, channel, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->channel_is_authoritative(channel))
	{
		if (!session->handle_membership_update(message))
		{
			event(_event_warning, "networking:messages:membership-update: failed to handle authoritative membership-update (%s update %d) from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				message->update_number,
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:membership-update: channel '%s' ignoring membership-update (channel is not authoritative)",
			channel->get_name());
	}
}

void c_network_message_handler::handle_out_of_band_message(const transport_address* address, e_network_message_type message_type, int32 message_storage_size, const void* message_storage)
{
	//INVOKE_CLASS_MEMBER(0x0049D2C0, c_network_message_handler, handle_out_of_band_message, address, message_type, message_storage_size, message_storage);

	ASSERT(m_initialized);

	event(_event_verbose, "networking:messages:handler: %d/%s received out-of-band from '%s'",
		message_type,
		m_message_types->get_message_type_name(message_type),
		transport_address_get_string(address));

	network_message_converter_t converter = { .message_storage = message_storage };

	switch (message_type)
	{
	case _network_message_ping:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_ping));
		handle_ping(address, converter.message_ping);
	}
	break;
	case _network_message_pong:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_pong));
		handle_pong(address, converter.message_pong);
	}
	break;
	case _network_message_broadcast_search:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_broadcast_search));
		handle_broadcast_search(address, converter.message_broadcast_search);
	}
	break;
	case _custom_network_message_directed_search:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_directed_search));
		handle_directed_search(address, converter.message_directed_search);
	}
	break;
	case _network_message_broadcast_reply:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_broadcast_reply));
		handle_broadcast_reply(address, converter.message_broadcast_reply);
	}
	break;
	case _network_message_connect_request:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_connect_request));
		handle_connect_request(address, converter.message_connect_request);
	}
	break;
	case _network_message_connect_refuse:
	{
		c_network_channel* associated_channel = m_link->get_associated_channel(address);
		if (associated_channel)
		{
			ASSERT(message_storage_size == sizeof(s_network_message_connect_refuse));
			handle_connect_refuse(associated_channel, converter.message_connect_refuse);
		}
	}
	break;
	case _network_message_connect_establish:
	{
		c_network_channel* associated_channel = m_link->get_associated_channel(address);
		if (associated_channel)
		{
			ASSERT(message_storage_size == sizeof(s_network_message_connect_establish));
			handle_connect_establish(associated_channel, converter.message_connect_establish);
		}
	}
	break;
	case _network_message_connect_closed:
	{
		c_network_channel* associated_channel = m_link->get_associated_channel(address);
		if (associated_channel)
		{
			ASSERT(message_storage_size == sizeof(s_network_message_connect_closed));
			handle_connect_closed(associated_channel, converter.message_connect_closed);
		}
	}
	break;
	case _network_message_join_request:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_join_request));
		handle_join_request(address, converter.message_join_request);
	}
	break;
	case _network_message_peer_connect:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_peer_connect));
		handle_peer_connect(address, converter.message_peer_connect);
	}
	break;
	case _network_message_join_abort:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_join_abort));
		handle_join_abort(address, converter.message_join_abort);
	}
	break;
	case _network_message_join_refuse:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_join_refuse));
		handle_join_refuse(address, converter.message_join_refuse);
	}
	break;
	case _network_message_leave_session:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_leave_session));
		handle_leave_session(address, converter.message_leave_session);
	}
	break;
	case _network_message_leave_acknowledge:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_leave_acknowledge));
		handle_leave_acknowledge(address, converter.message_leave_acknowledge);
	}
	break;
	case _network_message_session_disband:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_session_disband));
		handle_session_disband(address, converter.message_session_disband);
	}
	break;
	case _network_message_session_boot:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_session_boot));
		handle_session_boot(address, converter.message_session_boot);
	}
	break;
	case _network_message_time_synchronize:
	{
		ASSERT(message_storage_size == sizeof(s_network_message_time_synchronize));
		handle_time_synchronize(address, converter.message_time_synchronize);
	}
	break;
	default:
	{
		event(_event_warning, "networking:messages:handler: %d/%s from '%s' cannot be handled out-of-band, discarding",
			message_type,
			m_message_types->get_message_type_name(message_type),
			transport_address_get_string(address));
	}
	break;
	}
}

void c_network_message_handler::handle_parameters_request(c_network_channel* channel, const s_network_message_parameters_request* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D4E0, c_network_message_handler, handle_parameters_request, channel, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->is_host())
	{
		if (!session->handle_parameters_request(channel, message))
		{
			event(_event_warning, "networking:messages:parameters-request-new: session failed to handle parameters-request (%s) from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:parameters-request-new: channel '%s' ignoring parameters-request (we are not the host)",
			channel->get_name());
	}
}

void c_network_message_handler::handle_parameters_update(c_network_channel* channel, const s_network_message_parameters_update* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D550, c_network_message_handler, handle_parameters_update, channel, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->channel_is_authoritative(channel))
	{
		if (!session->handle_parameters_update(message))
		{
			event(_event_warning, "networking:messages:parameters-update-new: session failed to handle authoritative parameters-update (%s) from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:parameters-update-new: channel '%s' ignoring parameters-update (channel is not authoritative)",
			channel->get_name());
	}
}

void c_network_message_handler::handle_peer_connect(const transport_address* address, const s_network_message_peer_connect* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D5C0, c_network_message_handler, handle_peer_connect, address, message);

	if (message->protocol == k_network_protocol_version)
	{
		c_network_session* session = m_session_manager->get_session(&message->session_id);
		if (session)
		{
			session->handle_peer_connect(address, message);
		}
		else
		{
			event(_event_warning, "networking:messages:peer-connect: no session, ignoring peer connect from '%s'",
				transport_address_get_string(address));
		}
	}
	else
	{
		event(_event_error, "networking:messages:peer-connect: received message with incorrect protocol version [%d!=%d]",
			message->protocol,
			k_network_protocol_version);
	}
}

void c_network_message_handler::handle_peer_establish(c_network_channel* channel, const s_network_message_peer_establish* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D620, c_network_message_handler, handle_peer_establish, channel, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->handle_peer_establish(channel, message))
	{
		event(_event_warning, "networking:messages:peer-establish: channel '%s' failed to handle peer-establish (%s)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));

		s_network_message_host_decline host_decline =
		{
			.session_id = message->session_id
		};

		channel->send_message(_network_message_host_decline, sizeof(s_network_message_host_decline), &host_decline);
	}
}

void c_network_message_handler::handle_peer_properties(c_network_channel* channel, const s_network_message_peer_properties* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D6B0, c_network_message_handler, handle_peer_properties, channel, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->is_host())
	{
		if (!session->handle_peer_properties(channel, message))
		{
			event(_event_warning, "networking:messages:peer-properties: session failed to handle peer-properties (%s) from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:peer-properties: channel '%s' ignoring peer-properties (%s) (not host)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
	}
}

void c_network_message_handler::handle_ping(const transport_address* address, const s_network_message_ping* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D720, c_network_message_handler, handle_ping, address, message);

	event(_event_message, "networking:test:ping: ping #%d received from '%s' at local %dms",
		message->unique_identifier,
		transport_address_get_string(address),
		network_time_get_exact());

	s_network_message_pong pong = { message->unique_identifier, message->origin_timestamp, _network_qos_response_accept };
	m_message_gateway->send_message_directed(address, _network_message_pong, sizeof(s_network_message_pong), &pong);
}

void c_network_message_handler::handle_player_acknowledge(c_network_channel* channel, const s_network_message_player_acknowledge* message)
{
	//DECLFUNC(0x0049D780, void, __cdecl, c_network_channel*, const s_network_message_player_acknowledge*)(channel, message);

	c_simulation_view* remote_view = simulation_get_remote_view_by_channel(channel);
	if (remote_view && remote_view->is_client_view())
	{
		if (!remote_view->handle_player_acknowledge(message->player_valid_mask, message->player_in_game_mask, message->player_identifiers))
		{
			event(_event_warning, "networking:messages:player-acknowledge: failed to handle (players 0x%04X 0x%04X) over channel '%s'",
				message->player_valid_mask,
				message->player_in_game_mask,
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:player-acknowledge: received (players 0x%04X 0x%04X) over channel '%s' with no simulation client view",
			message->player_valid_mask,
			message->player_in_game_mask,
			channel->get_name());
	}
}

void c_network_message_handler::handle_player_add(c_network_channel* channel, const s_network_message_player_add* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D7F0, c_network_message_handler, handle_player_add, channel, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->is_host())
	{
		if (!session->handle_player_add(channel, message))
		{
			event(_event_warning, "networking:messages:player-add: session failed to handle player-add (%s) from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:player-add: channel '%s' ignoring player-add (%s) (not host)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
	}
}

void c_network_message_handler::handle_player_properties(c_network_channel* channel, const s_network_message_player_properties* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D860, c_network_message_handler, handle_player_properties, channel, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->is_host())
	{
		if (!session->handle_player_properties(channel, message))
		{
			event(_event_warning, "networking:messages:player-properties: session failed to handle player-properties (%s) from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:player-properties: channel '%s' ignoring player-properties (%s) (not host)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
	}
}

void c_network_message_handler::handle_player_refuse(c_network_channel* channel, const s_network_message_player_refuse* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D8D0, c_network_message_handler, handle_player_refuse, channel, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->channel_is_authoritative(channel))
	{
		if (!session->handle_player_refuse(channel, message))
		{
			event(_event_warning, "networking:messages:player-refuse: session failed to handle player-refuse (%s) from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:player-refuse: channel '%s' ignoring player-refuse (%s) (channel not authoritative)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
	}
}

void c_network_message_handler::handle_player_remove(c_network_channel* channel, const s_network_message_player_remove* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D940, c_network_message_handler, handle_player_remove, channel, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session && session->is_host())
	{
		if (!session->handle_player_remove(channel, message))
		{
			event(_event_warning, "networking:messages:player-remove: session failed to handle player-remove (%s) from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:player-remove: channel '%s' ignoring player-remove (%s) (not host)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
	}
}

void c_network_message_handler::handle_pong(const transport_address* address, const s_network_message_pong* message)
{
	//INVOKE_CLASS_MEMBER(0x0049D9B0, c_network_message_handler, handle_pong, address, message);

	event(_event_message, "networking:test:ping: ping #%d returned from '%s' at local %dms (latency %dms)",
		message->unique_identifier,
		transport_address_get_string(address),
		network_time_get_exact(),
		network_time_since(message->origin_timestamp));
}

void c_network_message_handler::handle_session_boot(const transport_address* address, const s_network_message_session_boot* message)
{
	//INVOKE_CLASS_MEMBER(0x0049DA00, c_network_message_handler, handle_session_boot, address, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session)
	{
		if (!session->handle_session_boot(address, message))
		{
			event(_event_warning, "networking:messages:session-boot: session (%s) failed to handle session-boot from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				transport_address_get_string(address));
		}
	}
	else
	{
		event(_event_warning, "networking:messages:session-boot: ignoring session-boot from '%s' (no %s session)",
			transport_address_get_string(address),
			transport_secure_identifier_get_string(&message->session_id));
	}
}

void c_network_message_handler::handle_session_disband(const transport_address* address, const s_network_message_session_disband* message)
{
	//INVOKE_CLASS_MEMBER(0x0049DA80, c_network_message_handler, handle_session_disband, address, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session)
	{
		if (!session->handle_session_disband(address, message))
		{
			event(_event_warning, "networking:messages:session-disband: session (%s) failed to handle session-disband from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				transport_address_get_string(address));
		}
	}
	else
	{
		event(_event_warning, "networking:messages:session-disband: ignoring message from '%s' (no %s session)",
			transport_address_get_string(address),
			transport_secure_identifier_get_string(&message->session_id));
	}
}

void c_network_message_handler::handle_synchronous_acknowledge(c_network_channel* channel, const s_network_message_synchronous_acknowledge* message)
{
	//DECLFUNC(0x0049DB00, void, __cdecl, c_network_channel*, const s_network_message_synchronous_acknowledge*)(channel, message);

	c_simulation_view* remote_view = simulation_get_remote_view_by_channel(channel);
	if (remote_view)
	{
		if (remote_view->view_type() == _simulation_view_type_synchronous_to_remote_client)
		{
			if (!remote_view->handle_synchronous_acknowledge(message->current_update_number))
			{
				event(_event_warning, "networking:messages:synchronous-acknowledge: failed to handle ack of #%d over channel '%s' with view mode %d/%d",
					message->current_update_number,
					channel->get_name(),
					remote_view->get_view_establishment_mode(),
					remote_view->get_view_establishment_identifier());
			}
		}
		else
		{
			event(_event_warning, "networking:messages:synchronous-acknowledge: view not a client for ack of #%d over channel '%s' with view of type #%d",
				message->current_update_number,
				channel->get_name(),
				remote_view->view_type());
		}
	}
	else
	{
		event(_event_message, "networking:messages:synchronous-acknowledge: no view for ack of #%d over channel '%s'",
			message->current_update_number,
			channel->get_name());
	}
}

void c_network_message_handler::handle_synchronous_actions(c_network_channel* channel, const s_network_message_synchronous_actions* message)
{
	//DECLFUNC(0x0049DB80, void, __cdecl, c_network_channel*, const s_network_message_synchronous_actions*)(channel, message);

	c_simulation_view* remote_view = simulation_get_remote_view_by_channel(channel);
	if (remote_view)
	{
		if (remote_view->view_type() == _simulation_view_type_synchronous_to_remote_client)
		{
			if (!remote_view->handle_synchronous_actions(message->action_number, message->current_update_number, message->valid_user_mask, message->user_actions))
			{
				event(_event_warning, "networking:messages:synchronous-actions: failed to handle #%d over channel '%s' with view mode %d/%d",
					message->action_number,
					channel->get_name(),
					remote_view->get_view_establishment_mode(),
					remote_view->get_view_establishment_identifier());
			}
		}
		else
		{
			event(_event_warning, "networking:messages:synchronous-actions: view not a client for #%d over channel '%s' with view of type #%d",
				message->action_number,
				channel->get_name(),
				remote_view->view_type());
		}
	}
	else
	{
		event(_event_message, "networking:messages:synchronous-actions: no view for #%d over channel '%s'",
			message->action_number,
			channel->get_name());
	}
}

void c_network_message_handler::handle_synchronous_client_ready(c_network_channel* channel)
{
	//DECLFUNC(0x0049DC10, void, __cdecl, c_network_channel*)(channel);

	c_simulation_view* remote_view = simulation_get_remote_view_by_channel(channel);
	if (remote_view && remote_view->view_type() == _simulation_view_type_synchronous_to_remote_client)
	{
		if (remote_view->synchronous_catchup_in_progress())
		{
			event(_event_message, "networking:messages:synchronous_client_ready: view %s reporting that he is ready to go active",
				remote_view->get_view_description());
		}
		else
		{
			event(_event_warning, "networking:messages:synchronous_client_ready: view %s received catchup finished notification for client that we don't have performing catchup at this time. This is bad.",
				remote_view->get_view_description());
		}
	}
}

void c_network_message_handler::handle_synchronous_gamestate(c_network_channel* channel, const s_network_message_synchronous_gamestate* message, int32 chunk_size, const void* chunk_data)
{
	//DECLFUNC(0x0049DC70, void, __cdecl, c_network_channel*, const s_network_message_synchronous_gamestate*, int32, const void*)(channel, message, chunk_size, chunk);

	if (chunk_size == message->chunk_size)
	{
		c_simulation_view* remote_view = simulation_get_remote_view_by_channel(channel);
		if (remote_view)
		{
			if (remote_view->view_type() == _simulation_view_type_synchronous_to_remote_authority)
			{
				if (!remote_view->handle_synchronous_gamestate(message, chunk_data))
				{
					event(_event_warning, "networking:messages:synchronous-gamestate: failed to handle (%d@%d) over channel '%s' with view mode %d/%d",
						message->chunk_size,
						message->chunk_offset,
						channel->get_name(),
						remote_view->get_view_establishment_mode(),
						remote_view->get_view_establishment_identifier());
				}
			}
			else
			{
				event(_event_warning, "networking:messages:synchronous-gamestate: view not a client over channel '%s' with view of type #%d",
					channel->get_name(),
					remote_view->view_type());
			}
		}
		else
		{
			event(_event_warning, "networking:messages:synchronous-gamestate: no simulation view on channel '%s'",
				channel->get_name());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:synchronous-gamestate: had corrupt sizes #%d!=#%d over channel '%s'",
			chunk_size,
			message->chunk_size,
			channel->get_name());
	}
}

void c_network_message_handler::handle_synchronous_playback_control(c_network_channel* channel, const s_network_message_synchronous_playback_control* message)
{
	//DECLFUNC(0x0049DD10, void, __cdecl, c_network_channel*, const s_network_message_synchronous_playback_control*)(channel, message);

	c_simulation_view* remote_view = simulation_get_remote_view_by_channel(channel);
	if (remote_view)
	{
		if (remote_view->view_type() == _simulation_view_type_synchronous_to_remote_authority)
		{
			if (!remote_view->handle_synchronous_playback_control(message->type, message->identifier, message->update_number))
			{
				event(_event_warning, "networking:messages:synchronous-playback-control: failed to handle playback-control type #%d identifier #%d update #%d over channel '%s' with view mode %d/%d",
					message->type,
					message->identifier,
					message->update_number,
					channel->get_name(),
					remote_view->get_view_establishment_mode(),
					remote_view->get_view_establishment_identifier());
			}
		}
		else
		{
			event(_event_warning, "networking:messages:synchronous-playback-control: view not authority for playback-control type #%d identifier #%d update #%d over channel '%s' with view of type #%d",
				message->type,
				message->identifier,
				message->update_number,
				channel->get_name(),
				remote_view->view_type());
		}
	}
	else
	{
		event(_event_warning, "networking:messages:synchronous-playback-control: no simulation view for playback-control type #%d identifier #%d update #%d over channel '%s'",
			message->type,
			message->identifier,
			message->update_number,
			channel->get_name());
	}
}

void c_network_message_handler::handle_synchronous_update(c_network_channel* channel, const s_network_message_synchronous_update* message)
{
	//INVOKE_CLASS_MEMBER(0x0049DDA0, c_network_message_handler, handle_synchronous_update, channel, message);

	c_simulation_view* remote_view = simulation_get_remote_view_by_channel(channel);
	if (remote_view)
	{
		if (remote_view->view_type() == _simulation_view_type_synchronous_to_remote_authority)
		{
			if (remote_view->handle_synchronous_update(&message->update))
			{
				event(_event_warning, "networking:messages:synchronous-update: failed to handle #%d over channel '%s' with view mode %d/%d",
					message->update.update_number,
					channel->get_name(),
					remote_view->get_view_establishment_mode(),
					remote_view->get_view_establishment_identifier());
			}
		}
		else
		{
			event(_event_warning, "networking:messages:synchronous-update: view not authority for #%d over channel '%s' with view of type #%d",
				message->update.update_number,
				channel->get_name(),
				remote_view->view_type());
		}
	}
	else
	{
		event(_event_message, "networking:messages:synchronous-update: no simulation view for #%d over channel '%s'",
			message->update.update_number,
			channel->get_name());
	}
}

void c_network_message_handler::handle_time_synchronize(const transport_address* address, const s_network_message_time_synchronize* message)
{
	//INVOKE_CLASS_MEMBER(0x0049DE40, c_network_message_handler, handle_time_synchronize, address, message);

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session)
	{
		if (!session->handle_time_synchronize(address, message))
		{
			event(_event_warning, "networking:messages:time-synchronize: session failed to handle time-synchronize (%s %d) from '%s'",
				transport_secure_identifier_get_string(&message->session_id),
				message->synchronization_stage,
				transport_address_get_string(address));
		}
	}
	else
	{
		event(_event_warning, "networking:messages:time-synchronize: session doesn't exist to handle time-synchronize (%s %d) from '%s'",
			transport_secure_identifier_get_string(&message->session_id),
			message->synchronization_stage,
			transport_address_get_string(address));
	}
}

void c_network_message_handler::handle_view_establishment(c_network_channel* channel, const s_network_message_view_establishment* message)
{
	//DECLFUNC(0x0049DEC0, void, __cdecl, c_network_channel*, const s_network_message_view_establishment*)(channel, message);

	c_simulation_view* remote_view = simulation_get_remote_view_by_channel(channel);
	if (remote_view)
	{
		if (!remote_view->handle_remote_establishment(
			message->establishment_mode,
			message->establishment_identifier,
			message->signature_exists ? message->signature_size : NONE,
			message->signature_exists ? message->signature_data : nullptr))
		{
			event(_event_warning, "networking:messages:view-establishment: simulation view failed to handle view-establishment (%d/%d) over channel '%s'",
				message->establishment_mode,
				message->establishment_identifier,
				channel->get_name());
		}
	}
	else if (message->establishment_identifier == NONE)
	{
		event(_event_message, "networking:messages:view-establishment: discarding non-established view-establishment (%d/%d) over channel '%s' with no simulation view",
			message->establishment_mode,
			NONE,
			channel->get_name());
	}
	else
	{
		event(_event_warning, "networking:messages:view-establishment: discarding ESTABLISHED view-establishment (%d/%d) over channel '%s' with no simulation view",
			message->establishment_mode,
			message->establishment_identifier,
			channel->get_name());
	}
}

bool c_network_message_handler::initialize_handler(c_network_link* link, const c_network_message_type_collection* message_types, c_network_message_gateway* message_gateway)
{
	//INVOKE_CLASS_MEMBER(0x0049DF60, c_network_message_handler, initialize_handler, link, message_types, message_gateway);

	ASSERT(!m_initialized);
	ASSERT(link);
	ASSERT(message_types);
	ASSERT(message_gateway);

	m_link = link;
	m_message_types = message_types;
	m_message_gateway = message_gateway;
	message_gateway->attach_handler(this);
	m_observer = nullptr;
	m_session_manager = nullptr;
	m_initialized = true;

	return m_initialized;
}

void c_network_message_handler::register_observer(c_network_observer* observer)
{
	//INVOKE_CLASS_MEMBER(0x0049E020, c_network_message_handler, register_observer, observer);

	ASSERT(m_initialized);
	ASSERT(observer);
	ASSERT(m_observer == nullptr);

	m_observer = observer;
}

void c_network_message_handler::register_session_manager(c_network_session_manager* session_manager)
{
	//INVOKE_CLASS_MEMBER(0x0049E030, c_network_message_handler, register_session_manager, session_manager);

	ASSERT(m_initialized);
	ASSERT(session_manager);
	ASSERT(m_session_manager == nullptr);

	m_session_manager = session_manager;
}

void c_network_message_handler::handle_test(c_network_channel* channel, const s_network_message_test* message)
{
	//network_test_messages_handle_message(channel, message->in_sequence, message->id);
}

void c_network_message_handler::handle_directed_search(const transport_address* address, const s_network_message_directed_search* message)
{
	s_network_squad_status_data game_status{};
	if (!network_squad_session_build_status(&game_status))
	{
		event(_event_warning, "networking:messages:directed-search: unable to reply, cannot build local game status");
		return;
	}

	transport_address _address{};
	transport_address_ipv4_build(&_address, lookup_external_ip(), g_broadcast_port);
	game_status.update_host_player_identifier(&_address);

	s_network_message_broadcast_reply broadcast_reply =
	{
		.protocol_version = k_network_protocol_version,
		.search_nonce = message->nonce,
		.status_data = game_status
	};

	m_message_gateway->send_message_directed(address, _network_message_broadcast_reply, sizeof(s_network_message_broadcast_reply), &broadcast_reply);
}

void c_network_message_handler::handle_text_chat(c_network_channel* channel, const s_network_message_text_chat* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (session)
	{
		event(_event_warning, "networking:messages:text chat: received text chat message for valid session");
		//session->handle_text_chat(channel, message);
	}
	else
	{
		event(_event_warning, "networking:messages:text chat: received text chat message for invalid session");
	}
}

