#include "profiler/profiler.hpp"

#include "cseries/cseries.hpp"

void __cdecl profiler_initialize()
{
	INVOKE(0x00530230, profiler_initialize);
}

void __cdecl profiler_dispose()
{
	INVOKE(0x00530240, profiler_dispose);
}

void __cdecl profiler_initialize_for_new_map()
{
	INVOKE(0x00530250, profiler_initialize_for_new_map);
}

void __cdecl profiler_dispose_from_old_map()
{
	INVOKE(0x00530260, profiler_dispose_from_old_map);
}

