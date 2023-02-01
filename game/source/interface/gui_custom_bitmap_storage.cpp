#include "interface/gui_custom_bitmap_storage.hpp"

#include "bitmaps/bitmaps.hpp"
#include "cseries/console.hpp"
#include "interface/user_interface_memory.hpp"
#include "memory/module.hpp"
#include "networking/online/online_error.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_d3d_allocations.hpp"
#include "rasterizer/rasterizer_textures_xenon_formats.hpp"
#include "xbox/xgraphics.hpp"

#include <assert.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>

HOOK_DECLARE_CLASS(0x00B20460, c_gui_custom_bitmap_storage_item, dispose);
HOOK_DECLARE_CLASS(0x00B20470, c_gui_custom_bitmap_storage_item, initialize);
HOOK_DECLARE_CLASS(0x00B20480, c_gui_custom_bitmap_storage_item, sub_B20480);
HOOK_DECLARE_CLASS(0x00B20490, c_gui_custom_bitmap_storage_item, load_from_buffer);
HOOK_DECLARE_CLASS(0x00B204B0, c_gui_custom_bitmap_storage_item, sub_B204B0);
HOOK_DECLARE_CLASS(0x00B204D0, c_gui_custom_bitmap_storage_item, sub_B204D0);
HOOK_DECLARE_CLASS(0x00B204E0, c_gui_custom_bitmap_storage_item, unload_rendered_bitmap);

REFERENCE_DECLARE(0x05270C14, c_gui_custom_bitmap_storage_manager, g_gui_custom_bitmap_storage_manager);

void __fastcall c_gui_custom_bitmap_storage_item::dispose(c_gui_custom_bitmap_storage_item* _this, void* unused)
{
	assert(!_this->m_bitmap_ready);
	assert(!_this->m_hardware_format_bitmap.valid());

	if (_this->m_bitmap_pixel_buffer)
	{
		user_interface_free(_this->m_bitmap_pixel_buffer);
		_this->m_bitmap_pixel_buffer = nullptr;
	}

	_this->m_allocated = false;
}

const dword bitmap_pixel_buffer_alignment_bits = 12;
void __fastcall c_gui_custom_bitmap_storage_item::initialize(c_gui_custom_bitmap_storage_item* _this, void* unused, long width, long height, bool use_compressed_format)
{
	_this->m_use_compressed_format = use_compressed_format;
	bitmap_2d_initialize(&_this->m_bitmap, static_cast<short>(width), static_cast<short>(height), 0, _this->m_use_compressed_format ? _bitmap_format_dxt5 : _bitmap_format_a8r8g8b8, FLAG(3) | FLAG(6), false, false);
	_this->m_bitmap.curve = 3;

	c_rasterizer_texture_ref::allocate(_this->m_hardware_format_bitmap, width, height, 1, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, false, 0, 0);
	_this->texture_header = new D3DBaseTexture();

	dword base_size = 0;
	dword mip_size = 0;
	dword resource_bytes = XGSetTextureHeader(_this->m_bitmap.width, _this->m_bitmap.height, 1, 4, _this->m_use_compressed_format ? D3DFMT_DXT5 : D3DFMT_A8R8G8B8, 0, 0, -1, 0, _this->texture_header, &base_size, &mip_size);
	if (resource_bytes)
	{
		long allocate_bytes = resource_bytes + FLAG(bitmap_pixel_buffer_alignment_bits);
		_this->m_bitmap_pixel_buffer = (char*)user_interface_malloc_tracked(allocate_bytes, __FILE__, __LINE__);
		if (_this->m_bitmap_pixel_buffer)
		{
			_this->m_width = width;
			_this->m_height = height;
			_this->m_bitmap_pixel_buffer_base = (char*)ALIGN((long)_this->m_bitmap_pixel_buffer, bitmap_pixel_buffer_alignment_bits);
			_this->m_bitmap_pixel_buffer_length = allocate_bytes - (_this->m_bitmap_pixel_buffer_base - _this->m_bitmap_pixel_buffer);
			assert(_this->m_bitmap_pixel_buffer_length <= allocate_bytes);

			XGOffsetResourceAddress(_this->texture_header, _this->m_bitmap_pixel_buffer_base);
			_this->m_allocated = true;
		}
	}
}

bool __fastcall c_gui_custom_bitmap_storage_item::sub_B20480(c_gui_custom_bitmap_storage_item* _this, void* unused, long a1, long a2, long a3, long a4, long a5)
{
	return false;
}

// buffer:       blf, async_file_buffer
// buffer_size:  blf, async_file_buffer_size
// buffer2:      c_gui_custom_bitmap_storage_manager::m_buffer
// buffer2_size: c_gui_custom_bitmap_storage_manager::m_buffer_size
bool __fastcall c_gui_custom_bitmap_storage_item::load_from_buffer(c_gui_custom_bitmap_storage_item* _this, long storage_item_index, char const* buffer, long buffer_size, void* buffer2, long buffer2_size, long a6)
{
	assert(buffer);
	assert(!_this->m_bitmap_ready);

	bool result = false;

	if (_this->m_allocated)
	{
		if (_this->m_hardware_format_bitmap.valid())
		{
			_this->m_bitmap.flags |= FLAG(6);

			IDirect3DTexture9* d3d_texture = _this->m_hardware_format_bitmap.get_d3d_texture();
			if (d3d_texture != NULL)
			{
				IDirect3DSurface9* d3d_surface = NULL;
				HRESULT get_surface_level_result = d3d_texture->GetSurfaceLevel(0, &d3d_surface);
				if (SUCCEEDED(get_surface_level_result))
				{
					HRESULT load_surface_result = D3DXLoadSurfaceFromFileInMemory(d3d_surface, NULL, NULL, buffer, buffer_size, NULL, D3DX_DEFAULT, 0, NULL);
					d3d_surface->Release();

					if (SUCCEEDED(load_surface_result))
					{
						dword bytes = XGSetTextureHeader(_this->m_bitmap.width, _this->m_bitmap.height, 1, 4, _this->m_use_compressed_format ? D3DFMT_DXT5 : D3DFMT_A8R8G8B8, 1, 0, -1, 0, _this->texture_header, 0, 0);
						assert(bytes > 0);

						XGOffsetResourceAddress(_this->texture_header, _this->m_bitmap_pixel_buffer_base);

						_this->m_bitmap_ready = true;
						result = true;
					}
					else
					{
						c_console::write_line("ui:custom_bitmaps: D3DXLoadSurfaceFromFile failed with error code 0x%08X", load_surface_result);
					}
				}
			}
			else
			{
				c_console::write_line("ui:custom_bitmaps: d3d_texture is null");
			}

			return result;
		}
	}

	return result;
}

bool __fastcall c_gui_custom_bitmap_storage_item::sub_B204B0(c_gui_custom_bitmap_storage_item* _this, void* unused, long a1, long a2, long a3, long a4, long a5, long a6)
{
	return false;
}

void __fastcall c_gui_custom_bitmap_storage_item::sub_B204D0(c_gui_custom_bitmap_storage_item* _this, void* unused)
{
	return;
}

void __fastcall c_gui_custom_bitmap_storage_item::unload_rendered_bitmap(c_gui_custom_bitmap_storage_item* _this, void* unused)
{
	if (_this->m_hardware_format_bitmap.valid())
		c_rasterizer_texture_ref::release(_this->m_hardware_format_bitmap);
	
	_this->m_bitmap_ready = false;
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
