#include "hs/hs_library_external.hpp"

#include "cseries/cseries_events.hpp"
#include "interface/terminal.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x0096D8B0, hs_log_print);
HOOK_DECLARE(0x0096EF60, hs_print);

void __cdecl hs_log_print(char const* text)
{
	generate_event(_event_level_warning, "hs: %s", text);
}
//HOOK_DECLARE_CALL(0x007302EA, hs_log_print);

void __cdecl hs_print(char const* text)
{
	terminal_printf(global_real_argb_green, text);
}
//HOOK_DECLARE_CALL(0x0072FE9A, hs_print);

