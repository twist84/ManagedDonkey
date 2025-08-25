#pragma once

#include "cache/cache_file_codec_work.hpp"
#include "cache/cache_file_runtime_codecs.hpp"
#include "cache/cache_file_tag_resource_datum_handler.hpp"
#include "cache/cache_file_tag_resource_location_table.hpp"
#include "cache/cache_file_tag_resource_vtable_list.hpp"
#include "cache/optional_cache.hpp"
#include "cache/physical_memory_map.hpp"
#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "multithreading/synchronized_value.hpp"
#include "scenario/scenario.hpp"
#include "tag_files/tag_resource_cache_predictor.hpp"
#include "tag_files/tag_resource_cache_stoler.hpp"
#include "tag_files/tag_resource_threading.hpp"

#define TAG_RESOURCE_GET(TYPE, RESOURCE) ((TYPE*)tag_resource_get(RESOURCE))
#define TAG_RESOURCE_TRY_TO_GET(TYPE, RESOURCE) ((TYPE*)tag_resource_try_to_get(RESOURCE))

struct s_cache_file_local_resource_location
{
	uns32 flags : 2;
	uns32 file_size : 30;
	uns32 memory_size;
	s_network_http_request_hash entire_checksum;
};
static_assert(sizeof(s_cache_file_local_resource_location) == 0x1C);

struct s_cache_file_insertion_point_resource_usage
{
	//byte __data[0xB4];

	int8 initial_zone_set_index;
	byte pad[0x3];
	c_static_flags<1024> shared_required_locations;
	c_static_flags<320> local_required_locations;

	// $TODO idk what this could be, something from ODST?
	byte __dataAC[0x8];
};
static_assert(sizeof(s_cache_file_insertion_point_resource_usage) == 0xB4);

struct s_cache_file_shared_resource_usage
{
	s_tag_persistent_identifier shared_layout_identifier;
	uns16 shared_location_count;
	uns16 local_location_count;
	uns32 first_file_offset;
	s_tag_persistent_identifier codec_identifier;
	c_static_array<s_cache_file_local_resource_location, 320> local_locations;
	uns8 insertion_point_usage_count;
	int8 pad[0x3];
	c_static_array<s_cache_file_insertion_point_resource_usage, 9> insertion_point_usages;
};
static_assert(sizeof(s_cache_file_shared_resource_usage) == 0x2980);

template<typename t_type, int32 k_depth>
class c_typed_allocation_data_no_destruct
{
public:
	t_type* get()
	{
		ASSERT(m_live_object);

		return m_live_object;
	}

	bool alive()
	{
		return m_live_object != nullptr;
	}

protected:
	c_basic_buffer<void> m_opaque_storage;
	t_type* m_live_object;
	c_allocation_base* m_allocator;
};
static_assert(sizeof(c_typed_allocation_data_no_destruct<int32, 0>) == 0x10);

class c_cache_file_streamed_sublocation_decompressor :
	public c_cache_file_decompressor
{
public:
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

struct s_cache_file_resource_runtime_active_zone_state
{
	s_scenario_zone_state zone_state;
	int32 prefetching_shared_file_index;
};
static_assert(sizeof(s_cache_file_resource_runtime_active_zone_state) == 0x24);

struct s_cache_file_resource_prefetch_map_state
{
	int32 insertion_point_index;
	c_static_string<k_tag_long_string_length> map_name;
	bool __unknown104;
};
static_assert(sizeof(s_cache_file_resource_prefetch_map_state) == 0x108);

struct s_cache_file_resource_runtime_prefetching_state
{
	int32 prefetching_shared_file_index;
	s_cache_file_resource_prefetch_map_state active_prefetch;
	s_cache_file_resource_prefetch_map_state requested_prefetch;
};
static_assert(sizeof(s_cache_file_resource_runtime_prefetching_state) == 0x214);

class c_cache_file_tag_resource_runtime_control_allocation :
	c_allocation_base
{
public:
	c_basic_buffer<void> m_available_region;
	c_basic_buffer<void> m_allocated_region;
};
static_assert(sizeof(c_cache_file_tag_resource_runtime_control_allocation) == 0x14);

class c_tag_resource_cache_file_location_handler
{
public:
	struct
	{
		void* get_location_sub_location;
		void* get_location_memory_size;
		void* try_to_get_location_checksum;
		void* get_location_indirect_identifier;
		void* get_location_file_sort_key;
	}*__vftable;
};
static_assert(sizeof(c_tag_resource_cache_file_location_handler) == 0x4);

class c_cache_file_resource_rollover_table
{
public:
	struct s_rollover_entry_estimated
	{
		int32 file_size;
		int32 size;
		c_basic_buffer<void> range;
	};
	static_assert(sizeof(s_rollover_entry_estimated) == 0x10);

	c_static_sized_dynamic_array<s_rollover_entry_estimated, 16384> m_estimated_rollover_entries;
	c_basic_buffer<void> m_entry_bounds;
	int32 m_next_moved_entry_index;
};
static_assert(sizeof(c_cache_file_resource_rollover_table) == 0x40010);

class c_indirect_cache_file_decompressor_service
{
public:
	virtual c_cache_file_decompressor* begin_decompression(uns64, int32, c_basic_buffer<void>) = 0;
	virtual void dispose_decompressor(uns64, int32, c_cache_file_decompressor*) = 0;
};
static_assert(sizeof(c_indirect_cache_file_decompressor_service) == 0x4);

class c_cache_file_uncompressed_decompressor;
class c_cache_file_tag_resource_codec_service :
	c_indirect_cache_file_decompressor_service
{
public:
	virtual c_cache_file_decompressor* begin_decompression(uns64, int32, c_basic_buffer<void>)
	{
		throw;
	}
	virtual void dispose_decompressor(uns64, int32, c_cache_file_decompressor*)
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

class c_cache_file_resource_stoler;
class c_cache_file_resource_optional_cache_backend :
	public c_optional_cache_backend
{
public:
	c_tag_resource_cache_stoler m_cache_stoler;
	c_static_flags<32> m_flags;
	c_cache_file_resource_stoler* m_resource_stoler;
};
static_assert(sizeof(c_cache_file_resource_optional_cache_backend) == 0x90);

struct s_tag_resource_definition;
class c_tag_resource_runtime_listener
{
public:
	virtual bool register_resource(int32 tag_index, int32 tag_resource_type_index, void* data);
	virtual void unregister_resource(int32 tag_index, int32 tag_resource_type_index, void* data);
};
static_assert(sizeof(c_tag_resource_runtime_listener) == 0x4);

class c_tag_resource_runtime_active_set
{
public:
	virtual bool any_resources_active() const;
	virtual bool is_resource_required(int32 resource_handle, int32 resource_owner) const;
	virtual bool is_resource_deferred(int32 resource_handle, int32 resource_owner) const;
	virtual bool is_resource_pending(int32 resource_handle, int32 resource_owner) const;
};
static_assert(sizeof(c_tag_resource_runtime_active_set) == 0x4);

struct s_tag_resource_location_handle_struct;
class c_tag_resource_cache_file_prefetch_set
{
public:
	virtual bool should_prefetch_location(s_tag_resource_location_handle_struct* location_handle)const;
};
static_assert(sizeof(c_tag_resource_cache_file_prefetch_set) == 0x4);

class c_tag_resource_page_range_allocator
{
public:
	virtual bool try_to_grab_restore_range(c_basic_buffer<void> desired_range, c_basic_buffer<void>* in_out_range);
	virtual bool try_to_resize_contiguous_range(c_basic_buffer<void>* in_out_range, uns32, uns32, uns32);
	virtual void shrink_for_buyback(c_basic_buffer<void>* in_out_range, uns32 new_size);
	virtual void reclaim_stolen_memory(c_basic_buffer<void>* in_out_range);
	virtual void release_allocation(c_basic_buffer<void>* in_out_range);
	virtual void make_range_writeable(c_basic_buffer<void> range);
	virtual void make_range_read_only(c_basic_buffer<void> range);
};
static_assert(sizeof(c_tag_resource_page_range_allocator) == 0x4);

class c_tag_resource_cache_file_reader
{
public:
	class c_tag_resource_cache_file_reader_vtbl* __vftable /*VFT*/;
};

struct s_indirect_cache_file_location;
class c_indirect_cache_file_location_atlas
{
public:
	virtual bool get_location(uns64, s_indirect_cache_file_location* out_location);
};
static_assert(sizeof(c_indirect_cache_file_location_atlas) == 0x4);

class c_tag_resource_prediction_atom_collector;
class c_tag_resource_prediction_atom_generator
{
public:
	virtual bool collect_tag_resources(int32 tag_index, int32 prediction_atom_handle, c_tag_resource_prediction_atom_collector* atom_collector);
};
static_assert(sizeof(c_tag_resource_prediction_atom_generator) == 0x4);

class c_cache_file_resource_stoler
{
public:
	virtual void* try_to_steal_memory(uns32 size);
	virtual void* steal_memory_shouldnt_fail(uns32 size);
	virtual void return_memory(void* stolen_memory, uns32 size);
};
static_assert(sizeof(c_cache_file_resource_stoler) == 0x4);

struct s_cache_file_tag_resource_runtime_shared_file :
	public s_datum_header
{
	uns16 flags;
	s_file_handle async_file_handle;
	s_file_handle overlapped_handle;
	s_indirect_file indirect_file;
	const s_cache_file_shared_resource_usage* shared_resource_usage;
	uns32 resource_section_offset;
	int32 map_file_index;
};
static_assert(sizeof(s_cache_file_tag_resource_runtime_shared_file) == 0x1C);

class c_tag_resource_cache_file_datum_handler
{
public:
	class c_tag_resource_cache_file_datum_handler_vtbl* __vftable /*VFT*/;
};

class c_io_result;
struct s_cache_file_resource_gestalt;
struct s_cache_file_resource_runtime_data_new;
class c_cache_file_tag_resource_runtime_manager :
	public c_tag_resource_runtime_listener,
	public c_tag_resource_runtime_active_set,
	public c_tag_resource_cache_file_prefetch_set,
	public c_tag_resource_page_range_allocator,
	public c_tag_resource_cache_file_reader,
	public c_indirect_cache_file_location_atlas,
	private c_physical_memory_contiguous_region_listener,
	private c_tag_resource_prediction_atom_generator,
	private c_cache_file_resource_stoler
{
public:
	void commit_zone_state();
	void load_pending_resources_blocking(c_io_result* io_result);
	void load_required_resources_blocking(c_io_result* io_result);
	void lock_for_game();
	void lock_for_render();
	bool locked_for_current_thread_UGLY();
	bool locked_for_game_UGLY() const;
	void prepare_for_next_map();
	void pump_io();
	void stagnate_deferred_resources();
	void unlock_for_game();
	void idle();
	void initialize(c_allocation_base* allocation);
	void __thiscall initialize_files(e_game_mode game_mode);
	void initialize_for_new_map(
		e_game_mode game_mode,
		int32 cache_file_resource_gestalt_index,
		int32 resource_vtable_list_count,
		const s_cache_file_tag_resource_vtable** resource_vtable_list,
		c_cache_file_runtime_decompressor_registry* runtime_decompressor_registry);

	void* get_cached_resource_data(int32 resource_handle)
	{
		return m_threaded_tag_resource_cache.get_resource_data(resource_handle);
	}

//protected:
	class c_cache_file_tag_resource_datum_handler :
		public c_tag_resource_cache_file_datum_handler
	{
	private:
		s_cache_file_resource_gestalt* m_resource_gestalt;
		c_wrapped_array<s_cache_file_resource_runtime_data_new> m_resource_runtime_data;
		bool m_cache_streaming_data;
		c_cache_file_resource_uber_location_table* m_file_location_table;
	};
	static_assert(sizeof(c_cache_file_tag_resource_datum_handler) == 0x18);

	class c_cache_file_tag_resource_location_handler :
		public c_tag_resource_cache_file_location_handler
	{
	private:
		c_cache_file_resource_uber_location_table* m_file_location_table;
	};
	static_assert(sizeof(c_cache_file_tag_resource_location_handler) == 0x8);

	s_cache_file_resource_gestalt* m_resource_gestalt;
	s_cache_file_resource_runtime_active_zone_state m_active_zone_state;
	s_cache_file_resource_runtime_prefetching_state m_prefetching_state;
	bool m_loaded_any_resources;
	bool m_dirty_active_resource_mask;
	bool m_dirty_pending_resource_mask;
	bool m_dirty_prefetch_map_state;
	c_typed_allocation_data_no_destruct<c_tag_resource_cache_dynamic_predictor, 0> m_dynamic_resource_predictor;
	c_tag_resource_cache_precompiled_predictor m_precomputed_resource_predictor;
	c_static_flags<32767> m_active_resources_mask;
	c_static_flags<32767> m_pending_resources_mask;
	c_static_array<const s_cache_file_tag_resource_vtable*, 16> m_vtable_mapping;
	c_wrapped_array<void*> m_resource_runtime_data;
	c_basic_buffer<void> m_resource_interop_buffer;
	c_cache_file_tag_resource_datum_handler m_resource_datum_handler;
	c_cache_file_tag_resource_location_handler m_resource_location_handler;
	c_basic_buffer<void> m_resource_storage_range;
	c_basic_buffer<void> m_desired_resource_storage_range;
	c_basic_buffer<void> m_actual_resource_storage_range;
	c_basic_buffer<void> m_stoler_range;
	bool m_actual_storage_range_read_locked;

	byte __pad2A32D[0x3];

	c_basic_buffer<void> m_writeable_range;

	byte __pad2A33C[0xC];

	c_thread_safeish_tag_resource_cache m_threaded_tag_resource_cache;
	c_cache_file_tag_resource_runtime_control_allocation m_cache_file_resource_allocation;
	c_basic_buffer<void> m_cache_file_resource_allocation_region;
	c_static_array<int32, 7> m_shared_file_datum_indices;
	int32 m_last_shared_file_datum_index;
	c_wrapped_data_array<s_cache_file_tag_resource_runtime_shared_file> m_shared_file_handles;
	c_cache_file_resource_uber_location_table m_uber_location_table;
	e_game_mode m_last_game_mode;
	s_cache_file_resource_prefetch_map_state m_last_ui_prefetch_state;
	s_cache_file_resource_prefetch_map_state m_last_game_prefetch_state;
	c_cache_file_resource_rollover_table m_rollover_table;
	c_cache_file_tag_resource_codec_service m_resource_codec_service;
	c_cache_file_resource_optional_cache_backend m_optional_cache_backend;
	e_game_mode m_active_game_mode;
	bool m_cache_streaming_data;
	bool m_cache_pages_for_next_map;

	// $TODO map this
	byte __data6ACAA[0x16];
};
static_assert(sizeof(c_cache_file_tag_resource_runtime_manager) == 0x6ACC0);
static_assert(0x00024 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_gestalt));
static_assert(0x00028 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_active_zone_state));
static_assert(0x0004C == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_prefetching_state));
static_assert(0x00260 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_loaded_any_resources));
static_assert(0x00261 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_dirty_active_resource_mask));
static_assert(0x00262 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_dirty_pending_resource_mask));
static_assert(0x00263 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_dirty_prefetch_map_state));
static_assert(0x00264 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_dynamic_resource_predictor));
static_assert(0x00274 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_precomputed_resource_predictor));
static_assert(0x28298 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_active_resources_mask));
static_assert(0x29298 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_pending_resources_mask));
static_assert(0x2A298 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_vtable_mapping));
static_assert(0x2A2D8 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_runtime_data));
static_assert(0x2A2E0 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_interop_buffer));
static_assert(0x2A2E8 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_datum_handler));
static_assert(0x2A300 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_location_handler));
static_assert(0x2A308 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_storage_range));
static_assert(0x2A310 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_desired_resource_storage_range));
static_assert(0x2A318 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_actual_resource_storage_range));
static_assert(0x2A320 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_stoler_range));
static_assert(0x2A328 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_actual_storage_range_read_locked));
static_assert(0x2A32C == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_writeable_range));
static_assert(0x2A340 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_threaded_tag_resource_cache));
static_assert(0x2A740 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_cache_file_resource_allocation));
static_assert(0x2A754 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_cache_file_resource_allocation_region));
static_assert(0x2A75C == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_shared_file_datum_indices));
static_assert(0x2A778 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_last_shared_file_datum_index));
static_assert(0x2A77C == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_shared_file_handles));
static_assert(0x2A780 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_uber_location_table));
static_assert(0x2A784 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_last_game_mode));
static_assert(0x2A788 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_last_ui_prefetch_state));
static_assert(0x2A890 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_last_game_prefetch_state));
static_assert(0x2A998 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_rollover_table));
static_assert(0x6A9A8 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_resource_codec_service));
static_assert(0x6AC14 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_optional_cache_backend));
static_assert(0x6ACA4 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_active_game_mode));
static_assert(0x6ACA8 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_cache_streaming_data));
static_assert(0x6ACA9 == OFFSETOF(c_cache_file_tag_resource_runtime_manager, m_cache_pages_for_next_map));
static_assert(0x6ACAA == OFFSETOF(c_cache_file_tag_resource_runtime_manager, __data6ACAA));

class c_cache_file_tag_resource_runtime_manager_allocation :
	public c_typed_allocation_data_no_destruct<c_cache_file_tag_resource_runtime_manager, 1>
{
public:
	void construct();
};
extern c_cache_file_tag_resource_runtime_manager_allocation& g_resource_runtime_manager;

class c_scenario_resource_registry;
struct s_scenario_game_state;

extern void __cdecl cache_file_tag_resources_dispose();
extern void __cdecl cache_file_tag_resources_dispose_from_old_map();
extern void __cdecl cache_file_tag_resources_get_active_tag_set(uns32 active_bsp_zone_mask, uns32 touched_bsp_zone_mask, uns32 active_designer_zone_mask, uns32 active_cinematic_zone_mask, c_scenario_resource_registry* out_active_tags_flags);
extern void __cdecl cache_file_tag_resources_initialize();
extern void __cdecl cache_file_tag_resources_initialize_for_new_map(e_game_mode game_mode);
extern void __cdecl cache_file_tag_resources_load_pending_resources_blocking(c_io_result* io_result);
extern void __cdecl cache_file_tag_resources_load_required_resources_blocking(c_io_result* io_result);
extern bool __cdecl cache_file_tag_resources_prefetch_update_required();
extern void __cdecl cache_file_tag_resources_prepare_for_next_map();
extern void __cdecl cache_file_tag_resources_update_prefetch_state();
extern void __cdecl cache_file_tag_resources_set_zone_state(int32 scenario_index, int32 zone_set_name, const s_scenario_zone_state* zone_state);
extern void __cdecl cache_file_tag_resources_start_map_prefetch(int16 campaign_id, const char* scenario_path);
extern void __cdecl cache_file_tag_resources_stop_map_prefetch();

extern bool __cdecl tag_resource_available(const s_tag_resource* resource);
extern void* __cdecl tag_resource_get(const s_tag_resource* resource);
extern void* __cdecl tag_resource_try_to_get(const s_tag_resource* resource);
extern int32 __cdecl tag_resources_lock_game();
extern void __cdecl tag_resources_lock_render();
extern bool __cdecl tag_resources_locked_for_current_thread_UGLY();
extern void __cdecl tag_resources_main_loop_idle();
extern void __cdecl tag_resources_prepare_for_new_map();
extern void __cdecl tag_resources_pump_io();
extern void __cdecl tag_resources_stagnate_deferred_resources();
extern void __cdecl tag_resources_unlock_game(int32& lock);

struct s_resource_file_header
{
	// tag info
	tag group_tag;
	int32 tag_index;

	// resource data size and offset from file begin
	uns32 file_size;
	int32 resource_index;
};
static_assert(sizeof(s_resource_file_header) == 0x10);

extern c_static_sized_dynamic_array<const s_resource_file_header*, 1024> g_resource_file_headers;

