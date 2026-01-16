#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/files_windows.hpp"
#include "tag_files/tag_groups.hpp"

struct s_cache_file_tag_resource_data;
class c_cache_file_resource_uber_location_table
{
	c_wrapped_array<s_cache_file_tag_resource_data*>* m_locations;
};
COMPILE_ASSERT(sizeof(c_cache_file_resource_uber_location_table) == 0x4);

struct s_cache_file_resource_streaming_sublocation
{
	int32 memory_offset;
	int32 memory_size;
};
COMPILE_ASSERT(sizeof(s_cache_file_resource_streaming_sublocation) == 0x8);

struct s_cache_file_resource_streaming_sublocation_table
{
	int32 total_memory_size;
	c_typed_tag_block<s_cache_file_resource_streaming_sublocation> streaming_sublocations;
};
COMPILE_ASSERT(sizeof(s_cache_file_resource_streaming_sublocation_table) == 0x10);

