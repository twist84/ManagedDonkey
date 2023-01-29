#pragma once

#include "bitmaps/bitmaps.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "rasterizer/rasterizer_text.hpp"

struct D3DBaseTexture
{
	short width;
	short height;
	char depth;
	char mipmap_count_plus_one;
	bool high_res_pixels_size_nonzero;
	char type;
	dword xenon_d3d_format;
	byte __dataC[0x28];
};
static_assert(sizeof(D3DBaseTexture) == 0x34);

struct c_gui_custom_bitmap_storage_item
{
	static void __fastcall dispose(c_gui_custom_bitmap_storage_item* _this, void* unused);
	static void __fastcall initialize(c_gui_custom_bitmap_storage_item* _this, void* unused, long width, long height, bool use_compressed_format);
	static bool __fastcall sub_B20480(c_gui_custom_bitmap_storage_item* _this, void* unused, long a1, long a2, long a3, long a4, long a5);
	static bool __fastcall load_from_buffer(c_gui_custom_bitmap_storage_item* _this, long storage_item_index, char const* buffer, long buffer_size, long a4);
	static bool __fastcall sub_B204B0(c_gui_custom_bitmap_storage_item* _this, void* unused, long a1, long a2, long a3, long a4, long a5, long a6);
	static void __fastcall sub_B204D0(c_gui_custom_bitmap_storage_item* _this, void* unused);
	static void __fastcall unload_rendered_bitmap(c_gui_custom_bitmap_storage_item* _this, void* unused);

	long m_width;
	long m_height;

	// true for `dxt5`, false for `a8r8g8b8`
	bool m_use_compressed_format;

	bitmap_data_block_def m_bitmap;

	bool m_allocated;
	byte __pad3D[3];

	c_rasterizer_texture_ref m_hardware_format_bitmap;
	bool m_bitmap_ready;
	char* m_bitmap_pixel_buffer;

	// unsure on the name
	char* m_bitmap_pixel_buffer_base;

	long m_bitmap_pixel_buffer_length;

	// in `halo3_cache_debug` this isn't a pointer making the struct size 0x90
	D3DBaseTexture* texture_header;

	byte __data58[0x8];
};
static_assert(sizeof(c_gui_custom_bitmap_storage_item) == 0x60);

struct c_gui_custom_bitmap_storage_manager
{
	struct s_bitmap_storage_handle_datum : s_datum_header
	{
		word __unknown2;
		long reference_count;
		long __unknown8;
		c_gui_custom_bitmap_storage_item storage_item;
	};
	static_assert(sizeof(s_bitmap_storage_handle_datum) == 0x6C);

	static c_gui_custom_bitmap_storage_manager* __cdecl get();
	bool __cdecl load_bitmap_from_buffer(long storage_item_index, char const* buffer, long buffer_size, long a5);

	c_smart_data_array<s_bitmap_storage_handle_datum>* m_bitmap_storage_items;
	void* m_buffer;
	long m_buffer_size;
};
static_assert(sizeof(c_gui_custom_bitmap_storage_manager) == 0xC);

extern c_gui_custom_bitmap_storage_manager& g_gui_custom_bitmap_storage_manager;