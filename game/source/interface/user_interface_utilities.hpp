#pragma once

#include "text/unicode.hpp"

struct c_gui_widget;
union argb_color;

extern void __cdecl tint_widget_to_change_color(c_gui_widget* widget, long change_color_index, bool change_color_is_team_color);
extern void __cdecl tint_widget_to_change_argb_color(c_gui_widget* widget, argb_color color);
extern bool __cdecl user_interface_global_string_get(long id, c_static_wchar_string<1024>* text);
extern bool __cdecl user_interface_play_sound(e_user_interface_sound_effect ui_sound_effect, long ui_sound_effect_definition_index);

