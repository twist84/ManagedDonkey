#pragma once

#include "tag_files/files.hpp"

enum e_event_log_flags
{
	_event_log_append_to_file_bit = 0,
	_event_log_place_in_root_bit,
	_event_log_only_for_custom_subfolder,
	_event_log_disable_file_trimming,

	k_event_log_flag_count,
};
typedef c_flags<e_event_log_flags, uns16, k_event_log_flag_count> c_event_log_flags;

struct c_file_output_buffer
{
	s_file_reference* m_file;
	char m_buffer[16384];
	int32 m_buffer_length;
};
static_assert(sizeof(c_file_output_buffer) == 0x4008);

struct s_event_log
{
	char name[256];
	c_event_log_flags event_log_flags;
	c_file_output_buffer output_buffer;
	s_file_reference stream;
	bool opened_at_least_once;
	bool first_line_displayed;
	bool stream_open;
};
static_assert(sizeof(s_event_log) == 0x4220);

struct s_event_log_globals
{
	bool initialized;
	bool cache_event_log_output;
	char subfolder[128];
	c_static_array<s_event_log, 32> event_logs;
	int32 event_log_count;
};
static_assert(sizeof(s_event_log_globals) == 0x84488);

struct s_event_log_cache_entry
{
	c_flags<int32, uns32, 32> event_log_indices;
	const char* error_string;
};
static_assert(sizeof(s_event_log_cache_entry) == 0x8);

struct s_event_log_cache
{
	uns32 last_flush_time;
	c_flags<int32, uns32, 32> cached_categories;
	c_static_stack<s_event_log_cache_entry, 1024> entry_stack;
	char entry_string_cache[32768];
	int32 entry_string_cache_size;
};
static_assert(sizeof(s_event_log_cache) == 0xA010);

extern s_event_log_globals event_log_globals;
extern s_event_log_cache g_event_log_cache;

extern s_event_log* event_log_get(int32 event_log_index);
extern int32 event_log_new(const char* event_log_name, c_event_log_flags event_log_flags);
extern void event_logs_close();
extern void event_logs_dispose();
extern void event_logs_dispose_atexit();
extern void event_logs_flush();
extern bool event_logs_get_active_subfolder(const char** p_subfolder);
extern void event_logs_initialize();
extern void event_logs_obtain_report_directory_lock();
extern void event_logs_release_report_directory_lock();
extern void event_logs_snapshot();
extern void event_logs_specify_subfolder(const char* subfolder);
extern bool event_logs_usable();
extern bool event_logs_using_subfolder();
extern void event_logs_work_function();
extern void flush_event_log_cache();
extern void write_event_log_cache_entry(bool use_report_buffers, int32 event_log_index, const char* string, bool flush);
extern void write_to_event_log(const int32* event_log_indices, int32 event_log_count, const char* string);
extern void write_to_event_log_cache(const long* event_log_indices, long event_log_count, const char* string);

