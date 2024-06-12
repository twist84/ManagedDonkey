#pragma once

#include "cache/cache_file_bulk_reader.hpp"
#include "cache/cache_file_codec_work.hpp"
#include "cache/cache_files.hpp"
#include "cache/optional_cache.hpp"
#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files_windows.hpp"

enum e_map_file_index
{
	_map_file_index_shared_ui = 0,
	_map_file_index_shared_resources,
	_map_file_index_shared_textures,
	_map_file_index_shared_textures_b,
	_map_file_index_shared_audio,
	_map_file_index_shared_video,

	k_cached_map_file_shared_count = 6,
	k_total_tracked_cached_map_files_count = 15,
	k_no_cached_map_file_index = -1,
};

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
	char const* file_path;
	e_map_file_index index;
	e_map_file_index previous_index;
};
static_assert(sizeof(s_cache_file_share_map) == 0xC);

struct c_cache_file_copy_optional_cache_callback :
	public c_optional_cache_user_callback
{
};
static_assert(sizeof(c_cache_file_copy_optional_cache_callback) == sizeof(c_optional_cache_user_callback));

struct s_cached_map_file
{
	s_file_handle file_handle;

	// io_lock_count?
	dword __unknown4;

	s_cache_file_header header;
	s_indirect_file indirect_file;
	s_file_handle overlapped_handle;
};
static_assert(sizeof(s_cached_map_file) == 0x33A0);

struct s_failed_map
{
	c_static_string<k_tag_long_string_length> path;
	dword time;
	long index;
};
static_assert(sizeof(s_failed_map) == 0x108);

struct s_cache_file_table_of_contents
{
	c_static_array<s_cached_map_file, k_total_tracked_cached_map_files_count> map_files;
	e_map_file_index open_map_file_index;
	e_map_file_index locked_map_file_index;

	long __unknown30668; // best_map_file_index
	long load_action; // e_cache_file_load_action

	// it seems the cache file system still tries to load the shared and campaign map files
	// failed_maps[0].path == "maps\shared.map"
	// failed_maps[1].path == "maps\campaign.map"
	c_static_array<s_failed_map, 8> failed_maps;
};
static_assert(sizeof(s_cache_file_table_of_contents) == 0x30EB0);

enum e_cache_copy_state
{
	_cache_copy_state_idle = 0,
	_cache_copy_state_mount_dlc,
	_cache_copy_state_create_source_file,
	_cache_copy_state_verify_create_source_file,
	_cache_copy_state_read_dvd_header,
	_cache_copy_state_verify_read_dvd_header,
	_cache_copy_state_get_dvd_file_size,
	_cache_copy_state_verify_get_dvd_file_size,
	_cache_copy_state_find_free_map_and_clear_header,
	_cache_copy_state_verify_find_free_map_and_clear_header,
	_cache_copy_state_flush_cleared_header,
	_cache_copy_state_start_copying_map_data,
	_cache_copy_state_copy_map_data,
	_cache_copy_state_verify_copy_map_data,
	_cache_copy_state_prepare_copy_thread,
	_cache_copy_state_kick_off_copy_thread,
	_cache_copy_state_finish_copy_from_thread,
	_cache_copy_state_flush_after_copy,
	_cache_copy_state_write_header,
	_cache_copy_state_verify_write_header,
	_cache_copy_state_mark_file_as_loaded,
	_cache_copy_state_halt,
	_cache_copy_state_close_source_file,
	_cache_copy_state_finish,

	k_number_of_cache_copy_states
};

struct s_cache_file_load_action
{
	c_static_string<k_tag_long_string_length> map_name;
	long action; // e_cache_file_load_action
};
static_assert(sizeof(s_cache_file_load_action) == 0x104);

struct s_cache_file_copy_globals
{
	s_cache_file_header header;

	c_synchronized_long copy_size;
	dword copy_time;

	// some size
	dword __unknown3398;

	dword total_copy_bytes_transferred;

	bool async_write_position_succeeded;
	bool async_validify_file_succeeded;

	dword source_file_size;

	long copy_task_id;

	c_enum<e_cache_copy_state, long, _cache_copy_state_idle, k_number_of_cache_copy_states> valid_copy_state;
	c_enum<e_cache_copy_state, long, _cache_copy_state_idle, k_number_of_cache_copy_states> copy_state;

	// 0: _cache_copy_state_idle
	// 1: _cache_copy_state_mark_file_as_loaded
	// 2: _cache_copy_state_halt
	// 3: also _cache_copy_state_halt?
	long __unknown33B4;

	c_synchronized_long copy_task_is_done;

	e_map_file_index map_file_index;
	s_file_handle source_file_handle;

	// cache_files_copy_pause
	// cache_files_copy_resume
	c_reference_count<long> reference_count;

	c_synchronized_long copy_task_abort_signal;
	bool copy_task_decompression_success;

	c_basic_buffer<void> buffer;

	bool copy_paused;

	c_static_string<k_tag_long_string_length> source_file;

	byte __pad34D9[3];

	s_cache_file_load_action current_load_action;
	s_cache_file_load_action pending_load_action;

	dword checksum;
};
static_assert(sizeof(s_cache_file_copy_globals) == 0x36E8);

struct c_cache_file_copy_fake_decompressor :
	public c_cache_file_decompressor
{
	virtual bool begin(c_basic_buffer<void> a1)
	{
		throw;
	}
	virtual bool decompress_buffer(c_basic_buffer<void> a1, c_basic_buffer<void>* a2)
	{
		throw;
	}
	virtual bool finish(c_basic_buffer<void>* a1)
	{
		throw;
	}

	void setup(s_file_handle file_handle, dword offset, dword checksum, c_basic_buffer<void> buffer);
	void teardown();

	s_file_handle m_file_handle;
	c_synchronized_long m_file_offset;
	c_synchronized_long m_done;
	dword m_checksum;
	bool __unknown14;
	bool m_overall_copy_in_progress;
	byte __unknown16;
	byte __unknown17;
	s_simple_read_file_ex_overlapped_result m_overlapped_result;
	byte m_overlapped[0x14]; // OVERLAPPED
	c_basic_buffer<void> m_buffer;
};
static_assert(sizeof(c_cache_file_copy_fake_decompressor) == sizeof(c_cache_file_decompressor) + 0x40);

extern c_cache_file_copy_optional_cache_callback& g_cache_file_copy_optional_cache_callback;
extern c_cache_file_copy_fake_decompressor& g_copy_decompressor;
extern bool& g_cache_files_are_absolute;
extern s_cache_file_table_of_contents& cache_file_table_of_contents;
extern s_cache_file_copy_globals& cache_file_copy_globals;
extern c_asynchronous_io_arena& g_cache_file_io_arena;

enum e_async_category;
enum e_async_priority;

extern bool __cdecl cached_map_file_is_shared(e_map_file_index map_file_index);
extern void __cdecl cache_file_block_until_not_busy();
extern void __cdecl cache_file_close();
extern void __cdecl cache_file_copy_allocate_buffer(long size);
extern bool __cdecl cache_file_copy_do_action();
extern void __cdecl cache_file_copy_release_buffer();
extern long __cdecl cache_file_get_absolute_maximum_size();
extern bool __cdecl cache_file_get_async_file_handle_from_index(e_map_file_index map_file_index, s_file_handle* out_handle);
extern c_static_string<256> __cdecl cache_file_get_canonical_path(char const* scenario_path);
extern long __cdecl cache_file_get_failed_count(char const* map_path);
extern bool __cdecl cache_file_get_indirect_file_handle_from_index(e_map_file_index map_file_index, s_indirect_file* out_handle);
extern bool __cdecl cache_file_get_master_async_file_handle(s_file_handle* out_handle);
extern bool __cdecl cache_file_get_master_indirect_file_handle(s_indirect_file* out_handle);
extern bool __cdecl cache_file_get_master_overlapped_file_handle(s_file_handle* out_handle);
extern bool __cdecl cache_file_get_master_resource_section_offset(dword* out_offset);
extern bool __cdecl cache_file_get_overlapped_file_handle_from_index(e_map_file_index map_file_index, s_file_handle* out_handle);
extern void __cdecl cache_file_map_clear_all_failures();
extern void __cdecl cache_file_map_has_failed(char const* scenario_path);
extern bool __cdecl cache_file_open(char const* scenario_path, void* header);
extern long __cdecl cache_file_round_up_read_size(long size);
extern s_cache_file_shared_resource_usage const* __cdecl cache_file_try_to_get_master_shared_resource_usage();
extern void __cdecl cache_files_copy_do_work();
extern void __cdecl cache_files_copy_halt();
extern bool __cdecl cache_files_copy_halting();
extern bool __cdecl cache_files_copy_in_progress();
extern bool __cdecl cache_files_copy_in_progress_internal(real* out_progress);
extern bool __cdecl cache_files_copy_map(char const* scenario_path, long load_action);
extern bool __cdecl cache_files_copy_map_start_only(char const* scenario_path, long load_action);
extern void __cdecl cache_files_copy_pause();
extern void __cdecl cache_files_copy_resume();
extern void __cdecl cache_files_copy_stop(char const* scenario_path);
extern void __cdecl cache_files_delete_all();
extern void __cdecl cache_files_delete_if_build_number_has_changed();
extern void __cdecl cache_files_delete_if_language_has_changed();
extern long __cdecl cache_files_get_file_status(char const* scenario_path);
extern bool __cdecl cache_files_has_map_terminal_failure(char const* scenario_path);
extern void __cdecl cache_files_initialize();
extern bool __cdecl cache_files_running_off_dvd();
extern long __cdecl cache_map_file_nuke(e_map_file_index map_file_index, e_async_category category, e_async_priority priority, bool* success, c_synchronized_long* done);
extern void __cdecl cache_requests_flush();
extern void __cdecl cached_map_file_close(e_map_file_index map_file_index);
extern bool __cdecl cached_map_file_dependencies_loaded(s_cache_file_header const* header, dword* shared_files_flags);
extern s_cached_map_file* __cdecl cached_map_file_get(e_map_file_index map_file_index);
extern s_file_handle __cdecl cached_map_file_get_handle(e_map_file_index map_file_index);
extern dword __cdecl cached_map_file_get_size(e_map_file_index map_file_index);
extern bool __cdecl cached_map_file_load(e_map_file_index map_file_index, char const* scenario_path);
extern void __cdecl cached_map_files_delete(e_map_file_index start_map_file_index, e_map_file_index end_map_file_index);
extern e_map_file_index __cdecl cached_map_files_find_free_utility_drive_map(long size, short map_type);
extern e_map_file_index __cdecl cached_map_files_find_map(char const* scenario_path);
extern void __cdecl cached_map_files_open_all(bool* success);
extern void __cdecl canonicalize_map_path(char const* path, c_static_string<256>* out_path);
extern void __cdecl copy_and_strip_suffix(char const* path, c_static_string<256>* stripped_path);
extern s_failed_map* __cdecl find_failed_map(char const* scenario_path);
extern bool __cdecl map_names_are_equal(char const* map_name_a, char const* map_name_b);

