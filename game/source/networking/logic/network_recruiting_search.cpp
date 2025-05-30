#include "networking/logic/network_recruiting_search.hpp"

#include "networking/logic/network_search.hpp"
#include "networking/network_memory.hpp"
#include "networking/network_time.hpp"


REFERENCE_DECLARE(0x0228E6E0, s_network_recruiting_search_globals, g_recruiting_search_globals);

void c_recruiting_seeker::update()
{
	INVOKE_CLASS_MEMBER(0x004E6E20, c_recruiting_seeker, update);
}

bool __cdecl network_recruiting_search_begin(int32 controller_index,int32 squad_search_flags,int32 maximum_session_count, s_available_session* session_storage)
{
	return INVOKE(0x004DBE00, network_recruiting_search_begin, controller_index, squad_search_flags, maximum_session_count, session_storage);

	//ASSERT(g_recruiting_search_globals.initialized);
	//ASSERT(maximum_session_count > 0);
	//ASSERT(session_storage);
	//
	//if (!g_recruiting_search_globals.search_active)
	//{
	//	g_recruiting_search_globals.matchmaking_quality.initialize();
	//	if (g_recruiting_search_globals.recruiting_seeker.search_begin(
	//		controller_index,
	//		squad_search_flags,
	//		false,
	//		&g_recruiting_search_globals.matchmaking_quality))
	//	{
	//		g_recruiting_search_globals.maximum_session_count = maximum_session_count;
	//		g_recruiting_search_globals.search_active = true;
	//		g_recruiting_search_globals.sessions_updated = false;
	//		g_recruiting_search_globals.session_storage = session_storage;
	//		csmemset(session_storage, 0, sizeof(s_available_session) * maximum_session_count);
	//	}
	//}
	//
	//return g_recruiting_search_globals.search_active;
}

void __cdecl network_recruiting_search_dispose()
{
	//INVOKE(0x004DBE70, network_recruiting_search_dispose);

	csmemset(&g_recruiting_search_globals, 0, sizeof(g_recruiting_search_globals));
}

void __cdecl network_recruiting_search_end()
{
	INVOKE(0x004DBE90, network_recruiting_search_dispose);

	//ASSERT(g_recruiting_search_globals.initialized);
}

bool __cdecl network_recruiting_search_initialize()
{
	//return INVOKE(0x004DBEF0, network_recruiting_search_initialize);

	ASSERT(!g_recruiting_search_globals.initialized);

	csmemset(&g_recruiting_search_globals, 0, sizeof(g_recruiting_search_globals));
	g_recruiting_search_globals.initialized = true;

	return true;
}

void __cdecl network_recruiting_search_update()
{
	//INVOKE(0x004DBF10, network_recruiting_search_update);

	ASSERT(g_recruiting_search_globals.initialized);

	if (g_recruiting_search_globals.search_active)
	{
		g_recruiting_search_globals.recruiting_seeker.update();
		for (int32 i = 0; i < g_recruiting_search_globals.maximum_session_count; i++)
		{
			s_available_session* session = g_recruiting_search_globals.session_storage + i;

			if (session->session_valid)
			{
				if (network_time_since(session->last_update_timestamp) > 10000)
				{
					csmemset(session, 0, sizeof(s_available_session));
					g_recruiting_search_globals.sessions_updated = true;
				}
			}
		}
	}
}

