#pragma once

#include "cseries/cseries.hpp"

struct s_spamming_event
{
	dword hit_time;
	long hit_count;
	char text[2048];
	bool valid;
};
static_assert(sizeof(s_spamming_event) == 0x80C);

extern bool g_events_initialized;
extern bool g_events_debug_render_enable;

extern c_static_array<s_spamming_event, 64> g_spamming_events;

extern void __cdecl events_debug_render();
extern void __cdecl network_debug_print(const char* format, ...);

