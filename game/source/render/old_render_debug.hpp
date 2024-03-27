#pragma once

#include "math/real_math.hpp"

extern bool debug_structure_markers;
extern bool debug_structure_surface_references;
extern bool debug_structure_soft_ceilings;
extern bool debug_structure_soft_kill;
extern bool debug_structure_slip_surfaces;
extern bool debug_structure_cluster_skies;
extern bool debug_structure_invisible;
extern bool debug_structure;
extern bool debug_structure_automatic;
extern long debug_plane_index;
extern bool debug_structure_unique_colors;
extern bool debug_structure_complexity;
extern bool debug_structure_seam_edges;
extern bool debug_instanced_geometry;
extern bool debug_instanced_geometry_bounding_spheres;
extern bool debug_instanced_geometry_names;
extern bool debug_instanced_geometry_vertex_counts;
extern bool debug_instanced_geometry_collision_geometry;
extern bool debug_zone_set_critical_portals;
extern bool debug_structure_water;
extern bool debug_input;
extern bool debug_input_abstraction;
extern bool debug_input_mouse_state;
extern bool debug_player;
extern bool debug_render_freeze;
extern bool debug_camera;
extern bool debug_tangent_space;
extern bool debug_permanent_decals;

extern void render_debug_input();
extern void render_debug_player();
extern void render_debug_camera();
extern void render_debug_bsp();
extern void render_debug_structure_decals();
extern bool __cdecl render_debug_lost_camera();
extern real_rgb_color* __cdecl set_real_rgb_color(real_rgb_color* color, real red, real green, real blue);
extern real_argb_color* __cdecl set_real_argb_color(real_argb_color* color, real alpha, real red, real green, real blue);

