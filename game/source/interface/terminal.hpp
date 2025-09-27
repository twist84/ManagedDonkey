#pragma once

#include "cseries/cseries.hpp"
#include "dialogs/edit_text.hpp"
#include "input/input_windows.hpp"
#include "memory/data.hpp"

struct output_line_datum :
	s_datum_header
{
	int32 newer_line_index;
	int32 older_line_index;
	bool tabstop;
	char buffer[255];
	real_argb_color color;
	real32 timer;
};
static_assert(sizeof(output_line_datum) == 0x120);

struct terminal_gets_state
{
	int16 key_count;
	s_key_state keys[32];// key_stroke keys[32];
	real_argb_color color;
	char prompt[32];
	char result[256];
	int32 horizontal_scroll_amount;
	edit_text edit;
};
//static_assert(sizeof(terminal_gets_state) == 0x11FC);

struct s_terminal_globals
{
	bool initialized;
	bool console_output;
	bool suppress_output;
	c_smart_data_array<output_line_datum> output_lines;
	int32 newest_output_line_index;
	int32 oldest_output_line_index;
	terminal_gets_state* input_state;
	bool insertion_point_visible;
	real32 insertion_point_toggle_timer;
};
static_assert(sizeof(s_terminal_globals) == 0x1C);

extern s_terminal_globals terminal_globals;
extern bool g_terminal_render_enable;

extern void terminal_clear();
extern void terminal_dispose();
extern void terminal_draw(rectangle2d* screen_bounds, rectangle2d* frame_bounds);
extern bool terminal_gets_active();
extern bool terminal_gets_begin(terminal_gets_state* state);
extern void terminal_gets_end(terminal_gets_state* state);
extern void terminal_handle_key(s_key_state* key);
extern void terminal_initialize();
extern int32 terminal_new_line(const char* buffer, const real_argb_color* color, bool tabstop);
extern void terminal_output_to_console(bool console_output);
extern void terminal_printf(const real_argb_color* color, const char* format, ...);
extern void terminal_remove_line(int32 line_index);
extern void terminal_suppress_output(bool suppress_output);
extern bool terminal_update(real32 shell_seconds_elapsed);
extern bool terminal_update_input(real32 shell_seconds_elapsed);
extern void terminal_update_output(real32 shell_seconds_elapsed);

