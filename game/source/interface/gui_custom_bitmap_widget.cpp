#include "interface/gui_custom_bitmap_widget.hpp"

#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"

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

// #TODO: reimplement `map_image_load_callback`
// after implementing `c_gui_custom_bitmap_storage_item` this function is now having access violation issues see `ENABLE_LOAD_FROM_BUFFER_HOOK`
long __cdecl map_image_load_callback(s_map_image_load_callback_data* callback_data)
{
    long result = 0;
    HOOK_INVOKE(result =, map_image_load_callback, callback_data);
    return result;
}

char const* __cdecl network_blf_buffer_reader_find_chunk(char const* buffer, long buffer_size, long signature, long chunk_version, long authentication_type, long* out_chunk_size)
{
    // #TODO: possibly replace this call with one that returns if the chunk should be byte-swapped
    char const* chunk = INVOKE(0x00462B40, network_blf_buffer_reader_find_chunk, buffer, buffer_size, signature, chunk_version, authentication_type, out_chunk_size);

    if (chunk)
    {
        if (*out_chunk_size > 8)
        {
            if (*chunk >= 0 && *chunk < 2)
            {
                long const& chunk_read_size = *reinterpret_cast<long const*>(chunk + 4);

                if (chunk_read_size == *out_chunk_size - 8)
                    return chunk;

                if (_byteswap_ulong(chunk_read_size) == *out_chunk_size - 8)
                {
                    // this is bad but necessary
                    const_cast<long&>(chunk_read_size) = _byteswap_ulong(chunk_read_size);
                }
            }
        }
    }

    return chunk;
}
HOOK_DECLARE_CALL(0x00AC3C73, network_blf_buffer_reader_find_chunk);

