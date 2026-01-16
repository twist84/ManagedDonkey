#pragma once

#include "cseries/cseries.hpp"

enum e_texture_camera_render_mode
{
    _texture_camera_render_default = 0,
    _texture_camera_render_target_only,

    k_texture_camera_render_mode_count
};

enum e_texture_camera_type
{
    _texture_camera_type_marker_id = 0,
    _texture_camera_type_position,

    k_texture_camera_type_count
};

enum e_texture_camera_orientation_type
{
    _texture_camera_orientation_target_auto = 0,
    _texture_camera_orientation_position_marker,
    _texture_camera_orientation_target_marker,
    _texture_camera_orientation_explicit,
    _texture_camera_orientation_attached_object_orient,

    k_texture_camera_orientation_type_count
};

enum e_texture_camera_projection_type
{
    _texture_camera_projection_projective = 0,

    k_texture_camera_projection_type_count
};

enum e_texture_camera_fov_type
{
    _texture_camera_fov_fixed = 0,
    _texture_camera_fov_frame_target,

    k_texture_camera_fov_type_count
};

struct s_render_texture_camera_globals
{
    bool active;
    bool dynamic_lights;
    e_texture_camera_render_mode render_mode;
    e_texture_camera_type position_type;
    e_texture_camera_type target_type;
    e_texture_camera_orientation_type orientation_type;
    e_texture_camera_projection_type projection_type;
    e_texture_camera_fov_type fov_type;

    union
    {
        struct
        {
            int32 object_index;
            int32 marker_id;
        };

        real_point3d position;

    } position;

    real_vector3d position_offset;

    union
    {
        struct
        {
            int32 object_index;
            int32 marker_id;
        };

        real_point3d position;

    } target;

    int32 __unknown40_object_index;

    union
    {
        struct
        {
            real_vector3d forward;
            real_vector3d up;
        };
    } orientation;

    union
    {
        real32 vertical_fov_radians;
        real32 target_scale;

    } fov;

    real32 aspect_ratio;
    int32 width;
    int32 height;
};
COMPILE_ASSERT(sizeof(s_render_texture_camera_globals) == 0x6C);

