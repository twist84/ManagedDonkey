#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_matchmaking_quality.hpp"

struct s_recruiting_session_search
{
	byte __data[0x2E8];
};
static_assert(sizeof(s_recruiting_session_search) == 0x2E8);

struct c_recruiting_seeker
{
	void update();

	byte_flags m_flags;
	dword_flags m_squad_search_flags;
	long m_controller_index;
	dword __unknownC;
	long m_current_session_search_index;
	byte __data14[0x14];
	long m_initial_search_start_time;
	byte __data2C[0x4];
	c_static_array<s_recruiting_session_search, 16> m_session_search_data;
	byte __data2EB0[0x8];
};
static_assert(sizeof(c_recruiting_seeker) == 0x2EB8);

struct s_available_session;
struct s_network_recruiting_search_globals
{
	bool initialized;
	dword __unknown4;
	c_recruiting_seeker recruiting_seeker;
	c_matchmaking_quality matchmaking_quality;
	bool search_active;
	bool __unknownC7B9;
	long maximum_session_count;
	s_available_session* available_sessions;
	dword __unknownC7C4;
};
static_assert(sizeof(s_network_recruiting_search_globals) == 0xC7C8);

extern s_network_recruiting_search_globals& g_recruiting_search_globals;

extern bool __cdecl network_recruiting_search_begin(long controller_index, long squad_search_flags, long maximum_session_count, s_available_session* session_storage);
extern void __cdecl network_recruiting_search_dispose();
extern void __cdecl network_recruiting_search_end();
extern bool __cdecl network_recruiting_search_initialize();
extern void __cdecl network_recruiting_search_update();

