#pragma once

#include "cache/cache_file_bulk_reader.hpp"
#include "cache/cache_file_codec_work.hpp"
#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files_windows.hpp"

const long k_cached_map_files_count = 15;

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
	c_static_array<s_cached_map_file, k_cached_map_files_count> map_files;
	long open_map_file_index;
	long locked_map_file_index;

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

struct s_cache_file_copy_globals
{
	s_cache_file_header header;

	c_synchronized_long copy_size;
	dword copy_time;

	// some size
	dword __unknown3398;

	dword total_copy_bytes_transferred;

	bool async_write_position_succeed;
	bool async_validify_file_succeed;

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

	long map_file_index;
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

	struct
	{
		c_static_string<k_tag_long_string_length> map_name;
		long action; // e_cache_file_load_action

		c_static_string<k_tag_long_string_length> pending_map_name;
		long pending_action; // e_cache_file_load_action
	} current_action;

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

	s_file_handle m_file_handle;
	c_synchronized_long m_file_offset;
	c_synchronized_long m_done;
	dword m_checksum;
	bool __unknown14;
	s_simple_read_file_ex_overlapped_result m_overlapped_result;
	byte m_overlapped[0x14]; // OVERLAPPED
	c_basic_buffer<void> m_buffer;
};
static_assert(sizeof(c_cache_file_copy_fake_decompressor) == sizeof(c_cache_file_decompressor) + 0x40);

extern c_cache_file_copy_fake_decompressor& g_copy_decompressor;
extern s_cache_file_table_of_contents& cache_file_table_of_contents;
extern s_cache_file_copy_globals& cache_file_copy_globals;
extern c_asynchronous_io_arena& g_cache_file_io_arena;

