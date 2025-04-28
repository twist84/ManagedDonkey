#pragma once

#include "cseries/cseries.hpp"

struct c_network_session_manager;
struct s_network_time_globals
{
	bool time_locked;
	uint32 time_locked_timestamp;
	c_network_session_manager* session_manager;
};
static_assert(sizeof(s_network_time_globals) == 0xC);

extern s_network_time_globals& network_time_globals;

struct s_transport_secure_identifier;

extern int32 __cdecl network_time_difference_msec(uint32 timestamp1, uint32 timestamp2);
extern uint32 __cdecl network_time_get();
extern uint32 __cdecl network_time_get_exact();
extern int32 network_time_since(uint32 timestamp);
extern void __cdecl network_session_time_deregister_session_manager();
extern bool __cdecl network_session_time_exists(s_transport_secure_identifier const* session_id);
extern bool __cdecl network_session_time_get_id_and_time(int32 session_index, s_transport_secure_identifier* session_id, uint32* timestamp);
extern uint32 __cdecl network_session_time_get_time(s_transport_secure_identifier const* session_id);
extern void __cdecl network_session_time_register_session_manager(c_network_session_manager* session_manager);
extern void __cdecl network_time_lock(bool locked);

