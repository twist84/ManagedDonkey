#include "render/render_cameras.hpp"

#include "camera/observer.hpp"

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

void __cdecl render_view_compute_fullscreen_bounds(render_camera* camera)
{
	INVOKE(0x00A65F40, render_view_compute_fullscreen_bounds, camera);
}

