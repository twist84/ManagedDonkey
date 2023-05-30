#pragma once

#include "cseries/cseries.hpp"

struct s_main_time_globals
{
	dword __unknown0;
	dword __unknown4;
	dword last_milliseconds;
	dword __unknownC;
	dword __unknown10;
	dword __unknown14;
	dword __unknown18;
	dword __unknown1C;
	dword target_display_vblank_index;
	dword __unknown24;
	dword input_collection_time;
	dword __unknown2C;
	dword publishing_start_time;
	dword __unknown34;
	dword publishing_end_time;
	dword __unknown3C;
};
static_assert(sizeof(s_main_time_globals) == 0x40);

extern bool& display_framerate;

extern void __cdecl main_time_frame_rate_display();
extern void __cdecl main_time_reset();
extern void __cdecl main_time_throttle(qword a1);
extern real __cdecl main_time_update();

