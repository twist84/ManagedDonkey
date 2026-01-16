#pragma once

#include "bitmaps/bitmap_group_tag_definition.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "rasterizer/rasterizer_text.hpp"

class c_gui_custom_bitmap_storage_item
{
public:
	void __thiscall dispose();
	bool __thiscall initialize(int32 width, int32 height, bool use_compressed_format);
	bool __thiscall initialize_raw(int32 width, int32 height, char* buffer, int32 buffer_length, bool cpu_cached);
	bool __thiscall load_from_buffer(const char* buffer, int32 buffer_length, void* d3dx_scratch_buffer, int32 d3dx_scratch_buffer_length, e_custom_bitmap_desired_aspect_ratio aspect_ratio);
	bool __thiscall load_from_file_or_buffer(const char* filename, const char* buffer, int32 buffer_length, void* d3dx_scratch_buffer, int32 d3dx_scratch_buffer_length, e_custom_bitmap_desired_aspect_ratio aspect_ratio);
	void __thiscall unload_non_rendered_bitmap();
	void __thiscall unload_rendered_bitmap();

	int32 m_width;
	int32 m_height;
	bool m_use_compressed_format;
	bitmap_data m_bitmap_data;
	bool m_allocated;
	bool m_use_shader_custom_gamma_correct; // added by us
	byte pad[0x2]; // 0x3
	c_rasterizer_texture_ref m_hardware_format_bitmap;
	bool m_bitmap_ready;
	char* m_bitmap_pixel_buffer_allocation;
	char* m_bitmap_pixel_buffer;
	int32 m_bitmap_pixel_buffer_length;
};
COMPILE_ASSERT(sizeof(c_gui_custom_bitmap_storage_item) == 0x54);
COMPILE_ASSERT(0x00 == OFFSETOF(c_gui_custom_bitmap_storage_item, m_width));
COMPILE_ASSERT(0x04 == OFFSETOF(c_gui_custom_bitmap_storage_item, m_height));
COMPILE_ASSERT(0x08 == OFFSETOF(c_gui_custom_bitmap_storage_item, m_use_compressed_format));
COMPILE_ASSERT(0x0C == OFFSETOF(c_gui_custom_bitmap_storage_item, m_bitmap_data));
COMPILE_ASSERT(0x3C == OFFSETOF(c_gui_custom_bitmap_storage_item, m_allocated));
COMPILE_ASSERT(0x40 == OFFSETOF(c_gui_custom_bitmap_storage_item, m_hardware_format_bitmap));
COMPILE_ASSERT(0x44 == OFFSETOF(c_gui_custom_bitmap_storage_item, m_bitmap_ready));
COMPILE_ASSERT(0x48 == OFFSETOF(c_gui_custom_bitmap_storage_item, m_bitmap_pixel_buffer_allocation));
COMPILE_ASSERT(0x4C == OFFSETOF(c_gui_custom_bitmap_storage_item, m_bitmap_pixel_buffer));
COMPILE_ASSERT(0x50 == OFFSETOF(c_gui_custom_bitmap_storage_item, m_bitmap_pixel_buffer_length));

class c_gui_custom_bitmap_storage_manager
{
public:
	enum
	{
		k_maximum_number_of_bitmap_items = 32
	};

	enum e_bitmap_storage_state
	{
		_bitmap_storage_state_none = 0,
		_bitmap_storage_state_loading,
		_bitmap_storage_state_ready,
	};

	struct s_bitmap_storage_handle_datum :
		s_datum_header
	{
		int32 reference_count;
		e_bitmap_storage_state state;
		c_gui_custom_bitmap_storage_item storage_item;
	};
	COMPILE_ASSERT(sizeof(s_bitmap_storage_handle_datum) == 0x60);

	c_gui_custom_bitmap_storage_manager();
	void acquire_bitmap(int32 bitmap_storage_index);
	int32 allocate_bitmap(int32 width, int32 height, bool use_compressed_format);
	void dispose();
	void dispose_from_old_map();
	static c_gui_custom_bitmap_storage_manager* __cdecl get();
	const c_gui_custom_bitmap_storage_item* get_bitmap(int32 bitmap_storage_index);
	void initialize();
	void initialize_for_new_map();
	bool load_bitmap_from_buffer(int32 bitmap_storage_index, const char* buffer, int32 buffer_length, e_custom_bitmap_desired_aspect_ratio aspect_ratio);
	void release_bitmap(int32 bitmap_storage_index);
	void release_unused_bitmap(int32 bitmap_storage_index);
	void update_render();

	c_smart_data_array<s_bitmap_storage_handle_datum> m_bitmap_storage_items;
	void* m_d3dx_scratch_buffer;
	int32 m_d3dx_scratch_buffer_length;
};
COMPILE_ASSERT(sizeof(c_gui_custom_bitmap_storage_manager) == 0xC);
COMPILE_ASSERT(0x0 == OFFSETOF(c_gui_custom_bitmap_storage_manager, m_bitmap_storage_items));
COMPILE_ASSERT(0x4 == OFFSETOF(c_gui_custom_bitmap_storage_manager, m_d3dx_scratch_buffer));
COMPILE_ASSERT(0x8 == OFFSETOF(c_gui_custom_bitmap_storage_manager, m_d3dx_scratch_buffer_length));

extern c_gui_custom_bitmap_storage_manager& g_gui_custom_bitmap_storage_manager;
extern bool g_gui_custom_bitmaps_enabled;

