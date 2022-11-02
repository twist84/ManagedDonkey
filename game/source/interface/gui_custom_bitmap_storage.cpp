#include "interface/gui_custom_bitmap_storage.hpp"

#include "bitmaps/bitmaps.hpp"
#include "interface/user_interface_memory.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer_textures_xenon_formats.hpp"

#include <assert.h>
#include <stdio.h>

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
	return;
}

dword __stdcall XGSetTextureHeader(dword width, dword height, dword levels, dword usage, dword format, dword pool, dword base_offset, dword mip_offset, dword pitch, D3DBaseTexture* out_texture, dword* out_base_size, dword* out_mip_size)
{
	return INVOKE(0x00D7AA12, XGSetTextureHeader, width, height, levels, usage, format, pool, base_offset, mip_offset, pitch, out_texture, out_base_size, out_mip_size);
}

void __stdcall XGOffsetResourceAddress(D3DBaseTexture* out_resource, void* base_address)
{
	INVOKE(0x00D7AE34, XGOffsetResourceAddress, out_resource, base_address);
}

const dword bitmap_pixel_buffer_alignment_bits = 12;
void __fastcall c_gui_custom_bitmap_storage_item::initialize(c_gui_custom_bitmap_storage_item* _this, void* unused, long width, long height, bool allocate_bitmap_as_dxt5)
{
	short format = allocate_bitmap_as_dxt5 ? 16 : 11;
	dword hardware_format = rasterizer_bitmap_format_to_hardware_format_xenon_unchecked(format, true, false, false);
	_this->m_format_is_dxt5 = allocate_bitmap_as_dxt5;
	bitmap_2d_initialize(&_this->m_bitmap, static_cast<short>(width), static_cast<short>(height), 0, format, FLAG(3) | FLAG(6), false, false);
	_this->m_bitmap.curve = 3;

	_this->texture_header = new D3DBaseTexture();

	dword base_size = 0;
	dword mip_size = 0;
	dword resource_bytes = XGSetTextureHeader(_this->m_bitmap.width, _this->m_bitmap.height, 1, 4, hardware_format, 0, 0, -1, 0, _this->texture_header, &base_size, &mip_size);
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

bool __fastcall c_gui_custom_bitmap_storage_item::load_from_buffer(c_gui_custom_bitmap_storage_item* _this, long storage_item_index, char const* buffer, long buffer_size, long a4)
{
	return false;
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
	return;
}