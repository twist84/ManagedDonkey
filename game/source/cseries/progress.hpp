#pragma once

#include "cseries/cseries.hpp"

struct progress_callbacks
{
	void(__cdecl* progress_new_proc)(char const*, void*);
	void(__cdecl* progress_update_proc)(char const*, char const*, long, void*);
	void(__cdecl* progress_done_proc)(char const*, void*);
	void* progress_data;
};

// this struct is unnamed in the original code
struct s_progress_globals
{
	progress_callbacks default_callbacks;
	progress_callbacks custom_callbacks;
	bool disable_callbacks;
	dword start_time;
	bool currently_valid;
	char description[256];
	long last_progress;
	dword last_milliseconds;
};
static_assert(sizeof(s_progress_globals) == 0x134);

extern s_progress_globals& progress_globals;

extern void __cdecl progress_disable_callbacks(bool disable_callbacks);
extern void __cdecl progress_done();
extern void __cdecl progress_new(char const* description);
extern void __cdecl progress_set_custom_callbacks(progress_callbacks const* callbacks);
extern void __cdecl progress_set_default_callbacks(progress_callbacks const* callbacks);
extern long __cdecl progress_update(long current_amount, long total_amount);
extern long __cdecl progress_update_with_description(long current_amount, long total_amount, char const* optional_description);

