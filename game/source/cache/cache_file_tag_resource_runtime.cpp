#include "cache/cache_file_tag_resource_runtime.hpp"

#include "bitmaps/dds_file.hpp"
#include "cseries/cseries_console.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x0243F780, c_asynchronous_io_arena, g_cache_file_io_arena);

using resource_runtime_manager_typed_allocation_data_no_destruct_t = c_typed_allocation_data_no_destruct<c_cache_file_tag_resource_runtime_manager, 1>;
REFERENCE_DECLARE(0x023916C0, resource_runtime_manager_typed_allocation_data_no_destruct_t, g_resource_runtime_manager);

HOOK_DECLARE(0x00563E10, tag_resource_get);

#ifdef ISEXPERIMENTAL

struct c_custom_cache_file_decompressor :
	public c_cache_file_decompressor
{
	c_custom_cache_file_decompressor() :
		m_holding_buffer()
	{
	}

	~c_custom_cache_file_decompressor()
	{
	}

	virtual bool begin(c_basic_buffer<void> a1) override
	{
		m_holding_buffer = a1;

		return true;
	}

	virtual bool decompress_buffer(c_basic_buffer<void> in_buffer, c_basic_buffer<void>* out_buffer) override
	{
		for (long i = 0; i < NUMBEROF(k_resource_replacements); i++)
		{
			s_replacement_resource_info const& resource = k_resource_replacements[i];

			// check buffer size is the same as compressed file size we set as the tag index
			if (static_cast<dword>(resource.tag_index) != in_buffer.m_size)
				continue;

			if (resource.group_tag == 'bitm')
			{
				c_dds_file _dds_file(resource.filename);
				s_dds_file* dds_file = _dds_file.get();
				if (!dds_file)
					continue;

				ASSERT(m_holding_buffer.m_size == dds_file->header.linear_size);
				csmemcpy(m_holding_buffer.m_buffer, dds_file->data, m_holding_buffer.m_size);
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

struct c_custom_cache_file_decompressor_service :
	public c_single_instance_cache_file_decompressor_service<c_custom_cache_file_decompressor>
{
	virtual void initialize_decompressor(c_typed_opaque_data<c_custom_cache_file_decompressor, sizeof(c_custom_cache_file_decompressor), 3>* decompressor_storage)
	{
		c_custom_cache_file_decompressor* decompressor = decompressor_storage->get();
		if (decompressor)
		{
			csmemcpy(decompressor, new c_custom_cache_file_decompressor(), sizeof(c_custom_cache_file_decompressor));
			*reinterpret_cast<c_cache_file_decompressor**>(decompressor_storage + 1) = decompressor;
		}
		else
		{
			*reinterpret_cast<c_cache_file_decompressor**>(decompressor_storage + 1) = 0;
		}
	}
};
static_assert(sizeof(c_custom_cache_file_decompressor_service) == 0x18);

static c_custom_cache_file_decompressor_service g_custom_cache_file_decompressor_service = {};

void __fastcall cache_file_tag_resource_codec_service_initialize(c_cache_file_tag_resource_codec_service* _this, void* unused, c_allocation_base* allocator, c_cache_file_runtime_decompressor_registry* decompressor_registry, c_cache_file_resource_uber_location_table* uber_location_table)
{
	DECLFUNC(0x00561AB0, void, __thiscall, c_cache_file_tag_resource_codec_service*, c_allocation_base*, c_cache_file_runtime_decompressor_registry*, c_cache_file_resource_uber_location_table*)(_this, allocator, decompressor_registry, uber_location_table);

	if (NUMBEROF(k_resource_replacements))
	{
		_this->m_actual_runtime_decompressors[_this->m_actual_runtime_decompressors.new_element_index()] = &g_custom_cache_file_decompressor_service;
		ASSERT(_this->m_actual_runtime_decompressors[1] == &g_custom_cache_file_decompressor_service);
	}
}
HOOK_DECLARE_CALL(0x00561FA0, cache_file_tag_resource_codec_service_initialize);

#endif // ISEXPERIMENTAL

void* __cdecl tag_resource_get(s_tag_resource const* resource)
{
	ASSERT(resource);

	return g_resource_runtime_manager.get()->get_cached_resource_data(resource->resource_handle);
}


