#pragma once

extern void __cdecl display_debug_string(char const* format, ...);
extern unsigned long __cdecl system_get_current_thread_id();
extern void __cdecl system_get_date_and_time(char* buffer, short buffer_size, bool short_date_and_time);
extern unsigned long __cdecl system_milliseconds();
extern unsigned long __cdecl system_seconds();
extern void __cdecl system_abort();
extern void __cdecl system_exit();
extern void __cdecl system_set_dll_directory();
extern void system_set_maps_directory();

