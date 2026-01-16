#pragma once

#include "cseries/cseries.hpp"

struct progress_callbacks
{
	void(__cdecl* progress_new_proc)(const char*, void*);
	void(__cdecl* progress_update_proc)(const char*, const char*, int32, void*);
	void(__cdecl* progress_done_proc)(const char*, void*);
	void* progress_data;
};

// this struct is unnamed in the original code
struct s_progress_globals
{
	progress_callbacks default_callbacks;
	progress_callbacks custom_callbacks;
	bool disable_callbacks;
	uns32 start_time;
	bool currently_valid;
	char description[256];
	int32 last_progress;
	uns32 last_milliseconds;
};
COMPILE_ASSERT(sizeof(s_progress_globals) == 0x134);

extern s_progress_globals& progress_globals;

extern void __cdecl progress_disable_callbacks(bool disable_callbacks);
extern void __cdecl progress_done();
extern void __cdecl progress_new(const char* description);
extern void __cdecl progress_set_custom_callbacks(const progress_callbacks* callbacks);
extern void __cdecl progress_set_default_callbacks(const progress_callbacks* callbacks);
extern int32 __cdecl progress_update(int32 current_amount, int32 total_amount);
extern int32 __cdecl progress_update_with_description(int32 current_amount, int32 total_amount, const char* optional_description);

