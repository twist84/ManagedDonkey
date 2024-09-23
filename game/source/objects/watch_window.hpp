#pragma once

#include "cseries/cseries.hpp"

struct object_datum;
struct s_watch_object_results
{
	const char* object_information;
	object_datum* object;
};
static_assert(sizeof(s_watch_object_results) == 0x8);

extern dword __cdecl clear_watch_window_globals();
extern bool __cdecl object_index_valid_for_try_and_get(long object_index);
extern s_watch_object_results& __cdecl watch_object(long object_index);
extern char const* __cdecl watch_object_describe(long object_index);
extern char const* __cdecl watch_object_describe_internal(long object_index, char* buffer, long buffer_size);
extern void __cdecl watch_window_dispose();
extern void __cdecl watch_window_initialize();

