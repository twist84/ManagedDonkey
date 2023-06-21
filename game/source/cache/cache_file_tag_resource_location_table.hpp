#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "tag_files/files_windows.hpp"

struct s_cache_file_tag_resource_data;
struct c_cache_file_resource_uber_location_table
{
	c_wrapped_array<s_cache_file_tag_resource_data>* m_locations;
	long __unknown4;
};

struct s_cache_file_shared_resource_usage;
struct c_cache_file_resource_header_location_table
{
	struct s_header_file_location :
		s_datum_header
	{
		word_flags flags;
		s_file_handle async_file_handle;
		s_file_handle overlapped_handle;
		s_indirect_file indirect_file;
		s_cache_file_shared_resource_usage* shared_resource_usage;
		dword resource_section_offset;
		long map_file_index;
	};
	static_assert(sizeof(s_header_file_location) == 0x1C);

	c_static_array<long, 7> m_header_file_location_handles;
	long m_header_file_location_handle_index;
	c_smart_data_array<s_header_file_location>* m_header_file_locations;
	c_cache_file_resource_uber_location_table m_uber_location_table;
};
