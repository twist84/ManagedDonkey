#include "render/render_cameras.hpp"

#include "camera/observer.hpp"
#include "cutscene/cinematics.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer_globals.hpp"

HOOK_DECLARE(0x00A65E30, render_projection_sphere_diameter_in_pixels);

void __cdecl render_camera_build(render_camera* camera, s_observer_result const* result)
{
	INVOKE(0x00A63A00, render_camera_build, camera, result);
}

bool __cdecl render_camera_build_clipped_frustum_bounds(render_camera const* camera, real_rectangle2d const* clip, real_rectangle2d* frustum_bounds)
{
	ASSERT(camera);
	ASSERT(clip);
	ASSERT(frustum_bounds);

	return INVOKE(0x00A63B50, render_camera_build_clipped_frustum_bounds, camera, clip, frustum_bounds);
}

void __cdecl render_camera_build_orthogonal_projection(s_oriented_bounding_box const* camera, short_rectangle2d const* window_display_bounds, struct render_projection* projection, bool a4)
{
	ASSERT(camera);
	ASSERT(projection);

	INVOKE(0x00A63C70, render_camera_build_orthogonal_projection, camera, window_display_bounds, projection, a4);
}

void __cdecl render_camera_build_projection(render_camera const* camera, real_rectangle2d const* frustum_bounds, render_projection* projection, real a4)
{
	ASSERT(camera);
	ASSERT(projection);

	INVOKE(0x00A64140, render_camera_build_projection, camera, frustum_bounds, projection, a4);
}

void __cdecl render_camera_build_view_parameters(render_camera const* camera, real_rectangle2d const* frustum_bounds, render_view_parameters* parameters, real a4)
{
	ASSERT(camera);
	ASSERT(parameters);

	INVOKE(0x00A64780, render_camera_build_view_parameters, camera, frustum_bounds, parameters, a4);
}

void __cdecl render_camera_build_viewport_frustum_bounds(render_camera const* camera, real_rectangle2d* frustum_bounds)
{
	ASSERT(camera);
	ASSERT(frustum_bounds);
	ASSERT(camera->render_pixel_bounds.x0 < camera->render_pixel_bounds.x1);
	ASSERT(camera->render_pixel_bounds.y0 < camera->render_pixel_bounds.y1);

	INVOKE(0x00A64A50, render_camera_build_viewport_frustum_bounds, camera, frustum_bounds);
}

void __cdecl render_camera_mirror(render_camera const* camera, render_mirror const* mirror, render_camera* result)
{
	ASSERT(camera->render_pixel_bounds.x0 == 0);
	ASSERT(camera->render_pixel_bounds.y0 == 0);

	INVOKE(0x00A64CB0, render_camera_mirror, camera, mirror, result);
}

bool __cdecl render_camera_project_vector_to_screen(vector3d const* world_vector, real_point3d const* world_point, real_matrix4x3 const* matrix, vector2d const* a4, bool use_perspective, bool a6, vector2d* result)
{
	ASSERT(world_vector);
	ASSERT(world_point || !use_perspective);
	ASSERT(result);

	return INVOKE(0x00A651C0, render_camera_project_vector_to_screen, world_vector, world_point, matrix, a4, use_perspective, a6, result);
}

void __cdecl render_camera_screen_to_view(render_camera const* camera, render_projection const* projection, short_rectangle2d const* window_display_bounds, real_point2d const* screen_point, vector3d* view_vector)
{
	ASSERT(camera);
	ASSERT(projection);
	ASSERT(screen_point);
	ASSERT(view_vector);

	INVOKE(0x00A653E0, render_camera_screen_to_view, camera, projection, window_display_bounds, screen_point, view_vector);
}

void __cdecl render_camera_screen_to_world(render_camera const* camera, render_projection const* projection, real_point2d const* screen_point, real_point3d* world_point, vector3d* world_vector)
{
	ASSERT(camera);
	ASSERT(projection);
	ASSERT(screen_point);
	ASSERT(world_point);
	ASSERT(world_vector);

	INVOKE(0x00A654C0, render_camera_screen_to_world, camera, projection, screen_point, world_point, world_vector);

	//vector3d view_vector{};
	//render_camera_screen_to_view(camera, projection, 0, screen_point, &view_vector);
	//*world_point = camera->position;
	//matrix4x3_transform_vector(&projection->view_to_world, &view_vector, world_vector);
	//normalize3d(world_vector);
}

bool __cdecl render_camera_view_to_screen(render_camera const* camera, render_projection const* projection, short_rectangle2d const* window_display_bounds, real_point3d const* view_point, real_point2d* screen_point)
{
	ASSERT(camera);
	ASSERT(projection);
	ASSERT(view_point);
	ASSERT(screen_point);

	return INVOKE(0x00A65650, render_camera_view_to_screen, camera, projection, window_display_bounds, view_point, screen_point);
}

void __cdecl render_camera_world_sphere_to_screen_extents(render_camera const* camera, render_projection const* projection, real_point3d const* world_center, real a4, real_rectangle2d* screen_extents)
{
	ASSERT(camera);
	ASSERT(projection);
	ASSERT(world_center);
	ASSERT(screen_extents);

	INVOKE(0x00A657A0, render_camera_world_sphere_to_screen_extents, camera, projection, world_center, a4, screen_extents);
}

bool __cdecl render_camera_world_to_screen(render_camera const* camera, render_projection const* projection, real_point3d const* world_point, real_point2d* screen_point)
{
	ASSERT(camera);
	ASSERT(projection);
	ASSERT(world_point);
	ASSERT(screen_point);

	return INVOKE(0x00A65BE0, render_camera_world_to_screen, camera, projection, world_point, screen_point);

	//real_point3d point{};
	//matrix4x3_transform_point(&projection->world_to_view, world_point, &point);
	//return render_camera_view_to_screen(camera, projection, nullptr, &point, screen_point);
}

bool __cdecl render_camera_world_to_window(render_camera const* camera, render_projection const* projection, short_rectangle2d const* window_display_bounds, real_point3d const* world_point, real_point2d* screen_point)
{
	ASSERT(camera);
	ASSERT(projection);
	ASSERT(window_display_bounds);
	ASSERT(world_point);
	ASSERT(screen_point);

	return INVOKE(0x00A65C10, render_camera_world_to_window, camera, projection, window_display_bounds, world_point, screen_point);

	//real_point3d point{};
	//matrix4x3_transform_point(&projection->world_to_view, world_point, &point);
	//return render_camera_view_to_screen(camera, projection, window_display_bounds, &point, screen_point);
}

real __cdecl render_projection_sphere_diameter_in_pixels(render_projection const* projection, real_point3d const* point, real scale)
{
	//return INVOKE(0x00A65E30, render_projection_sphere_diameter_in_pixels, projection, point, scale);

	ASSERT(projection);
	ASSERT(point);

	real v0 = fabsf(((((projection->world_to_view.matrix.left.k * point->y) + (projection->world_to_view.matrix.forward.k * point->x)) + (projection->world_to_view.matrix.up.k * point->z)) + projection->world_to_view.center.z));

	if (v0 <= 0.1f)
		v0 = 0.1f;

	real v1 = projection->__unknownB8.j;
	if (cinematic_in_progress() && !rasterizer_get_is_widescreen())
		v1 *= real(4 / 3);

	return real(((scale * 10.0f /* default: 2.0f */) * v1) / v0);
}

void __cdecl render_view_compute_all_bounds(long player_index, long player_count, render_camera* camera)
{
	INVOKE(0x00A65EE0, render_view_compute_all_bounds, player_index, player_count, camera);

}

void __cdecl render_view_compute_fullscreen_bounds(render_camera* camera)
{
	INVOKE(0x00A65F40, render_view_compute_fullscreen_bounds, camera);
}

void __cdecl render_view_compute_window_bounds(long player_index, long player_count, short_rectangle2d* bounds, short_rectangle2d* safe_bounds)
{
	INVOKE(0x00A65F90, render_view_compute_window_bounds, player_index, player_count, bounds, safe_bounds);
}

