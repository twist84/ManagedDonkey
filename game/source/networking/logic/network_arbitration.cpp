#include "networking/logic/network_arbitration.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/online/online_arbitration_windows.hpp"
#include "networking/session/network_managed_session.hpp"

//REFERENCE_DECLARE_ARRAY(0x0189C4D8, const char*, g_network_arbitration_status_string, k_network_arbitration_status_count);
REFERENCE_DECLARE(0x0228E520, s_network_arbitration_globals, network_arbitration_globals);

HOOK_DECLARE(0x004D9AB0, network_arbitration_get_results);
HOOK_DECLARE(0x004D9B20, network_arbitration_get_status);
HOOK_DECLARE(0x004D9B60, network_arbitration_initiate);

void __cdecl network_arbitration_destory()
{
	//INVOKE(0x004D9A90, network_arbitration_destory);

	csmemset(&network_arbitration_globals, 0, sizeof(network_arbitration_globals));
	network_arbitration_globals.arbitration_managed_session_index = NONE;
}

bool __cdecl network_arbitration_get_results(int32 managed_session_index, s_arbitration_registration_result* registration_result)
{
	//return INVOKE(0x004D9AB0, network_arbitration_get_results, managed_session_index, registration_result);

	ASSERT(managed_session_index != NONE);
	ASSERT(registration_result);

	if (managed_session_index != network_arbitration_globals.arbitration_managed_session_index)
	{
		event(_event_error, "networking:leaderboards:arbitration: failed to get arbitration result, session mismatch [0x%08X != 0x%08X]",
			managed_session_index,
			network_arbitration_globals.arbitration_managed_session_index);

		return false;
	}

	if (network_arbitration_globals.arbitration_status != _network_arbitration_status_registered)
	{
		event(_event_error, "networking:leaderboards:arbitration: failed to get arbitration result, arbitration state not registered [0x%08X %s]",
			managed_session_index,
			g_network_arbitration_status_string[network_arbitration_globals.arbitration_status]);

		return false;
	}

	*registration_result = network_arbitration_globals.registration_result;
	return true;
}

e_network_arbitration_status __cdecl network_arbitration_get_status()
{
	//return INVOKE(0x004D9B20, network_arbitration_get_status);

	return network_arbitration_globals.arbitration_status;
}

bool __cdecl network_arbitration_initialize()
{
	//return INVOKE(0x004D9B30, network_arbitration_initialize);

	csmemset(&network_arbitration_globals, 0, sizeof(network_arbitration_globals));
	network_arbitration_globals.arbitration_managed_session_index = NONE;
	network_arbitration_globals.initialized = true;
	return true;
}

bool __cdecl network_arbitration_initiate(int32 managed_session_index, uns64 nonce)
{
	//return INVOKE(0x004D9B60, network_arbitration_initiate, managed_session_index, nonce);

	ASSERT(managed_session_index != NONE);
	ASSERT(nonce);
	ASSERT(network_arbitration_get_status() != _network_arbitration_status_registration_in_progress);

	network_arbitration_globals.arbitration_status = _network_arbitration_status_registration_failed;
	network_arbitration_globals.arbitration_managed_session_index = managed_session_index;

	void* session_handle = nullptr;
	if (!managed_session_get_handle(managed_session_index, &session_handle))
	{
		event(_event_warning, "networking:leaderboards:arbitration: failed to get session handle for session 0x%08X",
			managed_session_index);

		return false;
	}

	if (!online_arbitration_initiate_registration(session_handle, nonce))
	{
		event(_event_warning, "networking:leaderboards:arbitration: failed to initiate registration for session 0x%08X",
			managed_session_index);

		return false;
	}

	network_arbitration_globals.arbitration_status = _network_arbitration_status_registration_in_progress;
	return true;
}

void __cdecl network_arbitration_update()
{
	//INVOKE(0x004D9BB0, network_arbitration_update);

	network_arbitration_update_registration();
}

void __cdecl network_arbitration_update_registration()
{
	//INVOKE(0x004D9BE0, network_arbitration_update_registration);

	if (network_arbitration_globals.arbitration_status != _network_arbitration_status_registration_in_progress)
	{
		return;
	}

	e_online_arbitration_registration_status status = online_arbitration_registration_get_status();
	switch (status)
	{
	case _online_arbitration_registration_none:
	{
		event(_event_error, "networking:leaderboards:arbitration: registration failed for session 0x%08X (unexpected result)",
			network_arbitration_globals.arbitration_managed_session_index);

		network_arbitration_globals.arbitration_status = _network_arbitration_status_registration_failed;
	}
	break;
	case _online_arbitration_registration_in_progress:
	{
		event(_event_warning, "networking:leaderboards:arbitration: registration failed for session 0x%08X",
			network_arbitration_globals.arbitration_managed_session_index);

		network_arbitration_globals.arbitration_status = _network_arbitration_status_registration_failed;
	}
	break;
	case _online_arbitration_registration_complete_success:
	{
		event(_event_message, "networking:leaderboards:arbitration: registration completed for session 0x%08X",
			network_arbitration_globals.arbitration_managed_session_index);

		if (!online_arbitration_registration_get_result(&network_arbitration_globals.registration_result))
		{
			event(_event_message, "networking:leaderboards:arbitration: registration completed, but we failed to get results for session 0x%08X",
				network_arbitration_globals.arbitration_managed_session_index);

			network_arbitration_globals.arbitration_status = _network_arbitration_status_registration_failed;
			break;
		}

		network_arbitration_globals.arbitration_status = _network_arbitration_status_registered;
	}
	break;
	case _online_arbitration_registration_complete_failed:
	{
		event(_event_message, "networking:leaderboards:arbitration: registration completed, but we failed to get results for session 0x%08X",
			network_arbitration_globals.arbitration_managed_session_index);

		network_arbitration_globals.arbitration_status = _network_arbitration_status_registration_failed;
	}
	break;
	default:
	{
		event(_event_error, "networking:leaderboards:arbitration: registration failed for session 0x%08X (unexpected result)",
			network_arbitration_globals.arbitration_managed_session_index);

		network_arbitration_globals.arbitration_status = _network_arbitration_status_registration_failed;
	}
	break;
	}
}

const char* g_network_arbitration_status_string[k_network_arbitration_status_count]
{
	"arbitration-status-none",
	"arbitration-status-registration-in-progress",
	"arbitration-status-registered",
	"arbitration-status-registration-failed"
};

