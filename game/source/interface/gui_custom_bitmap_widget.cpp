#include "interface/gui_custom_bitmap_widget.hpp"

#include "interface/gui_custom_bitmap_storage.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"
#include "networking/tools/network_blf.hpp"

HOOK_DECLARE_CLASS(0x00AC3900, c_gui_custom_bitmap_widget, get_map_filename);
HOOK_DECLARE_CLASS(0x00AC3DE0, c_gui_custom_bitmap_widget, set_map_image);
HOOK_DECLARE(0x00AC3B80, map_image_load_callback);

bool __cdecl c_gui_custom_bitmap_widget::get_map_filename(e_custom_map_image_type type, long map_id, c_static_string<256>* out_filename)
{
    bool result = false;
    HOOK_INVOKE_CLASS(result =, c_gui_custom_bitmap_widget, get_map_filename, decltype(get_map_filename)*, type, map_id, out_filename);
    return result;
}

void __fastcall c_gui_custom_bitmap_widget::set_map_image(c_gui_custom_bitmap_widget* _this, void* unused, e_custom_map_image_type image_type, long map_id, bool use_compressed_format)
{
    static c_static_string<256> map_image_path;
    map_image_path.clear();

    if (get_map_filename(image_type, map_id, &map_image_path))
        _this->load_from_file_async(use_compressed_format, map_image_path.get_string());
    else
        _this->clear();
}

void __cdecl c_gui_custom_bitmap_widget::load_from_file_async(bool use_compressed_format, char const* file_path)
{
    m_path.set(file_path);
    m_use_compressed_format = use_compressed_format;
    __unknown268 = 0;
}

void __cdecl c_gui_custom_bitmap_widget::clear()
{
    m_path.clear();
}

long __cdecl map_image_load_callback(s_map_image_load_callback_data* callback_data)
{
    c_static_wchar_string<256> name_buffer;

    bool v2 = false;
    bool v3 = false;
    long v4 = callback_data->__unknown1C->peek();
    bool v5 = v4 != 0;

    switch (callback_data->async_load_stage)
    {
    case 0: // get file size
    {
        if (!v5)
        {
            constexpr long name_flags = FLAG(_name_directory_bit) | FLAG(_name_extension_bit) | FLAG(_name_file_bit);
            wchar_t* name = file_reference_get_name(callback_data->async_load_file, name_flags, &name_buffer, 256);
            
            callback_data->__unknown24 = DECLFUNC(0x005A5990, bool, __cdecl, wchar_t const*, long)(name, 1);// levels_dlc_open(name, 1);

            dword error = 0;
            if (file_open(callback_data->async_load_file, FLAG(_file_open_flag_desired_access_read), &error))
            {
                if (file_get_size(callback_data->async_load_file, &callback_data->async_load_file_size)
                    && callback_data->async_load_file_size < callback_data->async_load_buffer_size)
                {
                    callback_data->async_load_stage = 1;
                    v2 = true;

                    break;
                }

                file_close(callback_data->async_load_file);
            }
        }
    }
    break;
    case 1: // read file info memory
    {
        if (!v5 && file_read(callback_data->async_load_file, callback_data->async_load_file_size, FLAG(_file_open_flag_desired_access_read), callback_data->async_load_buffer))
        {
            file_close(callback_data->async_load_file);

            callback_data->async_load_stage = 2;
            v2 = true;

            break;
        }
        // left over from `case 0`
        file_close(callback_data->async_load_file);
    }
    break;
    case 2: // find chunk and load bitmap
    {
        if (!v5)
        {
            long chunk_size = 0;
            // c_network_blf_buffer_reader::find_chunk(async_load_buffer, async_load_file_size, s_blf_chunk_map_image::k_chunk_type, s_blf_chunk_map_image::k_version_major, _blf_file_authentication_type_rsa, &chunk_size);
            char const* chunk = DECLFUNC(0x00462B40, char const*, __cdecl, char const*, long, long, long, long, long*)(
                callback_data->async_load_buffer,
                callback_data->async_load_file_size,
                s_blf_chunk_map_image::k_chunk_type,
                s_blf_chunk_map_image::k_version_major,
                _blf_file_authentication_type_rsa,
                &chunk_size);

            if (chunk)
            {
                if (chunk_size > 8 && *chunk >= 0 && *chunk < 2)
                {
                    long buffer_size = *reinterpret_cast<long const*>(chunk + 4);
                    char const* buffer = reinterpret_cast<char const*>(chunk + 8);

                    // hack
                    if (_byteswap_ulong(buffer_size) == chunk_size - 8)
                        buffer_size = _byteswap_ulong(buffer_size);

                    if (buffer_size == chunk_size - 8)
                    {
                        if (c_gui_custom_bitmap_storage_manager::get()->load_bitmap_from_buffer(callback_data->bitmap_storage_item_index, buffer, buffer_size, callback_data->__unknown18))
                        {
                            callback_data->async_load_stage = 3;
                            v2 = true;
                            v3 = true;
                        }
                    }
                }
            }
        }
    }
    break;
    }

    *callback_data->__unknown20 = v3;

    if (v2 && !v3)
        return false;

    if (callback_data->__unknown24)
        DECLFUNC(0x005A52A0, void, __cdecl, bool)(true);// sub_5A52A0(true);
       
    return true;
}

