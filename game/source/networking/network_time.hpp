#pragma once

#include "cseries/cseries.hpp"

class c_network_session_manager;
struct s_network_time_globals
{
	bool time_locked;
	uns32 time_locked_timestamp;
	c_network_session_manager* session_manager;
};
static_assert(sizeof(s_network_time_globals) == 0xC);

extern s_network_time_globals& network_time_globals;

struct s_transport_secure_identifier;

extern int32 __cdecl network_time_difference_msec(uns32 timestamp1, uns32 timestamp2);
extern uns32 __cdecl network_time_get();
extern uns32 __cdecl network_time_get_exact();
extern int32 __cdecl network_time_since(uns32 timestamp);
extern void __cdecl network_session_time_deregister_session_manager();
extern bool __cdecl network_session_time_exists(const s_transport_secure_identifier* session_id);
extern bool __cdecl network_session_time_get_id_and_time(int32 session_index, s_transport_secure_identifier* session_id, uns32* timestamp);
extern uns32 __cdecl network_session_time_get_time(const s_transport_secure_identifier* session_id);
extern void __cdecl network_session_time_register_session_manager(c_network_session_manager* session_manager);
extern void __cdecl network_time_lock(bool locked);

