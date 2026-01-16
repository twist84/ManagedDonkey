#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files_windows.hpp"

struct s_async_io_file
{
	void* file;
	uns32 outstanding_io_count;
	uns32 io_offset;
};
COMPILE_ASSERT(sizeof(s_async_io_file) == 0xC);

// OVERLAPPED
struct s_overlapped
{
	uns32 internal;
	uns32 internal_high;
	uns32 offset;
	uns32 offset_high;
	void* event_handle;
};
COMPILE_ASSERT(sizeof(s_overlapped) == 0x14);

class c_asynchronous_io_marker
{
public:
	s_overlapped m_overlapped;
	uns32 m_unaligned_requested_size;
	c_synchronized_long m_io_status;
};
COMPILE_ASSERT(sizeof(c_asynchronous_io_marker) == 0x1C);

class c_asynchronous_io_arena
{
public:
	void close_file(int32 file_index);
	void close_file(s_indirect_file* indirect_file_handle);

	class c_idle_io_handler
	{
	public:
		enum e_state
		{
		};

		e_state m_state;
		uns32 m_idle_start_milliseconds;
		int32 m_on_idle_file_index;
		uns32 m_on_idle_file_offset;
		c_asynchronous_io_marker m_submit_on_idle_marker;
	};
	COMPILE_ASSERT(sizeof(c_idle_io_handler) == 0x2C);

	s_file_handle m_io_completion_port;
	c_static_flags<32> m_valid_files;
	c_static_array<s_async_io_file, 32> m_files;
	int32 m_outstanding_io_requests;
	c_idle_io_handler io_handler;
};
COMPILE_ASSERT(sizeof(c_asynchronous_io_arena) == 0x1B8);

