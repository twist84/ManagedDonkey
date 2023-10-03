#include "hs/hs_library_external.hpp"

#include "cseries/cseries_events.hpp"
#include "game/game_time.hpp"
#include "interface/terminal.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x005942E0, hs_breakpoint);
HOOK_DECLARE(0x0096D8B0, hs_log_print);
HOOK_DECLARE(0x0096EF60, hs_print);

void __cdecl hs_breakpoint(const char* s)
{
	if (breakpoints_enabled)
	{
		debug_game_speed = 0.0f;
		char buffer[1024]{};
		csstrnzcpy(buffer, "BREAKPOINT: ", sizeof(buffer));
		csstrnzcat(buffer, s, sizeof(buffer));
		hs_print(buffer);
	}
}

void __cdecl hs_log_print(char const* s)
{
	generate_event(_event_level_warning, "hs: %s", s);
}

void __cdecl hs_print(char const* s)
{
	terminal_printf(global_real_argb_green, s);
}

