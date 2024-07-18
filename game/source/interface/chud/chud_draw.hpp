#pragma once

#include "cseries/cseries.hpp"

extern bool& chud_enabled;
extern bool& chud_debug_messages;
extern bool& chud_debug_grid;
extern bool& chud_debug_crosshair;

enum e_output_user_index;

extern void __cdecl chud_debug_draw(e_output_user_index output_user_index);
extern void __cdecl chud_debug_draw_reticle(e_output_user_index output_user_index, real a2, real_argb_color const* color);
extern void __cdecl chud_debug_draw_reticle_labeled(e_output_user_index output_user_index, real a2, char const* label, real a4, real_argb_color const* color);
extern void __cdecl chud_debug_render();

