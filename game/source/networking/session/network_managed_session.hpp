#pragma once

#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries.hpp"
#include "networking/online/online_session.hpp"
#include "networking/transport/transport_security.hpp"

enum e_session_overlapped_task_context
{
	// XSessionCreate
	_session_overlapped_task_context_create = 0,

	// XSessionDelete
	_session_overlapped_task_context_delete,

	// XSessionMigrateHost
	_session_overlapped_task_context_migrate_host,

	// XSessionModify
	_session_overlapped_task_context_modify,

	// XSessionJoinRemote
	_session_overlapped_task_context_add_players,

	// XSessionLeaveRemote
	_session_overlapped_task_context_remove_players,

	// XSessionStart
	_session_overlapped_task_context_start,

	// XSessionEnd
	_session_overlapped_task_context_end,

	k_session_overlapped_task_context_count
};

struct s_online_session;
struct c_managed_session_overlapped_task :
	public c_overlapped_task
{
	void __thiscall process_add_players(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session, qword const* a4, bool const* a5, bool const* a6, long a7);
	void __thiscall process_create(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session, word_flags flags);
	void __thiscall process_delete(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session);
	void __thiscall process_game_end(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session);
	void __thiscall process_modify(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session, s_online_session* desired_session, s_online_session* actual_session);
	void __thiscall process_game_start(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session);
	void __thiscall process_remove_players(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session, qword const* a4, bool const* a5, long player_count);
	void __thiscall process_session_host_migrate(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session, bool is_host, s_transport_session_description* host_migration_description);
	dword __thiscall start_(void* overlapped);

	void __thiscall complete_();
	void __thiscall failure_(dword calling_result, dword overlapped_error, dword overlapped_extended_error);
	void __thiscall success_(dword return_result);

	c_enum<e_session_overlapped_task_context, long, _session_overlapped_task_context_create, k_session_overlapped_task_context_count> m_context;

	s_online_session* m_session;
	s_online_session* m_desired_session;
	s_online_session* m_actual_session;
	long m_managed_session_index;

	void(__cdecl* m_callback)(long, bool, dword);
	bool m_callback_succeeded;

	byte __pad29[0x3];

	dword m_callback_return_result;

	word_flags m_online_session_flags;
	bool m_is_host;
	byte __unknown33;
	s_transport_session_description* m_host_migration_description;

	long m_player_count;

	byte __pad3C[0x4];

	qword m_player_xuids[16];
	dword_flags m_private_slots[16];
};
static_assert(sizeof(c_managed_session_overlapped_task) == 0x100);
static_assert(0x29 == offsetof(c_managed_session_overlapped_task, __pad29));
static_assert(0x33 == offsetof(c_managed_session_overlapped_task, __unknown33));
static_assert(0x3C == offsetof(c_managed_session_overlapped_task, __pad3C));

struct s_online_context
{
	long id;
	long value;
};
static_assert(sizeof(s_online_context) == 0x8);

#pragma pack(push, 1)
struct s_online_managed_session
{
	// e_network_session_class
	long session_class;

	// e_transport_platform
	long platform;

	dword_flags flags;
	word_flags current_operation_flags;
	word_flags pending_operation_flags;

	c_managed_session_overlapped_task managed_session_overlapped_task;

	dword time;

	bool live_check;
	bool ip_check;
	bool conflicting_session_check;
	bool __unknown117;

	s_online_session desired_online_session_state;
	s_online_session transitory_online_session_state;
	s_online_session actual_online_session_state;

	c_static_array<qword, 16> xuids;
	c_static_array<byte, 16> xuid_flags;
	c_static_array<byte, 16> xuid_next_flags;
	long session_player_operation_count;

	bool is_host;
	bool __unknown5AD;
	s_transport_session_description host_migration_description;
	byte __data5DE[0x2];

	c_static_array<s_online_context, 4> creation_contexts;
	long context_count;

	byte __data604[0x4];
};
static_assert(sizeof(s_online_managed_session) == 0x608);
static_assert(0x117 == offsetof(s_online_managed_session, __unknown117));
static_assert(0x5AD == offsetof(s_online_managed_session, __unknown5AD));
static_assert(0x5DE == offsetof(s_online_managed_session, __data5DE));
static_assert(0x604 == offsetof(s_online_managed_session, __data604));
#pragma pack(pop)

struct s_online_session_manager_globals
{
	word_flags current_operation_flags;
	byte __data2[0x6];
	c_static_array<s_online_managed_session, 8> managed_sessions;
};
static_assert(sizeof(s_online_session_manager_globals) == 0x3048);

extern s_online_session_manager_globals& online_session_manager_globals;

enum e_network_session_class;
enum e_transport_platform;
struct s_transport_secure_identifier;
struct c_network_session_membership;

extern void __cdecl add_to_player_list(s_online_session_player* players, long player_count, qword const* xuids, bool const* xuids_left_game, long xuid_count);
extern void __cdecl managed_session_add_players(long index, qword const* xuids, bool const* xuids_left_game, long xuid_count);
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
extern long __cdecl managed_session_create_client(e_network_session_class session_class, byte creation_flags, e_transport_platform platform, long private_slot_count, long public_slot_count, s_transport_secure_identifier const* session_id, s_transport_secure_address const* host_address, s_transport_secure_key const* key, s_online_context const* creation_contexts, long creation_context_count);
extern long __cdecl managed_session_create_host(e_network_session_class session_class, byte creation_flags, e_transport_platform platform, long private_slot_count, long public_slot_count, s_online_context const* creation_contexts, long creation_context_count);
extern void __cdecl managed_session_create_host_migration_internal(long index, s_online_managed_session* managed_session);
extern void __cdecl managed_session_create_session_internal(long index, s_online_managed_session* managed_session);
extern void __cdecl managed_session_creation_complete(long index, bool succeeded, dword a3);
extern void __cdecl managed_session_delete(long index);
extern void __cdecl managed_session_delete_host_migration_internal(long index, s_online_managed_session* managed_session);
extern void __cdecl managed_session_delete_session_internal(long index, s_online_managed_session* managed_session);
extern void __cdecl managed_session_deletion_complete(long index, bool succeeded, dword a3);
extern void __cdecl managed_session_free(long index);
extern void __cdecl managed_session_game_end(long index);
extern void __cdecl managed_session_game_end_complete(long index, bool succeeded, dword a3);
extern void __cdecl managed_session_game_start(long index);
extern void __cdecl managed_session_game_start_complete(long index, bool succeeded, dword a3);
extern s_online_managed_session* __cdecl managed_session_get(long index);
extern bool __cdecl managed_session_get_handle(long index, void** handle_out);
extern bool __cdecl managed_session_get_id(long index, s_transport_secure_identifier* session_id);
extern char const* __cdecl managed_session_get_id_string(long managed_session_index);
extern void __cdecl managed_session_get_new_host_information(long index, s_transport_session_description* session_description, e_transport_platform* platform);
extern bool __cdecl managed_session_get_nonce(long index, qword* nonce);
extern bool __cdecl managed_session_get_security_information(long index, bool, s_transport_session_description* session_description, e_transport_platform* platform);
extern word __cdecl managed_session_get_status(long index);
extern bool __cdecl managed_session_handle_valid(void* handle);
extern void __cdecl managed_session_host_migration_complete(long index, bool succeeded, dword a3);
extern bool __cdecl managed_session_is_host(long index);
extern bool __cdecl managed_session_is_live(long index);
extern bool __cdecl managed_session_is_master_session(long index);
extern void __cdecl managed_session_mark_session_to_be_created_offline(long index);
extern void __cdecl managed_session_modify_complete(long index, bool succeeded, dword a3);
extern void __cdecl managed_session_modify_slot_counts(long index, long private_slot_count, long public_slot_count, bool friends_only, bool slots_locked);
extern void __cdecl managed_session_players_add_complete(long index, bool succeeded, dword a3);
extern void __cdecl managed_session_players_remove_complete(long index, bool succeeded, dword a3);
extern void __cdecl managed_session_process_pending_operations(long index);
extern void __cdecl managed_session_remove_players(long index, qword const* xuids, long xuid_count);
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
extern void __cdecl remove_from_player_list(s_online_session_player* players, long player_count, qword const* xuids, long xuid_count);

