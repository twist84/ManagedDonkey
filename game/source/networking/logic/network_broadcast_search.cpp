#include "networking/logic/network_broadcast_search.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/delivery/network_link.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/messages/network_messages_session_protocol.hpp"
#include "networking/network_memory.hpp"
#include "networking/network_time.hpp"
#include "xbox/xnet.hpp"

REFERENCE_DECLARE(0x0228E6B8, s_network_broadcast_search_globals, g_broadcast_search_globals);

HOOK_DECLARE(0x004D9C40, network_broadcast_search_active);
HOOK_DECLARE(0x004D9C70, network_broadcast_search_begin);
HOOK_DECLARE(0x004D9CF0, network_broadcast_search_dispose);
HOOK_DECLARE(0x004D9D20, network_broadcast_search_end);
HOOK_DECLARE(0x004D9D30, network_broadcast_search_handle_reply);
HOOK_DECLARE(0x004D9EA0, network_broadcast_search_initialize);
HOOK_DECLARE(0x004D9EC0, network_broadcast_search_update);

void(__cdecl* network_broadcast_search_update_callback)(transport_address* outgoing_address) = nullptr;

bool __cdecl network_broadcast_search_active(uns64* search_nonce)
{
	//return INVOKE(0x004D9C40, network_broadcast_search_active, search_nonce);

	ASSERT(g_broadcast_search_globals.initialized);

	if (g_broadcast_search_globals.search_active && search_nonce)
		*search_nonce = g_broadcast_search_globals.search_nonce;

	return g_broadcast_search_globals.search_active;
}

bool __cdecl network_broadcast_search_begin(int32 controller_index, int32 maximum_session_count, s_available_session* session_storage)
{
	//return INVOKE(0x004D9C70, network_broadcast_search_begin, controller_index, maximum_session_count, session_storage);

	ASSERT(g_broadcast_search_globals.initialized);
	ASSERT(maximum_session_count > 0);
	ASSERT(session_storage);

	if (!g_broadcast_search_globals.search_active)
	{
		if (c_network_link::physical_link_available())
		{
			g_broadcast_search_globals.search_active = true;
			g_broadcast_search_globals.sessions_updated = false;
			g_broadcast_search_globals.last_broadcast_message_sent = 0;
			g_broadcast_search_globals.search_nonce = transport_secure_nonce_generate();
			g_broadcast_search_globals.maximum_session_count = maximum_session_count;
			g_broadcast_search_globals.available_sessions = session_storage;

			csmemset(session_storage, 0, sizeof(s_available_session) * maximum_session_count);
		}
		else
		{
			event(_event_error, "networking:logic:broadcast-search: no physical link available");
		}
	}

	return g_broadcast_search_globals.search_active;
}

void __cdecl network_broadcast_search_dispose()
{
	//INVOKE(0x004D9CF0, network_broadcast_search_dispose);

	csmemset(&g_broadcast_search_globals, 0, sizeof(s_network_broadcast_search_globals));
}

void __cdecl network_broadcast_search_end()
{
	//INVOKE(0x004D9D20, network_broadcast_search_end);

	g_broadcast_search_globals.search_active = 0;
}

void __cdecl network_broadcast_search_handle_reply(const transport_address* address, const s_network_message_broadcast_reply* message)
{
	//INVOKE(0x004D9D30, network_broadcast_search_handle_reply, address, message);

	ASSERT(g_broadcast_search_globals.initialized);

	if (g_broadcast_search_globals.search_active)
	{
		int32 existing_session_index = NONE;
		int32 v7 = NONE;
		int32 v6 = NONE;
		int32 v5 = NONE;

		for (int32 i = 0; i < g_broadcast_search_globals.maximum_session_count; i++)
		{
			s_available_session* session = &g_broadcast_search_globals.available_sessions[i];

			if (session->session_valid)
			{
				if (transport_secure_address_compare(&session->status_data.game_details.description.host_address, &message->status_data.game_details.description.host_address))
				{
					ASSERT(existing_session_index == NONE);
					existing_session_index = i;
					break;
				}
			}
			else if (v7 == NONE)
			{
				v7 = i;
			}
		}

		if (existing_session_index == NONE)
		{
			if (v7 == NONE)
			{
				if (v6 == NONE)
					v5 = NONE;
				else
					v5 = v6;
			}
			else
			{
				v5 = v7;
			}
		}
		else
		{
			v5 = existing_session_index;
		}

		if (v5 == NONE)
		{
			event(_event_error, "networking:logic:broadcast-search: too many games on the network, can't store reply");
		}
		else
		{
			s_available_session* session = &g_broadcast_search_globals.available_sessions[v5];

			if (v5 != existing_session_index)
				csmemset(session, 0, sizeof(s_available_session));

			if (csmemcmp(&session->status_data, &message->status_data, sizeof(s_network_squad_status_data)))
			{
				csmemcpy(&session->status_data, &message->status_data, sizeof(s_network_squad_status_data));

				session->status_data_valid = true;
				g_broadcast_search_globals.sessions_updated = true;
			}

			session->session_valid = true;
			session->last_update_timestamp = network_time_get();
			session->connect_established = true;
		}
	}

	bool add_session = false;
	for (int32 i = 0; i < g_broadcast_search_globals.maximum_session_count; i++)
	{
		s_available_session const session = g_broadcast_search_globals.available_sessions[i];

		if (session.session_valid && transport_secure_identifier_compare(&session.status_data.game_details.description.id, &message->status_data.game_details.description.id))
			add_session = true;
	}

	if (add_session)
		XNetAddEntry(address, &message->status_data.game_details.description.host_address, &message->status_data.game_details.description.id);
}

bool __cdecl network_broadcast_search_initialize(c_network_link* link, c_network_message_gateway* message_gateway)
{
	//INVOKE(0x004D9EA0, network_broadcast_search_initialize, link, message_gateway);

	g_broadcast_search_globals.link = link;
	g_broadcast_search_globals.message_gateway = message_gateway;
	g_broadcast_search_globals.initialized = true;

	return true;
}

void __cdecl network_broadcast_search_update()
{
	//INVOKE(0x004D9EC0, network_broadcast_search_update);

	ASSERT(g_broadcast_search_globals.initialized);

	if (g_broadcast_search_globals.search_active)
	{
		if (network_time_since(g_broadcast_search_globals.last_broadcast_message_sent) > k_network_broadcast_search_interval)
		{
			s_network_message_directed_search message;
			message.protocol_version = k_network_protocol_version;
			message.nonce = g_broadcast_search_globals.search_nonce;

			if (network_broadcast_search_update_callback)
			{
				transport_address outgoing_address{};
				network_broadcast_search_update_callback(&outgoing_address);
				if (transport_address_valid(&outgoing_address))
					g_broadcast_search_globals.message_gateway->send_message_directed(&outgoing_address, _custom_network_message_directed_search, sizeof(message), &message);
			}

			for (uns16 broadcast_port = k_broadcast_port; broadcast_port < k_broadcast_port + k_broadcast_port_alt_ammount; broadcast_port++)
			{
				if (broadcast_port == g_broadcast_port)
					continue;

				g_broadcast_search_globals.message_gateway->send_message_broadcast(_network_message_broadcast_search, sizeof(message), &message, broadcast_port);
			}

			g_broadcast_search_globals.last_broadcast_message_sent = network_time_get();
		}

		for (int32 i = 0; i < g_broadcast_search_globals.maximum_session_count; i++)
		{
			s_available_session* session = &g_broadcast_search_globals.available_sessions[i];
			if (session->session_valid)
			{
				if (network_time_since(session->last_update_timestamp) > 4000)
				{
					csmemset(session, 0, sizeof(s_available_session));
					g_broadcast_search_globals.sessions_updated = true;
				}
			}
		}
	}
}

