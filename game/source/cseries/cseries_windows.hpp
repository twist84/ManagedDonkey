#pragma once

struct s_system_memory_information
{
	long free;
	long total;
	long pad[2];
};
static_assert(sizeof(s_system_memory_information) == 0x10);

extern void __cdecl display_debug_string(char const* format, ...);
extern unsigned long __cdecl system_get_current_thread_id();
extern void __cdecl system_get_date_and_time(char* buffer, short buffer_size, bool short_date_and_time);
extern void __cdecl system_memory_information_get(s_system_memory_information* information);
extern unsigned long __cdecl system_milliseconds();
extern unsigned long __cdecl system_seconds();
extern void __cdecl system_abort();
extern void __cdecl system_exit();
extern void __cdecl system_set_dll_directory();
extern void system_set_maps_directory();

