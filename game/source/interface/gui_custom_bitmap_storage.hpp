#pragma once

#include "bitmaps/bitmaps.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "rasterizer/rasterizer_text.hpp"

struct c_gui_custom_bitmap_storage_item
{
	long m_width;
	long m_height;

	// true for `dxt5`, false for `a8r8g8b8`
	bool m_format_is_dxt5;

	bitmap_data m_bitmap;
	bool m_allocated;
	c_rasterizer_texture_ref m_hardware_format_bitmap;
	bool m_bitmap_ready;
	char* m_bitmap_pixel_buffer;

	// unsure on the name
	char* m_bitmap_pixel_buffer_end_aligned;

	long m_bitmap_pixel_buffer_length;
	byte __data54[0xC];
};
static_assert(sizeof(c_gui_custom_bitmap_storage_item) == 0x60);

struct c_gui_custom_bitmap_storage_manager
{
	struct s_bitmap_storage_handle_datum : s_datum_header
	{
		word __unknown2;
		long __unknown4;
		long __unknown8;
		c_gui_custom_bitmap_storage_item storage_item;
	};
	static_assert(sizeof(s_bitmap_storage_handle_datum) == 0x6C);

	c_smart_data_array<s_bitmap_storage_handle_datum>* m_bitmap_storage_items;
	void* m_buffer;
	long m_buffer_size;
};
static_assert(sizeof(c_gui_custom_bitmap_storage_manager) == 0xC);

extern c_gui_custom_bitmap_storage_manager& g_gui_custom_bitmap_storage_manager;