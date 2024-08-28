#include "objects/watch_window.hpp"

//.text:0068C600 ; dword __cdecl clear_watch_window_globals()
//.text:0068C630 ; bool __cdecl object_index_valid_for_try_and_get(long)
//.text:0068C680 ; s_watch_object_results& __cdecl watch_object(long)
//.text:0068C750 ; char const* __cdecl watch_object_describe(long)
//.text:0068C810 ; char const* __cdecl watch_object_describe_internal(long, char*, long)

void __cdecl watch_window_dispose()
{
	INVOKE(0x0068C8E0, watch_window_dispose);
}

void __cdecl watch_window_initialize()
{
	INVOKE(0x0068C8F0, watch_window_initialize);
}

