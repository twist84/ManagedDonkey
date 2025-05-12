#include "interface/gui_custom_bitmap_storage.hpp"

#include "bitmaps/bitmaps.hpp"
#include "cseries/cseries_events.hpp"
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

void __thiscall c_gui_custom_bitmap_storage_item::dispose()
{
	ASSERT(!m_bitmap_ready);
	ASSERT(!m_hardware_format_bitmap.valid());

	if (m_bitmap_pixel_buffer_allocation)
	{
		user_interface_free(m_bitmap_pixel_buffer_allocation);
		m_bitmap_pixel_buffer_allocation = NULL;
	}

	m_allocated = false;
}

void __thiscall c_gui_custom_bitmap_storage_item::initialize(int32 width, int32 height, bool use_compressed_format)
{
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
}

bool __thiscall c_gui_custom_bitmap_storage_item::initialize_raw(int32 width, int32 height, char* buffer, int32 buffer_length, bool cpu_cached)
{
	return false;
}

bool __thiscall c_gui_custom_bitmap_storage_item::load_from_buffer(char const* buffer, int32 buffer_length, void* d3dx_scratch_buffer, int32 d3dx_scratch_buffer_length, int32 aspect_ratio)
{
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

bool __thiscall c_gui_custom_bitmap_storage_item::load_from_file_or_buffer(char const* filename, char const* buffer, int32 buffer_length, void* d3dx_scratch_buffer, int32 d3dx_scratch_buffer_length, int32 aspect_ratio)
{
	return false;
}

void __thiscall c_gui_custom_bitmap_storage_item::unload_non_rendered_bitmap()
{
	return;
}

void __thiscall c_gui_custom_bitmap_storage_item::unload_rendered_bitmap()
{
	if (m_hardware_format_bitmap.valid())
	{
		c_rasterizer_texture_ref::release(m_hardware_format_bitmap);
	}

	m_bitmap_ready = false;
};

c_gui_custom_bitmap_storage_manager* __cdecl c_gui_custom_bitmap_storage_manager::get()
{
	return &g_gui_custom_bitmap_storage_manager;
}

c_gui_custom_bitmap_storage_item const* c_gui_custom_bitmap_storage_manager::get_bitmap(int32 bitmap_storage_index)
{
	c_gui_custom_bitmap_storage_item const* storage_item = NULL;

	c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);

	s_bitmap_storage_handle_datum* bitmap_storage_handle_datum = DATUM_TRY_AND_GET(m_bitmap_storage_items, s_bitmap_storage_handle_datum, bitmap_storage_index);
	if (bitmap_storage_handle_datum && bitmap_storage_handle_datum->reference_count > 0 && bitmap_storage_handle_datum->state == _bitmap_storage_state_ready)
	{
		storage_item = &bitmap_storage_handle_datum->storage_item;
	}

	return storage_item;
}

bool __cdecl c_gui_custom_bitmap_storage_manager::load_bitmap_from_buffer(int32 bitmap_storage_index, char const* buffer, int32 buffer_size, int32 aspect_ratio)
{
	s_bitmap_storage_handle_datum* bitmap_storage_handle_datum = NULL;
	{
		c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
		if (bitmap_storage_handle_datum = DATUM_TRY_AND_GET(m_bitmap_storage_items, s_bitmap_storage_handle_datum, bitmap_storage_index))
			bitmap_storage_handle_datum->state = _bitmap_storage_state_loading;
	}

	if (!bitmap_storage_handle_datum)
	{
		return false;
	}

	event(_event_message, "ui:custom_bitmaps: load_bitmap_from_buffer starting %d",
		bitmap_storage_index);

	bool bitmap_ready = bitmap_storage_handle_datum->storage_item.load_from_buffer(buffer, buffer_size, m_buffer, m_buffer_size, aspect_ratio);

	{
		c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
		ASSERT(bitmap_storage_handle_datum->state == _bitmap_storage_state_loading);
		bitmap_storage_handle_datum->state = bitmap_ready ? _bitmap_storage_state_ready : _bitmap_storage_state_none;
	}

	event(_event_message, "ui:custom_bitmaps: load_bitmap_from_buffer ending %d",
		bitmap_storage_index);

	return bitmap_ready;
}
