#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_matchmaking_quality.hpp"

struct s_recruiting_session_search
{
	uint8 __data[0x2E8];
};
static_assert(sizeof(s_recruiting_session_search) == 0x2E8);

struct c_recruiting_seeker
{
	void update();

	uint8 m_flags;
	long m_squad_search_flags;
	long m_controller_index;
	long m_session_search_count;
	long m_current_session_search_index;
	long m_total_session_search_failures;
	long m_total_session_searches;
	long m_total_sessions_found;
	long m_current_sessions_contacted_count;
	long m_total_session_contacted_count;
	uint32 m_search_start_time;
	uint32 m_current_search_completed_time;
	c_static_array<s_recruiting_session_search, 16> m_session_search_data;
	long m_game_mode;
	long m_game_type;
};
static_assert(sizeof(c_recruiting_seeker) == 0x2EB8);

struct s_available_session;
struct s_network_recruiting_search_globals
{
	bool initialized;
	uint32 __unknown4;
	c_recruiting_seeker recruiting_seeker;
	c_matchmaking_quality matchmaking_quality;
	bool search_active;
	bool sessions_updated;
	long maximum_session_count;
	s_available_session* session_storage;
	uint32 __unknownC7C4;
};
static_assert(sizeof(s_network_recruiting_search_globals) == 0xC7C8);
static_assert(0x0008 == OFFSETOF(s_network_recruiting_search_globals, recruiting_seeker));
static_assert(0x2EC0 == OFFSETOF(s_network_recruiting_search_globals, matchmaking_quality));
static_assert(0xC7B8 == OFFSETOF(s_network_recruiting_search_globals, search_active));
static_assert(0xC7B9 == OFFSETOF(s_network_recruiting_search_globals, sessions_updated));
static_assert(0xC7BC == OFFSETOF(s_network_recruiting_search_globals, maximum_session_count));
static_assert(0xC7C0 == OFFSETOF(s_network_recruiting_search_globals, session_storage));
static_assert(0xC7C4 == OFFSETOF(s_network_recruiting_search_globals, __unknownC7C4));

extern s_network_recruiting_search_globals& g_recruiting_search_globals;

extern bool __cdecl network_recruiting_search_begin(long controller_index, long squad_search_flags, long maximum_session_count, s_available_session* session_storage);
extern void __cdecl network_recruiting_search_dispose();
extern void __cdecl network_recruiting_search_end();
extern bool __cdecl network_recruiting_search_initialize();
extern void __cdecl network_recruiting_search_update();

