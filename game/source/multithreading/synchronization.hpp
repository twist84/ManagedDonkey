#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "multithreading/threads.hpp"

struct s_list_entry // LIST_ENTRY
{
	s_list_entry* forward_link;
	s_list_entry* backward_link;
};
COMPILE_ASSERT(sizeof(s_list_entry) == 0x8);

struct s_critical_section_debug // RTL_CRITICAL_SECTION_DEBUG
{
	uns16 type;
	uns16 creator_back_trace_index;
	struct s_critical_section* critical_section;
	s_list_entry process_locks_list;
	uns32 entry_count;
	uns32 contention_count;
	uns32 flags;
	uns16 creator_back_trace_index_high;
	uns16 spare_word;
};
COMPILE_ASSERT(sizeof(s_critical_section_debug) == 0x20);

struct s_critical_section // RTL_CRITICAL_SECTION
{
	s_critical_section_debug* debug_info;

	//
	//  The following three fields control entering and exiting the critical
	//  section for the resource
	//

	int32 lock_count;
	int32 recursion_count;
	void* owning_thread;        // from the thread's ClientId->UniqueThread
	void* lock_semaphore;
	/* ULONG_PTR */ uns32 spin_count;        // force size on 64-bit systems when packed
};
COMPILE_ASSERT(sizeof(s_critical_section) == 0x18);

class c_synchronization_object
{
public:
	c_interlocked_long locking_thread;
	c_interlocked_long lock_count;
};
COMPILE_ASSERT(sizeof(c_synchronization_object) == 0x8);

class c_critical_section_data :
	public c_synchronization_object
{
public:
	s_critical_section critical_section;
};
COMPILE_ASSERT(sizeof(c_critical_section_data) == 0x20);

class c_synchronization_handle :
	public c_synchronization_object
{
public:
	void* handle;
};
COMPILE_ASSERT(sizeof(c_synchronization_handle) == 0xC);

class c_semaphore_handle :
	public c_synchronization_handle
{
public:
	int32 thread_reference_count[k_registered_thread_count];
};
COMPILE_ASSERT(sizeof(c_semaphore_handle) == 0x34);

struct s_synchronization_globals
{
	volatile bool initialized;
	c_critical_section_data critical_section[k_total_critical_sections];
	c_synchronization_handle mutex[k_total_synchronization_mutexes];
	c_synchronization_handle sync_event[k_total_synchronization_events];
	c_semaphore_handle semaphore[k_total_synchronization_semaphores];
};
COMPILE_ASSERT(sizeof(s_synchronization_globals) == 0x91C);

class c_critical_section_scope
{
public:
	c_critical_section_scope(int32 critical_section_id);
	c_critical_section_scope(int32 critical_section_id, int32 time_step, bool* out_lock_acquired);
	~c_critical_section_scope();

protected:
	c_synchronized_long m_critical_section_id;
	c_synchronized_long m_critical_section_entered;
};

extern s_synchronization_globals& g_synch_globals;

extern void __cdecl destroy_synchronization_objects();
extern bool __cdecl event_has_automatic_reset(int32 event_id);
extern const char* __cdecl get_sync_primitive_name(int32 type, int32 index);
extern void __cdecl initialize_synchronization_objects();
extern void __cdecl internal_critical_section_enter(int32 critical_section_id);
extern void __cdecl internal_critical_section_leave(int32 critical_section_id);
extern bool __cdecl internal_critical_section_try_and_enter(int32 critical_section_id);
extern void __cdecl internal_event_reset(int32 event_id);
extern void __cdecl internal_event_set(int32 event_id);
extern void __cdecl internal_event_wait(int32 event_id);
extern bool __cdecl internal_event_wait_timeout(int32 event_id, uns32 timeout_in_milliseconds);
extern void __cdecl internal_mutex_release(int32 mutex_id);
extern void __cdecl internal_mutex_take(int32 mutex_id);
extern bool __cdecl internal_mutex_take_timeout(int32 mutex_id, uns32 timeout_in_milliseconds);
extern int32 __cdecl internal_semaphore_release(int32 semaphore_id);
extern void __cdecl internal_semaphore_take(int32 semaphore_id);
extern void __cdecl release_all_critical_sections_owned_by_thread();
extern void __cdecl release_all_locks_owned_by_thread();
extern void __cdecl release_all_mutexes_owned_by_thread();
extern void __cdecl release_all_semaphores_owned_by_thread();
extern void __cdecl release_critical_section_owned_by_thread(int32 thread_index, e_critical_sections critical_section_id);
extern void __cdecl release_locks_safe_for_crash_release();
extern void __cdecl render_synchronization_stats();
extern int32 __cdecl sempahore_get_max_signal_count(int32 semaphore_id);
extern bool __cdecl synchronization_objects_initialized();
extern bool __cdecl wait_for_single_object_internal(void* handle, uns32 timeout_in_milliseconds);

