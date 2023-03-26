#include "networking/logic/network_search.hpp"

#include "interface/user_interface_memory.hpp"
#include "networking/logic/network_broadcast_search.hpp"
#include "networking/logic/network_recruiting_search.hpp"


REFERENCE_DECLARE(0x0229AEA8, s_network_search_globals, g_network_search_globals);

void network_search_active(long controller_index, bool active)
{
	//INVOKE(0x004E1030, network_search_active, controller_index, active);

	if (active)
	{
		ASSERT(g_network_search_globals.reference_count > 0);

		network_search_begin(controller_index);
	}
	else
	{
		network_search_end();
	}
}

bool __cdecl network_search_begin(long controller_index)
{
	//return INVOKE(0x004E1090, network_search_begin, controller_index);

	bool result = false;
	if (!g_network_search_globals.available_sessions)
	{
		g_network_search_globals.available_sessions = (s_available_session*)g_network_search_globals.available_sessions_allocator->allocate(sizeof(s_available_session) * g_network_search_globals.maximum_sessions, "session_array");
	}

	if (g_network_search_globals.available_sessions)
	{
		g_network_search_globals.available_session_count = g_network_search_globals.maximum_sessions;

		switch (g_network_search_globals.search_category)
		{
		case 0:
			result = network_broadcast_search_begin(controller_index, g_network_search_globals.available_session_count, g_network_search_globals.available_sessions);
			break;
		case 1:
			result = network_recruiting_search_begin(controller_index, g_network_search_globals.squad_search_flags, g_network_search_globals.available_session_count, g_network_search_globals.available_sessions);
			break;
		case 2:
			//ASSERT(false);
			break;
		default:
			throw("unreachable");
		}
	}

	return result;
}

void network_search_dispose()
{
	//INVOKE(0x004E1120, network_search_dispose);

	network_search_end();
}

void __cdecl network_search_end()
{
	//INVOKE(0x004E1170, network_search_end);

	switch (g_network_search_globals.search_category)
	{
	case 0:
		network_broadcast_search_end();
		break;
	case 1:
		network_recruiting_search_end();
		break;
	case 2:
		//ASSERT(false);
		break;
	default:
		throw("unreachable");
	}

	if (g_network_search_globals.available_sessions)
	{
		g_network_search_globals.available_sessions_allocator->deallocate(g_network_search_globals.available_sessions);
		g_network_search_globals.available_sessions = 0;
		g_network_search_globals.available_session_count = 0;
	}
}

bool network_search_initialize()
{
	//return INVOKE(0x004E11C0, network_search_initialize);

	csmemset(&g_network_search_globals, 0, sizeof(s_network_search_globals));

	return true;
}

s_available_session* network_search_session(long available_squad_index)
{
	//return INVOKE(0x004E1200, network_search_session, available_squad_index);

	if (available_squad_index >= 0 && available_squad_index < g_network_search_globals.available_session_count)
	{
		s_available_session* session = &g_network_search_globals.available_sessions[available_squad_index];
		if (network_search_session_valid(session))
			return session;
	}

	return nullptr;
}

long network_search_session_count()
{
	//return INVOKE(0x004E1230, network_search_session_count);

	return g_network_search_globals.available_session_count;
}

bool __cdecl network_search_session_valid(s_available_session* session)
{
	//return INVOKE(0x004E1240, network_search_session_valid, session);

	ASSERT(session);

	return session->initialized && session->status_data.header.platform == 1;
}

bool __cdecl network_search_start(long controller_index, long category, long maximum_sessions, dword_flags available_squad_search_flags, c_allocation_base* session_allocator)
{
	//return INVOKE(0x004E1260, network_search_start, controller_index, category, maximum_sessions, available_squad_search_flags, session_allocator);

	if (g_network_search_globals.reference_count <= 0)
	{
		g_network_search_globals.search_category = category;
		g_network_search_globals.maximum_sessions = maximum_sessions;
		g_network_search_globals.squad_search_flags = available_squad_search_flags;
		g_network_search_globals.available_sessions_allocator = session_allocator;
	}
	else
	{
		ASSERT(category == g_network_search_globals.search_category);
		ASSERT(maximum_sessions == g_network_search_globals.maximum_sessions);
		ASSERT(available_squad_search_flags == g_network_search_globals.squad_search_flags);
		ASSERT(session_allocator == g_network_search_globals.available_sessions_allocator);
	}

	if (!network_search_begin(controller_index))
		return false;

	g_network_search_globals.reference_count++;

	return true;
}

void network_search_stop()
{
	//INVOKE(0x004E12B0, network_search_stop);

	ASSERT(g_network_search_globals.reference_count > 0);

	if (--g_network_search_globals.reference_count <= 0)
	{
		network_search_end();
		g_network_search_globals.maximum_sessions = 0;
		g_network_search_globals.available_sessions_allocator = 0;
	}
}

