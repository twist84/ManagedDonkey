#include "camera/observer.hpp"

#include "memory/thread_local.hpp"
#include "render/render_cameras.hpp"
#include "render/render_debug.hpp"

bool g_debug_observer_render = false;

s_observer* observer_get(long user_index)
{
	ASSERT(user_index >= 0 && user_index < 4);

	if (!get_tls())
		return nullptr;

	TLS_DATA_GET_VALUE_REFERENCE(g_observer_globals);
	return g_observer_globals ? &g_observer_globals->observers[user_index] : nullptr;
}

s_observer_result const* observer_get_camera(e_output_user_index output_user_index)
{
	s_observer* observer = observer_get(output_user_index);
	ASSERT(observer);

	return &observer->result;
}

//e_director_mode __cdecl choose_appropriate_director(e_output_user_index output_user_index)
long __cdecl choose_appropriate_director(e_output_user_index output_user_index)
{
	return INVOKE(0x005916B0, choose_appropriate_director, output_user_index);
}

s_observer_result const* __cdecl observer_try_and_get_camera(e_output_user_index output_user_index)
{
	return INVOKE(0x00613960, observer_try_and_get_camera, output_user_index);
}

void __cdecl observer_update(real world_seconds_elapsed)
{
	INVOKE(0x00613A60, observer_update, world_seconds_elapsed);
}

void __cdecl observer_validate_camera_command(s_observer_command* result)
{
	INVOKE(0x00614D60, observer_validate_camera_command, result);
}

void __cdecl debug_render_observer()
{
	if (!g_debug_observer_render)
		return;

	long active_output_user = player_mapping_first_active_output_user();
	if (active_output_user == NONE)
		return;

	s_observer* observer = observer_get(active_output_user);

	render_camera camera{};
	render_camera_build(&camera, &observer->result);
	render_view_compute_fullscreen_bounds(&camera);

	render_projection projection{};
	render_camera_build_projection(&camera, nullptr, &projection, 0.0f);

	real_point2d screen_points[4]{};
	screen_points[0].x = static_cast<real>(camera.render_pixel_bounds.x0);
	screen_points[1].x = static_cast<real>(camera.render_pixel_bounds.x0);
	screen_points[2].x = static_cast<real>(camera.render_pixel_bounds.x1);
	screen_points[3].x = static_cast<real>(camera.render_pixel_bounds.x1);
	screen_points[0].y = static_cast<real>(camera.render_pixel_bounds.y0);
	screen_points[1].y = static_cast<real>(camera.render_pixel_bounds.y1);
	screen_points[2].y = static_cast<real>(camera.render_pixel_bounds.y1);
	screen_points[3].y = static_cast<real>(camera.render_pixel_bounds.y0);

	real_point3d point{};
	real_point3d points[4]{};
	for (long i = 0; i < NUMBEROF(points); i++)
	{
		vector3d vector{};
		render_camera_screen_to_world(&camera, &projection, &screen_points[i], &point, &vector);

		points[i].x = point.x + vector.i;
		points[i].y = point.y + vector.j;
		points[i].z = point.z + vector.k;

		render_debug_vector(false, &point, &vector, 1.0f, global_real_argb_red);
	}
	render_debug_polygon_edges(points, NUMBEROF(points), global_real_argb_red);

	if (observer->__unknownF6)
		matrix4x3_transform_point(&observer->focus_space, &point, &point);
}

