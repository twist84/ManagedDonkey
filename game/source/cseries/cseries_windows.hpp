#pragma once

struct s_system_memory_information
{
	int32 free;
	int32 total;
	int32 pad[0x2];
};
COMPILE_ASSERT(sizeof(s_system_memory_information) == 0x10);

extern void display_debug_string(const char* string);
extern void set_debug_output(void(__stdcall* output)(const char*));

extern uns32 __cdecl system_get_current_thread_id();
extern void __cdecl system_get_date_and_time(char* buffer, int16 buffer_size, bool short_date_and_time);
extern void __cdecl system_memory_information_get(s_system_memory_information* information);
extern uns32 __cdecl system_milliseconds();
extern uns32 __cdecl system_seconds();
extern void __cdecl system_abort();
extern void __cdecl system_exit();
extern void __cdecl system_set_dll_directory();
extern void system_set_maps_directory();

