#include "interface/gui_custom_bitmap_widget.hpp"

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

void __fastcall c_gui_custom_bitmap_widget::set_map_image(c_gui_custom_bitmap_widget* _this, void* unused, e_custom_map_image_type image_type, long map_id, bool allocate_bitmap)
{
    static c_static_string<256> map_image_path;
    map_image_path.clear();

    if (get_map_filename(image_type, map_id, &map_image_path))
        _this->load_from_file_async(allocate_bitmap, map_image_path.get_string());
    else
        _this->clear();
}

void __cdecl c_gui_custom_bitmap_widget::load_from_file_async(bool allocate_bitmap, char const* file_path)
{
    m_path.set(file_path);
    m_allocate_bitmap = allocate_bitmap;
    __unknown268 = 0;
}

void __cdecl c_gui_custom_bitmap_widget::clear()
{
    m_path.clear();
}

long __cdecl map_image_load_callback(void* callback_data)
{
    long result = 0;
    HOOK_INVOKE(result =, map_image_load_callback, callback_data);
    return result;
}
