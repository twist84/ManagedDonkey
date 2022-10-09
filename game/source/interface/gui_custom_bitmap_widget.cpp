#include "interface/gui_custom_bitmap_widget.hpp"

#include "memory/module.hpp"

HOOK_DECLARE_CLASS(0x00AC3900, c_gui_custom_bitmap_widget, get_map_filename);
HOOK_DECLARE(0x00AC3B80, map_image_load_callback);

bool __cdecl c_gui_custom_bitmap_widget::get_map_filename(e_custom_map_image_type type, long map_id, c_static_string<256>* out_filename)
{
    bool result = false;
    HOOK_INVOKE_CLASS(result =, c_gui_custom_bitmap_widget, get_map_filename, decltype(get_map_filename)*, type, map_id, out_filename);
    return result;
}

long __cdecl map_image_load_callback(void* callback_data)
{
    long result = 0;
    HOOK_INVOKE(result =, map_image_load_callback, callback_data);
    return result;
}