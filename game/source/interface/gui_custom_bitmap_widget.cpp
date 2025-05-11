#include "interface/gui_custom_bitmap_widget.hpp"

#include "cache/cache_files.hpp"
#include "interface/gui_custom_bitmap_storage.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"
#include "networking/tools/network_blf.hpp"
#include "saved_games/content_catalogue.hpp"
#include "tag_files/string_ids.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AC37B0, c_gui_custom_bitmap_widget, assemble_render_data);
HOOK_DECLARE_CLASS(0x00AC3900, c_gui_custom_bitmap_widget, get_map_filename);
HOOK_DECLARE_CLASS_MEMBER(0x00AC3DE0, c_gui_custom_bitmap_widget, set_map_image_);
HOOK_DECLARE(0x00AC3B80, load_image_from_blf_file_callback);

bool __cdecl c_gui_custom_bitmap_widget::get_map_filename(e_custom_map_image_type type, e_map_id map_id, c_static_string<256>* out_filename)
{
	bool result = false;
	HOOK_INVOKE_CLASS(result =, c_gui_custom_bitmap_widget, get_map_filename, decltype(&get_map_filename), type, map_id, out_filename);
	return result;
}

void __thiscall c_gui_custom_bitmap_widget::set_map_image_(e_custom_map_image_type image_type, e_map_id map_id, bool use_compressed_format)
{
	// using a compressed format seems to cause a hang in D3D, overriding until a solution is found
	use_compressed_format = false;

	static c_static_string<256> map_image_path;
	map_image_path.clear();

	if (get_map_filename(image_type, map_id, &map_image_path))
	{
		load_from_file_async(use_compressed_format, map_image_path.get_string());
	}
	else
	{
		clear();
	}
}

void __cdecl c_gui_custom_bitmap_widget::load_from_file_async(bool use_compressed_format, char const* file_path)
{
	m_desired_async_file_to_display.set(file_path);
	m_use_compressed_format = use_compressed_format;
	m_desired_aspect_ratio = _custom_bitmap_desired_aspect_ratio_stretch_to_fit;
}

void __thiscall c_gui_custom_bitmap_widget::assemble_render_data(s_gui_bitmap_widget_render_data* render_data, rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	//INVOKE_CLASS_MEMBER(0x00AC37B0, c_gui_custom_bitmap_widget, assemble_render_data, render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);

	c_gui_bitmap_widget::assemble_render_data(render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);
	render_data->flags.set(s_gui_bitmap_widget_render_data::_render_as_custom_storage_bitmap_bit, true);
	render_data->source.custom_bitmap.storage_index = m_storage_item_index;
}

void __cdecl c_gui_custom_bitmap_widget::clear()
{
	m_desired_async_file_to_display.clear();
}

int32 __cdecl load_image_from_blf_file_callback(s_load_image_from_file_task* callback_data)
{
	wchar_t name_buffer[256];

	bool v2 = false;
	bool success = false;
	bool cancelled = callback_data->cancelled->peek() != 0;

	switch (callback_data->state)
	{
	case s_load_image_from_file_task::_state_starting:
	{
		if (!cancelled)
		{
			constexpr int32 name_flags = FLAG(_name_directory_bit) | FLAG(_name_extension_bit) | FLAG(_name_file_bit);
			wchar_t* name = file_reference_get_name_wide(callback_data->file, name_flags, name_buffer, NUMBEROF(name_buffer));

			callback_data->image_source_was_dlc = content_catalogue_open_dlc(name, true);

			uns32 error = 0;
			if (file_open(callback_data->file, FLAG(_file_open_flag_desired_access_read), &error))
			{
				if (file_get_size(callback_data->file, &callback_data->file_size)
					&& callback_data->file_size < (uns32)callback_data->load_buffer_length)
				{
					callback_data->state = s_load_image_from_file_task::_state_reading;
					v2 = true;

					break;
				}

				file_close(callback_data->file);
			}
		}
	}
	break;
	case s_load_image_from_file_task::_state_reading:
	{
		if (!cancelled && file_read(callback_data->file, callback_data->file_size, true, callback_data->load_buffer))
		{
			file_close(callback_data->file);

			callback_data->state = s_load_image_from_file_task::_state_decompressing;
			v2 = true;

			break;
		}
		// left over from `case 0`
		file_close(callback_data->file);
	}
	break;
	case s_load_image_from_file_task::_state_decompressing:
	{
		if (!cancelled)
		{
			int32 image_data_length = 0;
			// c_network_blf_buffer_reader::find_chunk(load_buffer, file_size, s_blf_chunk_map_image::k_chunk_type, s_blf_chunk_map_image::k_version_major, _blf_file_authentication_type_rsa, &chunk_size);
			char const* chunk = DECLFUNC(0x00462B40, char const*, __cdecl, char const*, int32, int32, int32, int32, int32*)(
				callback_data->load_buffer,
				callback_data->file_size,
				s_blf_chunk_map_image::k_chunk_type,
				s_blf_chunk_map_image::k_version_major,
				_blf_file_authentication_type_rsa,
				&image_data_length);

			if (chunk)
			{
				if (image_data_length > 8 && VALID_INDEX(*chunk, k_map_image_type_count))
				{
					int32 buffer_size = *reinterpret_cast<int32 const*>(chunk + 4);
					char const* buffer = reinterpret_cast<char const*>(chunk + 8);

					// hack
					if (bswap_uns32(buffer_size) == image_data_length - 8)
						buffer_size = bswap_uns32(buffer_size);

					if (buffer_size == image_data_length - 8)
					{
						if (c_gui_custom_bitmap_storage_manager::get()->load_bitmap_from_buffer(callback_data->storage_item_index, buffer, buffer_size, callback_data->desired_aspect_ratio))
						{
							callback_data->state = s_load_image_from_file_task::_state_done;
							v2 = true;
							success = true;
						}
					}
				}
			}
		}
	}
	break;
	}

	*callback_data->success = success;

	if (v2 && !success)
		return false;

	if (callback_data->image_source_was_dlc)
		content_catalogue_close_all_dlc(true);

	return true;
}

