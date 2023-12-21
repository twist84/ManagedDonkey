#pragma once

#include "cseries/cseries.hpp"

struct progress_callbacks
{
	void(__cdecl* progress_new_proc)(char const*, void*);
	void(__cdecl* progress_update_proc)(char const*, char const*, long, void*);
	void(__cdecl* progress_done_proc)(char const*, void*);
	void* progress_data;
};

struct progress_globals_definition
{
	progress_callbacks default_callbacks;
	progress_callbacks custom_callbacks;
	bool disable_callbacks;
	dword start_time;
	bool __unknown28;
	char description[256];
	long update_progress;
	dword update_time;
};
static_assert(sizeof(progress_globals_definition) == 0x134);

extern progress_globals_definition& progress_globals;

extern void __cdecl progress_disable_callbacks(bool disable_callbacks);
extern void __cdecl progress_done();
extern void __cdecl progress_new(char const* description);
extern void __cdecl progress_set_custom_callbacks(progress_callbacks const* callbacks);
extern void __cdecl progress_set_default_callbacks(progress_callbacks const* callbacks);
extern long __cdecl progress_update(long index, long count);
extern long __cdecl progress_update_with_description(long index, long count, char const* description);

