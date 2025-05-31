#include "objects/object_scheduler.hpp"

//.text:00B99BD0 ; public: __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>::t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>()
//.text:00B99BF0 ; public: __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>::~t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>()
//.text:00B99C00 ; public: real32& __cdecl c_static_array<real32, 30>::operator[]<int32>(int32)
//.text:00B99C10 ; public: s_object_schedule& __cdecl c_static_array<s_object_schedule, 32>::operator[]<int32>(int32)
//.text:00B99C20 ; void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)
//.text:00B99C40 ; public: virtual void* __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>::allocate(unsigned int, const char*)
//.text:00B99C80 ; public: void __cdecl c_schedule_iterator::begin(const c_static_flags<32>*)
//.text:00B99CA0 ; public: virtual void __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>::deallocate(void*, int32)
//.text:00B99CD0 ; public: void __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>::free_memory()
//.text:00B99D00 ; bit manipulation related
//.text:00B99D10 ; bit manipulation related
//.text:00B99D20 ; public: int32 __cdecl c_schedule_iterator::next()
//.text:00B99D70 ; int32 __cdecl object_scheduler_allocate(int32)

void __cdecl object_scheduler_dispose()
{
	INVOKE(0x00B99E60, object_scheduler_dispose);
}

void __cdecl object_scheduler_dispose_from_old_map()
{
	INVOKE(0x00B99E80, object_scheduler_dispose_from_old_map);
}

void __cdecl object_scheduler_initialize()
{
	INVOKE(0x00B99E90, object_scheduler_initialize);
}

void __cdecl object_scheduler_initialize_for_new_map()
{
	INVOKE(0x00B99EF0, object_scheduler_initialize_for_new_map);
}

//.text:00B99F20 ; int32 __cdecl object_scheduler_optimal_phase_index_calculate(int32, int32, real32)

void __cdecl object_scheduler_update()
{
	INVOKE(0x00B9A020, object_scheduler_update);
}

//.text:00B9A180 ; bool __cdecl object_scheduler_update_phase_index(int32, int32, real32)
//.text:00B9A2C0 ; int32 __cdecl object_scheduler_window_size_get()
//.text:00B9A2D0 ; int32 __cdecl phase_index_from_time(int32, int32)
//.text:00B9A2F0 ; public: void* __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_object_schedule_globals_allocator(void*)>::reserve_memory(const char*, const char*, unsigned int, int32)

