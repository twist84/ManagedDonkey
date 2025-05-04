#pragma once

#include "cseries/cseries.hpp"

extern int16 __cdecl _random_range(uns32* seed, char const* purpose, char const* source_file, uns32 source_line, int16 lower_bound, int16 upper_bound);
extern bool __cdecl network_get_machine_name(wchar_t* name, int32 name_length);
extern void __cdecl network_remote_reporting_dispose();
extern void __cdecl network_remote_reporting_initialize();

extern void bandwidth_profiler_render();

