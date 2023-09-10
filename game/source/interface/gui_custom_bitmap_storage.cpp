#include "interface/gui_custom_bitmap_storage.hpp"

#include "interface/user_interface_memory.hpp"
#include "memory/module.hpp"
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

#if defined(ISEXPERIMENTAL)
HOOK_DECLARE_CLASS_MEMBER(0x00B20460, c_gui_custom_bitmap_storage_item, dispose);
HOOK_DECLARE_CLASS_MEMBER(0x00B20470, c_gui_custom_bitmap_storage_item, initialize);
HOOK_DECLARE_CLASS_MEMBER(0x00B20480, c_gui_custom_bitmap_storage_item, sub_B20480);
HOOK_DECLARE_CLASS_MEMBER(0x00B20490, c_gui_custom_bitmap_storage_item, load_from_buffer);
HOOK_DECLARE_CLASS_MEMBER(0x00B204B0, c_gui_custom_bitmap_storage_item, sub_B204B0);
HOOK_DECLARE_CLASS_MEMBER(0x00B204D0, c_gui_custom_bitmap_storage_item, sub_B204D0);
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

const dword bitmap_pixel_buffer_alignment_bits = 12;
void __thiscall c_gui_custom_bitmap_storage_item::initialize(long width, long height, bool use_compressed_format)
{
	m_use_compressed_format = use_compressed_format;
	bitmap_2d_initialize(&m_bitmap, static_cast<short>(width), static_cast<short>(height), 0, m_use_compressed_format ? _bitmap_format_dxt5 : _bitmap_format_a8r8g8b8, FLAG(3) | FLAG(6), false, false);
	m_bitmap.curve = 3;

	c_rasterizer_texture_ref::allocate(m_hardware_format_bitmap, width, height, 1, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, false, 0, 0);
	texture_header = new D3DBaseTexture();

	dword base_size = 0;
	dword mip_size = 0;
	dword resource_bytes = XGSetTextureHeader(m_bitmap.width, m_bitmap.height, 1, 4, m_use_compressed_format ? D3DFMT_DXT5 : D3DFMT_A8R8G8B8, 0, 0, -1, 0, texture_header, &base_size, &mip_size);
	if (resource_bytes)
	{
		long allocate_bytes = resource_bytes + FLAG(bitmap_pixel_buffer_alignment_bits);
		m_bitmap_pixel_buffer = (char*)user_interface_malloc_tracked(allocate_bytes, __FILE__, __LINE__);
		if (m_bitmap_pixel_buffer)
		{
			m_width = width;
			m_height = height;
			m_bitmap_pixel_buffer_base = (char*)ALIGN((long)m_bitmap_pixel_buffer, bitmap_pixel_buffer_alignment_bits);
			m_bitmap_pixel_buffer_length = allocate_bytes - (m_bitmap_pixel_buffer_base - m_bitmap_pixel_buffer);
			ASSERT(m_bitmap_pixel_buffer_length <= allocate_bytes);

			XGOffsetResourceAddress(texture_header, m_bitmap_pixel_buffer_base);
			m_allocated = true;
		}
	}
}

bool __thiscall c_gui_custom_bitmap_storage_item::sub_B20480(long a1, long a2, long a3, long a4, long a5)
{
	return false;
}

// buffer:       blf, async_file_buffer
// buffer_size:  blf, async_file_buffer_size
// buffer2:      c_gui_custom_bitmap_storage_manager::m_buffer
// buffer2_size: c_gui_custom_bitmap_storage_manager::m_buffer_size
bool __thiscall c_gui_custom_bitmap_storage_item::load_from_buffer(char const* buffer, long buffer_size, void* buffer2, long buffer2_size, long a6)
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
		c_console::write_line("ui:custom_bitmaps: d3d_texture is null");
		return false;
	}

	IDirect3DSurface9* d3d_surface = NULL;
	HRESULT get_surface_level_result = d3d_texture->GetSurfaceLevel(0, &d3d_surface);
	if (FAILED(get_surface_level_result))
		return false;

	HRESULT load_surface_result = D3DXLoadSurfaceFromFileInMemory(d3d_surface, NULL, NULL, buffer, buffer_size, NULL, D3DX_DEFAULT, 0, NULL);
	d3d_surface->Release();

	if (FAILED(load_surface_result))
	{
		c_console::write_line("ui:custom_bitmaps: D3DXLoadSurfaceFromFile failed with error code 0x%08X", load_surface_result);
		return false;
	}

	dword bytes = XGSetTextureHeader(m_bitmap.width, m_bitmap.height, 1, 4, m_use_compressed_format ? D3DFMT_DXT5 : D3DFMT_A8R8G8B8, 1, 0, -1, 0, texture_header, 0, 0);
	ASSERT(bytes > 0);

	XGOffsetResourceAddress(texture_header, m_bitmap_pixel_buffer_base);

	m_bitmap_ready = true;
	return true;
}

bool __thiscall c_gui_custom_bitmap_storage_item::sub_B204B0(long a1, long a2, long a3, long a4, long a5, long a6)
{
	return false;
}

void __thiscall c_gui_custom_bitmap_storage_item::sub_B204D0()
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

bool __cdecl c_gui_custom_bitmap_storage_manager::load_bitmap_from_buffer(long storage_item_index, char const* buffer, long buffer_size, long a5)
{
	// #TODO: investigate the error below, the calling convention is __thiscall in IDA
	// - Run-Time Check Failure #0 - The value of ESP was not properly saved across a function call.
	// - This is usually a result of calling a function declared with one calling convention with a function pointer declared with a different calling convention.
	// 
	//return DECLFUNC(0x00AE5440, bool, __thiscall, c_gui_custom_bitmap_storage_manager*, long, char const*, long, long)(this, storage_item_index, buffer, buffer_size, a5);

	return false;
}
