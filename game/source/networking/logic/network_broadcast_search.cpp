#include "networking/logic/network_broadcast_search.hpp"

#include "cseries/console.hpp"
#include "networking/delivery/network_link.hpp"
#include "networking/network_memory.hpp"
#include "networking/network_time.hpp"

#include <assert.h>

REFERENCE_DECLARE(0x0228E6B8, s_broadcast_search_globals, g_broadcast_search_globals);

bool __cdecl network_broadcast_search_active(qword* search_nonce)
{
	//return INVOKE(0x004D9C40, network_broadcast_search_active, search_nonce);

	assert(g_broadcast_search_globals.initialized);

	if (g_broadcast_search_globals.search_active && search_nonce)
		*search_nonce = g_broadcast_search_globals.search_nonce;

	return g_broadcast_search_globals.search_active;
}

bool __cdecl network_broadcast_search_begin(long controller_index, long maximum_session_count, s_available_session* session_storage)
{
	assert(g_broadcast_search_globals.initialized);
	assert(maximum_session_count > 0);
	assert(session_storage);

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
	INVOKE(0x004D9D30, network_broadcast_search_handle_reply, address, message);
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
	assert(g_broadcast_search_globals.initialized);

	INVOKE(0x004D9EC0, network_broadcast_search_update);

	//if (g_broadcast_search_globals.search_active)
	//{
	//	if (network_time_since(g_broadcast_search_globals.search_time) > 2000)
	//	{
	//		s_network_message_broadcast_search message;
	//		message.protocol_version = 9;
	//		message.nonce = g_broadcast_search_globals.search_nonce;
	//
	//		g_broadcast_search_globals.message_gateway->send_message_broadcast(_network_message_broadcast_search, sizeof(message), &message, 11774);
	//
	//		g_broadcast_search_globals.search_time = network_time_get();
	//	}
	//
	//	for (long i = 0; i < g_broadcast_search_globals.maximum_session_count; i++)
	//	{
	//		s_available_session* session = g_broadcast_search_globals.available_sessions + i;
	//		if (session->initialized)
	//		{
	//			if (network_time_since(session->time) > 4000)
	//			{
	//				csmemset(session, 0, sizeof(s_available_session));
	//				g_broadcast_search_globals.__unknownD = true;
	//			}
	//		}
	//	}
	//}
}

