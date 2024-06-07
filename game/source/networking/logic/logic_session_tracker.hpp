#pragma once

#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/transport/transport_qos.hpp"

struct s_network_session_tracker_session_data
{
	char name[16];
	s_transport_session_description description;
	s_transport_qos_result qos_result;
	s_network_session_status_data status_data;
};
static_assert(sizeof(s_network_session_tracker_session_data) == 0x16520);

struct s_network_session_tracker_session
{
	byte_flags flags;
	char name[16];
	long session_index;
	s_transport_session_description description;
	long __unknown48;
	long __unknown4C;
	bool qos_received[2];
	long qos_attempt_index;
	long qos_attempt_target_index;
	long qos_attempt_target_status;
	s_transport_qos_result qos_result;
	s_network_session_status_data qos_data;
	dword time;
	bool __unknown16544;
	long session_desirability;
	byte __data[4];
};
static_assert(sizeof(s_network_session_tracker_session) == 0x16550);

struct s_session_tracker_qos_attempt
{
	long qos_index;
	long qos_type;
	long target_count;
	dword time;
};
static_assert(sizeof(s_network_session_tracker_session) == 0x16550);

struct c_matchmaking_quality;
struct c_session_tracker
{
	long m_sort_method;
	long m_expected_qos_data_type;
	byte_flags m_flags;
	c_matchmaking_quality* m_matchmaking_quality;
	long m_session_count;
	long m_session_storage_size;
	s_network_session_tracker_session* m_sessions;
	long __unknown1C;
	long __unknown20;
	long m_unsuitable_session_count;
	s_network_session_tracker_session* m_unsuitable_sessions;
	long m_qos_attempt_count;
	s_session_tracker_qos_attempt m_qos_attempts[10];
};
static_assert(sizeof(c_session_tracker) == 0xD0);

struct s_session_tracker_globals
{
	c_session_tracker session_tracker;
};
static_assert(sizeof(s_session_tracker_globals) == sizeof(c_session_tracker));

extern void __cdecl network_session_tracker_dispose();
extern bool __cdecl network_session_tracker_initialize();
extern void __cdecl network_session_tracker_update();

