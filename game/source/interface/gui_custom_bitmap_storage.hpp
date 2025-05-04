#pragma once

#include "bitmaps/bitmap_group_tag_definition.hpp"
#include "bitmaps/bitmaps.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "rasterizer/rasterizer_text.hpp"

struct D3DBaseTexture
{
	int16 width;
	int16 height;
	int8 depth;
	int8 mipmap_count_plus_one;
	bool high_res_pixels_size_nonzero;
	int8 type;
	uns32 xenon_d3d_format;
	byte __dataC[0x28];
};
static_assert(sizeof(D3DBaseTexture) == 0x34);

struct c_gui_custom_bitmap_storage_item
{
	void __thiscall dispose();
	void __thiscall initialize(int32 width, int32 height, bool use_compressed_format);
	bool __thiscall initialize_raw(int32 width, int32 height, char* buffer, int32 buffer_length, bool cpu_cached);
	bool __thiscall load_from_buffer(char const* buffer, int32 buffer_length, void* d3dx_scratch_buffer, int32 d3dx_scratch_buffer_length, int32 aspect_ratio);
	bool __thiscall load_from_file_or_buffer(char const* filename, char const* buffer, int32 buffer_length, void* d3dx_scratch_buffer, int32 d3dx_scratch_buffer_length, int32 aspect_ratio);
	void __thiscall unload_non_rendered_bitmap();
	void __thiscall unload_rendered_bitmap();

	int32 m_width;
	int32 m_height;

	// true for `dxt5`, false for `a8r8g8b8`
	bool m_use_compressed_format;

	bitmap_data m_bitmap;

	bool m_allocated;
	byte __pad3D[0x3];

	c_rasterizer_texture_ref m_hardware_format_bitmap;
	bool m_bitmap_ready;
	char* m_bitmap_pixel_buffer;

	// unsure on the name
	char* m_bitmap_pixel_buffer_base;

	int32 m_bitmap_pixel_buffer_length;

	// in `halo3_cache_debug` this isn't a pointer making the struct size 0x90
	D3DBaseTexture* texture_header;

	byte __data58[0x8];
};
static_assert(sizeof(c_gui_custom_bitmap_storage_item) == 0x60);

struct c_gui_custom_bitmap_storage_manager
{
	struct s_bitmap_storage_handle_datum :
		s_datum_header
	{
		uns16 __unknown2;
		int32 reference_count;
		int32 __unknown8;
		c_gui_custom_bitmap_storage_item storage_item;
	};
	static_assert(sizeof(s_bitmap_storage_handle_datum) == 0x6C);

	static c_gui_custom_bitmap_storage_manager* __cdecl get();
	bool __cdecl load_bitmap_from_buffer(int32 storage_item_index, char const* buffer, int32 buffer_size, int32 a5);

	c_smart_data_array<s_bitmap_storage_handle_datum> m_bitmap_storage_items;
	void* m_buffer;
	int32 m_buffer_size;
};
static_assert(sizeof(c_gui_custom_bitmap_storage_manager) == 0xC);

extern c_gui_custom_bitmap_storage_manager& g_gui_custom_bitmap_storage_manager;