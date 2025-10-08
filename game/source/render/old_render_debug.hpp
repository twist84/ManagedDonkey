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
extern int32 debug_structure_cluster_structure_bsp_index;
extern int32 debug_structure_cluster_cluster_index;
extern int32 debug_plane_index;
extern int32 debug_surface_index;
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

extern void debug_structure_cluster(int32 structure_bsp_index, int32 cluster_index);
extern void render_debug_bsp();
extern void render_debug_camera();
extern void render_debug_input();
extern bool render_debug_lost_camera();
extern void render_debug_player();
extern void render_debug_structure_decals();

extern real_rgb_color* __cdecl set_real_rgb_color(real_rgb_color* color, real32 red, real32 green, real32 blue);
extern real_argb_color* __cdecl set_real_argb_color(real_argb_color* color, real32 alpha, real32 red, real32 green, real32 blue);

