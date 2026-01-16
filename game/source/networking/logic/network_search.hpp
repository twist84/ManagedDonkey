#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"

struct s_available_session
{
	bool session_valid;
	uns32 last_update_timestamp;
	bool connect_established;
	bool qos_successful;
	int32 qos_target_status;
	s_transport_qos_result qos_result;
	bool status_data_valid;
	byte __data31[0x7];
	s_network_squad_status_data status_data;
};
COMPILE_ASSERT(sizeof(s_available_session) == 0x164F8);

struct s_network_search_globals
{
	int32 search_category;
	int32 reference_count;
	int32 maximum_sessions;
	uns32 squad_search_flags;
	c_allocation_base* available_sessions_allocator;
	int32 available_session_count;
	s_available_session* available_sessions;
};
COMPILE_ASSERT(sizeof(s_network_search_globals) == 0x1C);

extern s_network_search_globals& g_network_search_globals;

extern void __cdecl network_search_active(int32 controller_index, bool active);
extern bool __cdecl network_search_begin(int32 controller_index);
extern void __cdecl network_search_dispose();
extern void __cdecl network_search_end();
extern bool __cdecl network_search_initialize();
extern s_available_session* __cdecl network_search_session(int32 available_squad_index);
extern int32 __cdecl network_search_session_count();
extern bool __cdecl network_search_session_valid(s_available_session* session);
extern bool __cdecl network_search_start(int32 controller_index, int32 category, int32 maximum_sessions, uns32 available_squad_search_flags, c_allocation_base* session_allocator);
extern void __cdecl network_search_stop();

