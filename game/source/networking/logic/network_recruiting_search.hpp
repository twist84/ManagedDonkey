#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_matchmaking_quality.hpp"

struct s_recruiting_session_search
{
	byte __data[0x2E8];
};
COMPILE_ASSERT(sizeof(s_recruiting_session_search) == 0x2E8);

class c_recruiting_seeker
{
public:
	void update();

	uns8 m_flags;
	int32 m_squad_search_flags;
	int32 m_controller_index;
	int32 m_session_search_count;
	int32 m_current_session_search_index;
	int32 m_total_session_search_failures;
	int32 m_total_session_searches;
	int32 m_total_sessions_found;
	int32 m_current_sessions_contacted_count;
	int32 m_total_session_contacted_count;
	uns32 m_search_start_time;
	uns32 m_current_search_completed_time;
	c_static_array<s_recruiting_session_search, 16> m_session_search_data;
	int32 m_game_mode;
	int32 m_game_type;
};
COMPILE_ASSERT(sizeof(c_recruiting_seeker) == 0x2EB8);

struct s_available_session;
struct s_network_recruiting_search_globals
{
	bool initialized;
	uns32 __unknown4;
	c_recruiting_seeker recruiting_seeker;
	c_matchmaking_quality matchmaking_quality;
	bool search_active;
	bool sessions_updated;
	int32 maximum_session_count;
	s_available_session* session_storage;
	uns32 __unknownC7C4;
};
COMPILE_ASSERT(sizeof(s_network_recruiting_search_globals) == 0xC7C8);
COMPILE_ASSERT(0x0008 == OFFSETOF(s_network_recruiting_search_globals, recruiting_seeker));
COMPILE_ASSERT(0x2EC0 == OFFSETOF(s_network_recruiting_search_globals, matchmaking_quality));
COMPILE_ASSERT(0xC7B8 == OFFSETOF(s_network_recruiting_search_globals, search_active));
COMPILE_ASSERT(0xC7B9 == OFFSETOF(s_network_recruiting_search_globals, sessions_updated));
COMPILE_ASSERT(0xC7BC == OFFSETOF(s_network_recruiting_search_globals, maximum_session_count));
COMPILE_ASSERT(0xC7C0 == OFFSETOF(s_network_recruiting_search_globals, session_storage));
COMPILE_ASSERT(0xC7C4 == OFFSETOF(s_network_recruiting_search_globals, __unknownC7C4));

extern s_network_recruiting_search_globals& g_recruiting_search_globals;

extern bool __cdecl network_recruiting_search_begin(int32 controller_index, int32 squad_search_flags, int32 maximum_session_count, s_available_session* session_storage);
extern void __cdecl network_recruiting_search_dispose();
extern void __cdecl network_recruiting_search_end();
extern bool __cdecl network_recruiting_search_initialize();
extern void __cdecl network_recruiting_search_update();

