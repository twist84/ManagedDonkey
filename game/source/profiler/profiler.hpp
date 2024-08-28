#pragma once

#define PROFILER(...)

extern void __cdecl profiler_initialize();
extern void __cdecl profiler_dispose();
extern void __cdecl profiler_initialize_for_new_map();
extern void __cdecl profiler_dispose_from_old_map();

