#pragma once

#include "cseries/cseries.hpp"
#include "memory/read_write_lock.hpp"

enum e_event_context_query_destination_type
{
	_event_context_query_destination_console = 0,
	_event_context_query_destination_log,
	_event_context_query_destination_remote_log,

	k_event_context_query_destination_console_type_count
};

struct s_spamming_event
{
	uns32 last_spam_time;
	int32 hit_count;
	char spam_text[2048];
	bool valid;
};
static_assert(sizeof(s_spamming_event) == 0x80C);

struct s_event_category_default_configuration
{
	const char* name;
	e_event_level initial_display_level;
	real_rgb_color initial_display_color;
	e_event_level initial_log_level;
	const char* log_name;
	void(__cdecl* log_format_func)(char*, int32);
	e_event_level initial_remote_log_level;
};
static_assert(sizeof(s_event_category_default_configuration) == 0x24);

struct s_event_category
{
	int16 depth;
	char name[64];
	int32 event_log_index;
	e_event_level current_display_level;
	real_rgb_color current_display_color;
	uns32 last_event_time;
	int32 possible_spam_event_count;
	e_event_level current_log_level;
	char log_name[256];
	void(__cdecl* log_format_func)(char*, int32);
	e_event_level current_remote_log_level;
	e_event_level current_debugger_break_level;
	e_event_level current_halt_level;
	e_event_level current_force_display_level;
	uns32 event_listeners;
	int32 parent_index;
	int32 first_child_index;
	int32 sibling_index;
};
static_assert(sizeof(s_event_category) == 0x188);

class c_event_listener_base
{
	char m_categories[128];
	int32 m_category_index;
	int32 m_event_listener_index;
};
static_assert(sizeof(c_event_listener_base) == 0x88);

class c_event_listener : 
	public c_event_listener_base
{
public:
	virtual void handle_event(e_event_level, const char*) = 0;
};
static_assert(sizeof(c_event_listener) == sizeof(c_event_listener_base) + sizeof(void*));

struct s_event_globals
{
	e_event_level current_display_level;
	e_event_level current_log_level;
	e_event_level current_remote_log_level;
	e_event_level current_minimum_level;
	e_event_level current_minimum_category_level;
	c_static_array<s_event_category, 1024> categories;
	int32 category_count;
	int32 console_suppression_old_time;
	int32 console_suppression_count;
	int32 console_suppression_old_line_check_time;
	int16 message_buffer_size;
	char message_buffer[k_error_message_buffer_maximum_size];
	int32 external_primary_event_log_index;
	int32 internal_primary_event_log_index;
	int32 internal_primary_full_event_log_index;
	int32 subfolder_internal_primary_event_log_index;
	int32 subfolder_internal_primary_full_event_log_index;
	int32 event_index;
	c_static_array<c_event_listener*, 8> event_listeners;
	c_static_array<s_spamming_event, 64> spamming_event_list;
	uns32 last_console_response_event_time;
	bool enable_events;
	bool enable_spam_suppression;
	bool dump_to_stderr;
	bool disable_event_suppression;
	bool disable_event_log_trimming;
	bool disable_event_logging;
	bool suppress_console_display_and_show_spinner;
	int32 permitted_thread_bits;
};
static_assert(sizeof(s_event_globals) == 0x82B70);

class c_event
{
public:
	c_event(e_event_level event_level, int32 event_category_index, uns32 event_response_suppress_flags);

	bool query();
	int32 generate(const char* format, ...);

protected:
	e_event_level m_event_level;
	int32 m_event_category_index;
	uns32 m_event_response_suppress_flags;
};
static_assert(sizeof(c_event) == 0xC);

class c_event_context_string_builder
{
public:
	c_event_context_string_builder(const char* description, ...);
	const char* get_string() const;

	char m_string[128];
};

class c_event_context
{
public:
	c_event_context(const char* type, bool display_to_console, c_event_context_string_builder* event_context_string_builder);
	~c_event_context();
};

struct s_event_context
{
	char type[64];
	char description[128];
	bool display_to_console;
};
static_assert(sizeof(s_event_context) == 0xC1);

extern bool g_events_debug_render_enable;
extern const char* const k_error_snapshot_directory;
extern const char* const k_reports_directory_root_name;
extern const char* const k_reports_directory_name;
extern const char* const k_primary_event_log_filename;
extern const char* const k_primary_full_event_log_filename;

inline thread_local bool g_recursion_lock = false;
inline thread_local int32 g_event_context_stack_depth = 0;
inline thread_local int32 g_event_context_stack_failure_depth = 0;
inline thread_local s_event_context g_event_context_stack[32]{};

extern s_event_globals event_globals;
extern bool g_events_initialized;
extern bool events_force_no_log;
extern c_read_write_lock g_event_read_write_lock;

extern const char* const k_event_level_names[k_event_level_count + 1];
extern const char* const k_event_level_severity_strings[k_event_level_count];

struct s_file_reference;
extern s_file_reference* __cdecl create_report_file_reference(s_file_reference* reference, const char* name, bool place_in_report_directory);
extern void events_clear();
extern void events_debug_render();
extern void events_dispose();
extern const char* events_get();
extern void events_initialize();
extern void event_initialize_primary_logs();
extern int32 event_interlocked_compare_exchange(int32 volatile* destination, int32 exchange, int32 comperand);
extern void reset_event_message_buffer();
extern void __cdecl network_debug_print(const char* format, ...);

//#define USE_CONSOLE_FOR_EVENTS

// $TODO decide if we want to wrap this macro in a debug check
//#if defined(_DEBUG)

#ifdef USE_CONSOLE_FOR_EVENTS
#define event(severity, ...) do { c_console::write_line(__VA_ARGS__); } while (false)
#define event_no_console(severity, ...) do { /* $IMPLEMENT */ } while (false)
#else
#define event(severity, ...) \
do { \
	static int32 volatile x_event_category_index = NONE; \
	c_event local_event(severity, x_event_category_index, 0); \
	if (local_event.query()) \
	{ \
		int32 event_category_index = local_event.generate(__VA_ARGS__); \
		if (x_event_category_index == NONE) \
		{ \
			event_interlocked_compare_exchange(&x_event_category_index, event_category_index, NONE); \
		} \
	} \
} while (false)
#define event_no_console(severity, ...) do { /* $IMPLEMENT */ } while (false)
#endif

//#else
//#define event(severity, ...) do { } while (false)
//#define event_no_console(severity, ...) do { } while (false)
//#endif

