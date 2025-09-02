#include "render_methods/render_method_definitions.hpp"

// 019102C8
const char* macro_entry_point_names[k_number_of_entry_points]
{
    "default",
    "albedo",
    "static_default",
    "static_per_pixel",
    "static_per_vertex",
    "static_sh",
    "static_prt_ambient",
    "static_prt_linear",
    "static_prt_quadratic",
    "dynamic_light",
    "shadow_generate",
    "shadow_apply",
    "active_camo",
    "lightmap_debug_mode",
    "static_per_vertex_color",
    "water_tessellation",
    "water_shading",
    "dynamic_light_cinematic",
    "z_only",
    "sfx_distort",
};

// 01910318
const char* macro_pixel_entry_point_names[k_number_of_entry_points]
{
    "default_ps",
    "albedo_ps",
    "static_default_ps",
    "static_per_pixel_ps",
    "static_per_vertex_ps",
    "static_sh_ps",
    "static_prt_ps",
    "static_prt_ps",
    "static_prt_ps",
    "dynamic_light_ps",
    "shadow_generate_ps",
    "shadow_apply_ps",
    "active_camo_ps",
    "lightmap_debug_mode_ps",
    "static_per_vertex_color_ps",
    "water_tessellation_ps",
    "water_shading_ps",
    "dynamic_light_cine_ps",
    "z_only_ps",
    "sfx_distort_ps",
};

// 01910368
const char* macro_vertex_entry_point_names[k_number_of_entry_points]
{
    "default_vs",
    "albedo_vs",
    "static_default_vs",
    "static_per_pixel_vs",
    "static_per_vertex_vs",
    "static_sh_vs",
    "static_prt_ambient_vs",
    "static_prt_linear_vs",
    "static_prt_quadratic_vs",
    "dynamic_light_vs",
    "shadow_generate_vs",
    "shadow_apply_vs",
    "active_camo_vs",
    "lightmap_debug_mode_vs",
    "static_per_vertex_color_vs",
    "water_tessellation_vs",
    "water_shading_vs",
    "dynamic_light_cine_vs",
    "z_only_vs",
    "sfx_distort_vs",
};

// 019103B8
const char* macro_vertex_type_names[k_number_of_vertex_types]
{
    "world",
    "rigid",
    "skinned",
    "particle_model",
    "flat_world",
    "flat_rigid",
    "flat_skinned",
    "screen",
    "debug",
    "transparent",
    "particle",
    "contrail",
    "light_volume",
    "chud_simple",
    "chud_fancy",
    "decorator",
    "tiny_position",
    "patchy_fog",
    "water",
    "ripple",
    "implicit",
    "beam",
    "skinned_dual_quat_blend",
};

// 01910418
const char* macro_hlsl_vertex_type_names[k_number_of_vertex_types]
{
    "s_world_vertex",
    "s_rigid_vertex",
    "s_skinned_vertex",
    "s_particle_model_vertex",
    "s_flat_world_vertex",
    "s_flat_rigid_vertex",
    "s_flat_skinned_vertex",
    "s_screen_vertex",
    "s_debug_vertex",
    "s_transparent_vertex",
    "s_particle_vertex",
    "s_contrail_vertex",
    "s_light_volume_vertex",
    "s_chud_vertex_simple",
    "s_chud_vertex_fancy",
    "s_decorator_vertex",
    "s_tiny_position_vertex",
    "s_patchy_fog_vertex",
    "s_water_vertex",
    "s_ripple_vertex",
    "s_implicit_vertex",
    "s_beam_vertex",
    "s_dual_quat_vertex",
};

// 01910478
const char* macro_vertex_deformer_names[k_number_of_vertex_types]
{
    "deform_world",
    "deform_rigid",
    "deform_skinned",
    "deform_particle_model",
    "deform_flat_world",
    "deform_flat_rigid",
    "deform_flat_skinned",
    "deform_screen",
    "deform_debug",
    "deform_transparent",
    "deform_particle",
    "deform_contrail",
    "deform_light_volume",
    "deform_chud_simple",
    "deform_chud_fancy",
    "deform_decorator",
    "deform_tiny_position",
    "deform_patchy_fog",
    "deform_water",
    "deform_vertex",
    "deform_implicit",
    "deform_beam",
    "deform_skinned_dual_quat_blend",
};

