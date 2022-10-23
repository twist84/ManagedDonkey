#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

struct c_gui_custom_bitmap_widget : c_gui_bitmap_widget
{
	enum e_custom_map_image_type
	{
		_custom_map_image_type_blf = 0,
		_custom_map_image_type_sm_blf,
		_custom_map_image_type_variant_blf,
		_custom_map_image_type_film_blf,
		_custom_map_image_type_clip_blf,

		k_custom_map_image_type_count
	};

	static bool __cdecl get_map_filename(e_custom_map_image_type type, long map_id, c_static_string<256>* out_filename);
	static void __fastcall set_map_image(c_gui_custom_bitmap_widget* _this, void* unused, e_custom_map_image_type image_type, long map_id, bool allocate_bitmap);
	void __cdecl load_from_file_async(bool allocate_bitmap, char const* file_path);
	void __cdecl clear();

	long m_storage_item_index;
	char* m_async_load_buffer;
	long m_async_load_buffer_size;
	long m_async_task_id;
	c_synchronized_long m_async_result;
	c_synchronized_long __unknown14C;
	c_synchronized_long __unknown150;
	s_file_reference m_file;
	bool m_allocate_bitmap;
	long __unknown268;
	c_static_string<256> m_path;
	c_static_string<256> m_name;
	byte __data46C[0x4];
};
static_assert(sizeof(c_gui_custom_bitmap_widget) == 0x470);

extern long __cdecl map_image_load_callback(void* callback_data);