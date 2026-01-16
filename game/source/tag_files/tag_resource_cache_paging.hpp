#pragma once

#include "cseries/cseries.hpp"

class c_tag_resource_cache_new;
class c_tag_resource_page_table_io_listener
{
public:
	c_tag_resource_cache_new* m_resource_cache_new;
};
COMPILE_ASSERT(sizeof(c_tag_resource_page_table_io_listener) == 0x4);

struct s_tag_resource_page_datum :
	s_datum_header
{
	// 1: c_io_result::set_disk_failure
	// 2: c_io_result::set_checksum_failure
	uns8 flags;

	uns8 state;
	byte __data4[0x2];
	c_reference_count<int16> reference_count;
	int32 reservation;
	c_basic_buffer<void> buffer;
};
COMPILE_ASSERT(sizeof(s_tag_resource_page_datum) == 0x14);

struct s_lruv_cache;
class c_tag_resource_page_range_allocator;
class c_tag_resource_cache_file_reader;
class c_cache_file_tag_resource_location_handler;
class c_tag_resource_cache_published_location_interface;
class c_tag_resource_page_table_control_interface;
class c_tag_resource_page_table
{
public:
	class c_tag_resource_lruv_cache
	{
		int32 m_allocation_alignment;
		c_tag_resource_page_range_allocator* m_storage_allocator;
		s_lruv_cache* m_lruv_cache;
		c_basic_buffer<void> m_lruv_data;
	};
	COMPILE_ASSERT(sizeof(c_tag_resource_lruv_cache) == 0x14);

	struct s_pending_state
	{
		c_data_iterator<void> page_iterator;
		bool finished_pending_iteration;
	};

	c_smart_data_array<s_tag_resource_page_datum> m_resource_page_data;
	c_tag_resource_page_table::c_tag_resource_lruv_cache m_resource_page_lruv_cache;
	s_data_array* m_pending_resource_requests;
	c_wrapped_flags m_ready_pages_flags;
	c_wrapped_flags m_pinned_pages_flags;
	c_tag_resource_cache_file_reader* m_cache_file_reader;
	c_cache_file_tag_resource_location_handler* m_resource_location_handler;
	c_tag_resource_cache_published_location_interface* m_published_resource_interface;
	c_allocation_base* m_tracking_data_allocator;
	void* m_tracking_allocation_base;
	c_tag_resource_page_table_control_interface* m_control_interface;
	c_tag_resource_page_table::s_pending_state m_pending_state;
	uns32 m_total_prefetch_bytes;
	uns32 m_current_prefetch_bytes;
	bool m_prefetching_can_run;
	bool m_status_lines_enabled;
};
COMPILE_ASSERT(sizeof(c_tag_resource_page_table) == 0x64);
COMPILE_ASSERT(0x00 == OFFSETOF(c_tag_resource_page_table, m_resource_page_data));
COMPILE_ASSERT(0x04 == OFFSETOF(c_tag_resource_page_table, m_resource_page_lruv_cache));
COMPILE_ASSERT(0x18 == OFFSETOF(c_tag_resource_page_table, m_pending_resource_requests));
COMPILE_ASSERT(0x1C == OFFSETOF(c_tag_resource_page_table, m_ready_pages_flags));
COMPILE_ASSERT(0x24 == OFFSETOF(c_tag_resource_page_table, m_pinned_pages_flags));
COMPILE_ASSERT(0x2C == OFFSETOF(c_tag_resource_page_table, m_cache_file_reader));
COMPILE_ASSERT(0x30 == OFFSETOF(c_tag_resource_page_table, m_resource_location_handler));
COMPILE_ASSERT(0x34 == OFFSETOF(c_tag_resource_page_table, m_published_resource_interface));
COMPILE_ASSERT(0x38 == OFFSETOF(c_tag_resource_page_table, m_tracking_data_allocator));
COMPILE_ASSERT(0x3C == OFFSETOF(c_tag_resource_page_table, m_tracking_allocation_base));
COMPILE_ASSERT(0x40 == OFFSETOF(c_tag_resource_page_table, m_control_interface));
COMPILE_ASSERT(0x44 == OFFSETOF(c_tag_resource_page_table, m_pending_state));
COMPILE_ASSERT(0x58 == OFFSETOF(c_tag_resource_page_table, m_total_prefetch_bytes));
COMPILE_ASSERT(0x5C == OFFSETOF(c_tag_resource_page_table, m_current_prefetch_bytes));
COMPILE_ASSERT(0x60 == OFFSETOF(c_tag_resource_page_table, m_prefetching_can_run));
COMPILE_ASSERT(0x61 == OFFSETOF(c_tag_resource_page_table, m_status_lines_enabled));

struct s_indirect_cache_file_sub_location
{
	uns32 offset;
	uns32 unaligned_read_size;
	uns32 used_storage_size;
	int32 decompressor_identifier;
	bool valid_checksum;
	uns32 checksum;
};
COMPILE_ASSERT(sizeof(s_indirect_cache_file_sub_location) == 0x18);

struct s_indirect_cache_file_read_request
{
	c_basic_buffer<void> buffer;
	uns64 location_identifier;
	s_indirect_cache_file_sub_location sub_location;
};
COMPILE_ASSERT(sizeof(s_indirect_cache_file_read_request) == 0x28);

enum e_indirect_cache_file_read_result
{
	_indirect_cache_file_read_result_success = 0,
	_indirect_cache_file_read_result_location_invalid,
	_indirect_cache_file_read_result_io_failure,
	_indirect_cache_file_read_result_decompression_failure,
	_indirect_cache_file_read_result_invalid_checksum,
	_indirect_cache_file_read_result_failure_unknown,

	k_indirect_cache_file_read_result_count
};

class c_indirect_cache_file_bulk_read_iterator
{
public:
	virtual void __cdecl request_data(int32 requested_page_handle, const s_indirect_cache_file_read_request* read_request);
};
COMPILE_ASSERT(sizeof(c_indirect_cache_file_bulk_read_iterator) == 0x4);

class c_tag_resource_cache_file_reader;
class c_tag_resource_cache_file_datum_handler;
#pragma	pack(push, 4)
class c_synchronous_resource_io :
	c_indirect_cache_file_bulk_read_iterator
{
public:
//protected:
	c_static_sized_dynamic_array<uns64, 1500> m_resource_sort_keys;
	c_static_sized_dynamic_array<s_indirect_cache_file_read_request, 1500> m_resource_read_requests;
	c_static_sized_dynamic_array<int32, 1500> m_requested_page_handles;
	c_static_sized_dynamic_array<e_indirect_cache_file_read_result, 1500> m_request_results;
	uns32 m_total_request_size;
	uns32 m_finished_io_request_size;
	uns32 __unknown14844;
	uns32 __unknown14848;
	uns32 m_outstanding_request_count;
	c_tag_resource_page_table* m_page_table;
	c_tag_resource_cache_file_reader* m_cache_file_reader;
	c_tag_resource_cache_file_datum_handler* m_cache_file_datum_handler;
};
#pragma	pack(pop)
COMPILE_ASSERT(sizeof(c_synchronous_resource_io) == 0x1485C);
COMPILE_ASSERT(0x00004 == OFFSETOF(c_synchronous_resource_io, m_resource_sort_keys));
COMPILE_ASSERT(0x02EEC == OFFSETOF(c_synchronous_resource_io, m_resource_read_requests));
COMPILE_ASSERT(0x11954 == OFFSETOF(c_synchronous_resource_io, m_requested_page_handles));
COMPILE_ASSERT(0x130C8 == OFFSETOF(c_synchronous_resource_io, m_request_results));
COMPILE_ASSERT(0x1483C == OFFSETOF(c_synchronous_resource_io, m_total_request_size));
COMPILE_ASSERT(0x14840 == OFFSETOF(c_synchronous_resource_io, m_finished_io_request_size));
COMPILE_ASSERT(0x14844 == OFFSETOF(c_synchronous_resource_io, __unknown14844));
COMPILE_ASSERT(0x14848 == OFFSETOF(c_synchronous_resource_io, __unknown14848));
COMPILE_ASSERT(0x1484C == OFFSETOF(c_synchronous_resource_io, m_outstanding_request_count));
COMPILE_ASSERT(0x14850 == OFFSETOF(c_synchronous_resource_io, m_page_table));
COMPILE_ASSERT(0x14854 == OFFSETOF(c_synchronous_resource_io, m_cache_file_reader));
COMPILE_ASSERT(0x14858 == OFFSETOF(c_synchronous_resource_io, m_cache_file_datum_handler));

