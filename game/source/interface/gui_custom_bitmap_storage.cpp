#include "interface/gui_custom_bitmap_storage.hpp"

#include "interface/user_interface_memory.hpp"
#include "memory/module.hpp"

#include <assert.h>
#include <stdio.h>

HOOK_DECLARE_CLASS(0x00B20460, c_gui_custom_bitmap_storage_item, dispose);
HOOK_DECLARE_CLASS(0x00B20470, c_gui_custom_bitmap_storage_item, initialize);
HOOK_DECLARE_CLASS(0x00B20480, c_gui_custom_bitmap_storage_item, sub_B20480);
HOOK_DECLARE_CLASS(0x00B20490, c_gui_custom_bitmap_storage_item, load_from_buffer);
HOOK_DECLARE_CLASS(0x00B204B0, c_gui_custom_bitmap_storage_item, sub_B204B0);
HOOK_DECLARE_CLASS(0x00B204D0, c_gui_custom_bitmap_storage_item, sub_B204D0);
HOOK_DECLARE_CLASS(0x00B204E0, c_gui_custom_bitmap_storage_item, unload_rendered_bitmap);

c_gui_custom_bitmap_storage_manager& g_gui_custom_bitmap_storage_manager = *reinterpret_cast<c_gui_custom_bitmap_storage_manager*>(0x05270C14);

void __fastcall c_gui_custom_bitmap_storage_item::dispose(c_gui_custom_bitmap_storage_item* _this, void* unused)
{
	return;
}

static dword __cdecl rasterizer_bitmap_format_to_hardware_format_xenon_unchecked(short format, bool a2, bool a3, bool a4)
{
	return INVOKE(0x00A7C950, rasterizer_bitmap_format_to_hardware_format_xenon_unchecked, format, a2, a3, a4);
}

void __cdecl bitmap_2d_initialize(bitmap_data* bitmap, short width, short height, short mipmap_count, short format, word flags, bool a7, bool a8)
{
	INVOKE(0x0086AAE0, bitmap_2d_initialize, bitmap, width, height, mipmap_count, format, flags, a7, a8);
}

dword __stdcall XGSetTextureHeader(
	dword width,
	dword height,
	dword mipmap_count,
	dword a4,
	dword hardware_format,
	dword a6,
	dword a7,
	dword a8,
	dword a9,
	D3DBaseTexture* texture_header,
	dword* a11,
	dword* a12)
{
	return INVOKE(0x00D7AA12, XGSetTextureHeader, width, height, mipmap_count, a4, hardware_format, a6, a7, a8, a9, texture_header, a11, a12);
}

void __stdcall XGOffsetResourceAddress(D3DBaseTexture* pResource, char const* base)
{
	INVOKE(0x00D7AE34, XGOffsetResourceAddress, pResource, base);
}

void __fastcall c_gui_custom_bitmap_storage_item::initialize(c_gui_custom_bitmap_storage_item* _this, void* unused, long width, long height, bool allocate_bitmap_as_dxt5)
{
	short format = allocate_bitmap_as_dxt5 ? 16 : 11;
	dword hardware_format = rasterizer_bitmap_format_to_hardware_format_xenon_unchecked(format, true, false, false);
	_this->m_format_is_dxt5 = allocate_bitmap_as_dxt5;
	bitmap_2d_initialize(&_this->m_bitmap, static_cast<short>(width), static_cast<short>(height), 0, format, FLAG(3) | FLAG(6), false, false);
	_this->m_bitmap.curve = 3;

	dword __unknown_a11 = 0;
	dword __unknown_a12 = 0;
	_this->texture_header = new D3DBaseTexture;
	dword xg_set_texture_header_result = XGSetTextureHeader(_this->m_bitmap.width, _this->m_bitmap.height, 1, 4, hardware_format, 0, 0, -1, 0, _this->texture_header, &__unknown_a11, &__unknown_a12);
	if (xg_set_texture_header_result)
	{
		long allocate_bytes = xg_set_texture_header_result + 4096;
		_this->m_bitmap_pixel_buffer = (char*)user_interface_malloc_tracked(allocate_bytes, __FILE__, __LINE__);
		if (_this->m_bitmap_pixel_buffer)
		{
			_this->m_width = width;
			_this->m_height = height;
			_this->m_bitmap_pixel_buffer_end_aligned = (char*)(((long)_this->m_bitmap_pixel_buffer & 0xFFFFF000) + 4096);
			_this->m_bitmap_pixel_buffer_length = allocate_bytes - (_this->m_bitmap_pixel_buffer_end_aligned - _this->m_bitmap_pixel_buffer);
			assert(_this->m_bitmap_pixel_buffer_length <= allocate_bytes);

			XGOffsetResourceAddress(_this->texture_header, _this->m_bitmap_pixel_buffer_end_aligned);
			_this->m_allocated = true;
		}
	}
}

bool __fastcall c_gui_custom_bitmap_storage_item::sub_B20480(c_gui_custom_bitmap_storage_item* _this, void* unused, long a1, long a2, long a3, long a4, long a5)
{
	return false;
}

bool __fastcall c_gui_custom_bitmap_storage_item::load_from_buffer(c_gui_custom_bitmap_storage_item* _this, void* unused, long storage_item_index, char const* buffer, long buffer_size, long a4)
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