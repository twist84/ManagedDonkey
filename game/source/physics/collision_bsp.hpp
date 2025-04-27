#pragma once

#include "cseries/cseries.hpp"
#include "physics/collision_bsp_references.hpp"

extern void render_debug_collision_surface(c_collision_bsp_reference bsp_reference, int32 surface_index, real_matrix4x3 const* matrix, real_argb_color const* color);
extern void render_debug_collision_edge(c_collision_bsp_reference bsp_reference, int32 edge_index, real_matrix4x3 const* matrix, real_argb_color const* color);

