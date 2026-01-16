#pragma once

#include "tag_files/tag_groups.hpp"

class c_render_method
{
public:
    enum
    {
        k_tag_type = RENDER_METHOD_TAG,
        k_maximum_parameters = 64,
        k_maximum_postprocess_textures = 64,
    };

    enum
    {
        k_shader_flag_atmosphere_fog = 0,
        k_shader_flag_atmosphere_custom_settings,
        k_shader_flag_calculate_Z_camera_bit,
        k_number_of_shader_flags,
    };

    s_tag_reference m_definition;
    s_tag_block m_options;
    s_tag_block m_parameters;
    s_tag_block m_postprocess_definition;
    uns16 m_flags;
    uns8 m_sort_layer;
    byte m_pad;
    int32 m_custom_setting_index;
    int32 m_predicted_resource_atom;
};
COMPILE_ASSERT(sizeof(c_render_method) == 0x40);

// what does `macro` mean in this context?

extern const char* macro_entry_point_names[k_number_of_entry_points];
extern const char* macro_pixel_entry_point_names[k_number_of_entry_points];
extern const char* macro_vertex_entry_point_names[k_number_of_entry_points];
extern const char* macro_vertex_type_names[k_number_of_vertex_types];
extern const char* macro_hlsl_vertex_type_names[k_number_of_vertex_types];
extern const char* macro_vertex_deformer_names[k_number_of_vertex_types];

