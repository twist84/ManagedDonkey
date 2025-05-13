#include "interface/gui_custom_bitmap_storage.hpp"

#include "bitmaps/bitmaps.hpp"
#include "cseries/cseries_events.hpp"
#include "cseries/runtime_state.hpp"
#include "interface/user_interface_memory.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>

REFERENCE_DECLARE(0x05270C14, c_gui_custom_bitmap_storage_manager, g_gui_custom_bitmap_storage_manager);

HOOK_DECLARE_CLASS_MEMBER(0x00B20460, c_gui_custom_bitmap_storage_item, dispose);
HOOK_DECLARE_CLASS_MEMBER(0x00B20470, c_gui_custom_bitmap_storage_item, initialize);
HOOK_DECLARE_CLASS_MEMBER(0x00B20480, c_gui_custom_bitmap_storage_item, initialize_raw);
HOOK_DECLARE_CLASS_MEMBER(0x00B20490, c_gui_custom_bitmap_storage_item, load_from_buffer);
HOOK_DECLARE_CLASS_MEMBER(0x00B204B0, c_gui_custom_bitmap_storage_item, load_from_file_or_buffer);
HOOK_DECLARE_CLASS_MEMBER(0x00B204D0, c_gui_custom_bitmap_storage_item, unload_non_rendered_bitmap);
HOOK_DECLARE_CLASS_MEMBER(0x00B204E0, c_gui_custom_bitmap_storage_item, unload_rendered_bitmap);

bool g_gui_custom_bitmaps_enabled = false;

//.text:00B20430 ; public: c_gui_custom_bitmap_storage_item::c_gui_custom_bitmap_storage_item()

void __thiscall c_gui_custom_bitmap_storage_item::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00B20460, c_gui_custom_bitmap_storage_item, dispose);

	ASSERT(!m_bitmap_ready);
	ASSERT(!m_hardware_format_bitmap.valid());

	if (m_bitmap_pixel_buffer_allocation)
	{
		user_interface_free(m_bitmap_pixel_buffer_allocation);
		m_bitmap_pixel_buffer_allocation = NULL;
	}

	m_allocated = false;
}

bool __thiscall c_gui_custom_bitmap_storage_item::initialize(int32 width, int32 height, bool use_compressed_format)
{
	//INVOKE_CLASS_MEMBER(0x00B20470, c_gui_custom_bitmap_storage_item, initialize, width, height, use_compressed_format);

	if (!g_gui_custom_bitmaps_enabled)
	{
		return false;
	}

	m_use_compressed_format = use_compressed_format;
	e_bitmap_format bitmap_format = m_use_compressed_format ? _bitmap_format_dxt5 : _bitmap_format_a8r8g8b8;
	D3DFORMAT hardware_format = m_use_compressed_format ? D3DFMT_DXT5 : D3DFMT_A8R8G8B8;
	constexpr uns16 flags = FLAG(_bitmap_free_on_delete_bit) | FLAG(_bitmap_hardware_only_bit);

	bitmap_2d_initialize(&m_bitmap_data, (int16)width, (int16)height, 0, bitmap_format, flags, false, true);
	m_bitmap_data.curve = _bitmap_curve_linear;

	c_rasterizer_texture_ref::release(m_hardware_format_bitmap);
	c_rasterizer_texture_ref::allocate(m_hardware_format_bitmap, width, height, 1, hardware_format, D3DPOOL_DEFAULT, true, _bitmap_type_2d, 0);

	m_width = m_bitmap_data.width;
	m_height = m_bitmap_data.height;
	m_allocated = true;
	m_use_shader_custom_gamma_correct = false; // added by us

	return m_allocated;
}

bool __thiscall c_gui_custom_bitmap_storage_item::initialize_raw(int32 width, int32 height, char* buffer, int32 buffer_length, bool cpu_cached)
{
	//return INVOKE_CLASS_MEMBER(0x00B20480, c_gui_custom_bitmap_storage_item, initialize_raw, width, height, buffer, buffer_length, cpu_cached);

	return false;
}

bool __thiscall c_gui_custom_bitmap_storage_item::load_from_buffer(char const* buffer, int32 buffer_length, void* d3dx_scratch_buffer, int32 d3dx_scratch_buffer_length, e_custom_bitmap_desired_aspect_ratio aspect_ratio)
{
	//return INVOKE_CLASS_MEMBER(0x00B20490, c_gui_custom_bitmap_storage_item, load_from_buffer, buffer, buffer_length, d3dx_scratch_buffer, d3dx_scratch_buffer_length, aspect_ratio);

	ASSERT(buffer);
	ASSERT(!m_bitmap_ready);

	if (!m_allocated)
	{
		return false;
	}

	if (!m_hardware_format_bitmap.valid())
	{
		return false;
	}

	IDirect3DTexture9* d3d_texture = m_hardware_format_bitmap.get_d3d_texture();
	if (d3d_texture == NULL)
	{
		event(_event_error, "ui:custom_bitmaps: d3d_texture is null");
		return false;
	}

	IDirect3DSurface9* d3d_surface = NULL;
	HRESULT get_surface_level_result = d3d_texture->GetSurfaceLevel(0, &d3d_surface);
	if (FAILED(get_surface_level_result))
	{
		d3d_surface->Release();
		return false;
	}

	D3DXIMAGE_INFO d3dximage_info{};
	HRESULT load_surface_result = D3DXLoadSurfaceFromFileInMemory(d3d_surface, NULL, NULL, buffer, buffer_length, NULL, D3DX_DEFAULT, 0, &d3dximage_info);

	d3d_surface->Release();

	if (FAILED(load_surface_result))
	{
		event(_event_error, "ui:custom_bitmaps: D3DXLoadSurfaceFromFileInMemory failed with error code 0x%08X", load_surface_result);
		return false;
	}

	m_bitmap_ready = true;
	return true;
}

bool __thiscall c_gui_custom_bitmap_storage_item::load_from_file_or_buffer(char const* filename, char const* buffer, int32 buffer_length, void* d3dx_scratch_buffer, int32 d3dx_scratch_buffer_length, e_custom_bitmap_desired_aspect_ratio aspect_ratio)
{
	//return INVOKE_CLASS_MEMBER(0x00B204B0, c_gui_custom_bitmap_storage_item, load_from_file_or_buffer, filename, buffer, buffer_length, d3dx_scratch_buffer, d3dx_scratch_buffer_length, aspect_ratio);

	return false;
}

void __thiscall c_gui_custom_bitmap_storage_item::unload_non_rendered_bitmap()
{
	//INVOKE_CLASS_MEMBER(0x00B204D0, c_gui_custom_bitmap_storage_item, unload_non_rendered_bitmap);
}

void __thiscall c_gui_custom_bitmap_storage_item::unload_rendered_bitmap()
{
	//INVOKE_CLASS_MEMBER(0x00B204E0, c_gui_custom_bitmap_storage_item, unload_rendered_bitmap);

	if (m_hardware_format_bitmap.valid())
	{
		c_rasterizer_texture_ref::release(m_hardware_format_bitmap);
	}

	m_bitmap_ready = false;
};

c_gui_custom_bitmap_storage_manager::c_gui_custom_bitmap_storage_manager() :
	m_bitmap_storage_items(0),
	m_d3dx_scratch_buffer(NULL),
	m_d3dx_scratch_buffer_length(0)
{
	//DECLFUNC(0x00AE5010, void, __thiscall, c_gui_custom_bitmap_storage_manager*)(this);
}

void c_gui_custom_bitmap_storage_manager::acquire_bitmap(int32 bitmap_storage_index)
{
	INVOKE_CLASS_MEMBER(0x00AE5030, c_gui_custom_bitmap_storage_manager, acquire_bitmap, bitmap_storage_index);

	//s_bitmap_storage_handle_datum* bitmap_storage_handle_datum = NULL;
	//{
	//	c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
	//	s_bitmap_storage_handle_datum* bitmap_storage_handle_datum = DATUM_TRY_AND_GET(m_bitmap_storage_items, s_bitmap_storage_handle_datum, bitmap_storage_index);
	//	if (bitmap_storage_handle_datum)
	//	{
	//		ASSERT(bitmap_storage_handle_datum->reference_count > 0);
	//		bitmap_storage_handle_datum->reference_count++;
	//	}
	//}
	//
	//if (bitmap_storage_handle_datum)
	//{
	//	event(_event_message, "ui:custom_bitmaps: acquire_bitmap %d",
	//		bitmap_storage_index);
	//}
}

int32 c_gui_custom_bitmap_storage_manager::allocate_bitmap(int32 width, int32 height, bool use_compressed_format)
{
	return INVOKE_CLASS_MEMBER(0x00AE50A0, c_gui_custom_bitmap_storage_manager, allocate_bitmap, width, height, use_compressed_format);

	//int32 bitmap_storage_index = NONE;
	//{
	//	c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
	//	bitmap_storage_index = datum_new(m_bitmap_storage_items);
	//	if (bitmap_storage_index != NONE)
	//	{
	//		s_bitmap_storage_handle_datum* bitmap_storage_handle_datum = DATUM_GET(m_bitmap_storage_items, s_bitmap_storage_handle_datum, bitmap_storage_index);
	//		bitmap_storage_handle_datum->reference_count = 1;
	//		if (!bitmap_storage_handle_datum->storage_item.initialize(width, height, use_compressed_format))
	//		{
	//			release_bitmap(bitmap_storage_index);
	//			bitmap_storage_index = NONE;
	//		}
	//	}
	//}
	//
	//if (bitmap_storage_index == NONE)
	//{
	//	event(_event_warning, "ui:custom_bitmaps: failed to allocate_bitmap");
	//}
	//else
	//{
	//	event(_event_message, "ui:custom_bitmaps: allocate_bitmap index %d",
	//		bitmap_storage_index);
	//}
	//
	//return bitmap_storage_index;
}

//.text:00AE5180 ; public: void c_data_iterator<c_gui_custom_bitmap_storage_manager::s_bitmap_storage_handle_datum>::begin(s_data_array*)

void c_gui_custom_bitmap_storage_manager::dispose()
{
	INVOKE_CLASS_MEMBER(0x00AE51A0, c_gui_custom_bitmap_storage_manager, dispose);

	//c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
	//if (m_bitmap_storage_items)
	//{
	//	ASSERT(m_bitmap_storage_items->actual_count == 0);
	//	data_dispose(m_bitmap_storage_items);
	//	m_bitmap_storage_items = NULL;
	//}
}

void c_gui_custom_bitmap_storage_manager::dispose_from_old_map()
{
	INVOKE_CLASS_MEMBER(0x00AE5210, c_gui_custom_bitmap_storage_manager, dispose_from_old_map);
}

c_gui_custom_bitmap_storage_manager* __cdecl c_gui_custom_bitmap_storage_manager::get()
{
	//return INVOKE(0x00AE52E0, c_gui_custom_bitmap_storage_manager::get);

	return &g_gui_custom_bitmap_storage_manager;
}

c_gui_custom_bitmap_storage_item const* c_gui_custom_bitmap_storage_manager::get_bitmap(int32 bitmap_storage_index)
{
	//return INVOKE_CLASS_MEMBER(0x00AE52F0, c_gui_custom_bitmap_storage_manager, get_bitmap, bitmap_storage_index);

	c_gui_custom_bitmap_storage_item const* storage_item = NULL;
	{
		c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
		s_bitmap_storage_handle_datum* bitmap_storage_handle_datum = DATUM_TRY_AND_GET(m_bitmap_storage_items, s_bitmap_storage_handle_datum, bitmap_storage_index);
		if (bitmap_storage_handle_datum && bitmap_storage_handle_datum->reference_count > 0 && bitmap_storage_handle_datum->state == _bitmap_storage_state_ready)
		{
			storage_item = &bitmap_storage_handle_datum->storage_item;
		}
	}
	return storage_item;
}

//.text:00AE5370 ; public: c_gui_custom_bitmap_storage_manager::s_bitmap_storage_handle_datum* c_data_iterator<c_gui_custom_bitmap_storage_manager::s_bitmap_storage_handle_datum>::get_datum() const
//.text:00AE5380 ; public: int32 c_data_iterator<c_gui_custom_bitmap_storage_manager::s_bitmap_storage_handle_datum>::get_index() const

void c_gui_custom_bitmap_storage_manager::initialize()
{
	INVOKE_CLASS_MEMBER(0x00AE5390, c_gui_custom_bitmap_storage_manager, initialize);

	//c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
	//ASSERT(m_bitmap_storage_items == NULL);
	//m_bitmap_storage_items = data_new("gui bitmap storage", k_maximum_number_of_bitmap_items, sizeof(s_bitmap_storage_handle_datum), 0, g_runtime_state_allocation);
	//ASSERT(m_bitmap_storage_items);
	//data_make_valid(m_bitmap_storage_items);
}

void c_gui_custom_bitmap_storage_manager::initialize_for_new_map()
{
	INVOKE_CLASS_MEMBER(0x00AE5410, c_gui_custom_bitmap_storage_manager, initialize_for_new_map);

	//if (game_is_ui_shell())
	//{
	//	ASSERT(m_d3dx_scratch_buffer == NULL);
	//	m_d3dx_scratch_buffer_length = 0xA00000;
	//	m_d3dx_scratch_buffer = user_interface_malloc(0xA00000);
	//}
}

bool c_gui_custom_bitmap_storage_manager::load_bitmap_from_buffer(int32 bitmap_storage_index, char const* buffer, int32 buffer_length, e_custom_bitmap_desired_aspect_ratio aspect_ratio)
{
	//return INVOKE_CLASS_MEMBER(0x00AE5440, c_gui_custom_bitmap_storage_manager, load_bitmap_from_buffer, bitmap_storage_index, buffer, buffer_length, aspect_ratio);

	s_bitmap_storage_handle_datum* bitmap_storage_handle_datum = NULL;
	{
		c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
		if (bitmap_storage_handle_datum = DATUM_TRY_AND_GET(m_bitmap_storage_items, s_bitmap_storage_handle_datum, bitmap_storage_index))
		{
			bitmap_storage_handle_datum->state = _bitmap_storage_state_loading;
		}
	}

	if (!bitmap_storage_handle_datum)
	{
		return false;
	}

	event(_event_message, "ui:custom_bitmaps: load_bitmap_from_buffer starting %d",
		bitmap_storage_index);

	bool bitmap_ready = bitmap_storage_handle_datum->storage_item.load_from_buffer(buffer, buffer_length, m_d3dx_scratch_buffer, m_d3dx_scratch_buffer_length, aspect_ratio);

	{
		c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
		ASSERT(bitmap_storage_handle_datum->state == _bitmap_storage_state_loading);
		bitmap_storage_handle_datum->state = bitmap_ready ? _bitmap_storage_state_ready : _bitmap_storage_state_none;
	}

	event(_event_message, "ui:custom_bitmaps: load_bitmap_from_buffer ending %d",
		bitmap_storage_index);

	return bitmap_ready;
}

//.text:00AE5500 ; public: bool c_data_iterator<c_gui_custom_bitmap_storage_manager::s_bitmap_storage_handle_datum>::next()

void c_gui_custom_bitmap_storage_manager::release_bitmap(int32 bitmap_storage_index)
{
	INVOKE_CLASS_MEMBER(0x00AE5520, c_gui_custom_bitmap_storage_manager, release_bitmap, bitmap_storage_index);

	//c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
	//if (s_bitmap_storage_handle_datum* bitmap_storage_handle_datum = DATUM_TRY_AND_GET(m_bitmap_storage_items, s_bitmap_storage_handle_datum, bitmap_storage_index))
	//{
	//	ASSERT(bitmap_storage_handle_datum->reference_count > 0);
	//	bitmap_storage_handle_datum->reference_count--;
	//}
}

void c_gui_custom_bitmap_storage_manager::release_unused_bitmap(int32 bitmap_storage_index)
{
	INVOKE_CLASS_MEMBER(0x00AE5590, c_gui_custom_bitmap_storage_manager, release_unused_bitmap, bitmap_storage_index);

	//s_bitmap_storage_handle_datum* bitmap_storage_handle_datum = NULL;
	//{
	//	c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
	//	if (bitmap_storage_handle_datum = DATUM_TRY_AND_GET(m_bitmap_storage_items, s_bitmap_storage_handle_datum, bitmap_storage_index))
	//	{
	//		ASSERT(bitmap_storage_handle_datum->reference_count == 1);
	//		ASSERT(bitmap_storage_handle_datum->state == _bitmap_storage_state_none);
	//		bitmap_storage_handle_datum->storage_item.unload_non_rendered_bitmap();
	//	}
	//}
	//
	//if (bitmap_storage_handle_datum)
	//{
	//	event(_event_message, "ui:custom_bitmaps: release_unused_bitmap %d",
	//		bitmap_storage_index);
	//}
}

void c_gui_custom_bitmap_storage_manager::update_render()
{
	INVOKE_CLASS_MEMBER(0x00AE5610, c_gui_custom_bitmap_storage_manager, update_render);
}

