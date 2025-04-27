#pragma once

#include "cseries/cseries.hpp"

struct c_tag_resource_cache_new;
struct c_tag_resource_page_table_io_listener
{
	c_tag_resource_cache_new* m_resource_cache_new;
};
static_assert(sizeof(c_tag_resource_page_table_io_listener) == 0x4);

struct s_tag_resource_page_datum :
	s_datum_header
{
	// 1: c_io_result::set_disk_failure
	// 2: c_io_result::set_checksum_failure
	uint8 flags;

	uint8 state;
	uint8 __data4[0x2];
	c_reference_count<int16> reference_count;
	int32 reservation;
	c_basic_buffer<void> buffer;
};
static_assert(sizeof(s_tag_resource_page_datum) == 0x14);

struct s_lruv_cache;
struct c_tag_resource_page_range_allocator;
struct c_tag_resource_cache_file_reader;
struct c_cache_file_tag_resource_location_handler;
struct c_tag_resource_cache_published_location_interface;
struct c_tag_resource_page_table_control_interface;
struct c_tag_resource_page_table
{
	struct c_tag_resource_lruv_cache
	{
		int32 m_allocation_alignment;
		c_tag_resource_page_range_allocator* m_storage_allocator;
		s_lruv_cache* m_lruv_cache;
		c_basic_buffer<void> m_lruv_data;
	};
	static_assert(sizeof(c_tag_resource_lruv_cache) == 0x14);

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
	uint32 m_total_prefetch_bytes;
	uint32 m_current_prefetch_bytes;
	bool m_prefetching_can_run;
	bool m_status_lines_enabled;
};
static_assert(sizeof(c_tag_resource_page_table) == 0x64);
static_assert(0x00 == OFFSETOF(c_tag_resource_page_table, m_resource_page_data));
static_assert(0x04 == OFFSETOF(c_tag_resource_page_table, m_resource_page_lruv_cache));
static_assert(0x18 == OFFSETOF(c_tag_resource_page_table, m_pending_resource_requests));
static_assert(0x1C == OFFSETOF(c_tag_resource_page_table, m_ready_pages_flags));
static_assert(0x24 == OFFSETOF(c_tag_resource_page_table, m_pinned_pages_flags));
static_assert(0x2C == OFFSETOF(c_tag_resource_page_table, m_cache_file_reader));
static_assert(0x30 == OFFSETOF(c_tag_resource_page_table, m_resource_location_handler));
static_assert(0x34 == OFFSETOF(c_tag_resource_page_table, m_published_resource_interface));
static_assert(0x38 == OFFSETOF(c_tag_resource_page_table, m_tracking_data_allocator));
static_assert(0x3C == OFFSETOF(c_tag_resource_page_table, m_tracking_allocation_base));
static_assert(0x40 == OFFSETOF(c_tag_resource_page_table, m_control_interface));
static_assert(0x44 == OFFSETOF(c_tag_resource_page_table, m_pending_state));
static_assert(0x58 == OFFSETOF(c_tag_resource_page_table, m_total_prefetch_bytes));
static_assert(0x5C == OFFSETOF(c_tag_resource_page_table, m_current_prefetch_bytes));
static_assert(0x60 == OFFSETOF(c_tag_resource_page_table, m_prefetching_can_run));
static_assert(0x61 == OFFSETOF(c_tag_resource_page_table, m_status_lines_enabled));

struct s_indirect_cache_file_sub_location
{
	uint32 offset;
	uint32 unaligned_read_size;
	uint32 used_storage_size;
	int32 decompressor_identifier;
	bool valid_checksum;
	uint32 checksum;
};
static_assert(sizeof(s_indirect_cache_file_sub_location) == 0x18);

struct s_indirect_cache_file_read_request
{
	c_basic_buffer<void> buffer;
	uint64 location_identifier;
	s_indirect_cache_file_sub_location sub_location;
};
static_assert(sizeof(s_indirect_cache_file_read_request) == 0x28);

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

struct c_indirect_cache_file_bulk_read_iterator
{
public:
	virtual void __cdecl request_data(int32 requested_page_handle, s_indirect_cache_file_read_request const* read_request);
};
static_assert(sizeof(c_indirect_cache_file_bulk_read_iterator) == 0x4);

struct c_tag_resource_cache_file_reader;
struct c_tag_resource_cache_file_datum_handler;
#pragma	pack(push, 4)
struct c_synchronous_resource_io :
	c_indirect_cache_file_bulk_read_iterator
{
//protected:
	c_static_sized_dynamic_array<uint64, 1500> m_resource_sort_keys;
	c_static_sized_dynamic_array<s_indirect_cache_file_read_request, 1500> m_resource_read_requests;
	c_static_sized_dynamic_array<int32, 1500> m_requested_page_handles;
	c_static_sized_dynamic_array<e_indirect_cache_file_read_result, 1500> m_request_results;
	uint32 m_total_request_size;
	uint32 m_finished_io_request_size;
	uint32 __unknown14844;
	uint32 __unknown14848;
	uint32 m_outstanding_request_count;
	c_tag_resource_page_table* m_page_table;
	c_tag_resource_cache_file_reader* m_cache_file_reader;
	c_tag_resource_cache_file_datum_handler* m_cache_file_datum_handler;
};
#pragma	pack(pop)
static_assert(sizeof(c_synchronous_resource_io) == 0x1485C);
static_assert(0x00004 == OFFSETOF(c_synchronous_resource_io, m_resource_sort_keys));
static_assert(0x02EEC == OFFSETOF(c_synchronous_resource_io, m_resource_read_requests));
static_assert(0x11954 == OFFSETOF(c_synchronous_resource_io, m_requested_page_handles));
static_assert(0x130C8 == OFFSETOF(c_synchronous_resource_io, m_request_results));
static_assert(0x1483C == OFFSETOF(c_synchronous_resource_io, m_total_request_size));
static_assert(0x14840 == OFFSETOF(c_synchronous_resource_io, m_finished_io_request_size));
static_assert(0x14844 == OFFSETOF(c_synchronous_resource_io, __unknown14844));
static_assert(0x14848 == OFFSETOF(c_synchronous_resource_io, __unknown14848));
static_assert(0x1484C == OFFSETOF(c_synchronous_resource_io, m_outstanding_request_count));
static_assert(0x14850 == OFFSETOF(c_synchronous_resource_io, m_page_table));
static_assert(0x14854 == OFFSETOF(c_synchronous_resource_io, m_cache_file_reader));
static_assert(0x14858 == OFFSETOF(c_synchronous_resource_io, m_cache_file_datum_handler));

