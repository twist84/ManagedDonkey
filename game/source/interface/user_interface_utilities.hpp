#pragma once

#include "cseries/cseries.hpp"

struct c_gui_widget;
extern void tint_widget_to_change_color(c_gui_widget* widget, long color_list_index, bool team_game);
extern void tint_widget_to_change_argb_color(c_gui_widget* widget, argb_color color);

