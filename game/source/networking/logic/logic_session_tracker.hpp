#pragma once

#include "cseries/cseries.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/transport/transport_qos.hpp"

#define QOS_ATTEMPT_MIN_COUNT 2
#define QOS_ATTEMPT_MAX_COUNT 10

struct __declspec(align(8)) s_network_session_tracker_session_data
{
	char name[16];
	s_transport_session_description description;
	s_transport_qos_result qos_result;
	s_network_squad_status_data status_data;
	int32 desirability;
};
COMPILE_ASSERT(sizeof(s_network_session_tracker_session_data) == 0x16528);

struct s_network_session_tracker_session_status
{
	e_network_session_tracker_session_status status;
	e_network_session_tracker_session_unsuitable_reason unsuitable_reason;
	e_network_session_tracker_session_undesirable_reason undesirable_reason;
};
COMPILE_ASSERT(sizeof(s_network_session_tracker_session_status) == 0xC);

struct s_session_tracker_session_data
{
	c_flags<e_session_tracker_session_flags, uns8, k_session_tracker_session_flags_count> flags;
	char name[16];
	int32 insertion_index;
	s_transport_session_description description;
	e_network_session_tracker_session_undesirable_reason undesirable_reason;
	e_network_session_tracker_session_unsuitable_reason unsuitable_reason;
	bool qos_received[k_transport_qos_type_count];
	int32 qos_attempt_index;
	int32 qos_attempt_target_index;
	e_transport_qos_target_status qos_target_status;
	s_transport_qos_result qos_result;
	s_network_squad_status_data qos_data;
	uns32 status_data_timestamp;
	bool desirability_valid;
	int32 desirability;
	byte __data[0x4];
};
COMPILE_ASSERT(sizeof(s_session_tracker_session_data) == 0x16550);

struct s_session_tracker_qos_attempt
{
	int32 qos_index;
	e_transport_qos_type qos_type;
	int32 target_count;
	uns32 time;
};
COMPILE_ASSERT(sizeof(s_session_tracker_qos_attempt) == 0x10);

class c_matchmaking_quality;
class c_session_tracker
{
public:
	bool add_session(const char* name, const s_transport_session_description* description);

private:
	void add_session_to_unsuitable_sessions(const s_transport_secure_identifier* session_id);

public:
	bool allocate_storage(e_network_session_tracker_sort_method sort_method, e_network_session_qos_status_data_type qos_status_data_type, c_matchmaking_quality* matchmaking_quality);

private:
	void build_qos_target_list(e_transport_qos_type qos_type, int32* qos_targets_tracked_session_indices_out, int32 qos_target_count_out, int32* qos_target_count);
	void clear_qos_attempt(int32 qos_attempt_index);

public:
	void clear();
	void clear_unsuitable_sessions();
	void dispose();

private:
	int32 get_maximum_qos_target_count(e_transport_qos_type qos_type);

public:
	int32 get_session_count();
	bool get_session_data(int32 tracked_session_index, s_network_session_tracker_session_data* session_data_out);
	void get_session_status(int32 tracked_session_index, s_network_session_tracker_session_status* status_out);

private:
	int32 get_tracked_session_index(const s_transport_secure_identifier* session_id);

public:
	bool initialize(bool verify);
	bool mark_session_undesireable(const s_transport_session_description* session_description, e_network_session_tracker_session_undesirable_reason reason);
	bool mark_session_unsuitable(const s_transport_session_description* session_description, e_network_session_tracker_session_unsuitable_reason reason);

private:
	void mark_tracked_session_undesireable(s_session_tracker_session_data* session, e_network_session_tracker_session_undesirable_reason reason);
	void mark_tracked_session_unsuitable(s_session_tracker_session_data* session, e_network_session_tracker_session_unsuitable_reason reason);
	void release_qos_attempt(int32 qos_attempt_index);

public:
	void release_storage();
	bool session_completed_initial_qos(int32 tracked_session_index);

private:
	bool session_is_unsuitable(const s_transport_secure_identifier* session_id);

public:
	void update();

private:
	void update_qos_receive(int32 qos_attempt_index);
	void update_qos_send(int32 qos_attempt_index);
	void update_sessions_for_refresh();
	void update_sort();
	bool update_sort_by_desirability();

protected:
	e_network_session_tracker_sort_method m_sort_method;
	e_network_session_qos_status_data_type m_expected_qos_data_type;
	c_flags<e_session_tracker_flags, uns8, k_session_tracker_flags_count> m_flags;
	c_matchmaking_quality* m_matchmaking_quality;
	int32 m_session_count;
	int32 m_session_storage_size_bytes;
	s_session_tracker_session_data* m_session_storage;
	int32 m_unsuitable_session_count;
	int32 m_unsuitable_session_maximum_count;
	int32 m_unsuitable_session_storage_size_bytes;
	s_transport_secure_identifier* m_unsuitable_session_storage;
	int32 m_maximum_qos_tasks;
	c_static_array<s_session_tracker_qos_attempt, QOS_ATTEMPT_MAX_COUNT> m_qos_attempts;
};
COMPILE_ASSERT(sizeof(c_session_tracker) == 0xD0);

struct s_session_tracker_globals
{
	c_session_tracker session_tracker;
};
COMPILE_ASSERT(sizeof(s_session_tracker_globals) == sizeof(c_session_tracker));

extern s_session_tracker_globals& session_tracker_globals;

extern void __cdecl network_session_tracker_clear();
extern void __cdecl network_session_tracker_clear_unsuitable_sessions();
extern void __cdecl network_session_tracker_dispose();
extern int32 __cdecl network_session_tracker_get_session_count();
extern bool __cdecl network_session_tracker_get_session_data(int32 tracked_session_index, s_network_session_tracker_session_data* session_data);
extern void __cdecl network_session_tracker_get_session_status(int32 tracked_session_index, s_network_session_tracker_session_status* session_status);
extern bool __cdecl network_session_tracker_initialize();
extern bool __cdecl network_session_tracker_mark_session_undesirable(const s_transport_session_description* description, e_network_session_tracker_session_undesirable_reason undesirable_reason);
extern bool __cdecl network_session_tracker_mark_session_unsuitable(const s_transport_session_description* description, e_network_session_tracker_session_unsuitable_reason unsuitable_reason);
extern bool __cdecl network_session_tracker_session_contacted(int32 tracked_session_index);
extern bool __cdecl network_session_tracker_start(e_network_session_tracker_sort_method tracker_sort_method, e_network_session_qos_status_data_type qos_status_data_type, c_matchmaking_quality* matchmaking_quality);
extern void __cdecl network_session_tracker_stop();
extern bool __cdecl network_session_tracker_track_session(const char* name, const s_transport_session_description* description);
extern void __cdecl network_session_tracker_update();

