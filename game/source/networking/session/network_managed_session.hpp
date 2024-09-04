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
	void __thiscall success_(dword a1);

	c_enum<e_session_overlapped_task_context, long, _session_overlapped_task_context_create, k_session_overlapped_task_context_count> m_context;

	s_online_session* m_session;
	s_online_session* m_desired_session;
	s_online_session* m_actual_session;
	long m_managed_session_index;

	void(__cdecl* m_callback)(long, bool, dword);
	bool m_callback_value0;
	dword m_callback_value1;

	word_flags m_online_session_flags;
	bool m_is_host;
	byte __unknown33;
	s_transport_session_description* m_host_migration_description;

	long m_player_count;
	qword m_player_xuids[16];
	dword_flags m_private_slots[16];
};
static_assert(sizeof(c_managed_session_overlapped_task) == 0x100);

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
#pragma pack(pop)

struct s_online_session_manager_globals
{
	word_flags current_operation_flags;
	byte __data2[0x6];
	c_static_array<s_online_managed_session, 8> managed_sessions;
};
static_assert(sizeof(s_online_session_manager_globals) == 0x3048);

extern s_online_session_manager_globals& online_session_manager_globals;

struct s_transport_secure_identifier;
extern bool __cdecl managed_session_get_id(long index, s_transport_secure_identifier* session_id);
extern char const* __cdecl managed_session_get_id_string(long managed_session_index);
extern void __cdecl online_session_manager_dispose();
extern void __cdecl online_session_manager_initialize();
extern void __cdecl online_session_manager_update();

