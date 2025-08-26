#include "cache/cache_file_tag_resource_runtime.hpp"

#include "cache/cache_files.hpp"
#include "cache/cache_files_windows.hpp"
#include "cseries/runtime_state.hpp"
#include "game/game_globals.hpp"
#include "main/main_game.hpp"
#include "memory/module.hpp"
#include "scenario/scenario_zone_resources.hpp"

#include <DDS.h>

REFERENCE_DECLARE(0x01670A18, int32 const, g_cache_file_tag_resource_vtable_count);
REFERENCE_DECLARE_ARRAY(0x018EB7A8, const s_cache_file_tag_resource_vtable*, g_cache_file_tag_resource_vtable_list, 2);

REFERENCE_DECLARE(0x023916C0, c_cache_file_tag_resource_runtime_manager_allocation, g_resource_runtime_manager);

HOOK_DECLARE_CLASS_MEMBER(0x00561C00, c_cache_file_tag_resource_runtime_manager, initialize_files);
HOOK_DECLARE(0x00563E10, tag_resource_get);
HOOK_DECLARE(0x00563F80, tag_resources_lock_game);
HOOK_DECLARE(0x00563FC0, tag_resources_lock_render);
HOOK_DECLARE(0x00563FE0, tag_resources_locked_for_current_thread_UGLY);
HOOK_DECLARE(0x00564010, tag_resources_pump_io);
HOOK_DECLARE(0x005640B0, tag_resources_unlock_game);

c_static_sized_dynamic_array<const s_resource_file_header*, 1024> g_resource_file_headers;

// DO NOT ENABLE
//#define USE_SHARED_CACHE_FILES
// $TODO `c_cache_file_tag_resource_runtime_manager::get_shared_file_block`

#if defined(USE_SHARED_CACHE_FILES)
enum e_cache_file_resource_shared_file_bit
{
	_cache_file_resource_shared_file_use_header_io_offset_bit = 0,
	_cache_file_resource_shared_file_optional_bit,
	_cache_file_resource_shared_file_use_header_locations_bit,

	k_number_of_cache_file_resource_shared_file_bits,
};

struct s_cache_file_resource_shared_file
{
	c_static_string<256> file_name;
	int16 global_location_index_offset;
	c_flags_no_init<e_cache_file_resource_shared_file_bit, uns16, k_number_of_cache_file_resource_shared_file_bits> flags;
	uns32 io_offset;
};
static_assert(sizeof(s_cache_file_resource_shared_file) == 0x108);
#endif

void c_cache_file_tag_resource_runtime_manager::commit_zone_state()
{
	INVOKE_CLASS_MEMBER(0x0055FDC0, c_cache_file_tag_resource_runtime_manager, commit_zone_state);
}

void __thiscall c_cache_file_tag_resource_runtime_manager_allocation::construct()
{
	INVOKE_CLASS_MEMBER(0x0055FF10, c_cache_file_tag_resource_runtime_manager_allocation, construct);
}

void __cdecl cache_file_tag_resources_dispose()
{
	INVOKE(0x0055F650, cache_file_tag_resources_dispose);

	//g_resource_runtime_manager.get()->dispose();
	//g_resource_runtime_manager.destruct();
}

void __cdecl cache_file_tag_resources_dispose_from_old_map()
{
	INVOKE(0x0055F6B0, cache_file_tag_resources_dispose_from_old_map);

	//g_resource_runtime_manager.get()->dispose_from_old_map();
}

void __cdecl cache_file_tag_resources_get_active_tag_set(uns32 active_bsp_zone_mask, uns32 touched_bsp_zone_mask, uns32 active_designer_zone_mask, uns32 active_cinematic_zone_mask, c_scenario_resource_registry* out_active_tags_flags)
{
	INVOKE(0x0055F6C0, cache_file_tag_resources_get_active_tag_set, active_bsp_zone_mask, touched_bsp_zone_mask, active_designer_zone_mask, active_cinematic_zone_mask, out_active_tags_flags);

	//g_resource_runtime_manager.get()->mark_available_tags(active_bsp_zone_mask, touched_bsp_zone_mask, active_designer_zone_mask, active_cinematic_zone_mask, out_active_tags_flags->get_tag_instance_flags());
}

//.text:0055F6D0 ; bool __cdecl cache_file_tag_resources_get_control_data_section(const void**, uns32*)

void __cdecl cache_file_tag_resources_initialize()
{
	//INVOKE(0x0055F700, cache_file_tag_resources_initialize);

	g_resource_runtime_manager.construct();
	g_resource_runtime_manager.get()->initialize(g_runtime_state_allocation);
}

void __cdecl cache_file_tag_resources_initialize_for_new_map(e_game_mode game_mode)
{
	//INVOKE(0x0055F720, cache_file_tag_resources_initialize_for_new_map, game_mode);

	c_cache_file_runtime_decompressor_registry runtime_decompressor_registry{};
	g_resource_runtime_manager.get()->initialize_for_new_map(
		game_mode,
		NONE,
		g_cache_file_tag_resource_vtable_count,
		g_cache_file_tag_resource_vtable_list,
		&runtime_decompressor_registry);
}

void __cdecl cache_file_tag_resources_load_pending_resources_blocking(c_io_result* io_result)
{
	//INVOKE(0x0055F760, cache_file_tag_resources_load_pending_resources_blocking, io_result);

	g_resource_runtime_manager.get()->commit_zone_state();
	g_resource_runtime_manager.get()->load_pending_resources_blocking(io_result);
}

void __cdecl cache_file_tag_resources_load_required_resources_blocking(c_io_result* io_result)
{
	//INVOKE(0x0055F7C0, cache_file_tag_resources_load_pending_resources_blocking, io_result);

	g_resource_runtime_manager.get()->commit_zone_state();
	g_resource_runtime_manager.get()->load_required_resources_blocking(io_result);
}

//.text:0055F820 ; real32 __cdecl cache_file_tag_resources_map_prefetch_progress(int16, const char*)
//.text:0055F850 ; bool __cdecl cache_file_tag_resources_map_prefetched(int16, const char*)

bool __cdecl cache_file_tag_resources_prefetch_update_required()
{
	return INVOKE(0x0055F870, cache_file_tag_resources_prefetch_update_required);

	//if (g_resource_runtime_manager.alive())
	//	g_resource_runtime_manager.get()->need_prefetch_update();
}

void __cdecl cache_file_tag_resources_prepare_for_next_map()
{
	//INVOKE(0x0055F890, cache_file_tag_resources_prepare_for_next_map);

	g_resource_runtime_manager.get()->prepare_for_next_map();
}

void __cdecl cache_file_tag_resources_update_prefetch_state()
{
	return INVOKE(0x0055F960, cache_file_tag_resources_update_prefetch_state);
}

void __cdecl cache_file_tag_resources_set_zone_state(int32 scenario_index, int32 zone_set_name, const s_scenario_zone_state* zone_state)
{
	INVOKE(0x0055F8E0, cache_file_tag_resources_set_zone_state, scenario_index, zone_set_name, zone_state);
}

void __cdecl cache_file_tag_resources_start_map_prefetch(int16 campaign_id, const char* scenario_path)
{
	INVOKE(0x0055F900, cache_file_tag_resources_start_map_prefetch, campaign_id, scenario_path);
}

void __cdecl cache_file_tag_resources_stop_map_prefetch()
{
	INVOKE(0x0055F920, cache_file_tag_resources_stop_map_prefetch);
}

//.text:0055F960 ; void __cdecl cache_file_tag_resources_update_prefetch_state()
//.text:0055F9B0 ; 
//.text:0055F9F0 ; 
//.text:0055FA20 ; 
//.text:0055FA70 ; 
//.text:0055FAA0 ; 

void c_cache_file_tag_resource_runtime_manager::load_pending_resources_blocking(c_io_result* io_result)
{
	INVOKE_CLASS_MEMBER(0x005626C0, c_cache_file_tag_resource_runtime_manager, load_pending_resources_blocking, io_result);

	//if (m_resource_gestalt)
	//{
	//	m_threaded_tag_resource_cache.load_pending_data_only_blocking(io_result);
	//}
	//
	//if (m_dirty_prefetch_map_state)
	//{
	//	m_threaded_tag_resource_cache.gobble_up_memory();
	//	m_threaded_tag_resource_cache.restart_prefetching();
	//	m_dirty_prefetch_map_state = false;
	//}
}

void c_cache_file_tag_resource_runtime_manager::load_required_resources_blocking(c_io_result* io_result)
{
	INVOKE_CLASS_MEMBER(0x00562710, c_cache_file_tag_resource_runtime_manager, load_required_resources_blocking, io_result);

	//if (m_resource_gestalt)
	//{
	//	m_threaded_tag_resource_cache.load_required_data_only_blocking(io_result);
	//}
	//
	//m_loaded_any_resources = true;
	//if (m_dirty_prefetch_map_state)
	//{
	//	m_threaded_tag_resource_cache.gobble_up_memory();
	//	m_threaded_tag_resource_cache.restart_prefetching();
	//	m_dirty_prefetch_map_state = false;
	//}
}

void c_cache_file_tag_resource_runtime_manager::lock_for_game()
{
	INVOKE_CLASS_MEMBER(0x005627A0, c_cache_file_tag_resource_runtime_manager, lock_for_game);

	//if (m_resource_gestalt)
	//{
	//	m_threaded_tag_resource_cache.lock_for_current_thread();
	//}
}

void c_cache_file_tag_resource_runtime_manager::lock_for_render()
{
	INVOKE_CLASS_MEMBER(0x005627C0, c_cache_file_tag_resource_runtime_manager, lock_for_render);

	//if (m_resource_gestalt)
	//{
	//	m_threaded_tag_resource_cache.lock_for_current_thread();
	//}
}

bool c_cache_file_tag_resource_runtime_manager::locked_for_current_thread_UGLY()
{
	return INVOKE_CLASS_MEMBER(0x005627E0, c_cache_file_tag_resource_runtime_manager, locked_for_current_thread_UGLY);
}

bool c_cache_file_tag_resource_runtime_manager::locked_for_game_UGLY() const
{
	return INVOKE_CLASS_MEMBER(0x005627F0, c_cache_file_tag_resource_runtime_manager, locked_for_game_UGLY);
}

void c_cache_file_tag_resource_runtime_manager::prepare_for_next_map()
{
	INVOKE_CLASS_MEMBER(0x00562D90, c_cache_file_tag_resource_runtime_manager, prepare_for_next_map);
}

void c_cache_file_tag_resource_runtime_manager::pump_io()
{
	INVOKE_CLASS_MEMBER(0x005630E0, c_cache_file_tag_resource_runtime_manager, pump_io);
}

void c_cache_file_tag_resource_runtime_manager::stagnate_deferred_resources()
{
	INVOKE_CLASS_MEMBER(0x00563C70, c_cache_file_tag_resource_runtime_manager, stagnate_deferred_resources);
}

bool __cdecl tag_resource_available(const s_tag_resource* resource)
{
	return INVOKE(0x00563DC0, tag_resource_get, resource);
}

void* __cdecl tag_resource_get(const s_tag_resource* resource)
{
	//return INVOKE(0x00563E10, tag_resource_get, resource);

	ASSERT(resource);

	return g_resource_runtime_manager.get()->get_cached_resource_data(resource->resource_handle);
}

//.text:00563E50 ; s_tag_resource_state_snapshot __cdecl tag_resource_get_state_snapshot(int32)
//.text:00563E80 ; s_tag_resource_state_snapshot __cdecl tag_resource_get_state_snapshot(const s_tag_resource*)
//.text:00563EB0 ; bool __cdecl tag_resource_stream_async(const s_tag_resource*, bool, uns32, uns32, c_basic_buffer<void>, s_async_thread_marker*)
//.text:00563EE0 ; bool __cdecl tag_resource_test_access_state(const s_tag_resource*, e_tag_resource_access_state_bit)

void* __cdecl tag_resource_try_to_get(const s_tag_resource* resource)
{
	void* result = INVOKE(0x00563F30, tag_resource_try_to_get, resource);
	return result;
}

int32 __cdecl tag_resources_lock_game()
{
	//return INVOKE(0x00563F80, tag_resources_lock_game);

	if (g_resource_runtime_manager.get()->locked_for_game_UGLY())
	{
		return 0;
	}

	g_resource_runtime_manager.get()->lock_for_game();
	return 1;
}

void __cdecl tag_resources_lock_render()
{
	//INVOKE(0x00563FC0, tag_resources_lock_render);

	g_resource_runtime_manager.get()->lock_for_render();
}

bool __cdecl tag_resources_locked_for_current_thread_UGLY()
{
	//return INVOKE(0x00563FE0, tag_resources_locked_for_current_thread_UGLY);

	return g_resource_runtime_manager.get()->locked_for_current_thread_UGLY();
}

void __cdecl tag_resources_main_loop_idle()
{
	//INVOKE(0x00563FF0, tag_resources_main_loop_idle);

	g_resource_runtime_manager.get()->idle();
}

void __cdecl tag_resources_prepare_for_new_map() // nullsub
{
	//INVOKE(0x00564000, tag_resources_prepare_for_new_map);
}

void __cdecl tag_resources_pump_io()
{
	//INVOKE(0x00564010, tag_resources_pump_io);

	g_resource_runtime_manager.get()->pump_io();
}

//.text:00564070 ; tag_resources_demand_optional_resources

void __cdecl tag_resources_stagnate_deferred_resources()
{
	//INVOKE(0x00564090, tag_resources_stagnate_deferred_resources);

	g_resource_runtime_manager.get()->stagnate_deferred_resources();
}

void __cdecl tag_resources_unlock_game(int32& lock)
{
	//INVOKE(0x005640B0, tag_resources_unlock_game, lock);

	if (lock == 1)
	{
		g_resource_runtime_manager.get()->unlock_for_game();
		lock = 0;
	}
}

void c_cache_file_tag_resource_runtime_manager::unlock_for_game()
{
	INVOKE_CLASS_MEMBER(0x00564840, c_cache_file_tag_resource_runtime_manager, unlock_for_game);
}

void c_cache_file_tag_resource_runtime_manager::idle()
{
	INVOKE_CLASS_MEMBER(0x005619D0, c_cache_file_tag_resource_runtime_manager, idle);
}

void c_cache_file_tag_resource_runtime_manager::initialize(c_allocation_base* allocation)
{
	INVOKE_CLASS_MEMBER(0x00561B40, c_cache_file_tag_resource_runtime_manager, initialize, allocation);

	//m_running_off_dvd = cache_files_running_off_dvd();
	//m_in_level_memory_manager.m_resource_header_location_table.m_uber_location_table.__unknown4 = 0;
	//m_game_mode = _game_mode_none;
	//m_resource_gestalt = NULL;
	//m_in_level_memory_manager.m_tag_resource_cache.initialize();
	//physical_memory_create_resizeable_contiguous_region(this);
	//m_optional_cache_backend.initialize(_map_memory_configuration_none, NULL);
	//optional_cache_set_in_game_backend(&m_optional_cache_backend);
	//m_in_level_memory_manager.m_resource_header_location_table.m_header_file_locations
	//	= data_new("shared file handles", 8, sizeof(c_cache_file_resource_header_location_table::s_header_file_location), 0, g_runtime_state_allocation);
	//m_prefetch_map_states2A788[1].campaign_id = NONE;
	//m_prefetch_map_states2A788[1].scenario_path.clear();
	//m_prefetch_map_states2A788[1].__unknown104 = 0;
	//m_prefetch_map_states2A788[0].campaign_id = NONE;
	//m_prefetch_map_states2A788[0].scenario_path.clear();
	//m_prefetch_map_states2A788[0].__unknown104 = 0;
}

void __thiscall c_cache_file_tag_resource_runtime_manager::initialize_files(e_game_mode game_mode)
{
	//INVOKE_CLASS_MEMBER(0x00561C00, c_cache_file_tag_resource_runtime_manager, initialize_files, game_mode);

	data_make_valid(m_shared_file_handles);

#if defined(USE_SHARED_CACHE_FILES)
	const c_typed_tag_block<s_cache_file_resource_shared_file>* shared_file_block = c_cache_file_tag_resource_runtime_manager::get_shared_file_block();
	ASSERT(shared_file_block);
	m_shared_file_datum_indices.resize(shared_file_block->count);
#else
	m_shared_file_datum_indices.resize(6);
#endif

	for (int32& shared_file_datum_index : m_shared_file_datum_indices)
	{
		shared_file_datum_index = NONE;
	}

#if defined(USE_SHARED_CACHE_FILES)
	m_shared_file_datum_indices[m_shared_file_datum_indices.new_element_index()] = shared_file_block->count;
#else
	m_shared_file_datum_indices[m_shared_file_datum_indices.new_element_index()] = 6;
#endif

	int32* master_shared_file_index_reference = &m_shared_file_datum_indices[_map_file_index_shared_ui];
	ASSERT(*master_shared_file_index_reference == NONE);

	int32 master_shared_file_index = datum_new_at_absolute_index(m_shared_file_handles, _map_file_index_shared_ui);
	s_cache_file_tag_resource_runtime_shared_file* runtime_shared_file = DATUM_GET(m_shared_file_handles, s_cache_file_tag_resource_runtime_shared_file, master_shared_file_index);

	// $TODO assert "could not get the master file indirect handle from the cache file!"
	if (cache_file_get_master_indirect_file_handle(&runtime_shared_file->indirect_file))
	{
		// $TODO assert "could not get the master resource section offset from the cache file!"
		cache_file_get_master_resource_section_offset(&runtime_shared_file->resource_section_offset);
	}

	// $TODO assert "could not get the master file async handle from the cache file!"
	cache_file_get_master_async_file_handle(&runtime_shared_file->async_file_handle);

	// $TODO assert "could not get the master file overlapped handle from the cache file!"
	cache_file_get_master_overlapped_file_handle(&runtime_shared_file->overlapped_handle);

	runtime_shared_file->shared_resource_usage = cache_file_try_to_get_master_shared_resource_usage();
	*master_shared_file_index_reference = master_shared_file_index;

#if defined(USE_SHARED_CACHE_FILES)
	s_game_globals* game_globals = scenario_get_game_globals();
	e_language language = game_globals->language;
	for (int32 shared_file_index = 0; shared_file_index < shared_file_block->count; shared_file_index++)
	{
		s_cache_file_resource_shared_file* resource_shared_file = TAG_BLOCK_GET_ELEMENT(shared_file_block, shared_file_index, s_cache_file_resource_shared_file);
		if (resource_shared_file->flags.test(_cache_file_resource_shared_file_optional_bit))
		{
			bool open_success = c_cache_file_tag_resource_runtime_manager::try_to_open_shared_file(shared_file_index);
			ASSERT(open_success);
			if (c_cache_file_tag_resource_runtime_manager::is_shared_sound_file(shared_file_index))
			{
				language = cache_file_get_valid_shared_sound_map_language();
			}
		}
	}
	game_globals->language = language;
#else
	// psuedo shared cache file open
	if (m_cache_file_resource_gestalt->shared_files_available)
	{
		for (e_map_file_index map_file_index = _map_file_index_shared_resources; map_file_index < k_cached_map_file_shared_count; map_file_index++)
		{
			int32 next_shared_file_index = datum_new_at_absolute_index(m_shared_file_handles, map_file_index);
			s_cache_file_tag_resource_runtime_shared_file* next_header_file_location = DATUM_GET(m_shared_file_handles, s_cache_file_tag_resource_runtime_shared_file, next_shared_file_index);

			if (cached_map_file_is_shared(map_file_index - 1))
			{
				if (cache_file_get_indirect_file_handle_from_index(map_file_index, &next_header_file_location->indirect_file))
				{
					next_header_file_location->resource_section_offset = 0;
				}

				cache_file_get_async_file_handle_from_index(map_file_index, &next_header_file_location->async_file_handle);
				cache_file_get_overlapped_file_handle_from_index(map_file_index, &next_header_file_location->overlapped_handle);
			}

			m_shared_file_datum_indices[map_file_index] = next_shared_file_index;
		}
	}
#endif
}

void c_cache_file_tag_resource_runtime_manager::initialize_for_new_map(
	e_game_mode game_mode,
	int32 cache_file_resource_gestalt_index,
	int32 resource_vtable_list_count,
	const s_cache_file_tag_resource_vtable** resource_vtable_list,
	c_cache_file_runtime_decompressor_registry* runtime_decompressor_registry)
{
	INVOKE_CLASS_MEMBER(0x00561DF0, c_cache_file_tag_resource_runtime_manager, initialize_for_new_map,
		game_mode,
		cache_file_resource_gestalt_index,
		resource_vtable_list_count,
		resource_vtable_list,
		runtime_decompressor_registry);
}

