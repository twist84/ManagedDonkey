#include "main/main_time.hpp"

REFERENCE_DECLARE(0x022B47FC, bool, display_framerate);

void __cdecl main_time_frame_rate_display()
{
	INVOKE(0x00507B40, main_time_frame_rate_display);
}

void __cdecl main_time_reset()
{
	INVOKE(0x00508110, main_time_reset);
}

void __cdecl main_time_throttle(qword a1)
{
	INVOKE(0x00508160, main_time_throttle, a1);
}

real __cdecl main_time_update()
{
	return INVOKE(0x00508170, main_time_update);
}

