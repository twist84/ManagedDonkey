#pragma once

#include "cseries/cseries.hpp"
#include "interface/terminal.hpp"

struct s_console_globals
{
	bool is_active;
	bool status_render;
	real __time4;
	terminal_gets_state input_state;
};
//static_assert(sizeof(s_console_globals) == 0x1204);

extern s_console_globals console_globals;

extern bool console_dump_to_debug_display;

extern void __cdecl console_printf(char const* format, ...);
extern void __cdecl console_printf_color(real_argb_color const* color, char const* format, ...);
extern void __cdecl console_warning(char const* format, ...);
extern void __cdecl console_initialize();
extern void __cdecl console_dispose();
extern bool __cdecl console_is_active();
extern bool __cdecl console_is_empty();
extern void __cdecl console_open();
extern void __cdecl console_close();
extern void __cdecl console_clear();
extern void __cdecl console_update(real shell_seconds_elapsed);
extern void __cdecl console_execute_initial_commands();
extern bool __cdecl console_process_command(char const* command, bool a2);

