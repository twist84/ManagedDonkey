#pragma once

#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries.hpp"
#include "networking/transport/transport_security.hpp"

struct c_managed_session_overlapped_task : c_overlapped_task
{
	dword context;
	byte __data14[0xEC];
};

struct s_online_session_player
{
	word_flags flags;
	qword xuid;
};
static_assert(sizeof(s_online_session_player) == 0x10);

struct s_online_session
{
	long controller_index;
	word_flags public_slots_flags;
	word_flags private_slots_flags;
	long public_slot_count;
	long private_slot_count;
	s_transport_session_description description;
	qword nonce;
	void* handle;
	c_static_array<s_online_session_player, 16> players;
};
static_assert(sizeof(s_online_session) == 0x150);

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
	bool conflicking_session_check;
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

extern char const* __cdecl managed_session_get_id_string(long managed_session_index);

