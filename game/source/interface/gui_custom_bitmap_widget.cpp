#include "interface/gui_custom_bitmap_widget.hpp"

#include "memory/module.hpp"

HOOK_DECLARE_CLASS(0x00AC3900, c_gui_custom_bitmap_widget, get_map_filename);

bool __cdecl c_gui_custom_bitmap_widget::get_map_filename(e_custom_map_image_type type, long map_id, c_static_string<256>* out_filename)
{
    bool result = false;
    HOOK_INVOKE_CLASS(result =, c_gui_custom_bitmap_widget, get_map_filename, decltype(get_map_filename)*, type, map_id, out_filename);
    return result;
}
