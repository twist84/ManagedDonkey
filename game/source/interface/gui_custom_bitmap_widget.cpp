#include "interface/gui_custom_bitmap_widget.hpp"

#include "cache/cache_files.hpp"
#include "cseries/async.hpp"
#include "interface/gui_custom_bitmap_storage.hpp"
#include "interface/user_interface_memory.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"
#include "networking/tools/network_blf.hpp"
#include "saved_games/content_catalogue.hpp"
#include "tag_files/string_ids.hpp"
#include "main/levels.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AC37B0, c_gui_custom_bitmap_widget, assemble_render_data_);
HOOK_DECLARE_CLASS_MEMBER(0x00AC3A80, c_gui_custom_bitmap_widget, load_from_buffer_);
HOOK_DECLARE_CLASS_MEMBER(0x00AC3DE0, c_gui_custom_bitmap_widget, set_map_image_);
HOOK_DECLARE(0x00AC3B80, load_image_from_blf_file_callback);
HOOK_DECLARE(0x00AC3D00, load_image_from_buffer_callback);

void __thiscall c_gui_custom_bitmap_widget::assemble_render_data_(s_gui_widget_render_data* render_data, rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	//INVOKE_CLASS_MEMBER(0x00AC37B0, c_gui_custom_bitmap_widget, assemble_render_data, render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);

	c_gui_custom_bitmap_widget::assemble_render_data(render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);
}

bool __thiscall c_gui_custom_bitmap_widget::load_from_buffer_(bool use_compressed_format, const char* buffer, int32 buffer_length, e_custom_bitmap_desired_aspect_ratio aspect_ratio)
{
	return c_gui_custom_bitmap_widget::load_from_buffer(use_compressed_format, buffer, buffer_length, aspect_ratio);
}

void __thiscall c_gui_custom_bitmap_widget::set_map_image_(e_custom_map_image_type image_type, e_map_id map_id, bool use_compressed_format)
{
	c_gui_custom_bitmap_widget::set_map_image(image_type, map_id, use_compressed_format);
}

c_gui_custom_bitmap_widget::c_gui_custom_bitmap_widget() :
	c_gui_bitmap_widget(),
	m_storage_item_index(NONE),
	m_async_load_buffer(nullptr),
	m_async_load_buffer_count(0),
	m_async_task_id(INVALID_ASYNC_TASK_ID),
	m_async_task_signal(0),
	m_async_task_cancelled(0),
	m_async_task_success(0),
	m_use_compressed_format(),
	m_desired_aspect_ratio(),
	m_desired_async_file_to_display(),
	m_current_async_file_to_display(),
	__data46C()
{
	//DECLFUNC(0x00AC3610, void, __thiscall, c_gui_custom_bitmap_widget*)(this);
}

//.text:00AC36A0 ; 
//.text:00AC36B0 ; 

//.text:00AC36C0 ; public: virtual void* c_gui_custom_bitmap_widget::`scalar deleting destructor'(unsigned int)
//.text:00AC36F0 ; public: bool c_gui_custom_bitmap_widget::acquire_from_custom_bitmap(const c_gui_custom_bitmap_widget*)
//.text:00AC3730 ; public: bool c_gui_custom_bitmap_widget::allocate(bool)

void c_gui_custom_bitmap_widget::assemble_render_data(s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	//INVOKE_CLASS_MEMBER(0x00AC37B0, c_gui_custom_bitmap_widget, assemble_render_data, render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);

	s_gui_bitmap_widget_render_data* bitmap_render_data = (s_gui_bitmap_widget_render_data*)render_data;
	c_gui_bitmap_widget::assemble_render_data(bitmap_render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);
	bitmap_render_data->flags.set(s_gui_bitmap_widget_render_data::_render_as_custom_storage_bitmap_bit, true);
	bitmap_render_data->source.custom_bitmap.storage_index = m_storage_item_index;
}

//.text:00AC37F0 ; public: void c_gui_custom_bitmap_widget::blank()

void c_gui_custom_bitmap_widget::clear()
{
	//INVOKE_CLASS_MEMBER(0x00AC3800, c_gui_custom_bitmap_widget, clear);

	m_desired_async_file_to_display.clear();
}

void c_gui_custom_bitmap_widget::dispose()
{
	INVOKE_CLASS_MEMBER(0x00AC3820, c_gui_custom_bitmap_widget, dispose);

	c_gui_widget::dispose();
	c_gui_custom_bitmap_widget::reset();
}

//.text:00AC3830 ; 

const bitmap_data* c_gui_custom_bitmap_widget::get_current_bitmap() const
{
	//return INVOKE_CLASS_MEMBER(0x00AC3840, c_gui_custom_bitmap_widget, get_current_bitmap);

	const c_gui_custom_bitmap_storage_item* bitmap = c_gui_custom_bitmap_storage_manager::get()->get_bitmap(m_storage_item_index);
	if (!bitmap || !bitmap->m_bitmap_ready)
	{
		return nullptr;
	}

	return &bitmap->m_bitmap_data;
}

char* c_gui_custom_bitmap_widget::get_current_buffer() const
{
	//return INVOKE_CLASS_MEMBER(0x00AC3870, c_gui_custom_bitmap_widget, get_current_buffer);

	const c_gui_custom_bitmap_storage_item* bitmap = c_gui_custom_bitmap_storage_manager::get()->get_bitmap(m_storage_item_index);
	if (!bitmap)
	{
		return nullptr;
	}

	return bitmap->m_bitmap_pixel_buffer;
}

int32 c_gui_custom_bitmap_widget::get_current_buffer_length() const
{
	//return INVOKE_CLASS_MEMBER(0x00AC3890, c_gui_custom_bitmap_widget, get_current_buffer_length);

	const c_gui_custom_bitmap_storage_item* bitmap = c_gui_custom_bitmap_storage_manager::get()->get_bitmap(m_storage_item_index);
	if (!bitmap)
	{
		return 0;
	}

	return bitmap->m_bitmap_pixel_buffer_length;
}

bool c_gui_custom_bitmap_widget::get_dimensions(int32* out_width, int32* out_height)
{
	return INVOKE_CLASS_MEMBER(0x00AC38B0, c_gui_custom_bitmap_widget, get_dimensions, out_width, out_height);
}

bool __cdecl c_gui_custom_bitmap_widget::get_map_filename(e_custom_map_image_type type, e_map_id map_id, c_static_string<256>* out_filename)
{
	//return INVOKE(0x00AC3900, c_gui_custom_bitmap_widget::get_map_filename, type, map_id, out_filename);

	if (map_id == _map_id_none)
	{
		return false;
	}

	c_static_string<256> image_file_path;
	s_level_datum level{};

	if (levels_try_and_get_campaign_map(map_id, &level) || levels_try_and_get_multiplayer_map(map_id, &level))
	{
		image_file_path.set(level.image_file_base);
	}

	bool result = image_file_path.length() > 0;
	if (result)
	{
		switch (type)
		{
		case _custom_map_image_type_normal:
			out_filename->print("%s.blf", image_file_path.get_string());
			break;
		case _custom_map_image_type_small:
			out_filename->print("%s_sm.blf", image_file_path.get_string());
			break;
		case _custom_map_image_type_variant:
			out_filename->print("%s_variant.blf", image_file_path.get_string());
			break;
		case _custom_map_image_type_film:
			out_filename->print("%s_film.blf", image_file_path.get_string());
			break;
		case _custom_map_image_type_film_clip:
			out_filename->print("%s_clip.blf", image_file_path.get_string());
			break;
		}
	}

	return result;
}

//.text:00AC3A10 ; 
//.text:00AC3A20 ; 
//.text:00AC3A30 ; 

void c_gui_custom_bitmap_widget::load_from_file_async(bool use_compressed_format, const char* file_path, e_custom_bitmap_desired_aspect_ratio aspect_ratio)
{
	//INVOKE_CLASS_MEMBER(0x00AC3A40, c_gui_custom_bitmap_widget, load_from_file_async, use_compressed_format, file_path, aspect_ratio);

	m_desired_async_file_to_display.set(file_path);
	m_use_compressed_format = use_compressed_format;
	m_desired_aspect_ratio = aspect_ratio;
}

bool c_gui_custom_bitmap_widget::load_from_buffer(bool use_compressed_format, const char* buffer, int32 buffer_length, e_custom_bitmap_desired_aspect_ratio aspect_ratio)
{
	//return INVOKE_CLASS_MEMBER(0x00AC3A80, c_gui_custom_bitmap_widget, load_from_buffer, use_compressed_format, buffer, buffer_length, aspect_ratio);

	reset();

	int32 width = 0;
	int32 height = 0;
	if (!get_dimensions(&width, &height))
	{
		reset();
		return false;
	}

	m_storage_item_index = c_gui_custom_bitmap_storage_manager::get()->allocate_bitmap(width, height, use_compressed_format);
	if (m_storage_item_index == NONE)
	{
		reset();
		return false;
	}

	s_load_image_from_buffer_task task{};
	task.buffer = buffer;
	task.buffer_length = buffer_length;
	task.storage_item_index = m_storage_item_index;
	task.desired_aspect_ratio = aspect_ratio;
	task.success = &m_async_task_success;
	m_async_task_success.set(false);
	
	//while (m_async_task_success.peek() != 1 && load_image_from_buffer_callback(&task) != _async_completion_done)
	//{
	//	sleep(5);
	//}

	m_async_task_id = async_task_add(_async_priority_important_non_blocking, &task.dummy_for_size, _async_category_online_files, load_image_from_buffer_callback, &m_async_task_signal);
	if (!load_from_file_async_in_progress())
	{
		reset();
		return false;
	}

	return true;
}

bool c_gui_custom_bitmap_widget::load_from_file_async_in_progress() const
{
	//return INVOKE_CLASS_MEMBER(0x00AC3B70, c_gui_custom_bitmap_widget, load_from_file_async_in_progress);

	return m_async_task_id != INVALID_ASYNC_TASK_ID;
}

e_async_completion __cdecl load_image_from_blf_file_callback(s_async_task* work)
{
	//return INVOKE(0x00AC3B80, load_image_from_blf_file_callback, work);

	s_load_image_from_file_task* task = (s_load_image_from_file_task*)work;

	bool completed = false;
	bool success = false;
	bool cancelled = task->cancelled->peek() != 0;

	switch (task->state)
	{
	case s_load_image_from_file_task::_state_starting:
	{
		if (!cancelled)
		{
			wchar_t name_buffer[256]{};

			constexpr int32 name_flags = FLAG(_name_directory_bit) | FLAG(_name_extension_bit) | FLAG(_name_file_bit);
			wchar_t* name = file_reference_get_name_wide(task->file, name_flags, name_buffer, NUMBEROF(name_buffer));

			task->image_source_was_dlc = content_catalogue_open_dlc(name, true);

			uns32 error_code = 0;
			constexpr int32 opent_flags = FLAG(_permission_read_bit);
			if (file_open(task->file, opent_flags, &error_code))
			{
				if (file_get_size(task->file, &task->file_size) && task->file_size < (uns32)task->load_buffer_length)
				{
					task->state = s_load_image_from_file_task::_state_reading;
					completed = true;

					break;
				}

				file_close(task->file);
			}
		}
	}
	break;
	case s_load_image_from_file_task::_state_reading:
	{
		if (!cancelled && file_read(task->file, task->file_size, true, task->load_buffer))
		{
			file_close(task->file);

			task->state = s_load_image_from_file_task::_state_decompressing;
			completed = true;

			break;
		}

		// left over from `case 0`
		file_close(task->file);
	}
	break;
	case s_load_image_from_file_task::_state_decompressing:
	{
		if (!cancelled)
		{
			int32 image_data_length = 0;
			//const s_map_image_data* map_image_data = (const s_map_image_data*)c_network_blf_buffer_reader::find_chunk(task->load_buffer, task->file_size, s_blf_chunk_map_image::k_chunk_type, s_blf_chunk_map_image::k_chunk_major_version, _blf_file_authentication_type_rsa, &image_data_length);

			auto find_chunk = DECLFUNC(0x00462B40, const char*, __cdecl, const char*, int32, int32, int32, int32, int32*); // funk you
			const s_map_image_data* map_image_data = (const s_map_image_data*)find_chunk(task->load_buffer, task->file_size, s_blf_chunk_map_image::k_chunk_type, s_blf_chunk_map_image::k_chunk_major_version, _blf_file_authentication_type_rsa, &image_data_length);

			if (map_image_data)
			{
				s_map_image_data::e_image_type image_type = map_image_data->image_type;

				// hack
				if (!VALID_INDEX(bswap_uns32(map_image_data->image_type), s_map_image_data::k_image_type_count))
				{
					image_type = (s_map_image_data::e_image_type)bswap_uns32(map_image_data->image_type);
				}

				if (image_data_length > sizeof(s_map_image_data) && VALID_INDEX(image_type, s_map_image_data::k_image_type_count))
				{
					int32 image_data_bytes = map_image_data->image_data_bytes;

					// hack
					if (bswap_uns32(image_data_bytes) == image_data_length - sizeof(s_map_image_data))
					{
						image_data_bytes = bswap_uns32(image_data_bytes);
					}

					if (image_data_bytes == image_data_length - sizeof(s_map_image_data))
					{
						const char* image_data = (const char*)map_image_data + sizeof(s_map_image_data);
						if (c_gui_custom_bitmap_storage_manager::get()->load_bitmap_from_buffer(task->storage_item_index, image_data, image_data_bytes, task->desired_aspect_ratio))
						{
							task->state = s_load_image_from_file_task::_state_done;
							completed = true;
							success = true;
						}
					}
				}
			}
		}
	}
	break;
	}

	*task->success = success;

	e_async_completion result = _async_completion_retry;
	if (!completed || success)
	{
		if (task->image_source_was_dlc)
		{
			content_catalogue_close_all_dlc(true);
		}

		result = _async_completion_done;
	}
	return result;
}

e_async_completion __cdecl load_image_from_buffer_callback(s_async_task* work)
{
	//return INVOKE(0x00AC3D00, load_image_from_buffer_callback, work);

	s_load_image_from_buffer_task* task = (s_load_image_from_buffer_task*)work;

	if (c_gui_custom_bitmap_storage_manager::get()->load_bitmap_from_buffer(
		task->storage_item_index,
		task->buffer,
		task->buffer_length,
		task->desired_aspect_ratio))
	{
		task->success->set(true);
	}

	return _async_completion_done;
}

void c_gui_custom_bitmap_widget::__func39(c_tag_resource_demand_collector* demand_collector)
{
	//INVOKE_CLASS_MEMBER(0x00AC3D40, c_gui_custom_bitmap_widget, __func39, demand_collector);
}

void c_gui_custom_bitmap_widget::reset()
{
	//INVOKE_CLASS_MEMBER(0x00AC3D50, c_gui_custom_bitmap_widget, reset);

	if (load_from_file_async_in_progress())
	{
		m_async_task_cancelled.set(true);
		internal_async_yield_until_done(&m_async_task_signal, true, false, __FILE__, __LINE__);
		m_async_task_id = INVALID_ASYNC_TASK_ID;
	}

	if (m_storage_item_index != NONE)
	{
		c_gui_custom_bitmap_storage_manager::get()->release_bitmap(m_storage_item_index);
		m_storage_item_index = NONE;
	}

	if (m_async_load_buffer != nullptr)
	{
		user_interface_free(m_async_load_buffer);
		m_async_load_buffer = nullptr;
	}
}

void c_gui_custom_bitmap_widget::set_map_image(e_custom_map_image_type image_type, e_map_id map_id, bool use_compressed_format)
{
	//INVOKE_CLASS_MEMBER(0x00AC3DE0, c_gui_custom_bitmap_widget, set_map_image, image_type, map_id, use_compressed_format);

	// using a compressed format seems to cause a hang in D3D, overriding until a solution is found
	use_compressed_format = false;

	static c_static_string<256> map_image_path;
	map_image_path.clear();

	if (get_map_filename(image_type, map_id, &map_image_path))
	{
		load_from_file_async(use_compressed_format, map_image_path.get_string(), _custom_bitmap_desired_aspect_ratio_stretch_to_fit);
	}
	else
	{
		clear();
	}
}

//.text:00AC3E60 ; public: void c_gui_custom_bitmap_widget::set_map_image_from_metadata_datasource(c_gui_data*, int32, bool)

bool c_gui_custom_bitmap_widget::should_render(bool* add_to_render_list)
{
	return INVOKE_CLASS_MEMBER(0x00AC3F30, c_gui_custom_bitmap_widget, should_render, add_to_render_list);

	//return c_gui_widget::should_render(add_to_render_list) && load_from_file_async_in_progress();
}

void c_gui_custom_bitmap_widget::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00AC3F60, c_gui_custom_bitmap_widget, update, current_milliseconds);
}

