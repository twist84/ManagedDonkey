#include "multithreading/synchronization.hpp"

#include "rasterizer/rasterizer_main.hpp"

#include "windows.h"

REFERENCE_DECLARE(0x0238F45C, s_synchronization_globals, g_synch_globals);


// synchronization objects initialization arrays

// .data:0189DA28 ; critical sections
// const char* name;
// int32 spin_count;

// .data:0189DC00 ; mutexes
// const char* name;
// bool initial_owner;

// .data:0189DC88 ; events
// const char* name;
// bool manual_reset;
// bool initial_state;

// .data:0189DC78 ; semaphores
// const char* name;
// int32 initial_signal_count;
// int32 max_signal_count;
// bool dont_use_current_thread; //?

c_critical_section_scope::c_critical_section_scope(int32 critical_section_id)
{
	m_critical_section_id = critical_section_id;
	m_critical_section_entered = false;
	internal_critical_section_enter(m_critical_section_id);
	m_critical_section_entered = true;
}

c_critical_section_scope::c_critical_section_scope(int32 critical_section_id, int32 timeout_in_milliseconds, bool* out_lock_acquired)
{
	m_critical_section_id = critical_section_id;
	m_critical_section_entered = false;
	*out_lock_acquired = false;

	uns32 end_time = system_milliseconds() + timeout_in_milliseconds;
	while (true)
	{
		m_critical_section_entered = internal_critical_section_try_and_enter(m_critical_section_id);
		if (m_critical_section_entered)
		{
			break;
		}

		switch_to_thread();

		if (system_milliseconds() >= end_time)
		{
			return;
		}
	}

	*out_lock_acquired = true;
}

c_critical_section_scope::~c_critical_section_scope()
{
	if (m_critical_section_entered)
	{
		internal_critical_section_leave(m_critical_section_id);
	}
}

void __cdecl destroy_synchronization_objects()
{
	INVOKE(0x0052C1E0, destroy_synchronization_objects);
}

bool __cdecl event_has_automatic_reset(int32 event_id)
{
	return INVOKE(0x0052C250, event_has_automatic_reset, event_id);
}

const char* __cdecl get_sync_primitive_name(int32 type, int32 index)
{
	return INVOKE(0x0052C290, get_sync_primitive_name, type, index);
}

void __cdecl initialize_synchronization_objects()
{
	INVOKE(0x0052C360, initialize_synchronization_objects);
}

void __cdecl internal_critical_section_enter(int32 critical_section_id)
{
	INVOKE(0x0052C4A0, internal_critical_section_enter, critical_section_id);

	//if (critical_section_id != NONE)
	//{
	//	ASSERT(g_synch_globals.initialized);
	//	ASSERT(critical_section_id >= 0 && critical_section_id < k_total_critical_sections);
	//	EnterCriticalSection((LPCRITICAL_SECTION)&g_synch_globals.critical_sections[critical_section_id].critical_section);
	//	g_synch_globals.critical_sections[critical_section_id].object.increment_lock_count(get_current_thread_index());
	//}
}

void __cdecl internal_critical_section_leave(int32 critical_section_id)
{
	INVOKE(0x0052C4E0, internal_critical_section_leave, critical_section_id);

	//if (critical_section_id != NONE)
	//{
	//	ASSERT(g_synch_globals.initialized);
	//	ASSERT(critical_section_id >= 0 && critical_section_id < k_total_critical_sections);
	//	g_synch_globals.critical_sections[critical_section_id].object.decrement_lock_count();
	//	LeaveCriticalSection((LPCRITICAL_SECTION)&g_synch_globals.critical_sections[critical_section_id].critical_section);
	//}
}

bool __cdecl internal_critical_section_try_and_enter(int32 critical_section_id)
{
	return INVOKE(0x0052C510, internal_critical_section_try_and_enter, critical_section_id);

	//BOOL result = TRUE;
	//if (critical_section_id != NONE)
	//{
	//	ASSERT(g_synch_globals.initialized);
	//	ASSERT(critical_section_id >= 0 && critical_section_id < k_total_critical_sections);
	//	result = TryEnterCriticalSection((LPCRITICAL_SECTION)&g_synch_globals.critical_sections[critical_section_id].critical_section);
	//	if (result)
	//		g_synch_globals.critical_sections[critical_section_id].object.increment_lock_count(get_current_thread_index());
	//}
	//return result != FALSE;
}

void __cdecl internal_event_reset(int32 event_id)
{
	INVOKE(0x0052C560, internal_event_reset, event_id);
}

void __cdecl internal_event_set(int32 event_id)
{
	INVOKE(0x0052C580, internal_event_set, event_id);
}

void __cdecl internal_event_wait(int32 event_id)
{
	INVOKE(0x0052C5A0, internal_event_wait, event_id);
}

bool __cdecl internal_event_wait_timeout(int32 event_id, uns32 timeout_in_milliseconds)
{
	return INVOKE(0x0052C5D0, internal_event_wait_timeout, event_id, timeout_in_milliseconds);
}

void __cdecl internal_mutex_release(int32 mutex_id)
{
	INVOKE(0x0052C610, internal_mutex_release, mutex_id);
	
	//ASSERT(g_synch_globals.initialized);
	//ASSERT(mutex_id >= 0 && mutex_id < k_total_synchronization_mutexes);
	//g_synch_globals.mutexes[mutex_id].object.decrement_lock_count();
	//BOOL success = ReleaseMutex(g_synch_globals.mutexes[mutex_id].handle);
	//ASSERT(success);
}

void __cdecl internal_mutex_take(int32 mutex_id)
{
	INVOKE(0x0052C640, internal_mutex_take, mutex_id);

	//ASSERT(g_synch_globals.initialized);
	//ASSERT(mutex_id >= 0 && mutex_id < k_total_synchronization_mutexes);
	//bool result = wait_for_single_object_internal(g_synch_globals.mutexes[mutex_id].handle, INFINITE);
	//if (result)
	//	g_synch_globals.mutexes[mutex_id].object.increment_lock_count(get_current_thread_index());
	//ASSERT(result);
}

bool __cdecl internal_mutex_take_timeout(int32 mutex_id, uns32 timeout_in_milliseconds)
{
	return INVOKE(0x0052C690, internal_mutex_take_timeout, mutex_id, timeout_in_milliseconds);

	//ASSERT(g_synch_globals.initialized);
	//ASSERT(mutex_id >= 0 && mutex_id < k_total_synchronization_mutexes);
	//bool result = wait_for_single_object_internal(g_synch_globals.mutexes[mutex_id].handle, timeout_in_milliseconds);
	//if (result)
	//	g_synch_globals.mutexes[mutex_id].object.increment_lock_count(get_current_thread_index());
	//return result;
}

int32 __cdecl internal_semaphore_release(int32 semaphore_id)
{
	return INVOKE(0x0052C6F0, internal_semaphore_release, semaphore_id);

	//ASSERT(g_synch_globals.initialized);
	//ASSERT(semaphore_id >= 0 && semaphore_id < k_total_synchronization_semaphores);
	//int32 previous_count = 0;
	//if (ReleaseSemaphore(g_synch_globals.semaphores[semaphore_id].handle.handle, 1, &previous_count))
	//{
	//	int32 thread_index = get_current_thread_index();
	//	if (off_0189DC78[semaphore_id].dont_use_current_thread)
	//		thread_index = 0;
	//	g_synch_globals.semaphores[semaphore_id].remove_thread_reference(thread_index);
	//}
	//return previous_count;
}

void __cdecl internal_semaphore_take(int32 semaphore_id)
{
	INVOKE(0x0052C760, internal_semaphore_take, semaphore_id);

	//ASSERT(g_synch_globals.initialized);
	//ASSERT(semaphore_id >= 0 && semaphore_id < k_total_synchronization_semaphores);
	//bool result = wait_for_single_object_internal(g_synch_globals.semaphores[semaphore_id].handle.handle, INFINITE);
	//ASSERT(result);
	//if (result)
	//{
	//	int32 thread_index = get_current_thread_index();
	//	if (off_0189DC78[semaphore_id].dont_use_current_thread)
	//		thread_index = 0;
	//
	//	g_synch_globals.semaphores[semaphore_id].add_thread_reference(thread_index);
	//}
}

void __cdecl release_all_critical_sections_owned_by_thread()
{
	INVOKE(0x0052C7C0, release_all_critical_sections_owned_by_thread);

	//if (g_synch_globals.initialized)
	//{
	//	int32 thread_index = get_current_thread_index();
	//	for (int32 critical_section_id = 0; critical_section_id < k_total_critical_sections; critical_section_id++)
	//		release_critical_section_owned_by_thread(thread_index, static_cast<e_critical_sections>(critical_section_id));
	//}
}

void __cdecl release_all_locks_owned_by_thread()
{
	INVOKE(0x0052C830, release_all_locks_owned_by_thread);

	//if (g_synch_globals.initialized)
	//{
	//	release_all_semaphores_owned_by_thread();
	//	release_all_mutexes_owned_by_thread();
	//	release_all_critical_sections_owned_by_thread();
	//}
}

void __cdecl release_all_mutexes_owned_by_thread()
{
	INVOKE(0x0052C850, release_all_mutexes_owned_by_thread);

	//if (g_synch_globals.initialized)
	//{
	//	int32 thread_index = get_current_thread_index();
	//	for (int32 mutex_id = 0; mutex_id < k_total_synchronization_mutexes; mutex_id++)
	//	{
	//		if (g_synch_globals.mutexes[mutex_id].object.get_locking_thread() == thread_index)
	//		{
	//			while (g_synch_globals.mutexes[mutex_id].object.get_lock_count())
	//				internal_mutex_release(mutex_id);
	//		}
	//	}
	//}
}

void __cdecl release_all_semaphores_owned_by_thread()
{
	INVOKE(0x0052C8B0, release_all_semaphores_owned_by_thread);

	//if (g_synch_globals.initialized)
	//{
	//	int32 thread_index = get_current_thread_index();
	//	for (int32 semaphore_id = 0; semaphore_id < k_total_synchronization_semaphores; semaphore_id++)
	//	{
	//		if (off_0189DC78[semaphore_id].dont_use_current_thread)
	//			thread_index = 0;
	//
	//		while (g_synch_globals.semaphores[semaphore_id].get_thread_reference_count(thread_index) && internal_semaphore_release(semaphore_id))
	//			;
	//	}
	//}
}

void __cdecl release_critical_section_owned_by_thread(int32 thread_index, e_critical_sections critical_section_id)
{
	INVOKE(0x0052C960, release_critical_section_owned_by_thread, thread_index, critical_section_id);

	//if (g_synch_globals.critical_sections[critical_section_id].object.get_locking_thread() == thread_index)
	//{
	//	while (g_synch_globals.critical_sections[critical_section_id].object.get_lock_count())
	//		internal_critical_section_leave(critical_section_id);
	//}
}

void __cdecl release_locks_safe_for_crash_release()
{
	INVOKE(0x0052C9C0, release_locks_safe_for_crash_release);

	//if (g_synch_globals.initialized)
	//{
	//	int32 thread_index = get_current_thread_index();
	//	release_critical_section_owned_by_thread(thread_index, k_crit_section_event_logs);
	//	release_critical_section_owned_by_thread(thread_index, k_crit_section_data_mine);
	//	release_critical_section_owned_by_thread(thread_index, k_crit_section_ui_widgets_lock);
	//	release_critical_section_owned_by_thread(thread_index, k_crit_section_font_cache);
	//	release_critical_section_owned_by_thread(thread_index, k_crit_section_ui_custom_bitmaps_lock);
	//	release_critical_section_owned_by_thread(thread_index, k_crit_section_saved_game);
	//	release_critical_section_owned_by_thread(thread_index, k_crit_section_levels);
	//	release_critical_section_owned_by_thread(thread_index, k_crit_section_autosave_queue);
	//}
}

void __cdecl render_synchronization_stats()
{
	INVOKE(0x0052CC00, render_synchronization_stats);
}

int32 __cdecl sempahore_get_max_signal_count(int32 semaphore_id)
{
	return INVOKE(0x0052CC30, sempahore_get_max_signal_count, semaphore_id);

	//ASSERT(semaphore_id >= 0 && semaphore_id < k_total_synchronization_semaphores);
	//return off_0189DC78[semaphore_id].max_signal_count;
}

bool __cdecl synchronization_objects_initialized()
{
	return INVOKE(0x0052CC70, synchronization_objects_initialized);

	//return g_synch_globals.initialized;
}

bool __cdecl wait_for_single_object_internal(void* handle, uns32 timeout_in_milliseconds)
{
	return INVOKE(0x0052CC90, wait_for_single_object_internal, handle, timeout_in_milliseconds);

	// halo online

	//DWORD wait_result;
	//return !handle || (wait_result = WaitForSingleObjectEx(handle, timeout_in_milliseconds, FALSE)) == 0 || wait_result == WAIT_ABANDONED;

	// halo 3

	//if (handle)
	//{
	//	bool success = false;
	//	DWORD wait_result = WaitForSingleObjectEx(handle, timeout_in_milliseconds, FALSE);
	//	switch (wait_result)
	//	{
	//	case WAIT_ABANDONED:
	//		VASSERT("some thread terminated before releasing a sychronization primative");
	//		success = true;
	//		break;
	//	case WAIT_TIMEOUT:
	//		success = false;
	//		break;
	//	case WAIT_OBJECT_0:
	//		success = true;
	//		break;
	//	}
	//	ASSERT((success) || (timeout_in_milliseconds != INFINITE));
	//	return success;
	//}
	//return true;
}


