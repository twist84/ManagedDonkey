#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files_windows.hpp"

struct c_asynchronous_io_arena
{
public:
	void close_file(long a1);
	void close_file(s_indirect_file* indirect_file);

	struct s_async_io_file
	{
		s_file_handle file;
		long outstanding_io_count;
		dword __unknown8;
	};

	s_file_handle m_io_completion_port;
	c_static_flags<32> m_valid_files;
	c_static_array<s_async_io_file, 32> m_files;
	long m_outstanding_io_requests;
	long __unknown18C;

	byte __data190[4];
	long __unknown194;
	long __unknown198;
	byte __data19C[24];
	long __unknown1B4;
};
static_assert(sizeof(c_asynchronous_io_arena) == 0x1B8);

struct c_asynchronous_io_marker
{
	//LPOVERLAPPED
	void* m_overlapped;

	byte __data[0x10];
	dword m_unaligned_io_size;
	c_synchronized_long m_io_status;
};
static_assert(sizeof(c_asynchronous_io_marker) == 0x1C);

