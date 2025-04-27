#include "networking/session/network_managed_session.hpp"

#include "memory/module.hpp"
#include "networking/session/network_session_membership.hpp"

REFERENCE_DECLARE(0x02247448, s_online_session_manager_globals, online_session_manager_globals);

//HOOK_DECLARE_CLASS_MEMBER(0x00480420, c_managed_session_overlapped_task, complete_);
//HOOK_DECLARE_CLASS_MEMBER(0x00480440, c_managed_session_overlapped_task, failure_);
//HOOK_DECLARE_CLASS_MEMBER(0x00483CB0, c_managed_session_overlapped_task, success_);

//.text:00480200 ; c_managed_session_overlapped_task* __cdecl c_managed_session_overlapped_task::c_managed_session_overlapped_task(char const*, long);
//.text:00480220 ; c_managed_session_overlapped_task* __cdecl c_managed_session_overlapped_task::c_managed_session_overlapped_task();
// 
//.text:00480240 ; s_online_managed_session* __cdecl s_online_managed_session::s_online_managed_session();
//.text:004802A0 ; s_online_managed_session* __cdecl s_online_managed_session::~s_online_managed_session();

void __cdecl add_to_player_list(s_online_session_player* players, long player_count, uint64 const* xuids, bool const* xuids_left_game, long xuid_count)
{
	INVOKE(0x00480350, add_to_player_list, players, player_count, xuids, xuids_left_game, xuid_count);
}

//.text:00480420 ; virtual void __cdecl c_managed_session_overlapped_task::complete();
void __thiscall c_managed_session_overlapped_task::complete_()
{
	m_completion_routine(m_managed_session_index, m_result, m_return_result);
}

//.text:00480440 ; virtual void __cdecl c_managed_session_overlapped_task::failure(uint32, uint32, uint32);
void __thiscall c_managed_session_overlapped_task::failure_(uint32 calling_result, uint32 overlapped_error, uint32 overlapped_extended_error)
{
	m_return_result = overlapped_extended_error;
	m_result = false;
}

//.text:004804B0 ; virtual char const* __cdecl c_managed_session_overlapped_task::get_context_string() const;
//.text:00480510 ; e_controller_index __cdecl get_first_signed_in_controller_index()
//.text:00480590 ; public: bool __cdecl c_controller_interface::in_use() const

void __cdecl managed_session_add_players(long index, uint64 const* xuids, bool const* xuids_left_game, long xuid_count)
{
	INVOKE(0x00480630, managed_session_add_players, index, xuids, xuids_left_game, xuid_count);
}

long __cdecl managed_session_allocate()
{
	return INVOKE(0x00480680, managed_session_allocate);
}

bool __cdecl managed_session_build_add_xuid_list(long index)
{
	return INVOKE(0x00480790, managed_session_build_add_xuid_list, index);
}

bool __cdecl managed_session_build_removal_xuid_list(long index)
{
	return INVOKE(0x00480840, managed_session_build_removal_xuid_list, index);
}

bool __cdecl managed_session_can_create_session(e_network_session_class session_class)
{
	return INVOKE(0x00480900, managed_session_can_create_session, session_class);
}

void __cdecl managed_session_cancel_new_host(long index)
{
	INVOKE(0x00480950, managed_session_cancel_new_host, index);
}

void __cdecl managed_session_commit_new_host(long index)
{
	INVOKE(0x00480D10, managed_session_commit_new_host, index);
}

bool __cdecl managed_session_compare_id(long index, s_transport_secure_identifier const* session_id)
{
	return INVOKE(0x00480DD0, managed_session_compare_id, index, session_id);
}

void __cdecl managed_session_completely_reset_session_on_error(long index)
{
	INVOKE(0x00480E60, managed_session_completely_reset_session_on_error, index);
}

void __cdecl managed_session_connect_client_to_new_host(long index, s_transport_session_description const* session_description, e_transport_platform platform)
{
	INVOKE(0x00480EF0, managed_session_connect_client_to_new_host, index, session_description, platform);
}

long __cdecl managed_session_count_master_sessions()
{
	return INVOKE(0x00481070, managed_session_count_master_sessions);
}

//long __cdecl managed_session_create_client(e_network_session_class session_class, c_flags<e_managed_session_creation_flags, uint8, 4> creation_flags, e_transport_platform platform, long private_slot_count, long public_slot_count, s_transport_secure_identifier const* session_id, s_transport_secure_address const* host_address, s_transport_secure_key const* key, s_online_context const* creation_contexts, long context_count)
long __cdecl managed_session_create_client(e_network_session_class session_class, uint8 creation_flags, e_transport_platform platform, long private_slot_count, long public_slot_count, s_transport_secure_identifier const* session_id, s_transport_secure_address const* host_address, s_transport_secure_key const* key, s_online_context const* creation_contexts, long creation_context_count)
{
	return INVOKE(0x00481110, managed_session_create_client, session_class, creation_flags, platform, private_slot_count, public_slot_count, session_id, host_address, key, creation_contexts, creation_context_count);
}

//long __cdecl managed_session_create_host(e_network_session_class session_class, c_flags<e_managed_session_creation_flags, uint8, 4> creation_flags, e_transport_platform platform, long private_slot_count, long public_slot_count, s_online_context const* creation_contexts, long creation_context_count)
long __cdecl managed_session_create_host(e_network_session_class session_class, uint8 creation_flags, e_transport_platform platform, long private_slot_count, long public_slot_count, s_online_context const* creation_contexts, long creation_context_count)
{
	return INVOKE(0x00481270, managed_session_create_host, session_class, creation_flags, platform, private_slot_count, public_slot_count, creation_contexts, creation_context_count);
}

void __cdecl managed_session_create_host_migration_internal(long index, s_online_managed_session* managed_session)
{
	INVOKE(0x00481370, managed_session_create_host_migration_internal, index, managed_session);
}

void __cdecl managed_session_create_session_internal(long index, s_online_managed_session* managed_session)
{
	INVOKE(0x00481550, managed_session_create_session_internal, index, managed_session);
}

void __cdecl managed_session_creation_complete(long index, bool succeeded, uint32 a3)
{
	INVOKE(0x00481930, managed_session_creation_complete, index, succeeded, a3);
}

void __cdecl managed_session_delete(long index)
{
	INVOKE(0x00481A80, managed_session_delete, index);
}

void __cdecl managed_session_delete_host_migration_internal(long index, s_online_managed_session* managed_session)
{
	INVOKE(0x00481B10, managed_session_delete_host_migration_internal, index, managed_session);
}

void __cdecl managed_session_delete_session_internal(long index, s_online_managed_session* managed_session)
{
	INVOKE(0x00481C10, managed_session_delete_session_internal, index, managed_session);
}

void __cdecl managed_session_deletion_complete(long index, bool succeeded, uint32 a3)
{
	INVOKE(0x00481D30, managed_session_deletion_complete, index, succeeded, a3);
}

void __cdecl managed_session_free(long index)
{
	INVOKE(0x00481DE0, managed_session_free, index);
}

void __cdecl managed_session_game_end(long index)
{
	INVOKE(0x00481E10, managed_session_game_end, index);
}

void __cdecl managed_session_game_end_complete(long index, bool succeeded, uint32 a3)
{
	INVOKE(0x00481EB0, managed_session_game_end_complete, index, succeeded, a3);
}

void __cdecl managed_session_game_start(long index)
{
	INVOKE(0x00481F10, managed_session_game_start, index);
}

void __cdecl managed_session_game_start_complete(long index, bool succeeded, uint32 a3)
{
	INVOKE(0x00481F80, managed_session_game_start_complete, index, succeeded, a3);
}

s_online_managed_session* __cdecl managed_session_get(long index)
{
	return INVOKE(0x00481FE0, managed_session_get, index);
}

bool __cdecl managed_session_get_handle(long index, void** handle_out)
{
	return INVOKE(0x00482000, managed_session_get_handle, index, handle_out);
}

bool __cdecl managed_session_get_id(long index, s_transport_secure_identifier* session_id)
{
	return INVOKE(0x00482040, managed_session_get_id, index, session_id);
}

char const* __cdecl managed_session_get_id_string(long index)
{
	//INVOKE(0x004820A0, managed_session_get_id_string, index);

	if (index == NONE)
		return "00:00:00:00:00:00:00:00";
	else
		return transport_secure_identifier_get_string(&online_session_manager_globals.managed_sessions[index].actual_online_session_state.session_description.id);
}

void __cdecl managed_session_get_new_host_information(long index, s_transport_session_description* session_description, e_transport_platform* platform)
{
	INVOKE(0x004820D0, managed_session_get_new_host_information, index, session_description, platform);
}

bool __cdecl managed_session_get_nonce(long index, uint64* nonce)
{
	return INVOKE(0x00482140, managed_session_get_nonce, index, nonce);
}

bool __cdecl managed_session_get_security_information(long index, bool host_migrating, s_transport_session_description* session_description, e_transport_platform* platform)
{
	return INVOKE(0x00482170, managed_session_get_security_information, index, host_migrating, session_description, platform);
}

//c_flags<e_managed_session_status_flags, uint16, 14> __cdecl managed_session_get_status(long index)
uint16 __cdecl managed_session_get_status(long index)
{
	return INVOKE(0x00482280, managed_session_get_status, index);
}

bool __cdecl managed_session_handle_valid(void* handle)
{
	return INVOKE(0x004823C0, managed_session_handle_valid, handle);
}

void __cdecl managed_session_host_migration_complete(long index, bool succeeded, uint32 a3)
{
	INVOKE(0x00482410, managed_session_host_migration_complete, index, succeeded, a3);
}

bool __cdecl managed_session_is_host(long index)
{
	return INVOKE(0x00482500, managed_session_is_host, index);
}

bool __cdecl managed_session_is_live(long index)
{
	return INVOKE(0x00482530, managed_session_is_live, index);
}

bool __cdecl managed_session_is_master_session(long index)
{
	return INVOKE(0x00482560, managed_session_is_master_session, index);
}

void __cdecl managed_session_mark_session_to_be_created_offline(long index)
{
	INVOKE(0x00482590, managed_session_mark_session_to_be_created_offline, index);
}

void __cdecl managed_session_modify_complete(long index, bool succeeded, uint32 a3)
{
	INVOKE(0x004825B0, managed_session_modify_complete, index, succeeded, a3);
}

void __cdecl managed_session_modify_slot_counts(long index, long private_slot_count, long public_slot_count, bool friends_only, bool slots_locked)
{
	INVOKE(0x00482650, managed_session_modify_slot_counts, index, private_slot_count, public_slot_count, friends_only, slots_locked);
}

void __cdecl managed_session_players_add_complete(long index, bool succeeded, uint32 a3)
{
	INVOKE(0x00482750, managed_session_players_add_complete, index, succeeded, a3);
}

void __cdecl managed_session_players_remove_complete(long index, bool succeeded, uint32 a3)
{
	INVOKE(0x004827F0, managed_session_players_remove_complete, index, succeeded, a3);
}

void __cdecl managed_session_process_pending_operations(long index)
{
	INVOKE(0x00482880, managed_session_process_pending_operations, index);
}

void __cdecl managed_session_remove_players(long index, uint64 const* xuids, long xuid_count)
{
	INVOKE(0x00482D60, managed_session_remove_players, index, xuids, xuid_count);
}

void __cdecl managed_session_reset_players_add_status(long index)
{
	INVOKE(0x00482DB0, managed_session_reset_players_add_status, index);
}

void __cdecl managed_session_reset_session(long index, bool a2)
{
	INVOKE(0x00482DD0, managed_session_reset_session, index, a2);
}

long __cdecl managed_session_retrieve_security_information(transport_address const* usable_address, e_transport_platform platform, s_transport_session_description* session_description, s_transport_secure_address* secure_address)
{
	return INVOKE(0x00482E70, managed_session_retrieve_security_information, usable_address, platform, session_description, secure_address);
}

void __cdecl managed_session_start_host_migration(long index)
{
	INVOKE(0x00482FC0, managed_session_start_host_migration, index);
}

void __cdecl managed_session_successful_creation_complete(long index)
{
	INVOKE(0x00483040, managed_session_successful_creation_complete, index);
}

void __cdecl managed_session_successful_delete_complete(long index)
{
	INVOKE(0x004831F0, managed_session_successful_delete_complete, index);
}

void __cdecl managed_session_successful_delete_host_migration_complete(long index)
{
	INVOKE(0x00483290, managed_session_successful_delete_host_migration_complete, index);
}

void __cdecl managed_session_successful_game_end_complete(long index)
{
	INVOKE(0x004832D0, managed_session_successful_game_end_complete, index);
}

void __cdecl managed_session_successful_game_start_complete(long index)
{
	INVOKE(0x00483300, managed_session_successful_game_start_complete, index);
}

void __cdecl managed_session_successful_host_migration_creation_complete(long index)
{
	INVOKE(0x00483330, managed_session_successful_host_migration_creation_complete, index);
}

void __cdecl managed_session_successful_modify_complete(long index)
{
	INVOKE(0x004833F0, managed_session_successful_modify_complete, index);
}

void __cdecl managed_session_successful_players_add_complete(long index)
{
	INVOKE(0x00483440, managed_session_successful_players_add_complete, index);
}

void __cdecl managed_session_successful_players_remove_complete(long index)
{
	INVOKE(0x004834D0, managed_session_successful_players_remove_complete, index);
}

bool __cdecl managed_session_synchronize_to_player_list(long index, c_network_session_membership* session_membership)
{
	return INVOKE(0x00483540, managed_session_synchronize_to_player_list, index, session_membership);
}

void __cdecl managed_session_use_new_host(long index)
{
	INVOKE(0x004837D0, managed_session_use_new_host, index);
}

long __cdecl managed_session_who_has_this_session_id(s_transport_secure_identifier const* session_id)
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

void __cdecl remove_from_player_list(s_online_session_player* players, long player_count, uint64 const* xuids, long xuid_count)
{
	INVOKE(0x00483B50, remove_from_player_list, players, player_count, xuids, xuid_count);
}

//.text:00483CB0 ; virtual void __cdecl c_managed_session_overlapped_task::success(uint32)
void __thiscall c_managed_session_overlapped_task::success_(uint32 return_result)
{
	m_return_result = return_result;
	m_result = true;
}

