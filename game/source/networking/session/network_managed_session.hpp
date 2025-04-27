#pragma once

#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries.hpp"
#include "networking/online/online_session.hpp"
#include "networking/transport/transport_security.hpp"

struct s_online_session;
struct s_transport_secure_identifier;
struct c_network_session_membership;

struct c_managed_session_overlapped_task :
	public c_overlapped_task
{
	void __thiscall process_add_players(long managed_session_index, void(__cdecl* callback)(long, bool, uint32), s_online_session* session, uint64 const* a4, bool const* a5, bool const* a6, long a7);
	void __thiscall process_create(long managed_session_index, void(__cdecl* callback)(long, bool, uint32), s_online_session* session, uint16 flags);
	void __thiscall process_delete(long managed_session_index, void(__cdecl* callback)(long, bool, uint32), s_online_session* session);
	void __thiscall process_game_end(long managed_session_index, void(__cdecl* callback)(long, bool, uint32), s_online_session* session);
	void __thiscall process_modify(long managed_session_index, void(__cdecl* callback)(long, bool, uint32), s_online_session* session, s_online_session* desired_session, s_online_session* actual_session);
	void __thiscall process_game_start(long managed_session_index, void(__cdecl* callback)(long, bool, uint32), s_online_session* session);
	void __thiscall process_remove_players(long managed_session_index, void(__cdecl* callback)(long, bool, uint32), s_online_session* session, uint64 const* a4, bool const* a5, long player_count);
	void __thiscall process_session_host_migrate(long managed_session_index, void(__cdecl* callback)(long, bool, uint32), s_online_session* session, bool is_host, s_transport_session_description* host_migration_description);
	uint32 __thiscall start_(void* overlapped);

	void __thiscall complete_();
	void __thiscall failure_(uint32 calling_result, uint32 overlapped_error, uint32 overlapped_extended_error);
	void __thiscall success_(uint32 return_result);

	enum e_overlapped_task
	{
		_process_create = 0,
		_process_delete,
		_process_session_host_migrate,
		_process_modify,
		_process_add_players,
		_process_remove_players,
		_process_start,
		_process_end
	};

	e_overlapped_task m_context;
	s_online_session* m_session;
	s_online_session* m_desired_session;
	s_online_session* m_actual_session;
	long m_managed_session_index;
	void(__cdecl* m_completion_routine)(long, bool, uint32);
	bool m_result;
	uint32 m_return_result;
	uint16 m_mask;
	bool m_is_host;
	s_transport_session_description* m_host_migration_description;
	long m_player_count;
	uint64 m_player_xuids[16];
	uint32 m_private_slots[16];
};
static_assert(sizeof(c_managed_session_overlapped_task) == 0x100);

struct s_online_context
{
	long id;
	long value;
};
static_assert(sizeof(s_online_context) == 0x8);

struct s_online_managed_session
{
	long session_class;
	long secure_key_platform;
	c_flags<e_online_managed_session_flags, uint32, k_online_managed_session_flags_count> flags;
	c_flags<e_online_managed_session_operation_flags, uint16, k_online_managed_session_operation_flags_count> current_operation_flags;
	c_flags<e_online_managed_session_operation_flags, uint16, k_online_managed_session_operation_flags_count> pending_operation_flags;
	c_managed_session_overlapped_task overlapped_task;
	uint32 time_of_last_failure;
	bool not_connected_to_live;
	bool has_no_valid_address;
	bool has_conflicting_session_id;
	s_online_session desired_online_session_state;
	s_online_session transitory_online_session_state;
	s_online_session actual_online_session_state;
	uint64 xuids[16];
	uint8 xuid_flags[16];
	uint8 xuid_next_flags[16];
	long session_player_operation_count;
	bool we_are_the_new_host;
	bool we_were_the_host;

#pragma pack(push,1)
	s_transport_session_description host_migration_description;
#pragma pack(pop)

	s_online_context creation_contexts[4];
	long context_count;
};
static_assert(sizeof(s_online_managed_session) == 0x608);
static_assert(0x000 == offsetof(s_online_managed_session, session_class));
static_assert(0x004 == offsetof(s_online_managed_session, secure_key_platform));
static_assert(0x008 == offsetof(s_online_managed_session, flags));
static_assert(0x00C == offsetof(s_online_managed_session, current_operation_flags));
static_assert(0x00E == offsetof(s_online_managed_session, pending_operation_flags));
static_assert(0x010 == offsetof(s_online_managed_session, overlapped_task));
static_assert(0x110 == offsetof(s_online_managed_session, time_of_last_failure));
static_assert(0x114 == offsetof(s_online_managed_session, not_connected_to_live));
static_assert(0x115 == offsetof(s_online_managed_session, has_no_valid_address));
static_assert(0x116 == offsetof(s_online_managed_session, has_conflicting_session_id));
static_assert(0x118 == offsetof(s_online_managed_session, desired_online_session_state));
static_assert(0x268 == offsetof(s_online_managed_session, transitory_online_session_state));
static_assert(0x3B8 == offsetof(s_online_managed_session, actual_online_session_state));
static_assert(0x508 == offsetof(s_online_managed_session, xuids));
static_assert(0x588 == offsetof(s_online_managed_session, xuid_flags));
static_assert(0x598 == offsetof(s_online_managed_session, xuid_next_flags));
static_assert(0x5A8 == offsetof(s_online_managed_session, session_player_operation_count));
static_assert(0x5AC == offsetof(s_online_managed_session, we_are_the_new_host));
static_assert(0x5AD == offsetof(s_online_managed_session, we_were_the_host));
static_assert(0x5AE == offsetof(s_online_managed_session, host_migration_description));
static_assert(0x5E0 == offsetof(s_online_managed_session, creation_contexts));
static_assert(0x600 == offsetof(s_online_managed_session, context_count));

struct s_online_session_manager_globals
{
	c_flags<e_online_managed_session_operation_flags, uint16, k_online_managed_session_operation_flags_count> current_operation_flags;
	s_online_managed_session managed_sessions[8];
};
static_assert(sizeof(s_online_session_manager_globals) == 0x3048);
static_assert(0x0 == offsetof(s_online_session_manager_globals, current_operation_flags));
static_assert(0x8 == offsetof(s_online_session_manager_globals, managed_sessions));

extern s_online_session_manager_globals& online_session_manager_globals;

extern void __cdecl add_to_player_list(s_online_session_player* players, long player_count, uint64 const* xuids, bool const* xuids_left_game, long xuid_count);
extern void __cdecl managed_session_add_players(long index, uint64 const* xuids, bool const* xuids_left_game, long xuid_count);
extern long __cdecl managed_session_allocate();
extern bool __cdecl managed_session_build_add_xuid_list(long index);
extern bool __cdecl managed_session_build_removal_xuid_list(long index);
extern bool __cdecl managed_session_can_create_session(e_network_session_class session_class);
extern void __cdecl managed_session_cancel_new_host(long index);
extern void __cdecl managed_session_commit_new_host(long index);
extern bool __cdecl managed_session_compare_id(long index, s_transport_secure_identifier const* session_id);
extern void __cdecl managed_session_completely_reset_session_on_error(long index);
extern void __cdecl managed_session_connect_client_to_new_host(long index, s_transport_session_description const* session_description, e_transport_platform platform);
extern long __cdecl managed_session_count_master_sessions();
extern long __cdecl managed_session_create_client(e_network_session_class session_class, uint8 creation_flags, e_transport_platform platform, long private_slot_count, long public_slot_count, s_transport_secure_identifier const* session_id, s_transport_secure_address const* host_address, s_transport_secure_key const* key, s_online_context const* creation_contexts, long creation_context_count);
extern long __cdecl managed_session_create_host(e_network_session_class session_class, uint8 creation_flags, e_transport_platform platform, long private_slot_count, long public_slot_count, s_online_context const* creation_contexts, long creation_context_count);
extern void __cdecl managed_session_create_host_migration_internal(long index, s_online_managed_session* managed_session);
extern void __cdecl managed_session_create_session_internal(long index, s_online_managed_session* managed_session);
extern void __cdecl managed_session_creation_complete(long index, bool succeeded, uint32 a3);
extern void __cdecl managed_session_delete(long index);
extern void __cdecl managed_session_delete_host_migration_internal(long index, s_online_managed_session* managed_session);
extern void __cdecl managed_session_delete_session_internal(long index, s_online_managed_session* managed_session);
extern void __cdecl managed_session_deletion_complete(long index, bool succeeded, uint32 a3);
extern void __cdecl managed_session_free(long index);
extern void __cdecl managed_session_game_end(long index);
extern void __cdecl managed_session_game_end_complete(long index, bool succeeded, uint32 a3);
extern void __cdecl managed_session_game_start(long index);
extern void __cdecl managed_session_game_start_complete(long index, bool succeeded, uint32 a3);
extern s_online_managed_session* __cdecl managed_session_get(long index);
extern bool __cdecl managed_session_get_handle(long index, void** handle_out);
extern bool __cdecl managed_session_get_id(long index, s_transport_secure_identifier* session_id);
extern char const* __cdecl managed_session_get_id_string(long managed_session_index);
extern void __cdecl managed_session_get_new_host_information(long index, s_transport_session_description* session_description, e_transport_platform* platform);
extern bool __cdecl managed_session_get_nonce(long index, uint64* nonce);
extern bool __cdecl managed_session_get_security_information(long index, bool, s_transport_session_description* session_description, e_transport_platform* platform);
extern uint16 __cdecl managed_session_get_status(long index);
extern bool __cdecl managed_session_handle_valid(void* handle);
extern void __cdecl managed_session_host_migration_complete(long index, bool succeeded, uint32 a3);
extern bool __cdecl managed_session_is_host(long index);
extern bool __cdecl managed_session_is_live(long index);
extern bool __cdecl managed_session_is_master_session(long index);
extern void __cdecl managed_session_mark_session_to_be_created_offline(long index);
extern void __cdecl managed_session_modify_complete(long index, bool succeeded, uint32 a3);
extern void __cdecl managed_session_modify_slot_counts(long index, long private_slot_count, long public_slot_count, bool friends_only, bool slots_locked);
extern void __cdecl managed_session_players_add_complete(long index, bool succeeded, uint32 a3);
extern void __cdecl managed_session_players_remove_complete(long index, bool succeeded, uint32 a3);
extern void __cdecl managed_session_process_pending_operations(long index);
extern void __cdecl managed_session_remove_players(long index, uint64 const* xuids, long xuid_count);
extern void __cdecl managed_session_reset_players_add_status(long index);
extern void __cdecl managed_session_reset_session(long index, bool a2);
extern long __cdecl managed_session_retrieve_security_information(transport_address const* usable_address, e_transport_platform platform, s_transport_session_description* session_description, s_transport_secure_address* secure_address);
extern void __cdecl managed_session_start_host_migration(long index);
extern void __cdecl managed_session_successful_creation_complete(long index);
extern void __cdecl managed_session_successful_delete_complete(long index);
extern void __cdecl managed_session_successful_delete_host_migration_complete(long index);
extern void __cdecl managed_session_successful_game_end_complete(long index);
extern void __cdecl managed_session_successful_game_start_complete(long index);
extern void __cdecl managed_session_successful_host_migration_creation_complete(long index);
extern void __cdecl managed_session_successful_modify_complete(long index);
extern void __cdecl managed_session_successful_players_add_complete(long index);
extern void __cdecl managed_session_successful_players_remove_complete(long index);
extern bool __cdecl managed_session_synchronize_to_player_list(long index, c_network_session_membership* session_membership);
extern void __cdecl managed_session_use_new_host(long index);
extern long __cdecl managed_session_who_has_this_session_id(s_transport_secure_identifier const* session_id);
extern void __cdecl online_session_manager_dispose();
extern void __cdecl online_session_manager_initialize();
extern void __cdecl online_session_manager_update();
extern void __cdecl remove_from_player_list(s_online_session_player* players, long player_count, uint64 const* xuids, long xuid_count);

