#pragma once

#include "cache/cache_file_codec_work.hpp"
#include "cache/cache_file_runtime_codecs.hpp"
#include "cache/cache_file_tag_resource_datum_handler.hpp"
#include "cache/cache_file_tag_resource_location_table.hpp"
#include "cache/cache_file_tag_resource_vtable_list.hpp"
#include "cache/optional_cache.hpp"
#include "cache/physical_memory_map.hpp"
#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "scenario/scenario.hpp"
#include "shell/shell.hpp"
#include "tag_files/tag_resource_cache_predictor.hpp"
#include "tag_files/tag_resource_cache_stoler.hpp"
#include "tag_files/tag_resource_threading.hpp"

struct s_cache_file_tag_resource_data;
struct s_tag_resource
{
	union
	{
		dword resource_handle;
		s_cache_file_tag_resource_data* resource_data;
	};

	dword definition_address;
};
static_assert(sizeof(s_tag_resource) == 0x8);

template<typename t_resource_type, dword ...t_extra>
struct c_typed_tag_resource :
	s_tag_resource
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

	s_cache_file_resource_streaming_sublocation_table* m_streaming_sublocation_table;
	c_basic_buffer<void> __buffer8;
	byte* __unknown10;
	c_basic_buffer<void> __buffer14;
};
static_assert(sizeof(c_cache_file_streamed_sublocation_decompressor) == sizeof(c_cache_file_decompressor) + 0x18);

struct s_cache_file_resource_runtime_active_zone_state :
	s_scenario_zone_state
{
};
static_assert(sizeof(s_cache_file_resource_runtime_active_zone_state) == sizeof(s_scenario_zone_state));

struct s_cache_file_resource_prefetch_map_state
{
	long campaign_id;
	c_static_string<k_tag_long_string_length> scenario_path;
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

struct c_cache_file_tag_resource_runtime_control_allocation :
	c_allocation_base
{
	c_basic_buffer<void> m_available_region;
	c_basic_buffer<void> m_allocated_region;
};
static_assert(sizeof(c_cache_file_tag_resource_runtime_control_allocation) == 0x14);

struct c_cache_file_tag_resource_location_handler
{
	struct
	{
		void* get_location_sub_location;
		void* get_location_memory_size;
		void* try_to_get_location_checksum;
		void* get_location_indirect_identifier;
		void* get_location_file_sort_key;
	}*__vftable;

	c_cache_file_resource_uber_location_table* m_uber_location_table;
};
static_assert(sizeof(c_cache_file_tag_resource_location_handler) == 0x8);

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
static_assert(0x000 == offsetof(c_cache_file_tag_resource_runtime_in_level_memory_manager, m_resource_storage_range));
static_assert(0x008 == offsetof(c_cache_file_tag_resource_runtime_in_level_memory_manager, __unknown8));
static_assert(0x010 == offsetof(c_cache_file_tag_resource_runtime_in_level_memory_manager, m_actual_resource_storage_range));
static_assert(0x018 == offsetof(c_cache_file_tag_resource_runtime_in_level_memory_manager, m_stoler_range));
static_assert(0x020 == offsetof(c_cache_file_tag_resource_runtime_in_level_memory_manager, m_actual_storage_range_read_locked));
static_assert(0x024 == offsetof(c_cache_file_tag_resource_runtime_in_level_memory_manager, m_writeable_range));
static_assert(0x02C == offsetof(c_cache_file_tag_resource_runtime_in_level_memory_manager, __data2C));
static_assert(0x038 == offsetof(c_cache_file_tag_resource_runtime_in_level_memory_manager, m_tag_resource_cache));
static_assert(0x438 == offsetof(c_cache_file_tag_resource_runtime_in_level_memory_manager, m_cache_file_resource_allocator));
static_assert(0x44C == offsetof(c_cache_file_tag_resource_runtime_in_level_memory_manager, m_cache_file_resource_allocation_region));
static_assert(0x454 == offsetof(c_cache_file_tag_resource_runtime_in_level_memory_manager, m_resource_header_location_table));

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

struct c_cache_file_uncompressed_decompressor;
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
struct s_indirect_cache_file_read_request;
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

struct c_io_result;
struct s_cache_file_resource_gestalt;
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
public:
	void commit_zone_state();
	void load_pending_resources_blocking(c_io_result* io_result);
	void load_required_resources_blocking(c_io_result* io_result);
	void idle();
	void initialize(c_allocation_base* allocation);
	void __thiscall sub_561C00(e_game_mode game_mode);
	void initialize_for_new_map(
		e_game_mode game_mode,
		long cache_file_resource_gestalt_index,
		long resource_vtable_list_count,
		s_cache_file_tag_resource_vtable const** resource_vtable_list,
		c_cache_file_runtime_decompressor_registry* runtime_decompressor_registry);

	void* get_cached_resource_data(long resource_handle)
	{
		return m_in_level_memory_manager.m_tag_resource_cache.get_resource_data(resource_handle);
	}

//protected:
	s_cache_file_resource_gestalt* m_resource_gestalt;
	s_cache_file_resource_runtime_active_zone_state m_active_zone_state;
	s_cache_file_resource_runtime_prefetching_state m_prefetching_state;

	// #TODO: name this
	bool __unknown260;

	bool m_resources_required; // m_dirty_active_resource_mask?
	bool m_resources_pending;  // m_dirty_pending_resource_mask?
	bool m_resources_loading;

	c_typed_allocation_data_no_destruct<c_tag_resource_cache_dynamic_predictor, 0> m_dynamic_predictor;
	c_tag_resource_cache_precompiled_predictor m_precompiled_predictor;
	c_static_flags<32768> m_required_resources;
	c_static_flags<32768> m_pending_resources;
	c_static_array<s_cache_file_tag_resource_vtable const*, 16> m_tag_resource_vtables;
	c_wrapped_array<void*> m_resource_runtime_data;
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
static_assert(0x00024 == offsetof(c_cache_file_tag_resource_runtime_manager, m_resource_gestalt));
static_assert(0x00028 == offsetof(c_cache_file_tag_resource_runtime_manager, m_active_zone_state));
static_assert(0x00048 == offsetof(c_cache_file_tag_resource_runtime_manager, m_prefetching_state));
static_assert(0x00260 == offsetof(c_cache_file_tag_resource_runtime_manager, __unknown260));
static_assert(0x00264 == offsetof(c_cache_file_tag_resource_runtime_manager, m_dynamic_predictor));
static_assert(0x00274 == offsetof(c_cache_file_tag_resource_runtime_manager, m_precompiled_predictor));
static_assert(0x28298 == offsetof(c_cache_file_tag_resource_runtime_manager, m_required_resources));
static_assert(0x29298 == offsetof(c_cache_file_tag_resource_runtime_manager, m_pending_resources));
static_assert(0x2A298 == offsetof(c_cache_file_tag_resource_runtime_manager, m_tag_resource_vtables));
static_assert(0x2A2D8 == offsetof(c_cache_file_tag_resource_runtime_manager, m_resource_runtime_data));
static_assert(0x2A2E0 == offsetof(c_cache_file_tag_resource_runtime_manager, m_resource_interop_data_buffer));
static_assert(0x2A2E8 == offsetof(c_cache_file_tag_resource_runtime_manager, m_combined_tag_resource_datum_handler));
static_assert(0x2A300 == offsetof(c_cache_file_tag_resource_runtime_manager, m_resource_location_handler));
static_assert(0x2A308 == offsetof(c_cache_file_tag_resource_runtime_manager, m_in_level_memory_manager));
static_assert(0x2A788 == offsetof(c_cache_file_tag_resource_runtime_manager, m_prefetch_map_states2A788));
static_assert(0x2A998 == offsetof(c_cache_file_tag_resource_runtime_manager, m_rollover_table));
static_assert(0x6A9A8 == offsetof(c_cache_file_tag_resource_runtime_manager, m_resource_codec_service));
static_assert(0x6AC14 == offsetof(c_cache_file_tag_resource_runtime_manager, m_optional_cache_backend));
static_assert(0x6ACA4 == offsetof(c_cache_file_tag_resource_runtime_manager, m_game_mode));
static_assert(0x6ACA8 == offsetof(c_cache_file_tag_resource_runtime_manager, m_running_off_dvd));
static_assert(0x6ACA9 == offsetof(c_cache_file_tag_resource_runtime_manager, __unknown6ACA9));
static_assert(0x6ACAA == offsetof(c_cache_file_tag_resource_runtime_manager, __data6ACAA));

struct c_cache_file_tag_resource_runtime_manager_allocation :
	public c_typed_allocation_data_no_destruct<c_cache_file_tag_resource_runtime_manager, 1>
{
public:
	void construct();
};
extern c_cache_file_tag_resource_runtime_manager_allocation& g_resource_runtime_manager;

enum e_game_mode;
struct s_scenario_game_state;

extern void __cdecl cache_file_tag_resources_dispose();
extern void __cdecl cache_file_tag_resources_dispose_from_old_map();
extern void __cdecl cache_file_tag_resources_initialize();
extern void __cdecl cache_file_tag_resources_initialize_for_new_map(e_game_mode game_mode);
extern void __cdecl cache_file_tag_resources_load_pending_resources_blocking(c_io_result* io_result);
extern void __cdecl cache_file_tag_resources_load_required_resources_blocking(c_io_result* io_result);
extern bool __cdecl cache_file_tag_resources_prefetch_update_required();
extern void __cdecl cache_file_tag_resources_update_prefetch_state();
extern void __cdecl cache_file_tag_resources_set_zone_state(long scenario_index, long zone_set_name, s_scenario_zone_state const* zone_state);
extern void __cdecl cache_file_tag_resources_start_map_prefetch(short campaign_id, char const* scenario_path);
extern void __cdecl cache_file_tag_resources_stop_map_prefetch();

extern bool __cdecl tag_resource_available(s_tag_resource const* resource);
extern void* __cdecl tag_resource_get(s_tag_resource const* resource);
extern long __cdecl tag_resources_lock_game();
extern void __cdecl tag_resources_lock_render();
extern bool __cdecl tag_resources_locked_for_current_thread_UGLY();
extern void __cdecl tag_resources_main_loop_idle();
extern void __cdecl tag_resources_prepare_for_new_map();
extern void __cdecl tag_resources_pump_io();
extern void __cdecl tag_resources_stagnate_deferred_resources();
extern void __cdecl tag_resources_unlock_game(long& lock);

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

