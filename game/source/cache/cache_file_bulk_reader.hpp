#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files_windows.hpp"

struct s_async_io_file
{
	void* file;
	uint32 outstanding_io_count;
	uint32 io_offset;
};
static_assert(sizeof(s_async_io_file) == 0xC);

// OVERLAPPED
struct s_overlapped
{
	uint32 internal;
	uint32 internal_high;
	uint32 offset;
	uint32 offset_high;
	void* event_handle;
};
static_assert(sizeof(s_overlapped) == 0x14);

struct c_asynchronous_io_marker
{
	s_overlapped m_overlapped;
	uint32 m_unaligned_requested_size;
	c_synchronized_long m_io_status;
};
static_assert(sizeof(c_asynchronous_io_marker) == 0x1C);

struct c_asynchronous_io_arena
{
public:
	void close_file(int32 file_index);
	void close_file(s_indirect_file* indirect_file_handle);

	struct c_idle_io_handler
	{
		enum e_state
		{
		};

		e_state m_state;
		uint32 m_idle_start_milliseconds;
		int32 m_on_idle_file_index;
		uint32 m_on_idle_file_offset;
		c_asynchronous_io_marker m_submit_on_idle_marker;
	};
	static_assert(sizeof(c_idle_io_handler) == 0x2C);

	s_file_handle m_io_completion_port;
	c_static_flags<32> m_valid_files;
	c_static_array<s_async_io_file, 32> m_files;
	int32 m_outstanding_io_requests;
	c_idle_io_handler io_handler;
};
static_assert(sizeof(c_asynchronous_io_arena) == 0x1B8);

