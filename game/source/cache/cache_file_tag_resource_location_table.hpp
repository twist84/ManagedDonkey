#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/files_windows.hpp"
#include "tag_files/tag_groups.hpp"

struct s_cache_file_tag_resource_data;
struct c_cache_file_resource_uber_location_table
{
	c_wrapped_array<s_cache_file_tag_resource_data*>* m_locations;
};
static_assert(sizeof(c_cache_file_resource_uber_location_table) == 0x4);

struct s_cache_file_resource_streaming_sublocation
{
	long memory_offset;
	long memory_size;
};
static_assert(sizeof(s_cache_file_resource_streaming_sublocation) == 0x8);

struct s_cache_file_resource_streaming_sublocation_table
{
	long total_memory_size;
	c_typed_tag_block<s_cache_file_resource_streaming_sublocation> streaming_sublocations;
};
static_assert(sizeof(s_cache_file_resource_streaming_sublocation_table) == 0x10);

