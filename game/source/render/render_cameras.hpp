#pragma once

#include "cseries/cseries.hpp"

struct render_camera
{
	real_point3d position;
	vector3d forward;
	vector3d up;

	// mirrored cameras do not work
	bool mirrored;

	// here <<< H3EK has 8 bytes of something here, structure size increased to 0x90

	real vertical_field_of_view;
	real horizontal_field_of_view;

	// or here <<< H3EK has 8 bytes of something here, structure size increased to 0x90

	short_rectangle2d window_pixel_bounds;
	short_rectangle2d window_title_safe_pixel_bounds;
	short __unknown40;
	short __unknown42;
	short_rectangle2d render_pixel_bounds;
	short_rectangle2d render_title_safe_pixel_bounds;
	short_rectangle2d display_pixel_bounds;
	real z_near;
	real z_far;
	plane3d mirror_plane;

	// used in `render_camera_build_viewport_frustum_bounds`
	// these values are paired together
	bool __unknown74;
	real_rectangle2d __unknown78;
};
static_assert(sizeof(render_camera) == 0x88);

struct s_oriented_bounding_box
{
	real matrix[4][4];
};
static_assert(sizeof(s_oriented_bounding_box) == sizeof(real) * 4 * 4);

struct render_projection
{
	real_matrix4x3 world_to_view;
	real_matrix4x3 view_to_world;
	real_rectangle2d projection_bounds;
	s_oriented_bounding_box projection_matrix;
	vector2d __unknownB8;
};
static_assert(sizeof(render_projection) == 0xC0);

struct render_view_parameters
{
	real_rectangle2d frustum_bounds;
	real width;
	real hight;
	real __unknown18;
	real __unknown1C;
	real __unknown20;
	real __unknown24;
	real __unknown28;
	real __unknown2C;
	real_rectangle2d projection_bounds;
};
static_assert(sizeof(render_view_parameters) == 0x40);

extern bool debug_camera_projection;
extern bool debug_static_first_person;

extern real render_debug_aspect_ratio_scale;
extern bool g_reduce_widescreen_fov_during_cinematics;

struct s_observer_result;
struct render_mirror;
extern void __cdecl render_camera_build(render_camera* camera, s_observer_result const* result);
extern bool __cdecl render_camera_build_clipped_frustum_bounds(render_camera const* camera, real_rectangle2d const* clip, real_rectangle2d* frustum_bounds);
extern void __cdecl render_camera_build_orthogonal_projection(s_oriented_bounding_box const* camera, short_rectangle2d const* window_display_bounds, struct render_projection* projection, bool a4);
extern void __cdecl render_camera_build_projection(render_camera const* camera, real_rectangle2d const* frustum_bounds, render_projection* projection, real aspect_ratio);
extern void __cdecl render_camera_build_view_parameters(render_camera const* camera, real_rectangle2d const* frustum_bounds, render_view_parameters* parameters, real aspect_ratio);
extern void __cdecl render_camera_build_viewport_frustum_bounds(render_camera const* camera, real_rectangle2d* frustum_bounds);
extern void __cdecl render_camera_mirror(render_camera const* camera, render_mirror const* mirror, render_camera* result);
extern bool __cdecl render_camera_project_vector_to_screen(vector3d const* world_vector, real_point3d const* world_point, real_matrix4x3 const* matrix, vector2d const* a4, bool use_perspective, bool a6, vector2d* result);
extern void __cdecl render_camera_screen_to_view(render_camera const* camera, render_projection const* projection, short_rectangle2d const* window_display_bounds, real_point2d const* screen_point, vector3d* view_vector);
extern void __cdecl render_camera_screen_to_world(render_camera const* camera, render_projection const* projection, real_point2d const* screen_point, real_point3d* world_point, vector3d* world_vector);
extern bool __cdecl render_camera_view_to_screen(render_camera const* camera, render_projection const* projection, short_rectangle2d const* window_display_bounds, real_point3d const* world_point, real_point2d* screen_point);
extern void __cdecl render_camera_world_sphere_to_screen_extents(render_camera const* camera, render_projection const* projection, real_point3d const* world_center, real a4, real_rectangle2d* screen_extents);
extern bool __cdecl render_camera_world_to_screen(render_camera const* camera, render_projection const* projection, real_point3d const* world_point, real_point2d* screen_point);
extern bool __cdecl render_camera_world_to_window(render_camera const* camera, render_projection const* projection, short_rectangle2d const* window_display_bounds, real_point3d const* world_point, real_point2d* screen_point);
extern real __cdecl render_projection_sphere_diameter_in_pixels(render_projection const* projection, real_point3d const* point, real scale);
extern void __cdecl render_view_compute_all_bounds(long player_index, long player_count, render_camera* camera);
extern void __cdecl render_view_compute_fullscreen_bounds(render_camera* camera);
extern void __cdecl render_view_compute_window_bounds(long player_index, long player_count, short_rectangle2d* bounds, short_rectangle2d* safe_bounds);
extern void render_debug_camera_projection();

