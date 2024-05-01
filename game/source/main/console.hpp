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

enum e_status_line_flags
{
	_status_line_unknown_bit0,
	_status_line_unknown_bit1,
	_status_line_unknown_bit2,
	_status_line_unknown_bit3,

	k_status_line_flags
};

struct c_status_line
{
public:
	c_status_line();
	char const* printf(char const*, ...);
	char const* appendf(char const*, ...);
	char const* printf_va(char const*, char*);
	char const* appendf_va(char const*, char*);
	void set_flag(e_status_line_flags flag, bool enable);
	void set_alpha(real alpha);
	void set_color(real_rgb_color const& color);
	void set_in_use(bool in_use);
	void clear_text();
	char const* get_identifier() const;
	real_rgb_color const& get_color() const;
	real get_alpha() const;
	bool is_empty() const;
	char const* get_string() const;
	bool is_in_use_valid() const;
	bool is_in_use() const;
	bool test_flag(e_status_line_flags flag) const;
	void add_single();
	void remove_single();
	c_status_line* previous() const;
	c_status_line* next() const;
	void initialize_simple(bool*, char const*, long);

protected:
	c_static_string<256> m_string;
	real_rgb_color m_color;
	real m_alpha;
	c_flags<e_status_line_flags, dword, k_status_line_flags> m_flags;
	bool* m_in_use;
	char const* m_identifier;
	c_status_line* m_previous;
	c_status_line* m_next;
};
static_assert(sizeof(c_status_line) == 0x124);

struct s_status_string
{
	c_static_string<256> string;
	long __time100;
	c_status_line line;
};
static_assert(sizeof(s_status_string) == 0x228);

struct s_string_cache
{
	c_static_string<4096> string;
	real_argb_color color;
	long text_justification;
};
static_assert(sizeof(s_string_cache) == 0x1014);

extern s_console_globals console_globals;

extern s_console_global const* const k_console_globals[];
extern long const k_console_global_count;

extern bool console_dump_to_debug_display;

extern c_status_line* g_status_line_head;
extern c_status_line* g_status_line_tail;

extern s_status_string g_status_strings[20];

struct c_draw_string;
struct c_font_cache_base;

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

extern void status_lines_initialize(c_status_line* status_lines, bool* in_use, long count);
extern void status_lines_initialize_simple(c_status_line* status_lines, bool* in_use, char const* identifier, long count);
extern void status_lines_dispose(c_status_line* status_lines, long count);
extern void status_lines_clear_text(c_status_line* status_lines, long count);
extern void status_lines_set_flags(c_status_line* status_lines, e_status_line_flags flag, bool enable, long count);
extern void status_lines_enable(char const* identifier);
extern void status_lines_disable(char const* identifier);

extern void status_line_add_single(c_status_line* status_line);
extern void status_line_remove_single(c_status_line* status_line);
extern char const* status_line_printf(c_status_line* status_line, char const* format, ...);
extern char const* status_line_appendf(c_status_line* status_line, char const* format, ...);
extern void status_line_set_flag(c_status_line* status_line, e_status_line_flags flag, bool enable);
extern void status_line_set_alpha(c_status_line* status_line, real alpha);
extern void status_line_set_color(c_status_line* status_line, real_rgb_color const& color);
extern void status_line_clear_text(c_status_line* status_line);
extern char const* status_line_get_string(c_status_line* status_line);
extern void status_line_draw();
extern bool status_line_visible(c_status_line const* status_line);
extern void status_line_dump();

extern void status_printf(char const* format, ...);
extern void status_printf_va(char const* format, char* list);
extern void status_string_internal(char const* status, char const* message);
extern void status_strings(char const* status, char const* strings);

extern bool string_cache_add_string(s_string_cache* string_cache, char const* string, real alpha, real_rgb_color const& color, long text_justification);
extern void string_cache_flush(s_string_cache* string_cache, struct c_draw_string* draw_string, c_font_cache_base* font_cache);
