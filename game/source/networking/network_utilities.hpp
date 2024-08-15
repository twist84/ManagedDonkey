#pragma once

#include "cseries/cseries.hpp"

extern short __cdecl _random_range(dword* seed_ptr, char const* name, char const* filename, dword line, short range_begin, short range_end);
extern bool __cdecl network_get_machine_name(wchar_t* machine_name, long machine_name_len);
extern void __cdecl network_remote_reporting_dispose();
extern void __cdecl network_remote_reporting_initialize();

extern void bandwidth_profiler_render();

