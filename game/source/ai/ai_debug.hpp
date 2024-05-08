#pragma once

#include "cseries/cseries.hpp"

extern real_point3d global_ai_debug_drawstack_next_position;
extern real global_ai_debug_drawstack_height;
extern real_point3d global_ai_debug_drawstack_last_position;

extern void ai_debug_drawstack_setup(real_point3d const* position);
extern real_point3d* ai_debug_drawstack();
extern real_point3d* ai_debug_drawstack_offset(real offset);

extern bool ai_render_dialogue_variants_enabled;

extern void __cdecl ai_debug_render();

