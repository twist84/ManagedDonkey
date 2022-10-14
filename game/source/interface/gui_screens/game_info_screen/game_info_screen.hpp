#pragma once

#include "interface/c_gui_screen_widget.hpp"

struct c_gui_game_info_screen : c_gui_screen_widget
{
};
static_assert(sizeof(c_gui_game_info_screen) == sizeof(c_gui_screen_widget));