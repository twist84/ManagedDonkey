#include "networking/logic/network_broadcast_search.hpp"

#include "cseries/cseries_console.hpp"
#include "memory/module.hpp"
#include "networking/delivery/network_link.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/messages/network_messages_session_protocol.hpp"
#include "networking/network_memory.hpp"
#include "networking/network_time.hpp"
#include "xbox/xnet.hpp"

REFERENCE_DECLARE(0x0228E6B8, s_broadcast_search_globals, g_broadcast_search_globals);

HOOK_DECLARE(0x004D9C40, network_broadcast_search_active);
HOOK_DECLARE(0x004D9C70, network_broadcast_search_begin);
HOOK_DECLARE(0x004D9CF0, network_broadcast_search_dispose);
HOOK_DECLARE(0x004D9D20, network_broadcast_search_end);
HOOK_DECLARE(0x004D9D30, network_broadcast_search_handle_reply);
HOOK_DECLARE(0x004D9EA0, network_broadcast_search_initialize);
HOOK_DECLARE(0x004D9EC0, network_broadcast_search_update);

void(__cdecl* network_broadcast_search_update_callback)(transport_address* outgoing_address) = nullptr;

bool __cdecl network_broadcast_search_active(qword* search_nonce)
{
	//return INVOKE(0x004D9C40, network_broadcast_search_active, search_nonce);

	ASSERT(g_broadcast_search_globals.initialized);

	if (g_broadcast_search_globals.search_active && search_nonce)
		*search_nonce = g_broadcast_search_globals.search_nonce;

	return g_broadcast_search_globals.search_active;
}

bool __cdecl network_broadcast_search_begin(long controller_index, long maximum_session_count, s_available_session* session_storage)
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
			g_broadcast_search_globals.__unknownD = false;
			g_broadcast_search_globals.search_time = 0;
			g_broadcast_search_globals.search_nonce = transport_secure_nonce_generate();
			g_broadcast_search_globals.maximum_session_count = maximum_session_count;
			g_broadcast_search_globals.available_sessions = session_storage;

			memset(session_storage, 0, sizeof(s_available_session) * maximum_session_count);
		}
		else
		{
			c_console::write_line("networking:logic:broadcast-search: no physical link available");
		}
	}

	return g_broadcast_search_globals.search_active;
}

void __cdecl network_broadcast_search_dispose()
{
	//INVOKE(0x004D9CF0, network_broadcast_search_dispose);

	csmemset(&g_broadcast_search_globals, 0, sizeof(s_broadcast_search_globals));
}

void __cdecl network_broadcast_search_end()
{
	//INVOKE(0x004D9D20, network_broadcast_search_end);

	g_broadcast_search_globals.search_active = 0;
}

void __cdecl network_broadcast_search_handle_reply(transport_address const* address, s_network_message_broadcast_reply const* message)
{
	//INVOKE(0x004D9D30, network_broadcast_search_handle_reply, address, message);

	ASSERT(g_broadcast_search_globals.initialized);

	if (g_broadcast_search_globals.search_active)
	{
		long existing_session_index = -1;
		long v7 = -1;
		long v6 = -1;
		long v5 = -1;

		for (long i = 0; i < g_broadcast_search_globals.maximum_session_count; i++)
		{
			s_available_session* session = g_broadcast_search_globals.available_sessions + i;

			if (session->initialized)
			{
				if (transport_secure_address_compare(&session->status_data.host_address, &message->status_data.host_address))
				{
					ASSERT(existing_session_index == NONE);
					existing_session_index = i;
					break;
				}
			}
			else if (v7 == -1)
			{
				v7 = i;
			}
		}

		if (existing_session_index == -1)
		{
			if (v7 == -1)
			{
				if (v6 == -1)
					v5 = -1;
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

		if (v5 == -1)
		{
			c_console::write_line("networking:logic:broadcast-search: too many games on the network, can't store reply");
		}
		else
		{
			s_available_session* session = g_broadcast_search_globals.available_sessions + v5;

			if (v5 != existing_session_index)
				csmemset(session, 0, sizeof(s_available_session));

			if (csmemcmp(&session->status_data, &message->status_data, sizeof(s_network_session_status_data)))
			{
				csmemcpy(&session->status_data, &message->status_data, sizeof(s_network_session_status_data));

				session->__unknown30 = true;
				g_broadcast_search_globals.__unknownD = true;
			}

			session->initialized = true;
			session->time = network_time_get();
			session->has_time = true;
		}
	}

	bool add_session = false;
	for (long i = 0; i < g_broadcast_search_globals.maximum_session_count; i++)
	{
		s_available_session const session = g_broadcast_search_globals.available_sessions[i];

		if (session.initialized && transport_secure_identifier_compare(&session.status_data.session_id, &message->status_data.session_id))
			add_session = true;
	}

	if (add_session)
		XNetAddEntry(address, &message->status_data.host_address, &message->status_data.session_id);
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
		if (network_time_since(g_broadcast_search_globals.search_time) > k_network_broadcast_search_interval)
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

			for (word broadcast_port = k_broadcast_port; broadcast_port < k_broadcast_port + k_broadcast_port_alt_ammount; broadcast_port++)
			{
				if (broadcast_port == g_broadcast_port)
					continue;

				g_broadcast_search_globals.message_gateway->send_message_broadcast(_network_message_broadcast_search, sizeof(message), &message, broadcast_port);
			}

			g_broadcast_search_globals.search_time = network_time_get();
		}

		for (long i = 0; i < g_broadcast_search_globals.maximum_session_count; i++)
		{
			s_available_session* session = g_broadcast_search_globals.available_sessions + i;
			if (session->initialized)
			{
				if (network_time_since(session->time) > 4000)
				{
					csmemset(session, 0, sizeof(s_available_session));
					g_broadcast_search_globals.__unknownD = true;
				}
			}
		}
	}
}

