#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

struct c_gui_custom_bitmap_widget :
	public c_gui_bitmap_widget
{
public:

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
	void __thiscall _set_map_image(e_custom_map_image_type image_type, long map_id, bool use_compressed_format);
	void __thiscall _assemble_render_data(byte* render_data, rectangle2d* projected_bounds, e_controller_index controller_index, bool offset, bool scale_about_local_point, bool rotate_about_local_point);
	void __cdecl load_from_file_async(bool use_compressed_format, char const* file_path);
	void __cdecl clear();

	long m_storage_item_index;
	char* m_async_load_buffer;
	long m_async_load_buffer_size;
	long m_async_task_id;
	c_synchronized_long m_async_result;
	c_synchronized_long __unknown14C;
	c_synchronized_long __unknown150;
	s_file_reference m_file;
	bool m_use_compressed_format;
	long __unknown268;
	c_static_string<256> m_path;
	c_static_string<256> m_name;
	byte __data46C[0x4];
};
static_assert(sizeof(c_gui_custom_bitmap_widget) == 0x470);

struct s_map_image_load_callback_data
{
	long async_load_stage;

	s_file_reference* async_load_file;
	dword async_load_file_size;

	// size: 0xC000
	char* async_load_buffer;
	dword async_load_buffer_size;

	long bitmap_storage_item_index;

	// c_gui_custom_bitmap_widget::__unknown268
	long __unknown18;

	// &c_gui_custom_bitmap_widget::__unknown14C
	c_synchronized_long* __unknown1C;

	// &c_gui_custom_bitmap_widget::__unknown150
	c_synchronized_long* __unknown20;

	// `c_storage_device` related, mounted/opened?
	bool __unknown24;
};
static_assert(sizeof(s_map_image_load_callback_data) == 0x28);

extern void patch_gui_custom_bitmap_widget();
extern long __cdecl map_image_load_callback(s_map_image_load_callback_data* callback_data);

