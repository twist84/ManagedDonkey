#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/files_windows.hpp"

struct s_cache_file_shared_resource_usage
{
	byte __data[0x2328];
};
static_assert(sizeof(s_cache_file_shared_resource_usage) == 0x2328);

struct s_cache_file_insertion_point_resource_usage
{
	byte __data[0xB4];
};
static_assert(sizeof(s_cache_file_insertion_point_resource_usage) == 0xB4);

struct c_asynchronous_io_arena
{
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

extern c_asynchronous_io_arena& g_cache_file_io_arena;

struct s_tag_resource;
extern void* __cdecl tag_resource_get(s_tag_resource const* resource);

