#pragma once

#include "cache/cache_file_bulk_reader.hpp"
#include "cache/cache_file_codec_work.hpp"
#include "cache/cache_files.hpp"
#include "cache/optional_cache.hpp"
#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files_windows.hpp"

static inline e_map_file_index operator-(e_map_file_index& value, int increment)
{
	return e_map_file_index((int)value - increment);
}

static inline e_map_file_index operator+(e_map_file_index& value, int increment)
{
	return e_map_file_index((int)value + increment);
}

static inline e_map_file_index operator-=(e_map_file_index& value, int increment)
{
	return value = e_map_file_index((int)value - increment);
}

static inline e_map_file_index operator+=(e_map_file_index& value, int increment)
{
	return value = e_map_file_index((int)value + increment);
}

static inline e_map_file_index operator--(e_map_file_index& value, int increment)
{
	return value = e_map_file_index((int)value - 1);
}

static inline e_map_file_index operator++(e_map_file_index& value, int increment)
{
	return value = e_map_file_index((int)value + 1);
}

struct s_cache_file_share_map // this probably isn't the correct name
{
	const char* file_path;
	e_map_file_index index;
	e_map_file_index previous_index;
};
COMPILE_ASSERT(sizeof(s_cache_file_share_map) == 0xC);

class c_cache_file_copy_optional_cache_callback :
	public c_optional_cache_user_callback
{
};
COMPILE_ASSERT(sizeof(c_cache_file_copy_optional_cache_callback) == sizeof(c_optional_cache_user_callback));

struct s_cached_map_file
{
	s_file_handle file_handle;

	// io_lock_count?
	uns32 __unknown4;

	s_cache_file_header header;
	s_indirect_file bulk_read_handle;
	s_file_handle overlapped_handle;
};
COMPILE_ASSERT(sizeof(s_cached_map_file) == 0x33A0);

struct s_failed_map
{
	c_static_string<k_tag_long_string_length> path;
	uns32 time;
	int32 index;
};
COMPILE_ASSERT(sizeof(s_failed_map) == 0x108);

struct s_cache_file_table_of_contents
{
	c_static_array<s_cached_map_file, k_total_tracked_cached_map_files_count> map_files;
	e_map_file_index open_map_file_index;
	e_map_file_index locked_map_file_index;
	e_map_file_index pending_map_file_index;
	e_cache_file_load_action pending_action;
	s_failed_map failed_maps[8];
};
COMPILE_ASSERT(sizeof(s_cache_file_table_of_contents) == 0x30EB0);

struct s_cache_copy_request
{
	c_static_string<k_tag_long_string_length> map_name;
	e_cache_file_load_action action;
};
COMPILE_ASSERT(sizeof(s_cache_copy_request) == 0x104);

struct s_cache_file_copy_globals
{
	s_cache_file_header copy_file_header;
	c_synchronized_long copy_bytes_transferred;
	uns32 copy_start_time_ms;
	uns32 intended_copy_bytes_transferred;
	uns32 total_copy_bytes_transferred;
	bool nuke_cache_file_success;
	bool validify_cache_file_success;
	uns32 volatile source_file_size;
	int32 copy_task_id;
	e_cache_copy_state last_copy_state;
	e_cache_copy_state copy_state;
	e_cache_copy_finish_reason finish_reason;
	c_synchronized_long copy_task_is_done;
	e_map_file_index copying_to_map_file_index;
	s_file_handle source_file_handle;
	c_reference_count<int32> reference_count;
	c_synchronized_long copy_abort_signal;
	bool copy_finished_success;
	void* buffer;
	int32 buffer_size;
	bool release_buffer;
	char copying_map_file_path[k_tag_long_string_length];
	s_cache_copy_request current_request;
	s_cache_copy_request pending_request;
	uns32 running_realtime_checksum;
};
COMPILE_ASSERT(sizeof(s_cache_file_copy_globals) == 0x36E8);

class c_cache_file_copy_fake_decompressor :
	public c_cache_file_decompressor
{
	virtual bool begin(c_basic_buffer<void> output_buffer)
	{
		throw;
	}
	virtual bool decompress_buffer(c_basic_buffer<void> input_buffer, c_basic_buffer<void>* out_decompressed_buffer)
	{
		throw;
	}
	virtual bool finish(c_basic_buffer<void>* out_decompressed_buffer)
	{
		throw;
	}

	void setup(s_file_handle file_handle, uns32 offset, uns32 checksum, c_basic_buffer<void> buffer);
	void teardown();

	s_file_handle m_output_file;
	c_synchronized_long m_copy_bytes_progress;
	c_synchronized_long m_copy_in_progress;
	uns32 m_running_checksum;
	bool m_running_success;
	bool m_overall_copy_in_progress;
	s_simple_read_file_ex_overlapped_result m_overlapped_result;
	byte m_overlapped[0x14]; // OVERLAPPED
	c_basic_buffer<void> m_copy_buffer;
};
COMPILE_ASSERT(sizeof(c_cache_file_copy_fake_decompressor) == sizeof(c_cache_file_decompressor) + 0x40);

extern c_cache_file_copy_optional_cache_callback& g_cache_file_copy_optional_cache_callback;
extern c_cache_file_copy_fake_decompressor& g_copy_decompressor;
extern bool& g_cache_files_read_from_map_directory;
extern s_cache_file_table_of_contents& cache_file_table_of_contents;
extern s_cache_file_copy_globals& cache_file_copy_globals;
extern c_asynchronous_io_arena& g_cache_file_io_arena;

extern bool __cdecl cached_map_file_is_shared(e_map_file_index map_file_index);
extern void __cdecl cache_file_block_until_not_busy();
extern void __cdecl cache_file_close();
extern void __cdecl cache_file_copy_allocate_buffer(int32 size);
extern bool __cdecl cache_file_copy_do_action();
extern void __cdecl cache_file_copy_release_buffer();
extern int32 __cdecl cache_file_get_absolute_maximum_size();
extern bool __cdecl cache_file_get_async_file_handle_from_index(e_map_file_index map_file_index, s_file_handle* out_handle);
extern c_static_string<256> __cdecl cache_file_get_canonical_path(const char* scenario_path);
extern int32 __cdecl cache_file_get_failed_count(const char* map_path);
extern bool __cdecl cache_file_get_indirect_file_handle_from_index(e_map_file_index map_file_index, s_indirect_file* out_handle);
extern bool __cdecl cache_file_get_master_async_file_handle(s_file_handle* out_handle);
extern bool __cdecl cache_file_get_master_indirect_file_handle(s_indirect_file* out_handle);
extern bool __cdecl cache_file_get_master_overlapped_file_handle(s_file_handle* out_handle);
extern bool __cdecl cache_file_get_master_resource_section_offset(uns32* out_offset);
extern bool __cdecl cache_file_get_overlapped_file_handle_from_index(e_map_file_index map_file_index, s_file_handle* out_handle);
extern void __cdecl cache_file_map_clear_all_failures();
extern void __cdecl cache_file_map_has_failed(const char* scenario_path);
extern bool __cdecl cache_file_open(const char* scenario_path, void* header);
extern int32 __cdecl cache_file_read_ex(int32 cache_file_section, int32 section_offset, int32 buffer_size, void* buffer, c_synchronized_long* size, c_synchronized_long* done, e_async_category category, e_async_priority priority);
extern int32 __cdecl cache_file_round_up_read_size(int32 size);
extern const s_cache_file_shared_resource_usage* __cdecl cache_file_try_to_get_master_shared_resource_usage();
extern void __cdecl cache_files_copy_do_work();
extern void __cdecl cache_files_copy_halt();
extern bool __cdecl cache_files_copy_halting();
extern bool __cdecl cache_files_copy_in_progress();
extern bool __cdecl cache_files_copy_in_progress_internal(real32* out_progress);
extern bool __cdecl cache_files_copy_map(const char* scenario_path, int32 load_action);
extern bool __cdecl cache_files_copy_map_start_only(const char* scenario_path, int32 load_action);
extern void __cdecl cache_files_copy_pause();
extern void __cdecl cache_files_copy_resume();
extern void __cdecl cache_files_copy_stop(const char* scenario_path);
extern void __cdecl cache_files_delete_all();
extern void __cdecl cache_files_delete_if_build_number_has_changed();
extern void __cdecl cache_files_delete_if_language_has_changed();
extern int32 __cdecl cache_files_get_file_status(const char* scenario_path);
extern bool __cdecl cache_files_has_map_terminal_failure(const char* scenario_path);
extern void __cdecl cache_files_initialize();
extern bool __cdecl cache_files_read_from_map_directory();
extern bool __cdecl cache_files_running_off_dvd();
extern int32 __cdecl cache_map_file_nuke(e_map_file_index map_file_index, e_async_category category, e_async_priority priority, bool* success, c_synchronized_long* done);
extern void __cdecl cache_requests_flush();
extern void __cdecl cached_map_file_close(e_map_file_index map_file_index);
extern bool __cdecl cached_map_file_dependencies_loaded(const s_cache_file_header* header, uns32* shared_files_flags);
extern s_cached_map_file* __cdecl cached_map_file_get(e_map_file_index map_file_index);
extern s_file_handle __cdecl cached_map_file_get_handle(e_map_file_index map_file_index);
extern uns32 __cdecl cached_map_file_get_size(e_map_file_index map_file_index);
extern bool __cdecl cached_map_file_load(e_map_file_index map_file_index, const char* scenario_path);
extern void __cdecl cached_map_files_delete(e_map_file_index start_map_file_index, e_map_file_index end_map_file_index);
extern e_map_file_index __cdecl cached_map_files_find_free_utility_drive_map(int32 size, int16 map_type);
extern e_map_file_index __cdecl cached_map_files_find_map(const char* scenario_path);
extern void __cdecl cached_map_files_open_all(bool* success);
extern void __cdecl canonicalize_map_path(const char* path, c_static_string<256>* out_path);
extern void __cdecl copy_and_strip_suffix(const char* path, c_static_string<256>* stripped_path);
extern s_failed_map* __cdecl find_failed_map(const char* scenario_path);
extern bool __cdecl map_names_are_equal(const char* map_name_a, const char* map_name_b);

