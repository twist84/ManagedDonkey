#include "networking/session/network_managed_session.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/session/network_session_membership.hpp"

REFERENCE_DECLARE(0x02247448, s_online_session_manager_globals, online_session_manager_globals);

HOOK_DECLARE_CLASS_MEMBER(0x00480420, c_managed_session_overlapped_task, complete_);
//HOOK_DECLARE_CLASS_MEMBER(0x00480440, c_managed_session_overlapped_task, failure_);
HOOK_DECLARE(0x00481F10, managed_session_game_start);
HOOK_DECLARE(0x00481F80, managed_session_game_start_complete);
//HOOK_DECLARE_CLASS_MEMBER(0x00483CB0, c_managed_session_overlapped_task, success_);

void __thiscall c_managed_session_overlapped_task::complete_()
{
	c_managed_session_overlapped_task::complete();
}

void __thiscall c_managed_session_overlapped_task::failure_(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error)
{
	c_managed_session_overlapped_task::failure(calling_result, overlapped_error, overlapped_extended_error);
}

void __thiscall c_managed_session_overlapped_task::success_(uns32 return_result)
{
	c_managed_session_overlapped_task::success(return_result);
	m_return_result = return_result;
	m_result = true;
}

//.text:00480200 ; c_managed_session_overlapped_task* __cdecl c_managed_session_overlapped_task::c_managed_session_overlapped_task(const char*, int32);
//.text:00480220 ; c_managed_session_overlapped_task* __cdecl c_managed_session_overlapped_task::c_managed_session_overlapped_task();
//.text:00480240 ; s_online_managed_session* __cdecl s_online_managed_session::s_online_managed_session();
//.text:004802A0 ; s_online_managed_session* __cdecl s_online_managed_session::~s_online_managed_session();

void __cdecl add_to_player_list(s_online_session_player* players, int32 player_count, const uns64* xuids, const bool* xuids_left_game, int32 xuid_count)
{
	INVOKE(0x00480350, add_to_player_list, players, player_count, xuids, xuids_left_game, xuid_count);
}

void c_managed_session_overlapped_task::complete()
{
	//INVOKE_CLASS_MEMBER(0x00480420, c_managed_session_overlapped_task, complete);

	m_completion_routine(m_managed_session_index, m_result, m_return_result);
}

void c_managed_session_overlapped_task::failure(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error)
{
	//INVOKE_CLASS_MEMBER(0x00480440, c_managed_session_overlapped_task, failure, calling_result, overlapped_error, overlapped_extended_error);

	m_return_result = overlapped_extended_error;
	m_result = false;
}

const char* c_managed_session_overlapped_task::get_context_string() const
{
	//return INVOKE_CLASS_MEMBER(0x004804B0, c_managed_session_overlapped_task, get_context_string);

	const char* result = "unknown";
	switch (m_context)
	{
	case _process_create:
	{
		result = "session create";
	}
	break;
	case _process_delete:
	{
		result = "session delete";
	}
	break;
	case _process_modify:
	{
		result = "session modify";
	}
	break;
	case _process_add_players:
	{
		result = "session add players";
	}
	break;
	case _process_remove_players:
	{
		result = "session remove players";
	}
	break;
	case _process_start:
	{
		result = "session start";
	}
	break;
	case _process_end:
	{
		result = "session end";
	}
	break;
	}
	return result;
}

//.text:00480510 ; e_controller_index __cdecl get_first_signed_in_controller_index()
//.text:00480590 ; public: bool __cdecl c_controller_interface::in_use() const

void __cdecl managed_session_add_players(int32 managed_session_index, const uns64* xuids, const bool* as_privates, int32 xuid_count)
{
	INVOKE(0x00480630, managed_session_add_players, managed_session_index, xuids, as_privates, xuid_count);
}

int32 __cdecl managed_session_allocate()
{
	return INVOKE(0x00480680, managed_session_allocate);
}

bool __cdecl managed_session_build_add_xuid_list(int32 managed_session_index)
{
	//return INVOKE(0x00480790, managed_session_build_add_xuid_list, managed_session_index);

	s_online_managed_session* managed_session = &online_session_manager_globals.managed_sessions[managed_session_index];
	int32 desired_player_count = 16;
	s_online_session_player* desired_player = &managed_session->desired_online_session_state.players[desired_player_count - 1];
	do
	{
		desired_player = &managed_session->desired_online_session_state.players[desired_player_count - 1];
		if (desired_player->flags.test(_online_session_player_valid_bit))
		{
			int32 actual_player_index = 0;
			s_online_session_player* actual_player = &managed_session->actual_online_session_state.players[actual_player_index];
			while (!actual_player->flags.test(_online_session_player_valid_bit) || desired_player->xuid != actual_player->xuid)
			{
				actual_player = &managed_session->actual_online_session_state.players[actual_player_index];

				actual_player_index++;
				if (actual_player_index >= 16)
				{
					managed_session->xuids[managed_session->session_player_operation_count] = desired_player->xuid;
					managed_session->xuid_next_flags[managed_session->session_player_operation_count] = desired_player->flags.test(_online_session_player_occupies_private_slot_bit);
					managed_session->session_player_operation_count++;
					break;
				}
			}
		}
		desired_player_count--;
	}
	while (desired_player_count);

	event(_event_message, "networking:managed_session: session 0x%8X, operating on %d xuids",
		managed_session_index,
		managed_session->session_player_operation_count);

	//c_console::write_line("donkey:matchmaking managed_session_build_add_xuid_list returning %d", managed_session->session_player_operation_count > 0);
	//c_console::write_line("donkey:matchmaking desired player 1: I64u / %d", managed_session->desired_online_session_state.players[0].xuid, managed_session->desired_online_session_state.players[0].flags);
	//c_console::write_line("donkey:matchmaking actual player 1: I64u / %d", managed_session->actual_online_session_state.players[0].xuid, managed_session->actual_online_session_state.players[0].flags);

	//c_console::write_line("donkey:matchmaking desired player 2: I64u / %d", managed_session->desired_online_session_state.players[1].xuid, managed_session->desired_online_session_state.players[1].flags);
	//c_console::write_line("donkey:matchmaking actual player 2: I64u / %d", managed_session->actual_online_session_state.players[1].xuid, managed_session->actual_online_session_state.players[1].flags);

	//c_console::write_line("donkey:matchmaking desired player 3: I64u / %d", managed_session->desired_online_session_state.players[2].xuid, managed_session->desired_online_session_state.players[2].flags);
	//c_console::write_line("donkey:matchmaking actual player 3: I64u / %d", managed_session->actual_online_session_state.players[2].xuid, managed_session->actual_online_session_state.players[2].flags);

	return managed_session->session_player_operation_count > 0;
}

bool __cdecl managed_session_build_removal_xuid_list(int32 managed_session_index)
{
	return INVOKE(0x00480840, managed_session_build_removal_xuid_list, managed_session_index);
}

bool __cdecl managed_session_can_create_session(e_network_session_class session_class)
{
	return INVOKE(0x00480900, managed_session_can_create_session, session_class);
}

void __cdecl managed_session_cancel_new_host(int32 managed_session_index)
{
	INVOKE(0x00480950, managed_session_cancel_new_host, managed_session_index);
}

void __cdecl managed_session_commit_new_host(int32 managed_session_index)
{
	INVOKE(0x00480D10, managed_session_commit_new_host, managed_session_index);
}

bool __cdecl managed_session_compare_id(int32 managed_session_index, const s_transport_secure_identifier* id)
{
	return INVOKE(0x00480DD0, managed_session_compare_id, managed_session_index, id);
}

void __cdecl managed_session_completely_reset_session_on_error(int32 managed_session_index)
{
	INVOKE(0x00480E60, managed_session_completely_reset_session_on_error, managed_session_index);
}

void __cdecl managed_session_connect_client_to_new_host(int32 managed_session_index, const s_transport_session_description* session_description, e_transport_platform secure_key_platform)
{
	INVOKE(0x00480EF0, managed_session_connect_client_to_new_host, managed_session_index, session_description, secure_key_platform);
}

int32 __cdecl managed_session_count_master_sessions()
{
	return INVOKE(0x00481070, managed_session_count_master_sessions);
}

int32 __cdecl managed_session_create_client(e_network_session_class session_class, c_managed_session_creation_flags creation_flags, e_transport_platform secure_key_platform, int32 private_slot_count, int32 public_slot_count, const s_transport_secure_identifier* session_id, const s_transport_secure_address* host_address, const s_transport_secure_key* key, const s_online_context* creation_contexts, int32 context_count)
{
	return INVOKE(0x00481110, managed_session_create_client, session_class, creation_flags, secure_key_platform, private_slot_count, public_slot_count, session_id, host_address, key, creation_contexts, context_count);
}

int32 __cdecl managed_session_create_host(e_network_session_class session_class, c_managed_session_creation_flags creation_flags, e_transport_platform secure_key_platform, int32 private_slot_count, int32 public_slot_count, const s_online_context* creation_contexts, int32 context_count)
{
	return INVOKE(0x00481270, managed_session_create_host, session_class, creation_flags, secure_key_platform, private_slot_count, public_slot_count, creation_contexts, context_count);
}

void __cdecl managed_session_create_host_migration_internal(int32 managed_session_index, s_online_managed_session* managed_session)
{
	INVOKE(0x00481370, managed_session_create_host_migration_internal, managed_session_index, managed_session);
}

void __cdecl managed_session_create_session_internal(int32 managed_session_index, s_online_managed_session* managed_session)
{
	INVOKE(0x00481550, managed_session_create_session_internal, managed_session_index, managed_session);
}

void __cdecl managed_session_creation_complete(int32 managed_session_index, bool success, uns32 return_result)
{
	INVOKE(0x00481930, managed_session_creation_complete, managed_session_index, success, return_result);
}

void __cdecl managed_session_delete(int32 managed_session_index)
{
	INVOKE(0x00481A80, managed_session_delete, managed_session_index);
}

void __cdecl managed_session_delete_host_migration_internal(int32 managed_session_index, s_online_managed_session* managed_session)
{
	INVOKE(0x00481B10, managed_session_delete_host_migration_internal, managed_session_index, managed_session);
}

void __cdecl managed_session_delete_session_internal(int32 managed_session_index, s_online_managed_session* managed_session)
{
	INVOKE(0x00481C10, managed_session_delete_session_internal, managed_session_index, managed_session);
}

void __cdecl managed_session_deletion_complete(int32 managed_session_index, bool success, uns32 return_result)
{
	INVOKE(0x00481D30, managed_session_deletion_complete, managed_session_index, success, return_result);
}

void __cdecl managed_session_free(int32 managed_session_index)
{
	INVOKE(0x00481DE0, managed_session_free, managed_session_index);
}

void __cdecl managed_session_game_end(int32 managed_session_index)
{
	INVOKE(0x00481E10, managed_session_game_end, managed_session_index);
}

void __cdecl managed_session_game_end_complete(int32 managed_session_index, bool success, uns32 return_result)
{
	INVOKE(0x00481EB0, managed_session_game_end_complete, managed_session_index, success, return_result);
}

void __cdecl managed_session_game_start(int32 managed_session_index)
{
	//INVOKE(0x00481F10, managed_session_game_start, managed_session_index);

	c_managed_session_status_flags session_status = managed_session_get_status(managed_session_index);
	if (session_status.test(_managed_session_game_started_bit) &&
		!session_status.test(_managed_session_game_ended_bit) &&
		!session_status.test(_managed_session_game_end_in_progress_bit))
	{
		event(_event_warning, "networking:managed_session: [0x%08X] calling game start, and game end has not been called? ending",
			managed_session_index);

		managed_session_game_end(managed_session_index);
	}

	s_online_managed_session* managed_session = managed_session_get(managed_session_index);
	managed_session->pending_operation_flags.set(_online_managed_session_game_start_bit, true);
	managed_session->flags.set(_online_managed_session_pending_retry_bit, false);
	managed_session->flags.set(_online_managed_session_game_started_bit, false);
	managed_session->flags.set(_online_managed_session_game_start_failed_bit, false);
	managed_session->time_of_last_failure = 0;
}

void __cdecl managed_session_game_start_complete(int32 managed_session_index, bool success, uns32 return_result)
{
	//INVOKE(0x00481F80, managed_session_game_start_complete, managed_session_index, success, return_result);

	c_console::write_line("donkey:matchmaking managed_session_game_start_complete for session %d success? %s", managed_session_index, success ? "YES" : "NO");
	HOOK_INVOKE(, managed_session_game_start_complete, managed_session_index, success, return_result);
}

s_online_managed_session* __cdecl managed_session_get(int32 managed_session_index)
{
	return INVOKE(0x00481FE0, managed_session_get, managed_session_index);
}

bool __cdecl managed_session_get_handle(int32 managed_session_index, void** session_handle_out)
{
	return INVOKE(0x00482000, managed_session_get_handle, managed_session_index, session_handle_out);
}

bool __cdecl managed_session_get_id(int32 managed_session_index, s_transport_secure_identifier* id)
{
	return INVOKE(0x00482040, managed_session_get_id, managed_session_index, id);
}

const char* __cdecl managed_session_get_id_string(int32 managed_session_index)
{
	//INVOKE(0x004820A0, managed_session_get_id_string, managed_session_index);

	if (managed_session_index == NONE)
		return "00:00:00:00:00:00:00:00";
	else
		return transport_secure_identifier_get_string(&online_session_manager_globals.managed_sessions[managed_session_index].actual_online_session_state.session_description.id);
}

void __cdecl managed_session_get_new_host_information(int32 managed_session_index, s_transport_session_description* session_description, e_transport_platform* secure_key_platform)
{
	INVOKE(0x004820D0, managed_session_get_new_host_information, managed_session_index, session_description, secure_key_platform);
}

bool __cdecl managed_session_get_nonce(int32 managed_session_index, uns64* session_nonce_out)
{
	return INVOKE(0x00482140, managed_session_get_nonce, managed_session_index, session_nonce_out);
}

bool __cdecl managed_session_get_security_information(int32 managed_session_index, bool host_migrating, s_transport_session_description* session_description, e_transport_platform* platform)
{
	return INVOKE(0x00482170, managed_session_get_security_information, managed_session_index, host_migrating, session_description, platform);
}

c_managed_session_status_flags __cdecl managed_session_get_status(int32 managed_session_index)
{
	return INVOKE(0x00482280, managed_session_get_status, managed_session_index);
}

bool __cdecl managed_session_handle_valid(void* session_handle)
{
	return INVOKE(0x004823C0, managed_session_handle_valid, session_handle);
}

void __cdecl managed_session_host_migration_complete(int32 managed_session_index, bool success, uns32 return_result)
{
	INVOKE(0x00482410, managed_session_host_migration_complete, managed_session_index, success, return_result);
}

bool __cdecl managed_session_is_host(int32 managed_session_index)
{
	return INVOKE(0x00482500, managed_session_is_host, managed_session_index);
}

bool __cdecl managed_session_is_live(int32 managed_session_index)
{
	return INVOKE(0x00482530, managed_session_is_live, managed_session_index);
}

bool __cdecl managed_session_is_master_session(int32 managed_session_index)
{
	return INVOKE(0x00482560, managed_session_is_master_session, managed_session_index);
}

void __cdecl managed_session_mark_session_to_be_created_offline(int32 managed_session_index)
{
	INVOKE(0x00482590, managed_session_mark_session_to_be_created_offline, managed_session_index);
}

void __cdecl managed_session_modify_complete(int32 managed_session_index, bool success, uns32 return_result)
{
	INVOKE(0x004825B0, managed_session_modify_complete, managed_session_index, success, return_result);
}

void __cdecl managed_session_modify_slot_counts(int32 managed_session_index, int32 private_slot_count, int32 public_slot_count, bool friends_only, bool slots_locked)
{
	INVOKE(0x00482650, managed_session_modify_slot_counts, managed_session_index, private_slot_count, public_slot_count, friends_only, slots_locked);
}

void __cdecl managed_session_players_add_complete(int32 managed_session_index, bool success, uns32 return_result)
{
	INVOKE(0x00482750, managed_session_players_add_complete, managed_session_index, success, return_result);
}

void __cdecl managed_session_players_remove_complete(int32 managed_session_index, bool success, uns32 return_result)
{
	INVOKE(0x004827F0, managed_session_players_remove_complete, managed_session_index, success, return_result);
}

void __cdecl managed_session_process_pending_operations(int32 managed_session_index)
{
	INVOKE(0x00482880, managed_session_process_pending_operations, managed_session_index);
}

void __cdecl managed_session_remove_players(int32 managed_session_index, const uns64* xuids, int32 xuid_count)
{
	INVOKE(0x00482D60, managed_session_remove_players, managed_session_index, xuids, xuid_count);
}

void __cdecl managed_session_reset_players_add_status(int32 managed_session_index)
{
	INVOKE(0x00482DB0, managed_session_reset_players_add_status, managed_session_index);
}

void __cdecl managed_session_reset_session(int32 managed_session_index, bool a2)
{
	INVOKE(0x00482DD0, managed_session_reset_session, managed_session_index, a2);
}

int32 __cdecl managed_session_retrieve_security_information(const transport_address* usable_address, e_transport_platform platform, s_transport_session_description* session_description, s_transport_secure_address* secure_address)
{
	return INVOKE(0x00482E70, managed_session_retrieve_security_information, usable_address, platform, session_description, secure_address);
}

void __cdecl managed_session_start_host_migration(int32 managed_session_index)
{
	INVOKE(0x00482FC0, managed_session_start_host_migration, managed_session_index);
}

void __cdecl managed_session_successful_creation_complete(int32 managed_session_index)
{
	INVOKE(0x00483040, managed_session_successful_creation_complete, managed_session_index);
}

void __cdecl managed_session_successful_delete_complete(int32 managed_session_index)
{
	INVOKE(0x004831F0, managed_session_successful_delete_complete, managed_session_index);
}

void __cdecl managed_session_successful_delete_host_migration_complete(int32 managed_session_index)
{
	INVOKE(0x00483290, managed_session_successful_delete_host_migration_complete, managed_session_index);
}

void __cdecl managed_session_successful_game_end_complete(int32 managed_session_index)
{
	INVOKE(0x004832D0, managed_session_successful_game_end_complete, managed_session_index);
}

void __cdecl managed_session_successful_game_start_complete(int32 managed_session_index)
{
	INVOKE(0x00483300, managed_session_successful_game_start_complete, managed_session_index);
}

void __cdecl managed_session_successful_host_migration_creation_complete(int32 managed_session_index)
{
	INVOKE(0x00483330, managed_session_successful_host_migration_creation_complete, managed_session_index);
}

void __cdecl managed_session_successful_modify_complete(int32 managed_session_index)
{
	INVOKE(0x004833F0, managed_session_successful_modify_complete, managed_session_index);
}

void __cdecl managed_session_successful_players_add_complete(int32 managed_session_index)
{
	INVOKE(0x00483440, managed_session_successful_players_add_complete, managed_session_index);
}

void __cdecl managed_session_successful_players_remove_complete(int32 managed_session_index)
{
	INVOKE(0x004834D0, managed_session_successful_players_remove_complete, managed_session_index);
}

bool __cdecl managed_session_synchronize_to_player_list(int32 managed_session_index, c_network_session_membership* session_membership)
{
	return INVOKE(0x00483540, managed_session_synchronize_to_player_list, managed_session_index, session_membership);
}

void __cdecl managed_session_use_new_host(int32 managed_session_index)
{
	INVOKE(0x004837D0, managed_session_use_new_host, managed_session_index);
}

int32 __cdecl managed_session_who_has_this_session_id(const s_transport_secure_identifier* session_id)
{
	return INVOKE(0x004839C0, managed_session_who_has_this_session_id, session_id);
}

void __cdecl online_session_manager_dispose()
{
	INVOKE(0x00483AF0, online_session_manager_dispose);
}

void __cdecl online_session_manager_initialize()
{
	INVOKE(0x00483B10, online_session_manager_initialize);
}

void __cdecl online_session_manager_update()
{
	INVOKE(0x00483B20, online_session_manager_update);
}

void __cdecl remove_from_player_list(s_online_session_player* players, int32 player_count, const uns64* xuids, int32 xuid_count)
{
	INVOKE(0x00483B50, remove_from_player_list, players, player_count, xuids, xuid_count);
}

void c_managed_session_overlapped_task::success(uns32 return_result)
{
	INVOKE_CLASS_MEMBER(0x00483CB0, c_managed_session_overlapped_task, success, return_result);
}

