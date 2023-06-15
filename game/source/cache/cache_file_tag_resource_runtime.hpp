#pragma once

#include "cache/cache_file_builder_tag_resource_manager.hpp"
#include "cseries/cseries.hpp"
#include "interface/user_interface_memory.hpp"
#include "multithreading/synchronized_value.hpp"
#include "rasterizer/rasterizer_main.hpp"
#include "scenario/scenario.hpp"
#include "shell/shell.hpp"
#include "tag_files/files_windows.hpp"

struct s_tag_resource
{
	long resource_handle;
	dword definition_address;
};
static_assert(sizeof(s_tag_resource) == 0x8);

template<typename t_resource_type, dword ...t_extra>
struct c_typed_tag_resource : s_tag_resource
{
	t_resource_type* get()
	{
		return reinterpret_cast<t_resource_type*>(resource_handle);
	}
};
static_assert(sizeof(s_tag_resource) == sizeof(s_tag_resource));

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

struct c_cache_file_decompressor
{
public:
	virtual bool begin(c_basic_buffer<void> a1) = 0;
	virtual bool decompress_buffer(c_basic_buffer<void> a1, c_basic_buffer<void>* a2) = 0;
	virtual bool finish(c_basic_buffer<void>* a1) = 0;
};
static_assert(sizeof(c_cache_file_decompressor) == 0x4);

struct c_cache_file_uncompressed_decompressor :
	public c_cache_file_decompressor
{
	virtual bool begin(c_basic_buffer<void> a1)
	{
		throw;
	}
	virtual bool decompress_buffer(c_basic_buffer<void> a1, c_basic_buffer<void>* a2)
	{
		throw;
	}
	virtual bool finish(c_basic_buffer<void>* a1)
	{
		throw;
	}

	long m_bytes_processed;
	c_basic_buffer<void> __buffer;
};
static_assert(sizeof(c_cache_file_uncompressed_decompressor) == sizeof(c_cache_file_decompressor) + 0xC);

struct c_cache_file_streamed_sublocation_decompressor :
	public c_cache_file_decompressor
{
	virtual bool begin(c_basic_buffer<void> a1)
	{
		throw;
	}
	virtual bool decompress_buffer(c_basic_buffer<void> a1, c_basic_buffer<void>* a2)
	{
		throw;
	}
	virtual bool finish(c_basic_buffer<void>* a1)
	{
		throw;
	}

	struct // s_cache_file_resource_streaming_sublocation_table?
	{
		long __unknown0;
		long __unknown4;

		struct
		{
			long __unknown0;
			long __unknown4;
		}*__unknown8;
		static_assert(sizeof(*__unknown8) == 0x8);

	}*__unknown4;
	static_assert(sizeof(*__unknown4) == 0xC);

	c_basic_buffer<void> __buffer8;
	char* __unknown10;
	c_basic_buffer<void> __buffer14;
};
static_assert(sizeof(c_cache_file_streamed_sublocation_decompressor) == sizeof(c_cache_file_decompressor) + 0x18);

// #TODO: find a home
struct s_simple_read_file_ex_overlapped_result
{
	c_synchronized_long __unknown0;
	dword __unknown4;
	dword offset;
	dword size;
};
static_assert(sizeof(s_simple_read_file_ex_overlapped_result) == 0x10);

struct c_cache_file_copy_fake_decompressor :
	public c_cache_file_decompressor
{
	virtual bool begin(c_basic_buffer<void> a1)
	{
		throw;
	}
	virtual bool decompress_buffer(c_basic_buffer<void> a1, c_basic_buffer<void>* a2)
	{
		throw;
	}
	virtual bool finish(c_basic_buffer<void>* a1)
	{
		throw;
	}

	s_file_handle m_file_handle;
	c_synchronized_long m_file_offset;
	c_synchronized_long m_done;
	dword m_checksum;
	bool __unknown14;
	s_simple_read_file_ex_overlapped_result m_overlapped_result;
	byte m_overlapped[0x14]; // OVERLAPPED
	void* m_buffer;

	// are these bytes actually here?
	byte __data[0x8];
};
static_assert(sizeof(c_cache_file_copy_fake_decompressor) == sizeof(c_cache_file_decompressor) + 0x44);

struct c_xor_cache_file_decompressor :
	public c_cache_file_decompressor
{
	virtual bool begin(c_basic_buffer<void> a1)
	{
		throw;
	}
	virtual bool decompress_buffer(c_basic_buffer<void> a1, c_basic_buffer<void>* a2)
	{
		throw;
	}
	virtual bool finish(c_basic_buffer<void>* a1)
	{
		throw;
	}

	c_basic_buffer<void> __buffer0;
	c_basic_buffer<void> __buffer8;
	byte __unknownC; // xor value, *i++ ^= __unknownC
};
static_assert(sizeof(c_xor_cache_file_decompressor) == sizeof(c_cache_file_decompressor) + 0x14);

struct c_lz_cache_file_decompressor :
	public c_cache_file_decompressor
{
	virtual bool begin(c_basic_buffer<void> a1)
	{
		throw;
	}
	virtual bool decompress_buffer(c_basic_buffer<void> a1, c_basic_buffer<void>* a2)
	{
		throw;
	}
	virtual bool finish(c_basic_buffer<void>* a1)
	{
		throw;
	}

	c_basic_buffer<void> __buffer4;
	c_basic_buffer<void> __bufferC;
	bool __unknown14;
	c_basic_buffer<void> __buffer18;
};
static_assert(sizeof(c_lz_cache_file_decompressor) == sizeof(c_cache_file_decompressor) + 0x1C);

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

struct s_indirect_cache_file_read_request
{
	c_basic_buffer<void> buffer;
	qword __unknown8;
	dword __unknown10;
	dword processed_compressed_size;
	dword __unknown18;
	long codec_index;
	bool __unknown20;
	dword checksum;
};
static_assert(sizeof(s_indirect_cache_file_read_request) == 0x28);

#pragma region resource runtime manager

template<typename t_type, long k_depth>
struct c_typed_allocation_data_no_destruct
{
public:
	t_type* get()
	{
		ASSERT(m_live_object);

		return m_live_object;
	}

protected:
	c_basic_buffer<void> m_opaque_storage;
	t_type* m_live_object;
	c_allocation_base* m_allocator;
};
static_assert(sizeof(c_typed_allocation_data_no_destruct<long, 0>) == 0x10);

struct s_cache_file_tag_resource_data;

struct s_cache_file_resource_runtime_active_game_state : s_scenario_game_state
{
};
static_assert(sizeof(s_cache_file_resource_runtime_active_game_state) == sizeof(s_scenario_game_state));

struct s_cache_file_resource_prefetch_map_state
{
	long campaign_id;
	c_static_string<256> scenario_path;
	bool __unknown104;
};
static_assert(sizeof(s_cache_file_resource_prefetch_map_state) == 0x108);

struct s_cache_file_resource_runtime_prefetching_state
{
	long __datum_handle0;
	long __datum_handle4;
	s_cache_file_resource_prefetch_map_state __state8;
	s_cache_file_resource_prefetch_map_state __state110;
};
static_assert(sizeof(s_cache_file_resource_runtime_prefetching_state) == 0x218);

struct c_tag_index_hash_table
{
	// c_simple_hash_table<long, 8192, 3307, 3, 337>
	// c_static_hash_table<c_static_hash_table_data<long, 8192>, 3307, 3, 337>
	long m_total_count;

	struct
	{
		long __unknown0;
		long __unknown4;
		long __unknown8;
		long __unknownC;
	} __unknown4[8192];

	short __unknown20004[8192];
	short __unknown24004[8192];
};
static_assert(sizeof(c_tag_index_hash_table) == 0x28004);

struct s_tag_resource_prediction_quantum
{
	dword internal_resource_handle;
};
static_assert(sizeof(s_tag_resource_prediction_quantum) == 0x4);

struct s_tag_resource_prediction_atom
{
	word identifier;
	word predicted_resource_count;
	dword first_prediction_index;
};
static_assert(sizeof(s_tag_resource_prediction_atom) == 0x8);

struct s_tag_resource_prediction_molecule
{
	word predicted_atom_count;
	word first_predicted_atom_index;
	word predicted_resource_count;
	word first_predicted_resource_index;
};
static_assert(sizeof(s_tag_resource_prediction_molecule) == 0x8);

struct s_tag_resource_prediction_molecule_key
{
	long index_a;
	long index_b;
	long index_c;
};
static_assert(sizeof(s_tag_resource_prediction_molecule_key) == 0xC);

struct c_tag_resource_cache_prediction_table
{
	c_wrapped_array<s_tag_resource_prediction_quantum const> m_prediction_quanta;
	c_wrapped_array<s_tag_resource_prediction_atom const> m_prediction_atoms;
	c_wrapped_array<long const> m_prediction_molecule_atoms;
	c_wrapped_array<s_tag_resource_prediction_molecule const> m_prediction_molecules;
};
static_assert(sizeof(c_tag_resource_cache_prediction_table) == 0x20);

struct c_tag_resource_cache_precompiled_predictor
{
	// c_simple_hash_table<long, 8192, 3307, 3, 337>
	// c_static_hash_table<c_static_hash_table_data<long, 8192>, 3307, 3, 337>
	struct
	{
		long m_total_count;

		struct
		{
			long __unknown0;
			long __unknown4;
			long __unknown8;
			long __unknownC;
		} __unknown4[8192];

		short __unknown20004[8192];
		short __unknown24004[8192];

	} m_molecule_index_table;

	c_tag_resource_cache_prediction_table m_prediction_table;
};
static_assert(sizeof(c_tag_resource_cache_precompiled_predictor) == 0x28024);

struct c_tag_resource_cache_dynamic_predictor
{
	c_tag_resource_cache_precompiled_predictor m_precompiled_predictor;
	c_tag_index_hash_table m_index_hash_table;

	// struct?
	void* __unknown50028; // vftable
	word __unknown5002C;
	word __unknown5002E;

	c_static_sized_dynamic_array<s_tag_resource_prediction_quantum, 16384> m_prediction_quanta;
	c_static_sized_dynamic_array<s_tag_resource_prediction_atom, 63488> m_prediction_atoms;
	c_static_sized_dynamic_array<long, 63488> m_prediction_molecule_atoms;
	c_static_sized_dynamic_array<s_tag_resource_prediction_molecule, 8192> m_prediction_molecules;

	bool m_prediction_quanta_bool0;
	bool m_prediction_atoms_bool0;
	bool m_prediction_molecule_atoms_bool0;
	bool m_prediction_molecules_bool0;

	bool m_prediction_quanta_bool1;
	bool m_prediction_atoms_bool1;
	bool m_prediction_molecule_atoms_bool1;
	bool m_prediction_molecules_bool1;

	dword m_idle_time;
};
static_assert(sizeof(c_tag_resource_cache_dynamic_predictor) == 0x12A04C);

struct s_tag_persistent_identifier
{
	dword parts[4];
};
static_assert(sizeof(s_tag_persistent_identifier) == 0x10);

struct s_cache_file_tag_resource_vtable
{
	s_tag_persistent_identifier type_identifier;
	long(__cdecl* register_resource)(long resource_owner, void*);
	void(__cdecl* unregister_resource)(long resource_owner, void*);
};
static_assert(sizeof(s_cache_file_tag_resource_vtable) == 0x18);

struct c_tag_resource_cache_new;
struct c_tag_resource_page_table_io_listener
{
	c_tag_resource_cache_new* m_resource_cache_new;
};
static_assert(sizeof(c_tag_resource_page_table_io_listener) == 0x4);

struct s_lruv_cache;
struct c_tag_resource_cache_file_reader;
struct c_tag_resource_page_table
{
	struct c_tag_resource_lruv_cache
	{
		dword_flags* __unknown0;
		long __unknown4;
		s_lruv_cache* m_lruv_cache;
	};
	static_assert(sizeof(c_tag_resource_lruv_cache) == 0xC);

	s_data_array* __unknown0;
	c_tag_resource_lruv_cache __lruv_cache4;
	char const* __unknown10;
	char const* __unknown14;
	s_data_array* m_pending_resource_requests;
	c_tag_resource_lruv_cache __lruv_cache1C;

	byte __data28[0x4];

	c_tag_resource_cache_file_reader* m_cache_file_reader;
	void* __unknown30;

	byte __data34[0x14];

	s_data_iterator __unknown0_iterator;

	byte __data[0x10];
};
static_assert(sizeof(c_tag_resource_page_table) == 0x64);

struct c_tag_resource_cache_controller
{
	byte __data0[0x8];
	s_data_array* __data_array;
	byte __dataC[0x54];
};
static_assert(sizeof(c_tag_resource_cache_controller) == 0x60);

struct c_tag_resource_address_cache
{
	byte __data[0x44];
};
static_assert(sizeof(c_tag_resource_address_cache) == 0x44);

struct c_tag_resource_inverse_page_table
{
	struct s_page_resource_reference_range : s_datum_header
	{
		word __unknown2;
		word __unknown4;
		word __unknown6;
	};
	static_assert(sizeof(s_page_resource_reference_range) == 0x8);

	c_allocation_base* m_allocator;
	void* m_allocation;
	dword __unknown8;
	dword __unknownC;
	c_smart_data_array<s_page_resource_reference_range>* m_page_entries;
	byte_flags flags;
	word __unknown16;
};
static_assert(sizeof(c_tag_resource_inverse_page_table) == 0x18);

struct s_tag_resource_access_datum
{
	long resource_handle;
	void* resource_data;
};
static_assert(sizeof(s_tag_resource_access_datum) == 0x8);

template<typename t_type>
struct c_reference_count
{
	t_type m_reference_count;
};
static_assert(sizeof(c_reference_count<long>) == 0x4);

struct c_read_write_lock
{
	dword m_critical_section_id;
	long m_semaphore_id;
	long m_max_signal_count;
};
static_assert(sizeof(c_read_write_lock) == 0xC);

struct c_tag_resource_cache_file_access_cache
{
public:
	bool __cdecl cached_resource_available(long resource_handle)
	{
		if (resource_handle == NONE)
			return false;

		s_tag_resource_access_datum* cached_resource = m_cached_access_datums[resource_handle];
		if (!cached_resource)
			return false;

		//ASSERT(cached_resource->resource_handle == resource_handle || cached_resource->resource_handle == NONE);

		return cached_resource->resource_handle != NONE && cached_resource->resource_data;
	}

	void* __cdecl get_cached_resource_data(long resource_handle)
	{
		ASSERT(resource_handle != NONE);

		s_tag_resource_access_datum* cached_resource = m_cached_access_datums[DATUM_INDEX_TO_ABSOLUTE_INDEX(resource_handle)];
		//ASSERT(cached_resource->resource_handle == resource_handle);
		//ASSERT(cached_resource->resource_data);

		return cached_resource;
	}

	// m_handles[interop->descriptor]
	c_wrapped_array<qword> __unknown0;

	c_wrapped_array<s_tag_resource_access_datum> m_cached_access_datums;
	c_wrapped_array<dword> m_cached_resource_handles;
	c_wrapped_array<void> __unknown18;

	c_allocation_base* m_allocator;
	byte __data24[0x14];
};
static_assert(sizeof(c_tag_resource_cache_file_access_cache) == 0x38);

struct c_tag_resource_cache_thread_lock_lock_freeish
{
	byte __data0[0xC0];
	byte __dataC0[0x20];
	c_static_array<c_tag_resource_cache_file_access_cache, 3> m_access_cache_storage;
	c_reference_count<long> m_reference_count;
	c_read_write_lock m_read_write_lock;
	byte __data198[0x8];
	bool m_blocking_update;
	byte __data1A1[0x1F];
	bool __unknown1C0;
	bool __unknown1C1;
	bool __unknown1C2;
	bool m_thread_access_state_locked;
	bool __unknown1C4;
	bool __unknown1C5;
	bool __unknown1C6;
	bool __unknown1C7;
	bool __unknown1C8;
	bool __unknown1C9;
	bool __unknown1CA;
	bool __unknown1CB;
	bool __unknown1CC;
	dword __unknown1D0;
	byte __data1CC[0x6C];
};
static_assert(sizeof(c_tag_resource_cache_thread_lock_lock_freeish) == 0x240);

struct c_tag_resource_runtime_active_set;
struct c_tag_resource_cache_file_prefetch_set;
struct c_tag_resource_cache_new
{
	void* __unknown0;

	c_tag_resource_page_table_io_listener m_io_listener;
	c_tag_resource_page_table m_page_table;
	c_tag_resource_cache_controller m_cache_controller;
	c_tag_resource_address_cache m_address_cache;
	c_tag_resource_inverse_page_table m_inverse_page_table;

	// vtable?
	void* __unknown128;

	c_tag_resource_runtime_active_set* m_runtime_active_set;
	c_tag_resource_cache_file_prefetch_set* m_cache_file_prefetch_set;

	byte __data134[0xC];

	c_tag_resource_cache_thread_lock_lock_freeish m_thread_access_lock;
};
static_assert(sizeof(c_tag_resource_cache_new) == 0x380);

struct c_tag_resource_thread_access
{
public:
	bool __cdecl resource_available(long resource_handle)
	{
		c_tag_resource_cache_file_access_cache* access_cache = get_current_thread_access_cache();
		return access_cache && access_cache->cached_resource_available(resource_handle);
	}

	void* __cdecl get_resource_data(long resource_handle)
	{
		c_tag_resource_cache_file_access_cache* resource_access_cache = get_current_thread_access_cache();
		ASSERT(resource_access_cache);

		return resource_access_cache->get_cached_resource_data(resource_handle);
	}

//private:
	c_tag_resource_cache_file_access_cache* __cdecl get_current_thread_access_cache()
	{
		// I don't want to synchronize access to resources except for the main and render threads!
		long thread_index = get_current_thread_index();
		ASSERT(m_per_thread_acquired_access_cache[thread_index]);

		return m_per_thread_access_cache[thread_index];
	}

protected:
	long m_per_thread_acquired_access_cache[10];
	c_tag_resource_cache_file_access_cache* m_per_thread_access_cache[10];
	long m_per_thread_acquired_access_cache_index;
	long* m_current_per_thread_acquired_access_cache;
	long m_per_thread_access_cache_index;
	c_tag_resource_cache_file_access_cache** m_current_per_thread_access_cache;
	c_read_write_lock m_read_write_lock;
	c_tag_resource_cache_thread_lock_lock_freeish* m_resource_thread_access_lock;
	byte __data[0x10];
};
static_assert(sizeof(c_tag_resource_thread_access) == 0x80);

struct c_thread_safeish_tag_resource_cache
{
public:
	void* __cdecl get_resource_data(long resource_handle)
	{
		if (!m_resource_thread_access.resource_available(resource_handle))
			report_unavailable_resource(resource_handle);

		return get_thread_access()->get_resource_data(resource_handle);
	}

//private:
	void __cdecl report_unavailable_resource(long resource_handle)
	{
	}

	c_tag_resource_thread_access* __cdecl get_thread_access()
	{
		return &m_resource_thread_access;
	}

protected:
	c_tag_resource_cache_new m_resource_cache_new;
	c_tag_resource_thread_access m_resource_thread_access;
};
static_assert(sizeof(c_thread_safeish_tag_resource_cache) == 0x400);

struct c_cache_file_tag_resource_runtime_control_allocation :
	c_allocation_base
{
	c_basic_buffer<void> m_available_region;
	c_basic_buffer<void> m_allocated_region;
};
static_assert(sizeof(c_cache_file_tag_resource_runtime_control_allocation) == 0x14);

struct c_cache_file_resource_uber_location_table
{
	c_wrapped_array<s_cache_file_tag_resource_data>* m_locations;
	long __unknown4;
};

struct c_cache_file_combined_tag_resource_datum_handler
{
	void* __vftable;
	s_cache_file_resource_gestalt* m_resource_gestalt;
	dword __unknown8;
	c_basic_buffer<void>* m_interop_buffer;
	bool m_running_off_dvd;
	c_cache_file_resource_uber_location_table* m_uber_location_table;
};
static_assert(sizeof(c_cache_file_combined_tag_resource_datum_handler) == 0x18);

struct c_cache_file_tag_resource_location_handler
{
	void* __vftable;
	c_cache_file_resource_uber_location_table* m_uber_location_table;
};
static_assert(sizeof(c_cache_file_tag_resource_location_handler) == 0x8);

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

struct c_cache_file_tag_resource_runtime_in_level_memory_manager
{
	c_basic_buffer<void> m_resource_storage_range;

	// #TODO: name this
	c_basic_buffer<void> __unknown8;

	c_basic_buffer<void> m_actual_resource_storage_range;
	c_basic_buffer<void> m_stoler_range;
	bool m_actual_storage_range_read_locked;
	c_basic_buffer<void> m_writeable_range;

	// #TODO: map this
	byte __data2C[0xC];

	c_thread_safeish_tag_resource_cache m_tag_resource_cache;
	c_cache_file_tag_resource_runtime_control_allocation m_cache_file_resource_allocator;
	c_basic_buffer<void> m_cache_file_resource_allocation_region;
	c_cache_file_resource_header_location_table m_resource_header_location_table;
};
static_assert(sizeof(c_cache_file_tag_resource_runtime_in_level_memory_manager) == 0x480);

struct c_cache_file_resource_rollover_table
{
	struct s_rollover_entry_estimated
	{
		long file_size;
		long size;
		c_basic_buffer<void> range;
	};
	static_assert(sizeof(s_rollover_entry_estimated) == 0x10);

	c_static_sized_dynamic_array<s_rollover_entry_estimated, 16384> m_estimated_rollover_entries;
	c_basic_buffer<void> m_entry_bounds;
	long m_next_moved_entry_index;
};
static_assert(sizeof(c_cache_file_resource_rollover_table) == 0x40010);

struct c_indirect_cache_file_decompressor_service
{
	virtual c_cache_file_decompressor* begin_decompression(qword, long, c_basic_buffer<void>) = 0;
	virtual void dispose_decompressor(qword, long, c_cache_file_decompressor*) = 0;
};
static_assert(sizeof(c_indirect_cache_file_decompressor_service) == 0x4);

struct c_asynchronous_io_marker
{
	//LPOVERLAPPED
	void* m_overlapped;

	byte __data[0x10];
	dword m_unaligned_io_size;
	c_synchronized_long m_io_status;
};
static_assert(sizeof(c_asynchronous_io_marker) == 0x1C);

struct c_cache_file_async_decompression_task
{
	dword __unknown0;
	bool __unknown4;
	c_synchronized_long __unknown8;
	c_synchronized_long __unknownC;
	c_asynchronous_io_marker* m_async_request_done;
	bool __unknown14;
	long m_codec_index;
	c_cache_file_decompressor* m_decompressor;
	c_basic_buffer<void> m_decompression_buffer;
};
static_assert(sizeof(c_cache_file_async_decompression_task) == 0x28);

struct c_cache_file_decompressor_service
{
public:
	virtual bool decompressor_available() = 0;
	virtual c_cache_file_decompressor* create_decompressor() = 0;
	virtual c_cache_file_decompressor* begin_decompression(c_basic_buffer<void> buffer) = 0;
	virtual void dispose_decompressor(c_cache_file_decompressor* decompressor) = 0;
};
static_assert(sizeof(c_cache_file_decompressor_service) == 0x4);

template<typename t_type, long k_type_size = sizeof(t_type), long k_alignment_mask = __alignof(t_type) - 1>
struct c_typed_opaque_data
{
	t_type* get()
	{
		ASSERT(((dword)m_opaque_data & k_alignment_mask) == 0);
		return reinterpret_cast<t_type*>(((dword)m_opaque_data + k_alignment_mask) & ~k_alignment_mask);
	}

	byte m_opaque_data[k_type_size];
};

template<typename decompressor_class_t>
struct c_single_instance_cache_file_decompressor_service :
	public c_cache_file_decompressor_service
{
	virtual bool decompressor_available()
	{
		return m_decompressor == nullptr;
	}

	virtual c_cache_file_decompressor* create_decompressor()
	{
		initialize_decompressor(&m_decompressor_instance);
		return m_decompressor;
	}

	virtual c_cache_file_decompressor* begin_decompression(c_basic_buffer<void> buffer)
	{
		initialize_decompressor(&m_decompressor_instance);
		m_decompressor->begin(buffer);
		return m_decompressor;
	}

	virtual void dispose_decompressor(c_cache_file_decompressor* decompressor)
	{
		//ASSERT(m_decompressor_instance.alive());
		ASSERT(m_decompressor_instance.get() == decompressor);
		m_decompressor = nullptr;
		//ASSERT(!m_decompressor_instance.alive());
	}

	virtual void initialize_decompressor(c_typed_opaque_data<decompressor_class_t>* decompressor_instance) = 0;

	byte __data[4];
	c_typed_opaque_data<decompressor_class_t> m_decompressor_instance;
	c_cache_file_decompressor* m_decompressor;
};

struct c_xor_cache_file_decompressor_service : c_single_instance_cache_file_decompressor_service<c_xor_cache_file_decompressor>
{
};
static_assert(sizeof(c_xor_cache_file_decompressor_service) == 0x24);

struct c_lz_cache_file_decompressor_service : c_single_instance_cache_file_decompressor_service<c_lz_cache_file_decompressor>
{
};
static_assert(sizeof(c_lz_cache_file_decompressor_service) == 0x2C);

struct c_cache_file_decompressor_registry
{
	virtual c_cache_file_decompressor_service* find_decompressor_service(s_tag_persistent_identifier* identifier);
};
static_assert(sizeof(c_cache_file_decompressor_registry) == 0x4);

struct c_cache_file_runtime_decompressor_registry : c_cache_file_decompressor_registry
{
};
static_assert(sizeof(c_cache_file_runtime_decompressor_registry) == sizeof(c_cache_file_decompressor_registry));

struct c_cache_file_tag_resource_codec_service :
	c_indirect_cache_file_decompressor_service
{
	virtual c_cache_file_decompressor* begin_decompression(qword, long, c_basic_buffer<void>)
	{
		throw;
	}
	virtual void dispose_decompressor(qword, long, c_cache_file_decompressor*)
	{
		throw;
	}

	c_cache_file_resource_uber_location_table* m_uber_location_table;
	c_static_sized_dynamic_array<c_cache_file_decompressor_service*, 127> m_actual_runtime_decompressors;

	c_typed_opaque_data<c_cache_file_streamed_sublocation_decompressor> m_streamed_sublocation_decompressor;

	byte __data224[0x10];
	c_cache_file_uncompressed_decompressor* m_uncompressed_cache_file_decompressor;
	c_basic_buffer<void> m_decompression_buffer;
	bool m_decompression_buffer_locked;
	c_cache_file_async_decompression_task m_async_decompression_task;
};
static_assert(sizeof(c_cache_file_tag_resource_codec_service) == 0x26C);

enum e_map_memory_configuration;
struct s_optional_cache_user_memory_configuration;
enum e_optional_cache_user;
enum e_optional_cache_user_priority;
struct c_optional_cache_user_callback;
struct c_optional_cache_backend
{
private:
	virtual void initialize(e_map_memory_configuration memory_configuration, c_static_array<s_optional_cache_user_memory_configuration, 6> const*);
	virtual void dispose();
	virtual void* try_to_allocate(e_optional_cache_user, e_optional_cache_user_priority, long, c_optional_cache_user_callback*);
	virtual void deallocate(e_optional_cache_user, long, void*);

	bool m_active;
	dword __unknown8;
};
static_assert(sizeof(c_optional_cache_backend) == 0xC);

struct c_tag_resource_cache_stoler
{
	struct s_stolen_memory_user
	{
		byte __data[0x14];
	};
	static_assert(sizeof(s_stolen_memory_user) == 0x14);

	long m_age;
	c_static_array<c_tag_resource_cache_stoler::s_stolen_memory_user, 6> m_stolen_blocks;
};
static_assert(sizeof(c_tag_resource_cache_stoler) == 0x7C);

struct c_cache_file_resource_stoler;
struct c_cache_file_resource_optional_cache_backend :
	public c_optional_cache_backend
{
	c_tag_resource_cache_stoler m_cache_stoler;
	c_static_flags<32> m_flags;
	c_cache_file_resource_stoler* m_resource_stoler;
};
static_assert(sizeof(c_cache_file_resource_optional_cache_backend) == 0x90);

struct s_tag_resource_definition;
struct c_tag_resource_runtime_listener
{
public:
	virtual bool __cdecl register_resource(long tag_index, long tag_resource_type_index, void* data);
	virtual void __cdecl unregister_resource(long tag_index, long tag_resource_type_index, void* data);
};
static_assert(sizeof(c_tag_resource_runtime_listener) == 0x4);

struct c_tag_resource_runtime_active_set
{
public:
	virtual bool __cdecl any_resources_active() const;
	virtual bool __cdecl is_resource_required(long resource_handle, long resource_owner) const;
	virtual bool __cdecl is_resource_deferred(long resource_handle, long resource_owner) const;
	virtual bool __cdecl is_resource_pending(long resource_handle, long resource_owner) const;
};
static_assert(sizeof(c_tag_resource_runtime_active_set) == 0x4);

struct s_tag_resource_location_handle_struct;
struct c_tag_resource_cache_file_prefetch_set
{
public:
	virtual bool __cdecl should_prefetch_location(s_tag_resource_location_handle_struct* location_handle)const;
};
static_assert(sizeof(c_tag_resource_cache_file_prefetch_set) == 0x4);

struct c_tag_resource_page_range_allocator
{
public:
	virtual bool __cdecl try_to_grab_restore_range(c_basic_buffer<void> desired_range, c_basic_buffer<void>* in_out_range);
	virtual bool __cdecl try_to_resize_contiguous_range(c_basic_buffer<void>* in_out_range, dword, dword, dword);
	virtual void __cdecl shrink_for_buyback(c_basic_buffer<void>* in_out_range, dword new_size);
	virtual void __cdecl reclaim_stolen_memory(c_basic_buffer<void>* in_out_range);
	virtual void __cdecl release_allocation(c_basic_buffer<void>* in_out_range);
	virtual void __cdecl make_range_writeable(c_basic_buffer<void> range);
	virtual void __cdecl make_range_read_only(c_basic_buffer<void> range);
};
static_assert(sizeof(c_tag_resource_page_range_allocator) == 0x4);

struct c_indirect_cache_file_bulk_read_iterator;
struct c_tag_resource_cache_file_reader
{
public:
	virtual void __cdecl update_io_state();
	virtual void __cdecl spinning_for_blocking_io();
	virtual bool __cdecl request_data(c_indirect_cache_file_bulk_read_iterator* bulk_read_iterator);
	virtual bool __cdecl request_data_async(s_indirect_cache_file_read_request const* read_request, c_synchronized_long* out_size, c_asynchronous_io_marker* out_async_request_done);
};
static_assert(sizeof(c_tag_resource_cache_file_reader) == 0x4);

struct s_indirect_cache_file_location;
struct c_indirect_cache_file_location_atlas
{
public:
	virtual bool get_location(qword, s_indirect_cache_file_location* out_location);
};
static_assert(sizeof(c_indirect_cache_file_location_atlas) == 0x4);

struct c_physical_memory_contiguous_region_listener
{
public:
	virtual void initialize_resize_buffer(c_basic_buffer<void>);
	virtual void resize_no_fail(c_basic_buffer<void>, c_basic_buffer<void>);
	virtual void dispose_resize_buffer(c_basic_buffer<void>);
};
static_assert(sizeof(c_physical_memory_contiguous_region_listener) == 0x4);

struct c_tag_resource_prediction_atom_collector;
struct c_tag_resource_prediction_atom_generator
{
public:
	virtual bool collect_tag_resources(long tag_index, long prediction_atom_handle, c_tag_resource_prediction_atom_collector* atom_collector);
};
static_assert(sizeof(c_tag_resource_prediction_atom_generator) == 0x4);

struct c_cache_file_resource_stoler
{
public:
	virtual void* try_to_steal_memory(dword size);
	virtual void* steal_memory_shouldnt_fail(dword size);
	virtual void return_memory(void* stolen_memory, dword size);
};
static_assert(sizeof(c_cache_file_resource_stoler) == 0x4);

struct c_cache_file_tag_resource_runtime_manager :
	public c_tag_resource_runtime_listener,
	public c_tag_resource_runtime_active_set,
	public c_tag_resource_cache_file_prefetch_set,
	public c_tag_resource_page_range_allocator,
	public c_tag_resource_cache_file_reader,
	public c_indirect_cache_file_location_atlas,
	public c_physical_memory_contiguous_region_listener,
	public c_tag_resource_prediction_atom_generator,
	public c_cache_file_resource_stoler
{
	void* get_cached_resource_data(long resource_handle)
	{
		return m_in_level_memory_manager.m_tag_resource_cache.get_resource_data(resource_handle);
	}

	s_cache_file_resource_gestalt* m_resource_gestalt;
	s_cache_file_resource_runtime_active_game_state m_active_game_state;
	s_cache_file_resource_runtime_prefetching_state m_prefetching_state;

	// #TODO: name this
	bool __unknown260;

	bool m_resources_required;
	bool m_resources_pending;
	bool m_resources_loading;

	c_typed_allocation_data_no_destruct<c_tag_resource_cache_dynamic_predictor, 0> m_dynamic_predictor;
	c_tag_resource_cache_precompiled_predictor m_precompiled_predictor;
	c_static_flags<32768> m_required_resources;
	c_static_flags<32768> m_pending_resources;
	c_static_array<s_cache_file_tag_resource_vtable const*, 16> m_tag_resource_vtables;
	c_wrapped_array<void> m_resource_runtime_data;
	c_basic_buffer<void> m_resource_interop_data_buffer;
	c_cache_file_combined_tag_resource_datum_handler m_combined_tag_resource_datum_handler;
	c_cache_file_tag_resource_location_handler m_resource_location_handler;
	c_cache_file_tag_resource_runtime_in_level_memory_manager m_in_level_memory_manager;
	c_static_array<s_cache_file_resource_prefetch_map_state, 2> m_prefetch_map_states2A788;
	c_cache_file_resource_rollover_table m_rollover_table;
	c_cache_file_tag_resource_codec_service m_resource_codec_service;
	c_cache_file_resource_optional_cache_backend m_optional_cache_backend;

	c_enum<e_game_mode, long, _game_mode_none, k_game_mode_count> m_game_mode;
	bool m_running_off_dvd;

	// #TODO: name this
	bool __unknown6ACA9;

	// #TODO: map this
	byte __data6ACAA[0x16];
};
static_assert(sizeof(c_cache_file_tag_resource_runtime_manager) == 0x6ACC0);

#pragma endregion

extern c_asynchronous_io_arena& g_cache_file_io_arena;
extern c_typed_allocation_data_no_destruct<c_cache_file_tag_resource_runtime_manager, 1>& g_resource_runtime_manager;

extern void* __cdecl tag_resource_get(s_tag_resource const* resource);

struct s_resource_file_header
{
	// tag info
	tag group_tag;
	long tag_index;

	// resource data size and offset from file begin
	dword file_size;
	long resource_index;
};
static_assert(sizeof(s_resource_file_header) == 0x10);

extern c_static_sized_dynamic_array<s_resource_file_header const*, 1024> g_resource_file_headers;

