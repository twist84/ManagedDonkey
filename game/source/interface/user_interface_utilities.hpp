#pragma once

#include "cseries/cseries.hpp"

enum e_user_interface_sound_effect;
struct c_gui_widget;

extern void __cdecl tint_widget_to_change_color(c_gui_widget* widget, long color_list_index, bool team_game);
extern bool __cdecl user_interface_play_sound(e_user_interface_sound_effect ui_sound_effect, long ui_sound_effect_definition_index);
extern void __cdecl tint_widget_to_change_argb_color(c_gui_widget* widget, argb_color color);

