#include "networking/logic/logic_session_tracker.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/network_configuration.hpp"
#include "networking/network_memory.hpp"

#define SESSION_STORAGE_COUNT 16 // engine default is 100
#define SESSION_STORAGE_SIZE sizeof(s_session_tracker_session_data) * SESSION_STORAGE_COUNT

REFERENCE_DECLARE(0x0229EC20, s_session_tracker_globals, session_tracker_globals);

HOOK_DECLARE(0x004E2DB0, network_session_tracker_clear);
HOOK_DECLARE(0x004E2DC0, network_session_tracker_clear_unsuitable_sessions);
HOOK_DECLARE(0x004E2DF0, network_session_tracker_dispose);
HOOK_DECLARE(0x004E2E40, network_session_tracker_get_session_count);
HOOK_DECLARE(0x004E2E50, network_session_tracker_get_session_data);
HOOK_DECLARE(0x004E2E70, network_session_tracker_get_session_status);
HOOK_DECLARE(0x004E2E90, network_session_tracker_initialize);
HOOK_DECLARE(0x004E2EA0, network_session_tracker_mark_session_undesirable);
HOOK_DECLARE(0x004E2ED0, network_session_tracker_mark_session_unsuitable);
HOOK_DECLARE(0x004E2F00, network_session_tracker_session_contacted);
HOOK_DECLARE(0x004E2F30, network_session_tracker_start);
HOOK_DECLARE(0x004E2F50, network_session_tracker_stop);
HOOK_DECLARE(0x004E2F90, network_session_tracker_track_session);
HOOK_DECLARE(0x004E2FB0, network_session_tracker_update);

//// c_session_tracker::add_session
//const t_value_type<uns8> session_storage_count = { .value = SESSION_STORAGE_COUNT };
//DATA_PATCH_DECLARE(0x004E253A + 2, session_storage_count, session_storage_count.bytes); // cmp     esi, 100
//DATA_PATCH_DECLARE(0x004E253F + 1, session_storage_count, session_storage_count.bytes); // push    100
//
//// c_session_tracker::allocate_storage
//const t_value_type<uns32> session_storage_size = { .value = SESSION_STORAGE_SIZE };
//DATA_PATCH_DECLARE(0x004E2676 + 1, session_storage_size, session_storage_size.bytes); // push    8B9340h
//DATA_PATCH_DECLARE(0x004E267E + 3, session_storage_size, session_storage_size.bytes); // mov     dword ptr [esi+14h], 8B9340h

const char* k_network_session_tracker_session_undesirable_reason_descriptions[]
{
	"none",
	"latency"
};
static_assert(k_network_session_tracker_session_undesirable_reason_count == NUMBEROF(k_network_session_tracker_session_undesirable_reason_descriptions));

const char* k_network_session_tracker_session_unsuitable_reason_descriptions[]
{
	"none",
	"qos-refused",
	"qos-unreachable",
	"qos-failed-to-get-result",
	"qos-payload-empty",
	"qos-payload-failed-to-decode",
	"qos-payload-wrong-type",
	"qos-timeout",
	"qos-incompatible-versions",
	"previously-marked-unsuitable",
	"join-failed",
	"failed-to-get-session-data"
};
static_assert(k_network_session_tracker_session_unsuitable_reason_count == NUMBEROF(k_network_session_tracker_session_unsuitable_reason_descriptions));

bool c_session_tracker::add_session(const char* session_name, const s_transport_session_description* session_description)
{
	//return INVOKE_CLASS_MEMBER(0x004E2530, c_session_tracker, add_session, session_name, session_description);

	ASSERT(m_flags.test(_session_tracker_initialized_bit));

	if (m_session_count >= SESSION_STORAGE_COUNT)
	{
		event(_event_warning, "networking:logic:session_tracker: can't track session, session limit reached [%d]", SESSION_STORAGE_COUNT);
		return false;
	}

	s_session_tracker_session_data* tracked_session = &m_session_storage[m_session_count];
	ASSERT(!tracked_session->flags.test(_session_tracker_session_valid_bit));

	csmemset(tracked_session, 0, sizeof(s_session_tracker_session_data));

	csstrnzcpy(tracked_session->name, session_name, sizeof(tracked_session->name));
	tracked_session->description = *session_description;
	tracked_session->insertion_index = m_session_count;
	tracked_session->qos_attempt_index = NONE;
	tracked_session->qos_attempt_target_index = NONE;
	tracked_session->flags.set(_session_tracker_session_unsuitable_bit, session_is_unsuitable(&session_description->id));

	if (tracked_session->flags.test(_session_tracker_session_unsuitable_bit))
	{
		tracked_session->unsuitable_reason = _session_unsuitable_previously_marked_unsuitable;
	}

	tracked_session->flags.set(_session_tracker_session_valid_bit, true);
	m_session_count++;

	// Matchmaking hack: We don't have a valid QoS payload from transport_qos_get_result,
	// but we can just set these to tru to skip the requirement for now.
	// $TODO MM, Reimplement QoS payloads via API.
	tracked_session->qos_received[_transport_qos_type_probe_only] = true;
	tracked_session->qos_received[_transport_qos_type_default] = true;

	return false;
}

void c_session_tracker::add_session_to_unsuitable_sessions(const s_transport_secure_identifier* session_id)
{
	//INVOKE_CLASS_MEMBER(0x004E2600, c_session_tracker, add_session_to_unsuitable_sessions, session_id);

	if (session_is_unsuitable(session_id))
	{
		return;
	}

	if (m_unsuitable_session_count >= m_unsuitable_session_maximum_count)
	{
		return;
	}

	m_unsuitable_session_storage[m_unsuitable_session_count++] = *session_id;
}

bool c_session_tracker::allocate_storage(e_network_session_tracker_sort_method sort_method, e_network_session_qos_status_data_type qos_status_data_type, c_matchmaking_quality* matchmaking_quality)
{
	//return INVOKE_CLASS_MEMBER(0x004E2640, c_session_tracker, allocate_storage, sort_method, qos_status_data_type, matchmaking_quality);

	m_sort_method = sort_method;
	m_matchmaking_quality = matchmaking_quality;

	m_expected_qos_data_type = qos_status_data_type;
	m_maximum_qos_tasks = PIN(QOS_ATTEMPT_MIN_COUNT, g_network_configuration.logic.session_tracker.maximum_qos_tasks, QOS_ATTEMPT_MAX_COUNT);

	// logic-session-array
	m_session_storage_size_bytes = sizeof(s_session_tracker_session_data) * SESSION_STORAGE_COUNT;
	m_session_storage = (s_session_tracker_session_data*)network_heap_allocate_block(m_session_storage_size_bytes);

	// logic-unsuitable-session-array
	m_unsuitable_session_maximum_count = g_network_configuration.logic.session_tracker.unsuitable_session_cache_count;
	m_unsuitable_session_storage_size_bytes = sizeof(s_transport_secure_identifier) * m_unsuitable_session_maximum_count;
	m_unsuitable_session_storage = (s_transport_secure_identifier*)network_heap_allocate_block(sizeof(s_transport_secure_identifier) * m_unsuitable_session_maximum_count);

	if (!m_session_storage || !m_unsuitable_session_storage)
	{
		release_storage();

		char network_heap_description[1024]{};
		event(_event_error, "networking:logic:session_tracker: unable to allocate storage of sizes [%d/%d/%d] for session arrays [%s]",
			m_session_storage_size_bytes,
			m_session_storage_size_bytes,
			m_unsuitable_session_storage_size_bytes,
			network_heap_describe(network_heap_description, sizeof(network_heap_description)));

		return false;
	}

	m_flags.set(_session_tracker_initialized_bit, true);
	return true;
}

void c_session_tracker::build_qos_target_list(e_transport_qos_type qos_type, int32* qos_targets_tracked_session_indices_out, int32 max_tracked_session_indices, int32* qos_target_count_out)
{
	INVOKE_CLASS_MEMBER(0x004E2640, c_session_tracker, build_qos_target_list, qos_type, qos_targets_tracked_session_indices_out, max_tracked_session_indices, qos_target_count_out);
}

//.text:004E27D0 ; 

void c_session_tracker::clear()
{
	//INVOKE_CLASS_MEMBER(0x004E27E0, c_session_tracker, clear);

	ASSERT(m_flags.test(_session_tracker_initialized_bit));
	event(_event_message, "networking:logic:session_tracker: clearing tracked sessions");

	for (int32 qos_attemp_index = 0; qos_attemp_index < m_qos_attempts.get_count(); qos_attemp_index++)
	{
		release_qos_attempt(qos_attemp_index);
	}

	m_session_count = 0;
	csmemset(m_session_storage, 0, m_session_storage_size_bytes);
}

void c_session_tracker::clear_qos_attempt(int32 qos_attempt_index)
{
	//INVOKE_CLASS_MEMBER(0x004E2850, c_session_tracker, clear_qos_attempt, qos_attempt_index);

	s_session_tracker_qos_attempt& qos_attempt = m_qos_attempts[qos_attempt_index];
	qos_attempt.target_count = 0;
	qos_attempt.time = 0;
	qos_attempt.qos_index = NONE;
	qos_attempt.qos_type = k_transport_qos_type_none;
}

void c_session_tracker::clear_unsuitable_sessions()
{
	//INVOKE_CLASS_MEMBER(0x004E2880, c_session_tracker, clear_unsuitable_sessions);

	event(_event_message, "networking:logic:session_tracker: clearing unsuitable sessions");

	m_unsuitable_session_count = 0;
	csmemset(m_unsuitable_session_storage, 0, m_unsuitable_session_storage_size_bytes);
}

int __cdecl compare_session_desirability(const void* a, const void* b)
{
	return INVOKE(0x004E28B0, compare_session_desirability, a, b);
}

void c_session_tracker::dispose()
{
	//INVOKE_CLASS_MEMBER(0x004E2920, c_session_tracker, dispose);

	if (m_flags.test(_session_tracker_initialized_bit))
	{
		release_storage();
	}
}

//.text:004E2960 ; 
//.text:004E2970 ; 
//.text:004E2980 ; public: static long c_static_array<s_session_tracker_qos_attempt, 10>::get_count(void)

int32 c_session_tracker::get_maximum_qos_target_count(e_transport_qos_type qos_type)
{
	//return INVOKE_CLASS_MEMBER(0x004E2990, c_session_tracker, get_maximum_qos_target_count, qos_type);

	if (qos_type == _transport_qos_type_probe_only)
		return 64;

	if (qos_type == _transport_qos_type_default)
		return g_network_configuration.logic.session_tracker.maximum_target_sessions_per_default_qos_task;

	return 0;
}

int32 c_session_tracker::get_session_count()
{
	//return INVOKE_CLASS_MEMBER(0x004E29C0, c_session_tracker, get_session_count);

	ASSERT(m_flags.test(_session_tracker_initialized_bit));
	return m_session_count;
}

bool c_session_tracker::get_session_data(int32 tracked_session_index, s_network_session_tracker_session_data* session_data_out)
{
	return INVOKE_CLASS_MEMBER(0x004E29D0, c_session_tracker, get_session_data, tracked_session_index, session_data_out);
}

void c_session_tracker::get_session_status(int32 tracked_session_index, s_network_session_tracker_session_status* status_out)
{
	//INVOKE_CLASS_MEMBER(0x004E2AB0, c_session_tracker, get_session_status, tracked_session_index, status_out);

	s_session_tracker_session_data* tracked_session = &m_session_storage[tracked_session_index];

	ASSERT(m_flags.test(_session_tracker_initialized_bit));
	ASSERT(VALID_INDEX(tracked_session_index, m_session_count));

	csmemset(status_out, 0, sizeof(s_network_session_tracker_session_status));
	if (tracked_session->flags.test(_session_tracker_session_unsuitable_bit))
	{
		status_out->status = _tracked_session_unsuitable;
		status_out->unsuitable_reason = tracked_session->unsuitable_reason;
	}
	else if (tracked_session->flags.test(_session_tracker_session_undesireable_bit))
	{
		status_out->status = _tracked_session_undesireable;
		status_out->undesirable_reason = tracked_session->undesirable_reason;
	}
	else if (tracked_session->qos_received[_transport_qos_type_probe_only])
	{
		status_out->status = e_network_session_tracker_session_status(tracked_session->qos_received[_transport_qos_type_default] + 2);
	}
	else
	{
		status_out->status = _tracked_session_qos_needed;
	}
}

int32 c_session_tracker::get_tracked_session_index(const s_transport_secure_identifier* session_id)
{
	//return INVOKE_CLASS_MEMBER(0x004E2B20, c_session_tracker, get_tracked_session_index, session_id);

	for (int32 session_index = 0; session_index < m_session_count; session_index++)
	{
		s_session_tracker_session_data* session = &m_session_storage[session_index];
		if (csmemcmp(&session->description.id, session_id, sizeof(s_transport_secure_identifier)) == 0)
		{
			ASSERT(session->flags.test(_session_tracker_session_valid_bit));
			return session_index;
		}
	}

	return NONE;
}

bool c_session_tracker::initialize(bool verify)
{
	//return INVOKE_CLASS_MEMBER(0x004E2B90, c_session_tracker, initialize, verify);

	m_sort_method = _network_session_tracker_sort_none;

	m_session_count = 0;
	m_session_storage_size_bytes = 0;
	m_session_storage = NULL;

	m_unsuitable_session_count = 0;
	m_unsuitable_session_maximum_count = 0;
	m_unsuitable_session_storage_size_bytes = 0;
	m_unsuitable_session_storage = NULL;

	m_maximum_qos_tasks = 0;

	for (int32 qos_attempt_index = 0; qos_attempt_index < m_qos_attempts.get_count(); qos_attempt_index++)
	{
		if (verify)
		{
			s_session_tracker_qos_attempt* qos_attempt = &m_qos_attempts[qos_attempt_index];
			ASSERT(qos_attempt->qos_index == NONE);
		}

		clear_qos_attempt(qos_attempt_index);
	}

	m_flags.clear();

	return true;
}

bool c_session_tracker::mark_session_undesireable(const s_transport_session_description* session_description, e_network_session_tracker_session_undesirable_reason reason)
{
	//return INVOKE_CLASS_MEMBER(0x004E2D30, c_session_tracker, mark_session_undesireable, session_description, reason);

	int32 tracked_session_index = get_tracked_session_index(&session_description->id);
	ASSERT(m_flags.test(_session_tracker_initialized_bit));

	if (tracked_session_index == NONE)
	{
		event(_event_error, "networking:logic:session_tracker: not tracking session, can't mark as undesireable");
		return false;
	}

	ASSERT(VALID_INDEX(tracked_session_index, m_session_count));
	mark_tracked_session_undesireable(&m_session_storage[tracked_session_index], reason);
	return true;
}

bool c_session_tracker::mark_session_unsuitable(const s_transport_session_description* session_description, e_network_session_tracker_session_unsuitable_reason reason)
{
	//return INVOKE_CLASS_MEMBER(0x004E2D60, c_session_tracker, mark_session_unsuitable, session_description, reason);

	int32 tracked_session_index = get_tracked_session_index(&session_description->id);
	ASSERT(m_flags.test(_session_tracker_initialized_bit));

	if (tracked_session_index == NONE)
	{
		event(_event_error, "networking:logic:session_tracker: not tracking session, can't mark as unsuitable");
		return false;
	}

	ASSERT(VALID_INDEX(tracked_session_index, m_session_count));
	mark_tracked_session_unsuitable(&m_session_storage[tracked_session_index], reason);
	return true;
}

void c_session_tracker::mark_tracked_session_undesireable(s_session_tracker_session_data* session, e_network_session_tracker_session_undesirable_reason reason)
{
	//INVOKE_CLASS_MEMBER(0x004E2D90, c_session_tracker, mark_tracked_session_undesireable, session, reason);

	ASSERT(session->flags.test(_session_tracker_session_valid_bit));
	ASSERT(VALID_INDEX(reason, NUMBEROF(k_network_session_tracker_session_undesirable_reason_descriptions)));
	
	event(_event_message, "networking:logic:session_tracker: marking session #%d/%s/%I64d undesirable [%s]",
		session->insertion_index,
		session->name,
		*(uns64*)&session->description.id,
		k_network_session_tracker_session_undesirable_reason_descriptions[reason]);
	
	if (session->flags.test(_session_tracker_session_undesireable_bit))
	{
		return;
	}
	
	session->undesirable_reason = reason;
	session->flags.set(_session_tracker_session_undesireable_bit, true);
	m_flags.set(_session_tracker_sort_required_bit, true);
}

void c_session_tracker::mark_tracked_session_unsuitable(s_session_tracker_session_data* session, e_network_session_tracker_session_unsuitable_reason reason)
{
	//INVOKE_CLASS_MEMBER(0x004E2DA0, c_session_tracker, mark_tracked_session_unsuitable, session, reason);

	ASSERT(session->flags.test(_session_tracker_session_valid_bit));
	ASSERT(VALID_INDEX(reason, NUMBEROF(k_network_session_tracker_session_unsuitable_reason_descriptions)));
	
	event(_event_message,"networking:logic:session_tracker: marking session #%d/%s/%I64d unsuitable [%s]",
		session->insertion_index,
		session->name,
		*(uns64*)&session->description.id,
		k_network_session_tracker_session_unsuitable_reason_descriptions[reason]);
	
	if (session->flags.test(_session_tracker_session_unsuitable_bit))
	{
		return;
	}
	
	add_session_to_unsuitable_sessions(&session->description.id);
	session->unsuitable_reason = reason;
	session->flags.set(_session_tracker_session_unsuitable_bit, true);
	m_flags.set(_session_tracker_sort_required_bit, true);
}

void __cdecl network_session_tracker_clear()
{
	//INVOKE(0x004E2DB0, network_session_tracker_clear);

	session_tracker_globals.session_tracker.clear();
}

void __cdecl network_session_tracker_clear_unsuitable_sessions()
{
	//INVOKE(0x004E2DC0, network_session_tracker_clear_unsuitable_sessions);

	session_tracker_globals.session_tracker.clear_unsuitable_sessions();
}

void __cdecl network_session_tracker_dispose()
{
	//INVOKE(0x004E2DF0, network_session_tracker_dispose);

	session_tracker_globals.session_tracker.dispose();
}

int32 __cdecl network_session_tracker_get_session_count()
{
	//return INVOKE(0x004E2E40, network_session_tracker_get_session_count);

	return session_tracker_globals.session_tracker.get_session_count();
}

bool __cdecl network_session_tracker_get_session_data(int32 tracked_session_index, s_network_session_tracker_session_data* session_data)
{
	//return INVOKE(0x004E2E50, network_session_tracker_get_session_data, tracked_session_index, session_data);

	return session_tracker_globals.session_tracker.get_session_data(tracked_session_index, session_data);
}

void __cdecl network_session_tracker_get_session_status(int32 tracked_session_index, s_network_session_tracker_session_status* session_status)
{
	//return INVOKE(0x004E2E70, network_session_tracker_get_session_status, tracked_session_index, session_status);

	session_tracker_globals.session_tracker.get_session_status(tracked_session_index, session_status);
}

bool __cdecl network_session_tracker_initialize()
{
	//return INVOKE(0x004E2E90, network_session_tracker_initialize);

	return session_tracker_globals.session_tracker.initialize(false);
}

bool __cdecl network_session_tracker_mark_session_undesirable(const s_transport_session_description* description, e_network_session_tracker_session_undesirable_reason undesirable_reason)
{
	//return INVOKE(0x004E2EA0, network_session_tracker_mark_session_undesirable, description, undesirable_reason);

	return session_tracker_globals.session_tracker.mark_session_undesireable(description, undesirable_reason);
}

bool __cdecl network_session_tracker_mark_session_unsuitable(const s_transport_session_description* description, e_network_session_tracker_session_unsuitable_reason unsuitable_reason)
{
	//return INVOKE(0x004E2ED0, network_session_tracker_mark_session_unsuitable, description, unsuitable_reason);

	return session_tracker_globals.session_tracker.mark_session_unsuitable(description, unsuitable_reason);
}

bool __cdecl network_session_tracker_session_contacted(int32 tracked_session_index)
{
	//return INVOKE(0x004E2F00, network_session_tracker_session_contacted, tracked_session_index);

	return session_tracker_globals.session_tracker.session_completed_initial_qos(tracked_session_index);
}

bool __cdecl network_session_tracker_start(e_network_session_tracker_sort_method tracker_sort_method, e_network_session_qos_status_data_type qos_status_data_type, c_matchmaking_quality* matchmaking_quality)
{
	//return INVOKE(0x004E2F30, network_session_tracker_start, tracker_sort_method, qos_status_data_type, matchmaking_quality);

	return session_tracker_globals.session_tracker.allocate_storage(tracker_sort_method, qos_status_data_type, matchmaking_quality);
}

void __cdecl network_session_tracker_stop()
{
	//return INVOKE(0x004E2F50, network_session_tracker_stop);

	session_tracker_globals.session_tracker.release_storage();
}

bool __cdecl network_session_tracker_track_session(const char* name, const s_transport_session_description* description)
{
	//return INVOKE(0x004E2F90, network_session_tracker_track_session, name, description);

	return session_tracker_globals.session_tracker.add_session(name, description);
}

void __cdecl network_session_tracker_update()
{
	//INVOKE(0x004E2FB0, network_session_tracker_update);

	session_tracker_globals.session_tracker.update();
}

void c_session_tracker::release_qos_attempt(int32 qos_attempt_index)
{
	//INVOKE_CLASS_MEMBER(0x004E3020, c_session_tracker, release_qos_attempt, qos_attempt_index);

	s_session_tracker_qos_attempt& qos_attempt = m_qos_attempts[qos_attempt_index];
	if (qos_attempt.qos_index == NONE)
	{
		return;
	}

	transport_qos_delete(qos_attempt.qos_index);
	clear_qos_attempt(qos_attempt.qos_index);
}

void c_session_tracker::release_storage()
{
	//INVOKE_CLASS_MEMBER(0x004E3060, c_session_tracker, release_storage);

	if (m_session_storage)
	{
		network_heap_free_block(m_session_storage);
	}
	
	if (m_unsuitable_session_storage)
	{
		network_heap_free_block(m_unsuitable_session_storage);
	}
	
	clear();
	initialize(true);
}

bool c_session_tracker::session_completed_initial_qos(int32 tracked_session_index)
{
	//return INVOKE_CLASS_MEMBER(0x004E30A0, c_session_tracker, session_completed_initial_qos, tracked_session_index);

	ASSERT(m_flags.test(_session_tracker_initialized_bit));
	ASSERT(VALID_INDEX(tracked_session_index, m_session_count));

	s_session_tracker_session_data* tracked_session = &m_session_storage[tracked_session_index];
	return tracked_session->qos_received[_transport_qos_type_probe_only] || tracked_session->qos_received[_transport_qos_type_default];
}

bool c_session_tracker::session_is_unsuitable(const s_transport_secure_identifier* session_id)
{
	return INVOKE_CLASS_MEMBER(0x004E30D0, c_session_tracker, session_is_unsuitable, session_id);
}

void c_session_tracker::update()
{
	//INVOKE_CLASS_MEMBER(0x004E31C0, c_session_tracker, update);

	if (!m_flags.test(_session_tracker_initialized_bit))
	{
		return;
	}

	update_sessions_for_refresh();
	for (int32 qos_attempt_index = 0; qos_attempt_index < m_maximum_qos_tasks; qos_attempt_index++)
	{
		update_qos_receive(qos_attempt_index);
		update_qos_send(qos_attempt_index);
	}
	update_sort();
}

void c_session_tracker::update_qos_receive(int32 qos_attempt_index)
{
	INVOKE_CLASS_MEMBER(0x004E3220, c_session_tracker, update_qos_receive, qos_attempt_index);
}

void c_session_tracker::update_qos_send(int32 qos_attempt_index)
{
	INVOKE_CLASS_MEMBER(0x004E35D0, c_session_tracker, update_qos_send, qos_attempt_index);
}

void c_session_tracker::update_sessions_for_refresh()
{
	INVOKE_CLASS_MEMBER(0x004E3750, c_session_tracker, update_sessions_for_refresh);
}

void c_session_tracker::update_sort()
{
	INVOKE_CLASS_MEMBER(0x004E37D0, c_session_tracker, update_sort);
}

bool c_session_tracker::update_sort_by_desirability()
{
	return INVOKE_CLASS_MEMBER(0x004E3810, c_session_tracker, update_sort_by_desirability);
}

