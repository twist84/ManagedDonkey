#include "networking/online/online_arbitration_windows.hpp"

#include "cseries/cseries_events.hpp"
#include "networking/logic/network_arbitration.hpp"
#include "networking/online/online_guide_pc.hpp"

//s_online_arbitration_globals g_online_arbitration_globals{};

void online_arbitration_dispose()
{
	//overlapped_track_delete(g_online_arbitration_globals.arbitration_overlapped_task);
	//csmemset(&g_online_arbitration_globals, 0, sizeof(s_online_arbitration_globals));
}

void online_arbitration_initialize()
{
	//csmemset(&g_online_arbitration_globals, 0, sizeof(s_online_arbitration_globals));
	//g_online_arbitration_globals.arbitration_overlapped_task = (c_arbitration_overlapped_task*)overlapped_malloc_tracked(sizeof(c_arbitration_overlapped_task), __FILE__, __LINE__);
	//
	//g_online_arbitration_globals.arbitration_overlapped_task->constructor(__FILE__, __LINE__);
	//
	//ASSERT(g_online_arbitration_globals.arbitration_overlapped_task);
	//g_online_arbitration_globals.initialized = true;
}

bool online_arbitration_initiate_registration(void* session_handle, uns64 session_nonce)
{
	//ASSERT(online_arbitration_registration_get_status() != _online_arbitration_registration_in_progress);
	//
	//g_online_arbitration_globals.arbitration_overlapped_task->m_session_handle = session_handle;
	//g_online_arbitration_globals.arbitration_overlapped_task->m_session_nonce = session_nonce;
	//
	//bool started = overlapped_task_start_internal(g_online_arbitration_globals.arbitration_overlapped_task, __FILE__, __LINE__);
	//if (!started)
	//{
	//	event(_event_warning, "networking:online:arbitration: failed to initiate arbitration for session 0x%08X", (uns32)session_handle);
	//}
	//
	//return started;
	//
	//return false;

	return true;
}

bool online_arbitration_registration_get_result(s_arbitration_registration_result* arbitration_result_out)
{
	//ASSERT(arbitration_result_out);
	//ASSERT(online_arbitration_registration_get_status() == _online_arbitration_registration_complete_success);
	//
	//csmemset(arbitration_result_out, 0, sizeof(s_arbitration_registration_result));
	//arbitration_result_out->registrant_count = 0;
	//
	//// `XSESSION_REGISTRATION_RESULTS` loop

	return false;
}

e_online_arbitration_registration_status online_arbitration_registration_get_status()
{
	//switch (g_online_arbitration_globals.arbitration_overlapped_task->get_task_state())
	//{
	//case _overlapped_task_state_none:
	//{
	//	return _online_arbitration_registration_none;
	//}
	//case _overlapped_task_state_starting:
	//case _overlapped_task_state_pending:
	//case _overlapped_task_state_completing:
	//{
	//	return _online_arbitration_registration_in_progress;
	//}
	//case _overlapped_task_state_succeeded:
	//{
	//	return _online_arbitration_registration_complete_success;
	//}
	//case _overlapped_task_state_failed:
	//{
	//	return _online_arbitration_registration_complete_failed;
	//}
	//}

	return _online_arbitration_registration_none;
}

