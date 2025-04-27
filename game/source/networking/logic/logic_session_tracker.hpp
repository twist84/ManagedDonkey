#pragma once

#include "cseries/cseries.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/transport/transport_qos.hpp"

#define QOS_ATTEMPT_MIN_COUNT 2
#define QOS_ATTEMPT_MAX_COUNT 10

struct s_network_session_tracker_session_data
{
	char name[16];
	s_transport_session_description description;
	s_transport_qos_result qos_result;
	s_network_session_status_data status_data;
	int32 session_desirability;
};
static_assert(sizeof(s_network_session_tracker_session_data) == 0x16524);

struct s_network_session_tracker_session_status
{
	int32 type;
	int32 __unknown4;
	int32 __unknown8;
};
static_assert(sizeof(s_network_session_tracker_session_status) == 0xC);

struct s_network_session_tracker_session
{
	c_flags<e_session_tracker_session_flags, uint8, k_session_tracker_session_flags> flags;
	char name[16];
	int32 session_index;
	s_transport_session_description description;
	int32 __unknown48;
	int32 __unknown4C;
	bool qos_received[k_transport_qos_type_count];
	int32 qos_attempt_index;
	int32 qos_attempt_target_index;
	int32 qos_attempt_target_status;
	s_transport_qos_result qos_result;
	s_network_session_status_data qos_data;
	uint32 time;
	bool __unknown16544;
	int32 session_desirability;
	uint8 __data[4];
};
static_assert(sizeof(s_network_session_tracker_session) == 0x16550);

struct s_session_tracker_unsuitable_session
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_session_tracker_unsuitable_session) == 0x10);

struct s_session_tracker_qos_attempt
{
	int32 qos_index;
	e_transport_qos_type qos_type;
	int32 target_count;
	uint32 time;
};
static_assert(sizeof(s_session_tracker_qos_attempt) == 0x10);

enum e_session_tracker_flags
{
	_session_tracker_initialized_bit = 0,

	// if anyone runs into the name of this bit hit me up on discord
	_session_tracker_bit1,

	k_session_tracker_flags
};

struct c_matchmaking_quality;
struct c_session_tracker
{
public:
	bool add_session(char const* name, s_transport_session_description const* description);

private:
	void add_session_to_unsuitable_sessions(s_transport_secure_identifier const* session_id);

public:
	bool allocate_storage(int32 tracker_sort_method, int32 qos_status_data_type, c_matchmaking_quality* matchmaking_quality);

private:
	void build_qos_target_list(e_transport_qos_type qos_type, int32* qos_targets, int32 max_qos_target_count, int32* qos_target_count);
	void clear_qos_attempt(int32 qos_attempt_index);

public:
	void clear();
	void clear_unsuitable_sessions();
	void dispose();

private:
	int32 get_maximum_qos_target_count(e_transport_qos_type qos_type);

public:
	int32 get_session_count();
	bool get_session_data(int32 tracked_session_index, s_network_session_tracker_session_data* session_data);
	void get_session_status(int32 tracked_session_index, s_network_session_tracker_session_status* session_status);

private:
	int32 get_tracked_session_index(s_transport_secure_identifier const* session_id);

public:
	bool initialize(bool a1);
	bool mark_session_undesireable(s_transport_session_description const* description, int32 undesirable_reason);
	bool mark_session_unsuitable(s_transport_session_description const* description, int32 unsuitable_reason);

private:
	void release_qos_attempt(int32 qos_attempt_index);

public:
	void release_storage();
	bool session_completed_initial_qos(int32 tracked_session_index);

private:
	bool session_is_unsuitable(s_transport_secure_identifier const* session_id);

public:
	void update();

private:
	void update_qos_receive(int32 qos_attempt_index);
	void update_qos_send(int32 qos_attempt_index);
	void update_sessions_for_refresh();
	void update_sort();
	bool update_sort_by_desirability();

protected:
	int32 m_sort_method;
	int32 m_expected_qos_data_type;
	c_flags<e_session_tracker_flags, uint8, k_session_tracker_flags> m_flags;
	c_matchmaking_quality* m_matchmaking_quality;

	int32 m_session_count;
	int32 m_session_storage_size;
	s_network_session_tracker_session* m_sessions;

	int32 m_unsuitable_session_count;
	int32 m_unsuitable_session_maximum_count;
	int32 m_unsuitable_session_storage_size;
	s_session_tracker_unsuitable_session* m_unsuitable_sessions;

	int32 m_qos_attempt_count;
	c_static_array<s_session_tracker_qos_attempt, QOS_ATTEMPT_MAX_COUNT> m_qos_attempts;
};
static_assert(sizeof(c_session_tracker) == 0xD0);

struct s_session_tracker_globals
{
	c_session_tracker session_tracker;
};
static_assert(sizeof(s_session_tracker_globals) == sizeof(c_session_tracker));

extern s_session_tracker_globals& session_tracker_globals;

extern void __cdecl network_session_tracker_clear();
extern void __cdecl network_session_tracker_clear_unsuitable_sessions();
extern void __cdecl network_session_tracker_dispose();
extern int32 __cdecl network_session_tracker_get_session_count();
extern bool __cdecl network_session_tracker_get_session_data(int32 tracked_session_index, s_network_session_tracker_session_data* session_data);
extern void __cdecl network_session_tracker_get_session_status(int32 tracked_session_index, s_network_session_tracker_session_status* session_status);
extern bool __cdecl network_session_tracker_initialize();
extern bool __cdecl network_session_tracker_mark_session_undesirable(s_transport_session_description const* description, int32 undesirable_reason);
extern bool __cdecl network_session_tracker_mark_session_unsuitable(s_transport_session_description const* description, int32 unsuitable_reason);
extern bool __cdecl network_session_tracker_session_contacted(int32 tracked_session_index);
extern bool __cdecl network_session_tracker_start(int32 tracker_sort_method, int32 qos_status_data_type, c_matchmaking_quality* matchmaking_quality);
extern void __cdecl network_session_tracker_stop();
extern bool __cdecl network_session_tracker_track_session(char const* name, s_transport_session_description const* description);
extern void __cdecl network_session_tracker_update();

