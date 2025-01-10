#pragma once

enum e_window_index;
union rectangle2d;

extern void editor_render_debug();
extern void editor_render_scene(long a1);
extern void editor_render_ui(e_window_index window_index, rectangle2d const* bounds);

