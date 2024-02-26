#pragma once

#include "cseries/cseries.hpp"
#include "interface/terminal.hpp"
#include "hs/hs_scenario_definitions.hpp"

struct s_console_globals
{
	bool is_active;
	bool status_render;
	real __time4;
	terminal_gets_state input_state;
};
//static_assert(sizeof(s_console_globals) == 0x1204);

struct s_console_global
{
	char const* name;

	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> type;
	void* pointer;
};
static_assert(sizeof(s_console_global) == 0xC);

extern s_console_globals console_globals;

extern s_console_global const* const k_console_globals[];
extern long const k_console_global_count;

extern bool console_dump_to_debug_display;

extern bool __cdecl debugging_system_has_focus();
extern void __cdecl console_printf(char const* format, ...);
extern void __cdecl console_printf_color(real_argb_color const* color, char const* format, ...);
extern void __cdecl console_warning(char const* format, ...);
extern void __cdecl console_initialize();
extern void __cdecl console_dispose();
extern bool __cdecl console_is_active();
extern bool __cdecl console_is_empty();
extern void __cdecl console_open(bool debug_menu);
extern void __cdecl console_close();
extern void __cdecl console_clear();
extern void __cdecl console_update(real shell_seconds_elapsed);
extern void __cdecl console_execute_initial_commands();
extern bool __cdecl console_process_command(char const* command, bool a2);

