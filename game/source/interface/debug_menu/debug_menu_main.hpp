#pragma once

#include "cseries/cseries.hpp"

#define DEBUG_MENU_NUM_GLOBAL_CAPTIONS 8

union real_argb_color;
struct gamepad_state;
class c_debug_menu;

extern real_argb_color const* const debug_real_argb_grey;
extern real_argb_color const* const debug_real_argb_white;
extern real_argb_color const* const debug_real_argb_tv_white;
extern real_argb_color const* const debug_real_argb_tv_blue;
extern real_argb_color const* const debug_real_argb_tv_magenta;
extern real_argb_color const* const debug_real_argb_tv_orange;
extern real_argb_color const* const debug_real_argb_tv_green;

extern bool debug_menu_enabled;
extern bool g_debug_menu_rebuild_request;
extern c_static_stack<int32, 262144> g_debug_menu_stack;

extern void debug_menu_draw_rect(int16 x0, int16 y0, int16 x1, int16 y1, real32 alpha, real_argb_color const* color);
extern bool debug_menu_get_active();
extern void debug_menu_initialize();
extern void debug_menu_dispose();
extern void debug_menu_initialize_for_new_map();
extern void debug_menu_dispose_from_old_map();
extern void debug_menu_update();
extern void debug_menu_open();
extern void debug_menu_close();
extern void render_debug_debug_menu_game();
extern void render_debug_debug_menu();
extern gamepad_state const& debug_menu_get_gamepad_state();
extern gamepad_state const& debug_menu_get_last_gamepad_state();
extern c_debug_menu* debug_menu_get_active_menu();
extern void debug_menu_set_active_menu(c_debug_menu* active_menu, bool dont_open);
extern void debug_menu_set_caption(int16 caption_index, char const* caption);
extern char const* debug_menu_get_caption(int16 caption_index);
extern int32 debug_menu_get_time();
extern real32 debug_menu_get_item_margin();
extern real32 debug_menu_get_item_width();
extern real32 debug_menu_get_item_height();
extern real32 debug_menu_get_title_height();
extern real32 debug_menu_get_item_indent_x();
extern real32 debug_menu_get_item_indent_y();
extern void* debug_menu_malloc(int32 size);
extern void xor_buffers(void* dest, void const* source, int32 count);

