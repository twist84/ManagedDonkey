#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "multithreading/threads.hpp"

struct s_list_entry // LIST_ENTRY
{
	s_list_entry* forward_link;
	s_list_entry* backward_link;
};
static_assert(sizeof(s_list_entry) == 0x8);

struct s_critical_section_debug // RTL_CRITICAL_SECTION_DEBUG
{
	uint16 type;
	uint16 creator_back_trace_index;
	struct s_critical_section* critical_section;
	s_list_entry process_locks_list;
	uint32 entry_count;
	uint32 contention_count;
	uint32 flags;
	uint16 creator_back_trace_index_high;
	uint16 spare_word;
};
static_assert(sizeof(s_critical_section_debug) == 0x20);

struct s_critical_section // RTL_CRITICAL_SECTION
{
	s_critical_section_debug* debug_info;

	//
	//  The following three fields control entering and exiting the critical
	//  section for the resource
	//

	long lock_count;
	long recursion_count;
	void* owning_thread;        // from the thread's ClientId->UniqueThread
	void* lock_semaphore;
	/* ULONG_PTR */ uint32 spin_count;        // force size on 64-bit systems when packed
};
static_assert(sizeof(s_critical_section) == 0x18);

struct c_synchronization_object
{
	c_interlocked_long locking_thread;
	c_interlocked_long lock_count;
};
static_assert(sizeof(c_synchronization_object) == 0x8);

struct c_critical_section_data : c_synchronization_object
{
	s_critical_section critical_section;
};
static_assert(sizeof(c_critical_section_data) == 0x20);

struct c_synchronization_handle : c_synchronization_object
{
	void* handle;
};
static_assert(sizeof(c_synchronization_handle) == 0xC);

struct c_semaphore_handle : c_synchronization_handle
{
	long thread_reference_count[k_registered_thread_count];
};
static_assert(sizeof(c_semaphore_handle) == 0x34);

struct s_synchronization_globals
{
	volatile bool initialized;
	c_critical_section_data critical_section[k_total_critical_sections];
	c_synchronization_handle mutex[k_total_synchronization_mutexes];
	c_synchronization_handle sync_event[k_total_synchronization_events];
	c_semaphore_handle semaphore[k_total_synchronization_semaphores];
};
static_assert(sizeof(s_synchronization_globals) == 0x91C);

struct c_critical_section_scope
{
public:
	c_critical_section_scope(long critical_section_id);
	c_critical_section_scope(long critical_section_id, long time_step, bool* out_lock_acquired);
	~c_critical_section_scope();

protected:
	c_synchronized_long m_critical_section_id;
	c_synchronized_long m_critical_section_entered;
};

extern s_synchronization_globals& g_synch_globals;

extern bool __cdecl event_has_automatic_reset(long event_id);
extern char const* __cdecl get_sync_primitive_name(long type, long index);
extern void __cdecl initialize_synchronization_objects();
extern void __cdecl internal_critical_section_enter(long critical_section_id);
extern void __cdecl internal_critical_section_leave(long critical_section_id);
extern bool __cdecl internal_critical_section_try_and_enter(long critical_section_id);
extern void __cdecl internal_event_reset(long event_id);
extern void __cdecl internal_event_set(long event_id);
extern void __cdecl internal_event_wait(long event_id);
extern bool __cdecl internal_event_wait_timeout(long event_id, uint32 timeout_in_milliseconds);
extern void __cdecl internal_mutex_release(long mutex_id);
extern void __cdecl internal_mutex_take(long mutex_id);
extern bool __cdecl internal_mutex_take_timeout(long mutex_id, uint32 timeout_in_milliseconds);
extern long __cdecl internal_semaphore_release(long semaphore_id);
extern void __cdecl internal_semaphore_take(long semaphore_id);
extern void __cdecl release_all_critical_sections_owned_by_thread();
extern void __cdecl release_all_locks_owned_by_thread();
extern void __cdecl release_all_mutexes_owned_by_thread();
extern void __cdecl release_all_semaphores_owned_by_thread();
extern void __cdecl release_critical_section_owned_by_thread(long thread_index, e_critical_sections critical_section_id);
extern void __cdecl release_locks_safe_for_crash_release();
extern void __cdecl render_synchronization_stats();
extern long __cdecl sempahore_get_max_signal_count(long semaphore_id);
extern bool __cdecl synchronization_objects_initialized();
extern bool __cdecl wait_for_single_object_internal(void* handle, uint32 timeout_in_milliseconds);

