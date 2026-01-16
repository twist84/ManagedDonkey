#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_search.hpp"

class c_network_link;
class c_network_message_gateway;
struct s_available_session;

struct s_network_broadcast_search_globals
{
	bool initialized;
	c_network_link* link;
	c_network_message_gateway* message_gateway;
	bool search_active;
	bool sessions_updated;
	uns32 last_broadcast_message_sent;
	uns64 search_nonce;
	int32 maximum_session_count;
	s_available_session* available_sessions;
};
COMPILE_ASSERT(sizeof(s_network_broadcast_search_globals) == 0x28);

extern s_network_broadcast_search_globals& g_broadcast_search_globals;
int32 const k_network_broadcast_search_interval = 2000;

struct transport_address;
struct s_network_message_broadcast_reply;

extern void(__cdecl* network_broadcast_search_update_callback)(transport_address* outgoing_address);

extern bool __cdecl network_broadcast_search_active(uns64* search_nonce);
extern bool __cdecl network_broadcast_search_begin(int32 controller_index, int32 maximum_session_count, s_available_session* session_storage);
extern void __cdecl network_broadcast_search_dispose();
extern void __cdecl network_broadcast_search_end();
extern void __cdecl network_broadcast_search_handle_reply(const transport_address* address, const s_network_message_broadcast_reply* message);
extern bool __cdecl network_broadcast_search_initialize(c_network_link* link, c_network_message_gateway* message_gateway);
extern void __cdecl network_broadcast_search_update();

