#include "interface/gui_custom_bitmap_storage.hpp"

#include "interface/user_interface_memory.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "networking/online/online_error.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_d3d_allocations.hpp"
#include "rasterizer/rasterizer_textures_xenon_formats.hpp"
#include "xbox/xgraphics.hpp"

#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>

REFERENCE_DECLARE(0x05270C14, c_gui_custom_bitmap_storage_manager, g_gui_custom_bitmap_storage_manager);

//#define ISEXPERIMENTAL

#if defined(ISEXPERIMENTAL)
HOOK_DECLARE_CLASS_MEMBER(0x00B20460, c_gui_custom_bitmap_storage_item, dispose);
HOOK_DECLARE_CLASS_MEMBER(0x00B20470, c_gui_custom_bitmap_storage_item, initialize);
HOOK_DECLARE_CLASS_MEMBER(0x00B20480, c_gui_custom_bitmap_storage_item, initialize_raw);
HOOK_DECLARE_CLASS_MEMBER(0x00B20490, c_gui_custom_bitmap_storage_item, load_from_buffer);
HOOK_DECLARE_CLASS_MEMBER(0x00B204B0, c_gui_custom_bitmap_storage_item, load_from_file_or_buffer);
HOOK_DECLARE_CLASS_MEMBER(0x00B204D0, c_gui_custom_bitmap_storage_item, unload_non_rendered_bitmap);
HOOK_DECLARE_CLASS_MEMBER(0x00B204E0, c_gui_custom_bitmap_storage_item, unload_rendered_bitmap);
#endif // ISEXPERIMENTAL

void __thiscall c_gui_custom_bitmap_storage_item::dispose()
{
	ASSERT(!m_bitmap_ready);
	ASSERT(!m_hardware_format_bitmap.valid());

	if (m_bitmap_pixel_buffer)
	{
		user_interface_free(m_bitmap_pixel_buffer);
		m_bitmap_pixel_buffer = nullptr;
	}

	m_allocated = false;
}

const uns32 bitmap_pixel_buffer_alignment_bits = 12;
void __thiscall c_gui_custom_bitmap_storage_item::initialize(int32 width, int32 height, bool use_compressed_format)
{
	m_use_compressed_format = use_compressed_format;
	e_bitmap_format bitmap_format = m_use_compressed_format ? _bitmap_format_dxt5 : _bitmap_format_a8r8g8b8;
	D3DFORMAT d3d_format = m_use_compressed_format ? D3DFMT_DXT5 : D3DFMT_A8R8G8B8;

	bitmap_2d_initialize(&m_bitmap, static_cast<int16>(width), static_cast<int16>(height), 0, bitmap_format, FLAG(3) | FLAG(6), false, false);
	m_bitmap.curve = 3;

	c_rasterizer_texture_ref::allocate(m_hardware_format_bitmap, width, height, 1, d3d_format, D3DPOOL_DEFAULT, false, 0, 0);
	texture_header = new D3DBaseTexture();

	uns32 base_size = 0;
	uns32 mip_size = 0;
	uns32 resource_bytes = XGSetTextureHeader(m_bitmap.width, m_bitmap.height, 1, 4, d3d_format, 0, 0, -1, 0, texture_header, &base_size, &mip_size);
	if (resource_bytes)
	{
		int32 allocate_bytes = resource_bytes + FLAG(bitmap_pixel_buffer_alignment_bits);
		m_bitmap_pixel_buffer = (char*)user_interface_malloc_tracked(allocate_bytes, __FILE__, __LINE__);
		if (m_bitmap_pixel_buffer)
		{
			m_width = width;
			m_height = height;
			m_bitmap_pixel_buffer_base = (char*)ALIGN((int32)m_bitmap_pixel_buffer, bitmap_pixel_buffer_alignment_bits);
			m_bitmap_pixel_buffer_length = allocate_bytes - (m_bitmap_pixel_buffer_base - m_bitmap_pixel_buffer);
			ASSERT(m_bitmap_pixel_buffer_length <= allocate_bytes);

			XGOffsetResourceAddress(texture_header, m_bitmap_pixel_buffer_base);
			m_allocated = true;
		}
	}
}

bool __thiscall c_gui_custom_bitmap_storage_item::initialize_raw(int32 width, int32 height, char* buffer, int32 buffer_length, bool cpu_cached)
{
	return false;
}

bool __thiscall c_gui_custom_bitmap_storage_item::load_from_buffer(char const* buffer, int32 buffer_length, void* d3dx_scratch_buffer, int32 d3dx_scratch_buffer_length, int32 aspect_ratio)
{
	ASSERT(buffer);
	ASSERT(!m_bitmap_ready);

	bool result = false;

	if (!m_allocated)
		return false;

	if (!m_hardware_format_bitmap.valid())
		return false;

	IDirect3DTexture9* d3d_texture = m_hardware_format_bitmap.get_d3d_texture();
	if (d3d_texture == NULL)
	{
		event(_event_error, "ui:custom_bitmaps: d3d_texture is null");
		return false;
	}

	IDirect3DSurface9* d3d_surface = NULL;
	HRESULT get_surface_level_result = d3d_texture->GetSurfaceLevel(0, &d3d_surface);
	if (FAILED(get_surface_level_result))
		return false;

	HRESULT load_surface_result = D3DXLoadSurfaceFromFileInMemory(d3d_surface, NULL, NULL, buffer, buffer_length, NULL, D3DX_DEFAULT, 0, NULL);
	d3d_surface->Release();

	if (FAILED(load_surface_result))
	{
		event(_event_error, "ui:custom_bitmaps: D3DXLoadSurfaceFromFile failed with error code 0x%08X", load_surface_result);
		return false;
	}

	uns32 bytes = XGSetTextureHeader(m_bitmap.width, m_bitmap.height, 1, 4, m_use_compressed_format ? D3DFMT_DXT5 : D3DFMT_A8R8G8B8, 1, 0, -1, 0, texture_header, 0, 0);
	ASSERT(bytes > 0);

	XGOffsetResourceAddress(texture_header, m_bitmap_pixel_buffer_base);

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
		c_rasterizer_texture_ref::release(m_hardware_format_bitmap);

	m_bitmap_ready = false;
};

c_gui_custom_bitmap_storage_manager* __cdecl c_gui_custom_bitmap_storage_manager::get()
{
	return &g_gui_custom_bitmap_storage_manager;
}

bool __cdecl c_gui_custom_bitmap_storage_manager::load_bitmap_from_buffer(int32 storage_item_index, char const* buffer, int32 buffer_size, int32 aspect_ratio)
{
	s_bitmap_storage_handle_datum* storage_item = NULL;
	{
		c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
		if (storage_item = DATUM_TRY_AND_GET(m_bitmap_storage_items, s_bitmap_storage_handle_datum, storage_item_index))
			storage_item->__unknown8 = 1;
	}

	if (!storage_item)
		return false;

	bool result = storage_item->storage_item.load_from_buffer(buffer, buffer_size, m_buffer, m_buffer_size, aspect_ratio);

	{
		c_critical_section_scope section_scope(k_crit_section_ui_custom_bitmaps_lock);
		storage_item->__unknown8 = result ? 2 : 0;
	}

	return result;
}
