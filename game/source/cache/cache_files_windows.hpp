#pragma once

#include "cseries/cseries.hpp"
#include "cache/cache_files.hpp"
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
	char path[256];
	dword time;
	long index;
};
static_assert(sizeof(s_failed_map) == 0x108);

struct s_cache_file_table_of_contents
{
	c_static_array<s_cached_map_file, k_cached_map_files_count> map_files;
	long open_map_file_index;

	// s_map_file_table
	long map_file_index_start;
	long map_file_index_end;
	long map_file_size;

	c_static_array<s_failed_map, 8> failed_maps;
};
static_assert(sizeof(s_cache_file_table_of_contents) == 0x30EB0);

struct s_cache_file_copy_globals
{
	s_cache_file_header header;

	byte __data3390[0x358];
};
static_assert(sizeof(s_cache_file_copy_globals) == 0x36E8);

extern s_cache_file_table_of_contents& cache_file_table_of_contents;
extern s_cache_file_copy_globals& cache_file_copy_globals;