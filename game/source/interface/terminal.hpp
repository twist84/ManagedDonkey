#pragma once

#include "cseries/cseries.hpp"
#include <memory/data.hpp>

struct terminal_output_datum : s_datum_header
{
	long line_index;
	long line_count;
	bool tab_stop;
	char message[256];
	real_argb_color message_color;
	real shadow_color_alpha_scale;
};
static_assert(sizeof(terminal_output_datum) == 0x124);

struct terminal_gets_state;
struct s_terminal_globals
{
	bool initialized;
	bool output_to_console;
	bool suppress_output;
	c_smart_data_array<terminal_output_datum>* output_data;
	long line_count;
	long line_index;
	terminal_gets_state* state;
	bool should_draw;
	real draw_time;
};
static_assert(sizeof(s_terminal_globals) == 0x1C);

extern s_terminal_globals terminal_globals;

void terminal_printf(real_argb_color const* color, char const* format, ...);

