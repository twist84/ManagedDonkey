#include "networking/logic/network_recruiting_search.hpp"

#include "networking/logic/network_search.hpp"
#include "networking/network_memory.hpp"
#include "networking/network_time.hpp"


REFERENCE_DECLARE(0x0228E6E0, s_network_recruiting_search_globals, g_recruiting_search_globals);

void __cdecl c_recruiting_seeker::update()
{
	DECLFUNC(0x004E6E20, void, __thiscall, c_recruiting_seeker*)(this);
}

bool __cdecl network_recruiting_search_begin(long controller_index,long squad_search_flags,long maximum_session_count, s_available_session* session_storage)
{
	return INVOKE(0x004DBE00, network_recruiting_search_begin, controller_index, squad_search_flags, maximum_session_count, session_storage);

	//ASSERT(g_recruiting_search_globals.initialized);
	//ASSERT(maximum_session_count > 0);
	//ASSERT(session_storage);
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
		for (long i = 0; i < g_recruiting_search_globals.maximum_session_count; i++)
		{
			s_available_session* session = g_recruiting_search_globals.available_sessions + i;

			if (session->initialized)
			{
				if (network_time_since(session->time))
				{
					csmemset(session, 0, sizeof(s_available_session));
					g_recruiting_search_globals.__unknownC7B9 = true;
				}
			}
		}
	}
}

