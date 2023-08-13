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

void __cdecl render_camera_build_projection(render_camera const* camera, real_rectangle2d const* frustum_bounds, render_projection* projection, real a4)
{
	INVOKE(0x00A64140, render_camera_build_projection, camera, frustum_bounds, projection, a4);
}

void __cdecl render_camera_build_viewport_frustum_bounds(render_camera const* camera, real_rectangle2d* frustum_bounds)
{
	INVOKE(0x00A64A50, render_camera_build_viewport_frustum_bounds, camera, frustum_bounds);
}

real __cdecl render_projection_sphere_diameter_in_pixels(render_projection const* projection, real_point3d const* point, real scale)
{
	real v0 = fabsf(((((projection->world_to_view.matrix.left.k * point->y) + (projection->world_to_view.matrix.forward.k * point->x)) + (projection->world_to_view.matrix.up.k * point->z)) + projection->world_to_view.center.z));

	if (v0 <= 0.1f)
		v0 = 0.1f;

	real v1 = projection->__unknownBC;
	if (cinematic_in_progress() && !rasterizer_get_is_widescreen())
		v1 *= 1.3329999f;

	return real(((scale * 10.0 /* default: 2.0f */) * v1) / v0);
}

void __cdecl render_view_compute_fullscreen_bounds(render_camera* camera)
{
	INVOKE(0x00A65F40, render_view_compute_fullscreen_bounds, camera);
}

