#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files_windows.hpp"

struct s_simple_read_file_ex_overlapped_result
{
	c_synchronized_long done;
	uint32 error_code;
	uint32 bytes_transferred;
	uint32 bytes_requested;
};
static_assert(sizeof(s_simple_read_file_ex_overlapped_result) == 0x10);

struct c_asynchronous_io_marker;
struct c_cache_file_decompressor;
struct c_cache_file_async_decompression_task
{
	long m_task_id;
	bool m_task_done;
	c_synchronized_long m_task_done_async;
	c_synchronized_long m_task_abort_signal;
	c_asynchronous_io_marker* m_task_io_marker;
	bool m_task_success;
	long m_active_decompressor_identifier;
	c_cache_file_decompressor* m_active_decompressor;
	c_basic_buffer<void> m_active_decompression_buffer;
};
static_assert(sizeof(c_cache_file_async_decompression_task) == 0x28);

long __cdecl async_decompress_file_section(
	e_async_priority priority,
	s_file_handle file_handle,
	uint32 offset,
	uint32 size,
	uint32 const* verify_decompression_hash,
	c_basic_buffer<void> decompression_scratch_buffer,
	c_cache_file_decompressor* decompressor,
	c_basic_buffer<void> uncompressed_output_buffer,
	bool* out_decompression_success,
	c_synchronized_long* in_abort_signal,
	c_synchronized_long* out_bytes_read,
	c_synchronized_long* out_done);

