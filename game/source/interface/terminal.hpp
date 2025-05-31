#pragma once

#include "cseries/cseries.hpp"
#include "dialogs/edit_text.hpp"
#include "input/input_windows.hpp"
#include "memory/data.hpp"

struct output_line_datum :
	s_datum_header
{
	int32 line_index;
	int32 line_count;
	bool tabstop;
	c_static_string<255> buffer;
	real_argb_color color;
	real32 timer;
};
static_assert(sizeof(output_line_datum) == 0x120);

int32 const k_terminal_gets_state_input_text_size = 256;

//#pragma pack(push, 4)
struct terminal_gets_state
{
	int16 key_count;
	//c_static_array<key_stroke, 32> keys;
	c_static_array<s_key_state, 32> keys;
	real_argb_color color;
	c_static_string<k_tag_string_length> prompt;
	char result[k_terminal_gets_state_input_text_size];
	int32 horizontal_scroll_amount;
	edit_text edit;
};
//static_assert(sizeof(terminal_gets_state) == 0x11FC);
//#pragma pack(pop)

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

extern real32 const k_output_total_seconds;
extern int16 const k_tab_stops[];

extern void __cdecl terminal_printf(const real_argb_color* color, const char* format, ...);
extern void __cdecl terminal_initialize();
extern void __cdecl terminal_dispose();
extern void __cdecl terminal_clear();
extern bool __cdecl terminal_update_input(real32 shell_seconds_elapsed);
extern void __cdecl terminal_update_output(real32 shell_seconds_elapsed);
extern bool __cdecl terminal_update(real32 shell_seconds_elapsed);
extern void __cdecl terminal_remove_line(int32 line_index);
extern int32 __cdecl terminal_new_line(const char* buffer, const real_argb_color* color, bool tabstop);
extern void __cdecl terminal_output_to_console(bool console_output);
extern void __cdecl terminal_suppress_output(bool suppress_output);
extern bool __cdecl terminal_gets_active();
extern bool __cdecl terminal_gets_begin(terminal_gets_state* state);
extern void __cdecl terminal_gets_end(terminal_gets_state* state);
extern void __cdecl terminal_draw(rectangle2d* screen_bounds, rectangle2d* frame_bounds);

