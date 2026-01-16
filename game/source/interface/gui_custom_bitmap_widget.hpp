#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

struct s_async_task;

class c_gui_custom_bitmap_widget :
	public c_gui_bitmap_widget
{
public:
	enum
	{
		k_async_load_buffer_length = 0xC000
	};

	enum e_custom_map_image_type
	{
		_custom_map_image_type_normal = 0,
		_custom_map_image_type_small,
		_custom_map_image_type_variant,
		_custom_map_image_type_film,
		_custom_map_image_type_film_clip,
	};

public:
	void __thiscall assemble_render_data_(s_gui_widget_render_data* render_data, rectangle2d* projected_bounds, e_controller_index controller_index, bool offset, bool scale_about_local_point, bool rotate_about_local_point);
	bool __thiscall load_from_buffer_(bool use_compressed_format, const char* buffer, int32 buffer_length, e_custom_bitmap_desired_aspect_ratio aspect_ratio);
	void __thiscall set_map_image_(e_custom_map_image_type image_type, e_map_id map_id, bool use_compressed_format);

public:
	virtual ~c_gui_custom_bitmap_widget() = default;
	virtual void dispose() override;
	virtual bool should_render(bool* add_to_render_list) override;
	virtual void update(uns32 current_milliseconds) override;
	virtual void assemble_render_data(s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation) override;
	virtual const bitmap_data* get_current_bitmap() const override;
	virtual void __func39(c_tag_resource_demand_collector* demand_collector) override;
	virtual char* get_current_buffer() const;
	virtual int32 get_current_buffer_length() const;

public:
	c_gui_custom_bitmap_widget();
	void clear();

private:
	bool get_dimensions(int32* out_width, int32* out_height);

public:
	static bool __cdecl get_map_filename(e_custom_map_image_type type, e_map_id map_id, c_static_string<256>* out_filename);
	void load_from_file_async(bool use_compressed_format, const char* file_path, e_custom_bitmap_desired_aspect_ratio aspect_ratio);
	bool load_from_buffer(bool use_compressed_format, const char* buffer, int32 buffer_length, e_custom_bitmap_desired_aspect_ratio aspect_ratio);
	bool load_from_file_async_in_progress() const;

private:
	void reset();

public:
	void set_map_image(e_custom_map_image_type image_type, e_map_id map_id, bool use_compressed_format);

//protected:
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
COMPILE_ASSERT(sizeof(c_gui_custom_bitmap_widget) == 0x470);
COMPILE_ASSERT(sizeof(c_gui_custom_bitmap_widget) == sizeof(c_gui_bitmap_widget) + 0x338);
COMPILE_ASSERT(0x138 == OFFSETOF(c_gui_custom_bitmap_widget, m_storage_item_index));
COMPILE_ASSERT(0x13C == OFFSETOF(c_gui_custom_bitmap_widget, m_async_load_buffer));
COMPILE_ASSERT(0x140 == OFFSETOF(c_gui_custom_bitmap_widget, m_async_load_buffer_count));
COMPILE_ASSERT(0x144 == OFFSETOF(c_gui_custom_bitmap_widget, m_async_task_id));
COMPILE_ASSERT(0x148 == OFFSETOF(c_gui_custom_bitmap_widget, m_async_task_signal));
COMPILE_ASSERT(0x14C == OFFSETOF(c_gui_custom_bitmap_widget, m_async_task_cancelled));
COMPILE_ASSERT(0x150 == OFFSETOF(c_gui_custom_bitmap_widget, m_async_task_success));
COMPILE_ASSERT(0x154 == OFFSETOF(c_gui_custom_bitmap_widget, m_file));
COMPILE_ASSERT(0x264 == OFFSETOF(c_gui_custom_bitmap_widget, m_use_compressed_format));
COMPILE_ASSERT(0x268 == OFFSETOF(c_gui_custom_bitmap_widget, m_desired_aspect_ratio));
COMPILE_ASSERT(0x26C == OFFSETOF(c_gui_custom_bitmap_widget, m_desired_async_file_to_display));
COMPILE_ASSERT(0x36C == OFFSETOF(c_gui_custom_bitmap_widget, m_current_async_file_to_display));
COMPILE_ASSERT(0x46C == OFFSETOF(c_gui_custom_bitmap_widget, __data46C));

extern e_async_completion __cdecl load_image_from_blf_file_callback(s_async_task* work);
extern e_async_completion __cdecl load_image_from_buffer_callback(s_async_task* work);

