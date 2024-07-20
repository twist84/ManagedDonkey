#pragma once

#include "cseries/cseries.hpp"
#include "memory/read_write_lock.hpp"
#include "multithreading/synchronized_value.hpp"

enum e_event_level
{
	_event_level_none = NONE,

	_event_level_verbose,
	_event_level_status,
	_event_level_message,
	_event_level_warning,
	_event_level_error,
	_event_level_critical,

	k_event_level_count,
};

enum e_category_properties_flags
{
	_category_properties_flag_display_level_bit = 0,
	_category_properties_flag_log_level_bit,
	_category_properties_flag_remote_log_level_bit,
	_category_properties_flag_debugger_break_level_bit,
	_category_properties_flag_halt_level_bit,
	_category_properties_flag_bit5,

	k_category_properties_flags
};

struct s_spamming_event
{
	dword hit_time;
	long hit_count;
	char text[2048];
	bool valid;
};
static_assert(sizeof(s_spamming_event) == 0x80C);

struct s_event
{
	char const* name;
	e_event_level display_level;
	real_rgb_color color;
	e_event_level log_level;
	char const* log_file;
	void(__cdecl* build_buffer_for_log_proc)(char*, long);
	e_event_level remote_log_level;
};
static_assert(sizeof(s_event) == 0x24);

struct s_event_category
{
	short __unknown0;
	c_static_string<64> name;
	long event_log_index;
	e_event_level display_level;
	real_rgb_color color;
	dword __time58;
	long __unknown5C;
	e_event_level log_level;
	c_static_string<k_tag_long_string_length> log_file;
	void(__cdecl* build_buffer_for_log_proc)(char*, long);
	e_event_level remote_log_level;
	e_event_level debugger_break_level;
	e_event_level halt_level;
	long __unknown174;
	long parent_index;
	long __unknown17C;
	long __unknown180;
};
static_assert(sizeof(s_event_category) == 0x184);

struct s_event_globals
{
	e_event_level display_level;
	e_event_level log_level;
	e_event_level remote_log_level;
	e_event_level query_level;
	long __unknown10;

	c_static_array<s_event_category, 1024> categories;
	long category_count;

	long __unknown61018_time;
	e_event_level __unknown6101C_level;
	long __unknown61020_time;

	short error_message_length;
	char error_message_buffer[2048];

	long external_primary_event_log_index;
	long internal_primary_event_log_index;
	long internal_primary_full_event_log_index;
	long subfolder_internal_primary_event_log_index;
	long subfolder_internal_primary_full_event_log_index;

	long event_listener_count;
	c_static_array<struct c_event_listener*, 8> event_listeners;

	c_static_array<s_spamming_event, 64> spamming_events;

	bool enabled;
	bool spam_suppression_enabled;
	bool __unknown81B62;
	bool suppression_disabled;
	bool event_log_flags_bit3_enabled;
	bool __unknown81B65;
	short __unknown81B66;

	long thread_query_flags;
};
static_assert(sizeof(s_event_globals) == 0x81B6C);

struct c_event
{
public:
	c_event(e_event_level event_level, long category_index, dword_flags event_flags);

	bool query();
	long generate(char const* event_name, ...);

protected:
	e_event_level m_event_level;
	long m_category_index;
	dword_flags m_event_flags;
};
static_assert(sizeof(c_event) == 0xC);

extern s_event_globals event_globals;
extern bool g_events_initialized;
extern c_read_write_lock g_event_read_write_lock;

extern char const* const k_event_level_names[k_event_level_count + 1];
extern char const* const k_event_level_severity_strings[k_event_level_count];
extern char const* const k_primary_event_log_filename;
extern char const* const k_primary_full_event_log_filename;

extern bool g_events_debug_render_enable;

struct s_file_reference;
extern s_file_reference* __cdecl create_report_file_reference(s_file_reference* info, char const* filename, bool use_sub_directory);
extern void __cdecl events_debug_render();
extern char const* __cdecl events_get();
extern void __cdecl events_initialize();
extern long __cdecl event_interlocked_compare_exchange(c_interlocked_long& value, long ExChange, long Comperand);
extern void __cdecl network_debug_print(char const* format, ...);

// I don't like this :(
template<typename... parameters_t, long k_parameter_count = sizeof...(parameters_t)>
void generate_event(e_event_level event_level, char const* event_name, parameters_t... parameters)
{
	c_interlocked_long event_category = NONE;
	c_event _event(event_level, event_category, 0);
	if (_event.query())
	{
		//long category_index = _event.generate(event_name, parameters...);
		//if (event_category == NONE)
		//	event_interlocked_compare_exchange(event_category, category_index, NONE);

		c_console::write_line(event_name, parameters...);
	}
}

// I like this a little more than the one above :(
//template<typename char_t, typename... parameters_t, long k_parameter_count = sizeof...(parameters_t)>
//void generate_event(e_event_level event_level, char_t const* event_name, parameters_t... parameters)
//{
//	c_interlocked_long event_category = NONE;
//	c_event _event(event_level, event_category, 0);
//	if (_event.query())
//	{
//		long category_index = _event.generate(event_name, parameters...);
//		if (event_category == NONE)
//			event_interlocked_compare_exchange(event_category, category_index, NONE);
//
//		c_console::write_line(event_name, parameters...);
//	}
//}

