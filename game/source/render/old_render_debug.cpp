#include "render/old_render_debug.hpp"

#include "render/views/render_view.hpp"

bool debug_structure_markers = false;
bool debug_structure_surface_references = false;
bool debug_structure_soft_ceilings = false;
bool debug_structure_soft_kill = false;
bool debug_structure_slip_surfaces = false;
bool debug_structure_cluster_skies = false;
bool debug_structure_invisible = false;
bool debug_structure = false;
bool debug_structure_automatic = false;
long debug_plane_index = NONE;
bool debug_structure_unique_colors = false;
bool debug_structure_complexity = false;
bool debug_structure_seam_edges = false;
bool debug_instanced_geometry = false;
bool debug_instanced_geometry_bounding_spheres = false;
bool debug_instanced_geometry_names = false;
bool debug_instanced_geometry_vertex_counts = false;
bool debug_instanced_geometry_collision_geometry = false;
bool debug_zone_set_critical_portals = false;
bool debug_structure_water = false;

bool __cdecl render_debug_lost_camera()
{
    return c_player_view::get_global_player_view()->m_location.cluster_reference.bsp_index;
}

real_rgb_color* __cdecl set_real_rgb_color(real_rgb_color* color, real red, real green, real blue)
{
    ASSERT(color);

    color->red = red;
    color->green = green;
    color->blue = blue;

    return color;
}

real_argb_color* __cdecl set_real_argb_color(real_argb_color* color, real alpha, real red, real green, real blue)
{
    ASSERT(color);

    color->alpha = alpha;
    set_real_rgb_color(&color->color, red, green, blue);

    return color;
}

