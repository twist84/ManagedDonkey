#pragma once

#include "cseries/cseries.hpp"
#include "dialogs/edit_text.hpp"
#include "input/input_windows.hpp"
#include "memory/data.hpp"

struct terminal_output_datum :
	s_datum_header
{
	long line_index;
	long line_count;
	bool tab_stop;
	c_static_string<255> message;
	real_argb_color message_color;
	real shadow_color_alpha_scale;
};
static_assert(sizeof(terminal_output_datum) == 0x120);

long const k_terminal_gets_state_input_text_size = 256;

//#pragma pack(push, 4)
struct terminal_gets_state
{
	short key_count;
	//c_static_array<key_stroke, 32> keys;
	c_static_array<s_key_state, 32> keys;
	real_argb_color color;
	c_static_string<k_tag_string_length> prompt;
	char input_text[k_terminal_gets_state_input_text_size];
	long scroll_amount;
	edit_text edit;
	c_static_string<255> previous_inputs[16];
	short __unknown11F4;
	short previous_inputs_count;
	short __unknown11F8;
};
//static_assert(sizeof(terminal_gets_state) == 0x11FC);
//#pragma pack(pop)

struct s_terminal_globals
{
	bool initialized;
	bool output_to_console;
	bool suppress_output;
	c_smart_data_array<terminal_output_datum> output_data;
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

extern void __cdecl terminal_printf(real_argb_color const* color, char const* format, ...);
extern void __cdecl terminal_initialize();
extern void __cdecl terminal_dispose();
extern void __cdecl terminal_clear();
extern bool __cdecl terminal_update_input(real shell_seconds_elapsed);
extern void __cdecl terminal_update_output(real shell_seconds_elapsed);
extern bool __cdecl terminal_update(real shell_seconds_elapsed);
extern void __cdecl terminal_remove_line(long line_index);
extern long __cdecl terminal_new_line(char const* message, real_argb_color const* message_color, bool tab_stop);
extern void __cdecl terminal_output_to_console(bool output_to_console);
extern void __cdecl terminal_suppress_output(bool suppress_output);
extern bool __cdecl terminal_gets_active();
extern bool __cdecl terminal_gets_begin(terminal_gets_state* state);
extern void __cdecl terminal_gets_end(terminal_gets_state* state);
extern void __cdecl terminal_draw();

