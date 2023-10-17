#pragma once

enum e_window_index;
union short_rectangle2d;

extern void editor_render_debug();
extern void editor_render_scene(long a1);
extern void editor_render_ui(e_window_index window_index, short_rectangle2d const* bounds);

