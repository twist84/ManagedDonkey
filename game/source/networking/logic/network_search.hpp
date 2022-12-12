#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_session_interface.hpp"

struct s_available_session
{
	bool initialized;
	byte : 8;
	byte : 8;
	byte : 8;
	dword time;
	bool has_time;
	byte : 8;
	byte : 8;
	byte : 8;
	byte __dataC[0x24];
	bool __unknown30;
	byte __data31[0x7];
	s_network_session_status_data status_data;
};
static_assert(sizeof(s_available_session) == 0x164F8);

struct c_allocation_base;
struct s_network_search_globals
{
	long search_category;
	long reference_count;
	long maximum_sessions;
	dword_flags squad_search_flags;
	c_allocation_base* available_sessions_allocator;
	long available_session_count;
	s_available_session* available_sessions;
};
static_assert(sizeof(s_network_search_globals) == 0x1C);

extern s_network_search_globals& g_network_search_globals;

extern void __cdecl network_search_active(long controller_index, bool active);
extern bool __cdecl network_search_begin(long controller_index);
extern void __cdecl network_search_dispose();
extern void __cdecl network_search_end();
extern bool __cdecl network_search_initialize();
extern s_available_session* __cdecl network_search_session(long available_squad_index);
extern long __cdecl network_search_session_count();
extern bool __cdecl network_search_session_valid(s_available_session* session);
extern bool __cdecl network_search_start(long controller_index, long category, long maximum_sessions, dword_flags available_squad_search_flags, c_allocation_base* session_allocator);
extern void __cdecl network_search_stop();

