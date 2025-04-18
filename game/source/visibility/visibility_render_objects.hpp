#pragma once

#include "cseries/cseries.hpp"

struct s_lod_transparency;

extern void __cdecl visibility_render_objects_compute_lod_transparency(long object_index, real shadow_fade_distance, s_lod_transparency* desired_alpha);

