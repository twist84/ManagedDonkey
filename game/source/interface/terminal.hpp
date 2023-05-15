#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct terminal_output_datum : s_datum_header
{
	long line_index;
	long line_count;
	bool tab_stop;
	c_static_string<255> message;
	real_argb_color message_color;
	real shadow_color_alpha_scale;
};
static_assert(sizeof(terminal_output_datum) == 0x120);

struct terminal_gets_state;
struct s_terminal_globals
{
	bool initialized;
	bool output_to_console;
	bool suppress_output;
	c_smart_data_array<terminal_output_datum>* output_data;
	long line_count;
	long line_index;
	terminal_gets_state* input_state;
	bool should_draw;
	real draw_time;
};
static_assert(sizeof(s_terminal_globals) == 0x1C);

extern s_terminal_globals terminal_globals;
extern bool g_terminal_render_enable;

extern real const k_output_total_seconds;
extern short const k_tab_stops[];

extern void __cdecl terminal_initialize();
extern void __cdecl terminal_dispose();
extern void __cdecl terminal_clear();
extern void __cdecl terminal_update_output(real shell_seconds_elapsed);
extern void __cdecl terminal_remove_line(long line_index);
extern long __cdecl terminal_new_line(char const* message, real_argb_color const* message_color, bool tab_stop);
extern void __cdecl terminal_printf(real_argb_color const* color, char const* format, ...);
extern void __cdecl terminal_draw();
extern void __cdecl terminal_output_to_console(bool output_to_console);
extern void __cdecl terminal_suppress_output(bool suppress_output);
extern bool __cdecl terminal_gets_active();

