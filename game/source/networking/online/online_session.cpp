#include "networking/online/online_session.hpp"

#include "networking/session/network_managed_session.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_controller.hpp"
#include "memory/module.hpp"

#include <windows.h>

#define VALID_HANDLE(HANDLE) (HANDLE && HANDLE != INVALID_HANDLE_VALUE)

HOOK_DECLARE_CLASS_MEMBER(0x00442C00, c_managed_session_overlapped_task, process_add_players);
HOOK_DECLARE_CLASS_MEMBER(0x00442C10, c_managed_session_overlapped_task, process_add_players_immediately);
HOOK_DECLARE_CLASS_MEMBER(0x00442C20, c_managed_session_overlapped_task, process_create);
HOOK_DECLARE_CLASS_MEMBER(0x00442C30, c_managed_session_overlapped_task, process_delete);
HOOK_DECLARE_CLASS_MEMBER(0x00442C40, c_managed_session_overlapped_task, process_game_end);
HOOK_DECLARE_CLASS_MEMBER(0x00442C50, c_managed_session_overlapped_task, process_game_start);
HOOK_DECLARE_CLASS_MEMBER(0x00442C60, c_managed_session_overlapped_task, process_modify);
HOOK_DECLARE_CLASS(0x00442C70, c_managed_session_overlapped_task, process_modify_immediately);
HOOK_DECLARE_CLASS_MEMBER(0x00442C80, c_managed_session_overlapped_task, process_remove_players);
HOOK_DECLARE_CLASS_MEMBER(0x00442CA0, c_managed_session_overlapped_task, process_session_host_migrate);
HOOK_DECLARE_CLASS_MEMBER(0x00442CB0, c_managed_session_overlapped_task, start_);

uns32 __thiscall c_managed_session_overlapped_task::start_(void* overlapped)
{
	return c_managed_session_overlapped_task::start(overlapped);
}

void c_managed_session_overlapped_task::filter_local_users(int32 player_count, const uns64* players, const bool* online_enabled, const bool* private_slots)
{
	int32 filtered_player_count = 0;

	for (int32 player_index = 0; player_index < player_count; player_index++)
	{
		if (!online_enabled[player_index])
		{
			continue;
		}

		if (private_slots)
		{
			m_private_slots[filtered_player_count] = private_slots[player_index];
		}

		m_player_xuids[filtered_player_count] = players[player_index];
		filtered_player_count++;
	}
	
	m_player_count = filtered_player_count;
}

void c_managed_session_overlapped_task::process_add_players(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session, const uns64* player_xuids, const bool* online_enabled, const bool* private_slots, int32 player_count)
{
	//INVOKE_CLASS_MEMBER(0x00442C00, c_managed_session_overlapped_task, process_add_players, managed_session_index, completion_routine, session, player_xuids, online_enabled, private_slots, player_count);

	m_session = session;
	m_managed_session_index = managed_session_index;
	m_completion_routine = completion_routine;
	m_context = _process_add_players;
	m_result = false;
	m_return_result = 0;

	filter_local_users(player_count, player_xuids, online_enabled, private_slots);

	if (!m_player_count)
	{
		m_result = true;
		reset();
	}
	else if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
	{
		reset();
	}
}

bool c_managed_session_overlapped_task::process_add_players_immediately(s_online_session* session, const uns64* player_xuids, const bool* online_enabled, const bool* private_slots, int32 player_count)
{
	//return INVOKE_CLASS_MEMBER(0x00442C10, c_managed_session_overlapped_task, process_add_players_immediately, session, player_xuids, online_enabled, private_slots, player_count);

	filter_local_users(player_count, player_xuids, online_enabled, private_slots);

	return true;
}

void c_managed_session_overlapped_task::process_create(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session, uns16 mask)
{
	//INVOKE_CLASS_MEMBER(0x00442C20, c_managed_session_overlapped_task, process_create, managed_session_index, completion_routine, session, mask);

	m_context = _process_create;
	m_session = session;
	m_managed_session_index = managed_session_index;
	m_completion_routine = completion_routine;
	m_result = false;
	m_return_result = 0;
	m_mask = mask;

	if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
		reset();
}

void c_managed_session_overlapped_task::process_delete(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session)
{
	//INVOKE_CLASS_MEMBER(0x00442C30, c_managed_session_overlapped_task, process_delete, managed_session_index, completion_routine, session);

	m_managed_session_index = managed_session_index;
	m_completion_routine = completion_routine;
	m_session = session;
	m_context = _process_delete;
	m_result = false;
	m_return_result = 0;

	if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
		reset();
}

void c_managed_session_overlapped_task::process_game_end(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session)
{
	//INVOKE_CLASS_MEMBER(0x00442C40, c_managed_session_overlapped_task, process_game_end, managed_session_index, completion_routine, session);

	m_managed_session_index = managed_session_index;
	m_completion_routine = completion_routine;
	m_session = session;
	m_context = _process_end;
	m_result = false;
	m_return_result = 0;

	if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
		reset();
}

void c_managed_session_overlapped_task::process_game_start(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session)
{
	//INVOKE_CLASS_MEMBER(0x00442C50, c_managed_session_overlapped_task, process_game_start, managed_session_index, completion_routine, session);

	m_managed_session_index = managed_session_index;
	m_completion_routine = completion_routine;
	m_session = session;
	m_context = _process_start;
	m_result = false;
	m_return_result = 0;

	if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
		reset();
}

void c_managed_session_overlapped_task::process_modify(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session, s_online_session* desired_session, s_online_session* actual_session)
{
	//INVOKE_CLASS_MEMBER(0x00442C60, c_managed_session_overlapped_task, process_modify, managed_session_index, completion_routine, session, desired_session, actual_session);

	m_managed_session_index = managed_session_index;
	m_desired_session = desired_session;
	m_session = session;
	m_actual_session = actual_session;
	m_context = _process_modify;
	m_completion_routine = completion_routine;
	m_result = false;
	m_return_result = 0;

	if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
		reset();
}

bool __cdecl c_managed_session_overlapped_task::process_modify_immediately(s_online_session* desired_session, s_online_session* actual_session)
{
	//return INVOKE(0x00442C70, c_managed_session_overlapped_task::process_modify_immediately, desired_session, actual_session);

	// $IMPLEMENT

	return true;
}

void c_managed_session_overlapped_task::process_remove_players(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session, const uns64* player_xuids, const bool* online_enabled, int32 player_count)
{
	//INVOKE_CLASS_MEMBER(0x00442C80, c_managed_session_overlapped_task, process_remove_players, managed_session_index, completion_routine, session, player_xuids, online_enabled, player_count);

	m_session = session;
	m_managed_session_index = managed_session_index;
	m_completion_routine = completion_routine;
	m_context = _process_remove_players;
	m_result = false;
	m_return_result = 0;

	filter_local_users(player_count, player_xuids, online_enabled, NULL);

	if (!m_player_count)
	{
		m_result = true;
		reset();
	}
	else if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
	{
		reset();
	}
}

bool c_managed_session_overlapped_task::process_remove_players_immediately(s_online_session* session, const uns64* player_xuids, const bool* online_enabled, int32 player_count)
{
	//return INVOKE_CLASS_MEMBER(0x00442C90, c_managed_session_overlapped_task, process_remove_players_immediately, session, player_xuids, online_enabled, player_count);

	return true;
}

void c_managed_session_overlapped_task::process_session_host_migrate(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session, bool is_host, s_transport_session_description* host_migration_description)
{
	//INVOKE_CLASS_MEMBER(0x00442CA0, c_managed_session_overlapped_task, process_session_host_migrate, managed_session_index, completion_routine, session, is_host, host_migration_description);

	m_managed_session_index = managed_session_index;
	m_is_host = is_host;
	m_session = session;
	m_host_migration_description = host_migration_description;
	m_context = _process_session_host_migrate;
	m_completion_routine = completion_routine;
	m_result = false;
	m_return_result = 0;

	if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
		reset();
}

uns32 c_managed_session_overlapped_task::start(void* overlapped)
{
	//return INVOKE_CLASS_MEMBER(0x00442CB0, c_managed_session_overlapped_task, start, overlapped);

	uns32 result = 0;

	e_overlapped_task context = m_context;
	switch (context)
	{
	case _process_create:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		//ASSERT(m_session);
		//
		//DWORD dwFlags = m_session->controller_index;
		//DWORD dwUserIndex = m_session->controller_index;
		//DWORD dwMaxPublicSlots = m_session->public_slots_flags;
		//DWORD dwMaxPrivateSlots = m_session->private_slots_flags;
		//ULONGLONG qwSessionNonce = m_session->nonce;
		//XSESSION_INFO SessionInfo = m_session->description;
		//
		//if (controller_get(m_session->controller_index)->is_signed_in_to_live())
		//{
		//	result = XSessionCreate(dwFlags, dwUserIndex, dwMaxPublicSlots, dwMaxPrivateSlots, qwSessionNonce, SessionInfo, (PXOVERLAPPED)overlapped, handle);
		//}
		//else
		//{
		//	result = E_FAIL;
		//}
		//
		//event(_event_message, "networking:managed_session: created XSession handle %08X",
		//	m_session->handle);

		DWORD dwFlags = m_session->controller_index;
		DWORD dwUserIndex = m_session->controller_index;
		DWORD dwMaxPublicSlots = m_session->public_slot_count;
		DWORD dwMaxPrivateSlots = m_session->private_slot_count;

		s_transport_secure_identifier blank_identifier{};

		// If a valid session description exists, we use it.
		if (csmemcmp(&m_session->session_description.id, &blank_identifier, sizeof(s_transport_secure_identifier)) == 0)
		{
			transport_secure_random(sizeof(m_session->nonce), (byte*)&m_session->nonce);
			//transport_secure_random(sizeof(m_session->handle), (byte*)&m_session->handle);
			transport_secure_random(sizeof(m_session->session_description.id), m_session->session_description.id.data);
			transport_secure_address_get(&m_session->session_description.host_address);
			m_session->players[0].xuid = 1;
			//transport_secure_random(sizeof(m_session->session_description.host_address), m_session->session_description.host_address.data);
		}
		//transport_secure_random(sizeof(m_session->session_description.key), m_session->session_description.key.data); // not used

		//SessionInfo->hostAddress.inaOnline.S_un.S_addr = 0x7F000001;
		//SessionInfo->hostAddress.wPortOnline = 8080;
		//csmemset(SessionInfo->hostAddress.abEnet, 0x8B, sizeof(SessionInfo->hostAddress.abEnet));
		//csmemset(SessionInfo->hostAddress.abOnline, 0x56, sizeof(SessionInfo->hostAddress.abOnline));

		c_console::write_line("donkey-matchmaking: Made session with ID '%s'", transport_secure_identifier_get_string(&m_session->session_description.id));

		c_managed_session_overlapped_task::success(0);
		c_managed_session_overlapped_task::complete();
	}
	break;
	case _process_delete:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		//ASSERT(m_session);
		//ASSERT(VALID_HANDLE(m_session->handle));
		//
		//DWORD cbResultsBuffer = 640;
		//XSESSION_LOCAL_DETAILS SessionDetails{};
		//if (XSessionGetDetails(m_session->handle, &cbResultsBuffer, &SessionDetails, NULL))
		//{
		//	event(_event_warning, "networking:managed_session: attempting to delete XSession handle %08X, but GetDetails() failed, so we are assuming the session is toast!",
		//		m_session->handle);
		//	result = E_FAIL;
		//}
		//else
		//{
		//	result = XSessionDelete(m_session->handle, (PXOVERLAPPED)overlapped);
		//}

		c_managed_session_overlapped_task::success(0);
		c_managed_session_overlapped_task::complete();
	}
	break;
	case _process_session_host_migrate:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		//ASSERT(m_session);
		//
		//DWORD userIndex = m_is_host ? m_session->controller_index : 0xFE;
		//result = XSessionMigrateHost(m_session->handle, userIndex, m_host_migration_description, (PXOVERLAPPED)overlapped);

		c_managed_session_overlapped_task::success(0);
		c_managed_session_overlapped_task::complete();
	}
	break;
	case _process_modify:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		//ASSERT(VALID_HANDLE(m_session->handle));
		//
		//DWORD dwMaxPublicSlots;
		//DWORD dwMaxPrivateSlots;
		//DWORD dwFlags = calculate_slot_counts(m_desired_session, m_actual_session, &dwMaxPublicSlots, &dwMaxPrivateSlots);
		//
		//event(_event_message, "networking:managed_session:process_modify: flags=%X, private=%d, public=%d",
		//	dwFlags,
		//	dwMaxPublicSlots,
		//	dwMaxPrivateSlots);
		//
		//result = XSessionModify(m_session->handle, dwFlags, dwMaxPublicSlots, dwMaxPrivateSlots, (PXOVERLAPPED)overlapped);

		c_managed_session_overlapped_task::success(0);
		c_managed_session_overlapped_task::complete();
	}
	break;
	case _process_add_players:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		//ASSERT(VALID_HANDLE(m_session->handle));
		//ASSERT(m_player_xuids);
		//ASSERT(m_player_count > 0);
		//ASSERT(m_private_slots);
		//
		//result = XSessionJoinRemote(m_session->handle, m_player_count, m_player_xuids, m_private_slots, (PXOVERLAPPED)overlapped);

		c_managed_session_overlapped_task::success(0);
		c_managed_session_overlapped_task::complete();
	}
	break;
	case _process_remove_players:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		//ASSERT(m_player_xuids);
		//ASSERT(m_player_count > 0);
		//ASSERT(m_session);
		//ASSERT(VALID_HANDLE(m_session->handle));
		//
		//result = XSessionLeaveRemote(m_session->handle, m_player_count, m_player_xuids, (PXOVERLAPPED)overlapped);

		c_managed_session_overlapped_task::success(0);
		c_managed_session_overlapped_task::complete();
	}
	break;
	case _process_start:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		//ASSERT(m_session);
		//ASSERT(VALID_HANDLE(m_session->handle));
		//
		//result = XSessionStart(m_session->handle, (PXOVERLAPPED)overlapped);

		c_managed_session_overlapped_task::success(0);
		c_managed_session_overlapped_task::complete();
	}
	break;
	case _process_end:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		//ASSERT(m_session);
		//ASSERT(VALID_HANDLE(m_session->handle));
		//
		//result = XSessionEnd(m_session->handle, (PXOVERLAPPED)overlapped);

		c_managed_session_overlapped_task::success(0);
		c_managed_session_overlapped_task::complete();
	}
	break;
	default:
	{
		c_console::write_line("managed-session-context: unknown %d", context);
	}
	break;
	}

	return result;
}

