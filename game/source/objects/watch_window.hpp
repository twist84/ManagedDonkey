#pragma once

#include "cseries/cseries.hpp"

struct object_datum;
struct s_watch_object_results
{
	const char* object_information;
	object_datum* object;
};
COMPILE_ASSERT(sizeof(s_watch_object_results) == 0x8);

extern uns32 __cdecl clear_watch_window_globals();
extern bool __cdecl object_index_valid_for_try_and_get(int32 object_index);
extern s_watch_object_results& __cdecl watch_object(int32 object_index);
extern const char* __cdecl watch_object_describe(int32 object_index);
extern const char* __cdecl watch_object_describe_internal(int32 object_index, char* buffer, int32 buffer_size);
extern void __cdecl watch_window_dispose();
extern void __cdecl watch_window_initialize();

