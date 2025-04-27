#pragma once

#include "cseries/cseries.hpp"

extern int16 __cdecl _random_range(uint32* seed_ptr, char const* name, char const* filename, uint32 line, int16 range_begin, int16 range_end);
extern bool __cdecl network_get_machine_name(wchar_t* machine_name, int32 machine_name_len);
extern void __cdecl network_remote_reporting_dispose();
extern void __cdecl network_remote_reporting_initialize();

extern void bandwidth_profiler_render();

