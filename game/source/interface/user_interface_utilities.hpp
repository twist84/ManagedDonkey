#pragma once

#include "text/unicode.hpp"

class c_gui_widget;
union argb_color;

extern void __cdecl tint_widget_to_change_color(c_gui_widget* widget, int32 change_color_index, bool change_color_is_team_color);
extern void __cdecl tint_widget_to_change_argb_color(c_gui_widget* widget, argb_color color);
extern bool __cdecl user_interface_global_string_get(int32 id, c_static_wchar_string<1024>* text);
extern bool __cdecl user_interface_play_sound(e_user_interface_sound_effect sound, int32 override_sound_tag_index);
extern bool __cdecl user_interface_precache_bitmaps_from_tag(int32 bitmap_tag_index);

