#include "objects/object_scheduler.hpp"

//.text:00B99BD0 ; public: __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>::t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>()
//.text:00B99BF0 ; public: __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>::~t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>()
//.text:00B99C00 ; public: real& __cdecl c_static_array<real, 30>::operator[]<long>(long)
//.text:00B99C10 ; public: s_object_schedule& __cdecl c_static_array<s_object_schedule, 32>::operator[]<long>(long)
//.text:00B99C20 ; void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)
//.text:00B99C40 ; public: virtual void* __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>::allocate(unsigned int, char const*)
//.text:00B99C80 ; public: void __cdecl c_schedule_iterator::begin(c_static_flags<32> const*)
//.text:00B99CA0 ; public: virtual void __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>::deallocate(void*, long)
//.text:00B99CD0 ; public: void __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>::free_memory()
//.text:00B99D00 ; bit manipulation related
//.text:00B99D10 ; bit manipulation related
//.text:00B99D20 ; public: long __cdecl c_schedule_iterator::next()
//.text:00B99D70 ; long __cdecl object_scheduler_allocate(long)
//.text:00B99E60 ; void __cdecl object_scheduler_dispose()
//.text:00B99E80 ; void __cdecl object_scheduler_dispose_from_old_map()
//.text:00B99E90 ; void __cdecl object_scheduler_initialize()
//.text:00B99EF0 ; void __cdecl object_scheduler_initialize_for_new_map()
//.text:00B99F20 ; long __cdecl object_scheduler_optimal_phase_index_calculate(long, long, real)

void __cdecl object_scheduler_update()
{
	INVOKE(0x00B9A020, object_scheduler_update);
}

//.text:00B9A180 ; bool __cdecl object_scheduler_update_phase_index(long, long, real)
//.text:00B9A2C0 ; long __cdecl object_scheduler_window_size_get()
//.text:00B9A2D0 ; long __cdecl phase_index_from_time(long, long)
//.text:00B9A2F0 ; public: void* __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>::reserve_memory(char const*, char const*, unsigned int, long)

