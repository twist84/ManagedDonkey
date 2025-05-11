#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

struct c_gui_custom_bitmap_widget :
	public c_gui_bitmap_widget
{
public:
	enum
	{
		k_async_load_buffer_length = 0xC000
	};

	enum e_custom_map_image_type
	{
		_custom_map_image_type_blf = 0,
		_custom_map_image_type_sm_blf,
		_custom_map_image_type_variant_blf,
		_custom_map_image_type_film_blf,
		_custom_map_image_type_clip_blf,

		k_custom_map_image_type_count
	};

	void __thiscall assemble_render_data(s_gui_bitmap_widget_render_data* render_data, rectangle2d* projected_bounds, e_controller_index controller_index, bool offset, bool scale_about_local_point, bool rotate_about_local_point);
	static bool __cdecl get_map_filename(e_custom_map_image_type type, e_map_id map_id, c_static_string<256>* out_filename);
	void __thiscall set_map_image_(e_custom_map_image_type image_type, e_map_id map_id, bool use_compressed_format);
	void __cdecl load_from_file_async(bool use_compressed_format, char const* file_path);
	void __cdecl clear();

	int32 m_storage_item_index;
	char* m_async_load_buffer;
	int32 m_async_load_buffer_count;
	int32 m_async_task_id;
	c_synchronized_long m_async_task_signal;
	c_synchronized_long m_async_task_cancelled;
	c_synchronized_long m_async_task_success;
	s_file_reference m_file;
	bool m_use_compressed_format;
	e_custom_bitmap_desired_aspect_ratio m_desired_aspect_ratio;
	c_static_string<256> m_desired_async_file_to_display;
	c_static_string<256> m_current_async_file_to_display;
	byte __data46C[0x4];
};
static_assert(sizeof(c_gui_custom_bitmap_widget) == 0x470);

struct s_load_image_from_file_task
{
	enum e_state
	{
		_state_starting = 0,
		_state_reading,
		_state_decompressing,
		_state_done,
	};

	e_state state;
	s_file_reference* file;
	uns32 file_size;
	char* load_buffer;
	int32 load_buffer_length;
	int32 storage_item_index;
	e_custom_bitmap_desired_aspect_ratio desired_aspect_ratio;
	c_synchronized_long* cancelled;
	c_synchronized_long* success;
	bool image_source_was_dlc;
};
static_assert(sizeof(s_load_image_from_file_task) == 0x28);

extern int32 __cdecl load_image_from_blf_file_callback(s_load_image_from_file_task* callback_data);

