#include "cache/cache_file_tag_resource_runtime.hpp"

#include "cache/cache_files.hpp"
#include "memory/module.hpp"

#include <DDS.h>

REFERENCE_DECLARE(0x01670A18, long const, g_cache_file_tag_resource_vtable_count);
REFERENCE_DECLARE_ARRAY(0x018EB7A8, s_cache_file_tag_resource_vtable const*, g_cache_file_tag_resource_vtable_list, 2);

using resource_runtime_manager_typed_allocation_data_no_destruct_t = c_typed_allocation_data_no_destruct<c_cache_file_tag_resource_runtime_manager, 1>;
REFERENCE_DECLARE(0x023916C0, resource_runtime_manager_typed_allocation_data_no_destruct_t, g_resource_runtime_manager);

c_static_sized_dynamic_array<s_resource_file_header const*, 1024> g_resource_file_headers;

HOOK_DECLARE(0x00563E10, tag_resource_get);

#define ISEXPERIMENTAL

#if defined(ISEXPERIMENTAL)

struct c_runtime_resource_cache_file_decompressor :
	public c_cache_file_decompressor
{
	c_runtime_resource_cache_file_decompressor() :
		m_holding_buffer()
	{
	}

	~c_runtime_resource_cache_file_decompressor()
	{
	}

	virtual bool begin(c_basic_buffer<void> a1) override
	{
		m_holding_buffer = a1;

		return true;
	}

	virtual bool decompress_buffer(c_basic_buffer<void> in_buffer, c_basic_buffer<void>* out_buffer) override
	{
		for (long i = 0; i < g_resource_file_headers.count(); i++)
		{
			s_resource_file_header const* file_header = g_resource_file_headers[i];

			// check buffer size is the same as compressed file size we set as the tag index
			if (static_cast<dword>(file_header->tag_index) != in_buffer.m_size)
				continue;

			if (static_cast<dword>(file_header->resource_index) != m_holding_buffer.m_size - 1)
				continue;

			if (file_header->group_tag == BITMAP_TAG)
			{
				DirectX::DDS_FILE_HEADER const* dds_file = reinterpret_cast<DirectX::DDS_FILE_HEADER const*>(file_header + 1);
				if (!dds_file)
					continue;

				m_holding_buffer.m_size = dds_file->header.pitchOrLinearSize;
				csmemcpy(m_holding_buffer.m_buffer, dds_file + 1, m_holding_buffer.m_size);
			}
		}

		return true;
	}

	virtual bool finish(c_basic_buffer<void>* a1) override
	{
		*a1 = m_holding_buffer;

		return true;
	}

	c_basic_buffer<void> m_holding_buffer;
};

struct c_runtime_resource_cache_file_decompressor_service :
	public c_single_instance_cache_file_decompressor_service<c_runtime_resource_cache_file_decompressor>
{
	virtual void initialize_decompressor(c_typed_opaque_data<c_runtime_resource_cache_file_decompressor>* decompressor_storage)
	{
		c_runtime_resource_cache_file_decompressor* decompressor = decompressor_storage->get();
		if (decompressor)
		{
			csmemcpy(decompressor, new c_runtime_resource_cache_file_decompressor(), sizeof(c_runtime_resource_cache_file_decompressor));
			*reinterpret_cast<c_runtime_resource_cache_file_decompressor**>(decompressor_storage + 1) = decompressor;
		}
		else
		{
			*reinterpret_cast<c_runtime_resource_cache_file_decompressor**>(decompressor_storage + 1) = 0;
		}
	}
};
static_assert(sizeof(c_runtime_resource_cache_file_decompressor_service) == 0x18);

struct c_runtime_tag_resource_cache_file_decompressor :
	public c_cache_file_decompressor
{
	c_runtime_tag_resource_cache_file_decompressor() :
		m_holding_buffer()
	{
	}

	~c_runtime_tag_resource_cache_file_decompressor()
	{
	}

	virtual bool begin(c_basic_buffer<void> a1) override
	{
		m_holding_buffer = a1;

		return true;
	}

	virtual bool decompress_buffer(c_basic_buffer<void> in_buffer, c_basic_buffer<void>* out_buffer) override
	{
		cache_file_tag_instance* instance = g_cache_file_globals.tag_instances[g_cache_file_globals.tag_index_absolute_mapping[in_buffer.m_size]];
		csmemcpy(m_holding_buffer.m_buffer, instance->base + instance->total_size + 256, m_holding_buffer.m_size);

		return true;
	}

	virtual bool finish(c_basic_buffer<void>* a1) override
	{
		*a1 = m_holding_buffer;

		return true;
	}

	c_basic_buffer<void> m_holding_buffer;
};

struct c_runtime_tag_resource_cache_file_decompressor_service :
	public c_single_instance_cache_file_decompressor_service<c_runtime_tag_resource_cache_file_decompressor>
{
	virtual void initialize_decompressor(c_typed_opaque_data<c_runtime_tag_resource_cache_file_decompressor>* decompressor_storage)
	{
		c_runtime_tag_resource_cache_file_decompressor* decompressor = decompressor_storage->get();
		if (decompressor)
		{
			csmemcpy(decompressor, new c_runtime_tag_resource_cache_file_decompressor(), sizeof(c_runtime_tag_resource_cache_file_decompressor));
			*reinterpret_cast<c_runtime_tag_resource_cache_file_decompressor**>(decompressor_storage + 1) = decompressor;
		}
		else
		{
			*reinterpret_cast<c_runtime_tag_resource_cache_file_decompressor**>(decompressor_storage + 1) = 0;
		}
	}
};
static_assert(sizeof(c_runtime_tag_resource_cache_file_decompressor_service) == 0x18);

static c_runtime_resource_cache_file_decompressor_service g_runtime_resource_cache_file_decompressor_service = {};
static c_runtime_tag_resource_cache_file_decompressor_service g_runtime_tag_resource_cache_file_decompressor_service = {};

void __fastcall cache_file_tag_resource_codec_service_initialize(c_cache_file_tag_resource_codec_service* _this, void* unused, c_allocation_base* allocator, c_cache_file_runtime_decompressor_registry* decompressor_registry, c_cache_file_resource_uber_location_table* uber_location_table)
{
	DECLFUNC(0x00561AB0, void, __thiscall, c_cache_file_tag_resource_codec_service*, c_allocation_base*, c_cache_file_runtime_decompressor_registry*, c_cache_file_resource_uber_location_table*)(_this, allocator, decompressor_registry, uber_location_table);

	{
		long new_element_index = _this->m_actual_runtime_decompressors.new_element_index();
		ASSERT(new_element_index == _cache_file_compression_codec_runtime_resource);

		_this->m_actual_runtime_decompressors[new_element_index] = &g_runtime_resource_cache_file_decompressor_service;
		ASSERT(_this->m_actual_runtime_decompressors[_cache_file_compression_codec_runtime_resource] == &g_runtime_resource_cache_file_decompressor_service);
	}

	{
		long new_element_index = _this->m_actual_runtime_decompressors.new_element_index();
		ASSERT(new_element_index == _cache_file_compression_codec_runtime_tag_resource);

		_this->m_actual_runtime_decompressors[new_element_index] = &g_runtime_tag_resource_cache_file_decompressor_service;
		ASSERT(_this->m_actual_runtime_decompressors[_cache_file_compression_codec_runtime_tag_resource] == &g_runtime_tag_resource_cache_file_decompressor_service);
	}
}
HOOK_DECLARE_CALL(0x00561FA0, cache_file_tag_resource_codec_service_initialize);

#endif // ISEXPERIMENTAL

void __cdecl cache_file_tag_resources_dispose()
{
	INVOKE(0x0055F650, cache_file_tag_resources_dispose);
}

void __cdecl cache_file_tag_resources_dispose_from_old_map()
{
	INVOKE(0x0055F6B0, cache_file_tag_resources_dispose_from_old_map);
}

//.text:0055F6C0 ; void __cdecl cache_file_tag_resources_get_active_tag_set(dword, dword, dword, dword, c_scenario_resource_registry*)
//.text:0055F6D0 ; bool __cdecl cache_file_tag_resources_get_control_data_section(void const**, dword*)

void __cdecl cache_file_tag_resources_initialize()
{
	INVOKE(0x0055F700, cache_file_tag_resources_initialize);

	//g_resource_runtime_manager.construct();
	//g_resource_runtime_manager.initialize(g_runtime_state_allocation);
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

//.text:0055F760 ; void __cdecl cache_file_tag_resources_load_pending_resources_blocking(c_io_result*)
//.text:0055F7C0 ; void __cdecl cache_file_tag_resources_load_required_resources_blocking(c_io_result*)
//.text:0055F820 ; real __cdecl cache_file_tag_resources_map_prefetch_progress(short, char const*)
//.text:0055F850 ; bool __cdecl cache_file_tag_resources_map_prefetched(short, char const*)

bool __cdecl cache_file_tag_resources_prefetch_update_required()
{
	return INVOKE(0x0055F870, cache_file_tag_resources_prefetch_update_required);
}

//.text:0055F890 ; void __cdecl cache_file_tag_resources_prepare_for_next_map()

void __cdecl cache_file_tag_resources_update_prefetch_state()
{
	return INVOKE(0x0055F960, cache_file_tag_resources_update_prefetch_state);
}

void __cdecl cache_file_tag_resources_set_zone_state(long scenario_index, long zone_set_name, s_scenario_zone_state const* zone_state)
{
	INVOKE(0x0055F8E0, cache_file_tag_resources_set_zone_state, scenario_index, zone_set_name, zone_state);
}

//.text:0055F900 ; void __cdecl cache_file_tag_resources_start_map_prefetch(short, char const*)
//.text:0055F920 ; void __cdecl cache_file_tag_resources_stop_map_prefetch()
//.text:0055F960 ; void __cdecl cache_file_tag_resources_update_prefetch_state(void)
//.text:0055F9B0 ; 
//.text:0055F9F0 ; 
//.text:0055FA20 ; 
//.text:0055FA70 ; 
//.text:0055FAA0 ; 

bool __cdecl tag_resource_available(s_tag_resource const* resource)
{
	return INVOKE(0x00563DC0, tag_resource_get, resource);
}

void* __cdecl tag_resource_get(s_tag_resource const* resource)
{
	//return INVOKE(0x00563E10, tag_resource_get, resource);

	ASSERT(resource);

	return g_resource_runtime_manager.get()->get_cached_resource_data(resource->resource_handle);
}

long __cdecl tag_resources_lock_game()
{
	return INVOKE(0x00563F80, tag_resources_lock_game);
}

void __cdecl tag_resources_lock_render()
{
	INVOKE(0x00563FC0, tag_resources_lock_render);
}

bool __cdecl tag_resources_locked_for_current_thread_UGLY()
{
	return INVOKE(0x00563FE0, tag_resources_locked_for_current_thread_UGLY);
}

void __cdecl tag_resources_main_loop_idle()
{
	//INVOKE(0x00563FF0, tag_resources_main_loop_idle);

	g_resource_runtime_manager.get()->idle();
}

void __cdecl tag_resources_prepare_for_new_map()
{
	INVOKE(0x00564000, tag_resources_prepare_for_new_map);
}

void __cdecl tag_resources_pump_io()
{
	INVOKE(0x00564010, tag_resources_pump_io);
}

//.text:00564070 ; tag_resources_demand_optional_resources

void __cdecl tag_resources_stagnate_deferred_resources()
{
	INVOKE(0x00564090, tag_resources_stagnate_deferred_resources);
}

void __cdecl tag_resources_unlock_game(long& lock)
{
	INVOKE(0x005640B0, tag_resources_unlock_game, lock);
}

void c_cache_file_tag_resource_runtime_manager::idle()
{
	DECLFUNC(0x005619D0, void, __thiscall, c_cache_file_tag_resource_runtime_manager*)(this);
}

void c_cache_file_tag_resource_runtime_manager::initialize_for_new_map(e_game_mode game_mode, long cache_file_resource_gestalt_index, long resource_vtable_list_count, s_cache_file_tag_resource_vtable const** resource_vtable_list, c_cache_file_runtime_decompressor_registry* runtime_decompressor_registry)
{
	DECLFUNC(0x00561DF0, void, __thiscall, c_cache_file_tag_resource_runtime_manager*, e_game_mode, long, long, s_cache_file_tag_resource_vtable const**, c_cache_file_runtime_decompressor_registry*)(
		this,
		game_mode,
		cache_file_resource_gestalt_index,
		resource_vtable_list_count,
		resource_vtable_list,
		runtime_decompressor_registry);
}

