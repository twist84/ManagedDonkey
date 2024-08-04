#include "render/render_cameras.hpp"

#include "camera/observer.hpp"
#include "cutscene/cinematics.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer_globals.hpp"
#include "render/render_debug.hpp"
#include "render/views/render_view.hpp"

#include <math.h>

REFERENCE_DECLARE(0x05115B38, bool, debug_no_frustum_clip);

HOOK_DECLARE(0x00A64140, render_camera_build_projection);
HOOK_DECLARE(0x00A65E30, render_projection_sphere_diameter_in_pixels);

bool debug_camera_projection = false;
bool debug_static_first_person = false;

real render_debug_aspect_ratio_scale = 1.0f;
bool g_reduce_widescreen_fov_during_cinematics = true;

bool __cdecl c_rasterizer::get_is_using_floating_point_depth_buffer()
{
	//return INVOKE(0x00A63980, get_is_using_floating_point_depth_buffer);

	return m_use_floating_point_z_buffer;
}

void __cdecl render_camera_build(render_camera* camera, s_observer_result const* result)
{
	INVOKE(0x00A63A00, render_camera_build, camera, result);
}

bool __cdecl render_camera_build_clipped_frustum_bounds(render_camera const* camera, real_rectangle2d const* clip, real_rectangle2d* frustum_bounds)
{
	return INVOKE(0x00A63B50, render_camera_build_clipped_frustum_bounds, camera, clip, frustum_bounds);

	//ASSERT(camera);
	//ASSERT(clip);
	//ASSERT(frustum_bounds);
	//
	//bool result = true;
	//if (!debug_no_frustum_clip && clip->x0 < clip->x1 && clip->y0 < clip->y1)
	//{
	//	real v8 = real(camera->render_pixel_bounds.y1 - camera->render_pixel_bounds.y0) / real(camera->render_pixel_bounds.x1 - camera->render_pixel_bounds.x0);
	//	real v9 = 1.0f / tanf(real(camera->vertical_field_of_view / 2));
	//	frustum_bounds->x0 = clip->x0 * (v9 * v8);
	//	frustum_bounds->x1 = clip->x1 * (v9 * v8);
	//	frustum_bounds->y0 = clip->y0 * v9;
	//	frustum_bounds->y1 = clip->y1 * v9;
	//
	//	result = frustum_bounds->x0 >= frustum_bounds->x1 || frustum_bounds->y0 >= frustum_bounds->y1;
	//}
	//
	//if (result)
	//{
	//	frustum_bounds->y0 = -1.0;
	//	frustum_bounds->x0 = -1.0;
	//	frustum_bounds->y1 =  1.0;
	//	frustum_bounds->x1 =  1.0;
	//}
	//
	//return !result;
}

void __cdecl render_camera_build_orthogonal_projection(s_oriented_bounding_box const* camera, short_rectangle2d const* window_display_bounds, struct render_projection* projection, bool a4)
{
	ASSERT(camera);
	ASSERT(projection);

	INVOKE(0x00A63C70, render_camera_build_orthogonal_projection, camera, window_display_bounds, projection, a4);
}

void __cdecl render_camera_build_projection(render_camera const* camera, real_rectangle2d const* frustum_bounds, render_projection* projection, real aspect_ratio)
{
	//INVOKE(0x00A64140, render_camera_build_projection, camera, frustum_bounds, projection, aspect_ratio);

	ASSERT(camera);
	ASSERT(projection);

	real v0 = camera->z_far;
	real v1 = camera->z_near;
	if (c_rasterizer::get_is_using_floating_point_depth_buffer())
	{
		v0 = camera->z_near;
		v1 = camera->z_far;
	}

	render_view_parameters parameters{};
	render_camera_build_view_parameters(camera, frustum_bounds, &parameters, aspect_ratio);

	vector3d forward{};
	vector3d left{};
	vector3d up{};
	
	cross_product3d(&camera->forward, &camera->up, &forward);
	cross_product3d(&forward, &camera->forward, &left);
	negate_vector3d(&camera->forward, &up);
	
	normalize3d(&forward);
	normalize3d(&left);
	normalize3d(&up);
	
	projection->view_to_world.forward = forward;
	projection->view_to_world.left = left;
	projection->view_to_world.up = up;
	projection->view_to_world.position = camera->position;
	projection->view_to_world.scale = 1.0f;
	
	matrix4x3_inverse(&projection->view_to_world, &projection->world_to_view);
	
	//if (!valid_real_matrix4x3(&projection->world_to_view))
	//if (!valid_real_matrix4x3(&projection->view_to_world))
	
	projection->__unknownB8.i = real(parameters.width * parameters.__unknown28) / 2;
	projection->__unknownB8.j = real(parameters.hight * parameters.__unknown2C) / 2;
	projection->projection_bounds = parameters.projection_bounds;

	real v2 = real(camera->display_pixel_bounds.x1 - camera->display_pixel_bounds.x0);
	real v3 = real(camera->display_pixel_bounds.y1 - camera->display_pixel_bounds.y0);
	projection->__unknownB8.i *= (v2 > _real_epsilon ? real(c_rasterizer::render_globals.resolution_width) / v2 : 1.0f);
	projection->__unknownB8.j *= (v3 > _real_epsilon ? real(c_rasterizer::render_globals.resolution_height) / v3 : 1.0f);
	
	plane3d transformed_plane{};
	if (v1 == 0.0f)
	{
		matrix4x3_transform_plane(&projection->world_to_view, &camera->mirror_plane, &transformed_plane);
	}
	else
	{
		transformed_plane.n.i = 0.0f;
		transformed_plane.n.j = 0.0f;
		transformed_plane.n.k = 1.0f;
		transformed_plane.d = -v1;
	}
	
	real v6 = (real)-transformed_plane.d / transformed_plane.n.k;
	real v7 = real(1.0f + fabsf(real(transformed_plane.n.i / transformed_plane.n.k)));
	real v8 = fabsf(real(transformed_plane.n.j / transformed_plane.n.k));
	real v10 = v0 / real(real(v0 - v6) * real(v7 + v8));
	
	transformed_plane.n.i = real(v10 * transformed_plane.n.i) / transformed_plane.n.k;
	transformed_plane.n.j = real(v10 * transformed_plane.n.j) / transformed_plane.n.k;
	transformed_plane.n.k = v10;
	transformed_plane.d = -v10 * v6;
	if (transformed_plane.d > 0.0f && v1 == 0.0f)
	{
		transformed_plane.n.i = -transformed_plane.n.i;
		transformed_plane.n.j = -transformed_plane.n.j;
		transformed_plane.n.k = -transformed_plane.n.k;
		transformed_plane.d = -transformed_plane.d;
	}
	
	csmemset(projection->projection_matrix.matrix, 0, sizeof(s_oriented_bounding_box));
	projection->projection_matrix.matrix[0][0] = parameters.__unknown28;
	projection->projection_matrix.matrix[0][2] = -transformed_plane.n.i;
	projection->projection_matrix.matrix[1][1] = parameters.__unknown2C;
	projection->projection_matrix.matrix[1][2] = -transformed_plane.n.j;
	projection->projection_matrix.matrix[2][0] = -parameters.__unknown20;
	projection->projection_matrix.matrix[2][1] = -parameters.__unknown24;
	projection->projection_matrix.matrix[2][2] = -transformed_plane.n.k;
	projection->projection_matrix.matrix[2][3] = -1.0f;
	projection->projection_matrix.matrix[3][2] = transformed_plane.d;
}

void __cdecl render_camera_build_view_parameters(render_camera const* camera, real_rectangle2d const* frustum_bounds, render_view_parameters* parameters, real aspect_ratio)
{
	//INVOKE(0x00A64780, render_camera_build_view_parameters, camera, frustum_bounds, parameters, aspect_ratio);

	ASSERT(camera);
	ASSERT(parameters);
	ASSERT(camera->vertical_field_of_view < _pi - _real_epsilon);
	//ASSERT(camera->vertical_field_of_view > _real_epsilon);
	ASSERT(camera->z_near >= 0.0f);
	ASSERT(camera->render_pixel_bounds.x0 < camera->render_pixel_bounds.x1);
	ASSERT(camera->render_pixel_bounds.y0 < camera->render_pixel_bounds.y1);

	parameters->width = real((real)camera->window_pixel_bounds.x1 - (real)camera->window_pixel_bounds.x0) * real((real)camera->display_pixel_bounds.x1 / (real)camera->render_pixel_bounds.x1);
	parameters->hight = real((real)camera->window_pixel_bounds.y1 - (real)camera->window_pixel_bounds.y0) * real((real)camera->display_pixel_bounds.y1 / (real)camera->render_pixel_bounds.y1);

	if (frustum_bounds)
	{
		ASSERT(frustum_bounds->x0 < frustum_bounds->x1);
		ASSERT(frustum_bounds->y0 < frustum_bounds->y1);

		parameters->frustum_bounds = *frustum_bounds;
	}
	else
	{
		parameters->frustum_bounds.y0 = -1.0f;
		parameters->frustum_bounds.x0 = -1.0f;
		parameters->frustum_bounds.y1 =  1.0f;
		parameters->frustum_bounds.x1 =  1.0f;
	}

	parameters->__unknown18 = real(parameters->frustum_bounds.x1 - parameters->frustum_bounds.x0) / 2;
	parameters->__unknown1C = real(parameters->frustum_bounds.y1 - parameters->frustum_bounds.y0) / 2;
	parameters->__unknown20 = real(-real(parameters->frustum_bounds.x1 + parameters->frustum_bounds.x0) / 2) / parameters->__unknown18;
	parameters->__unknown24 = real(-real(parameters->frustum_bounds.y1 + parameters->frustum_bounds.y0) / 2) / parameters->__unknown1C;

	real vertical_field_of_view_tangent = tanf(camera->vertical_field_of_view / 2);
	real aspect_ratio_ = (real)parameters->width / (real)parameters->hight;

	if (aspect_ratio == 0.0f)
	{
		short_rectangle2d display_pixel_bounds{};
		c_rasterizer::get_display_pixel_bounds(&display_pixel_bounds);

		real pixel_bounds_aspect_ratio = real((display_pixel_bounds.x1 - display_pixel_bounds.x0) / real(display_pixel_bounds.y1 - display_pixel_bounds.y0));
		if (fabsf((pixel_bounds_aspect_ratio - c_rasterizer::get_aspect_ratio())))
			aspect_ratio_ = aspect_ratio_ * real(c_rasterizer::get_aspect_ratio() / pixel_bounds_aspect_ratio);
	}
	else
	{
		aspect_ratio_ = aspect_ratio;
	}

	aspect_ratio_ *= fminf(fmaxf(render_debug_aspect_ratio_scale, 0.25), 2.0f);

	if (cinematic_in_progress())
	{
		if (rasterizer_get_is_widescreen())
		{
			if (g_reduce_widescreen_fov_during_cinematics)
				vertical_field_of_view_tangent = 1.0f * ((vertical_field_of_view_tangent * 1.3333334f /* 4/3 */) * 0.5625f /* 9/16 */);
		}
		else
		{
			short_rectangle2d display_pixel_bounds{};
			c_rasterizer::get_display_pixel_bounds(&display_pixel_bounds);

			parameters->frustum_bounds.y0 *= real(parameters->hight / (real)display_pixel_bounds.y1);
			parameters->frustum_bounds.y1 *= real(parameters->hight / (real)display_pixel_bounds.y1);
		}
	}

	parameters->__unknown28 = 1.0f / ((aspect_ratio_ * parameters->__unknown18) * vertical_field_of_view_tangent);
	parameters->__unknown2C = 1.0f / (parameters->__unknown1C * vertical_field_of_view_tangent);

	parameters->projection_bounds.x0 = -real(parameters->__unknown20 + 1.0f) / parameters->__unknown28;
	parameters->projection_bounds.x1 = -real(parameters->__unknown20 - 1.0f) / parameters->__unknown28;
	parameters->projection_bounds.y0 = -real(parameters->__unknown24 + 1.0f) / parameters->__unknown2C;
	parameters->projection_bounds.y1 = -real(parameters->__unknown24 - 1.0f) / parameters->__unknown2C;

	ASSERT(parameters->projection_bounds.x0 < parameters->projection_bounds.x1);
	ASSERT(parameters->projection_bounds.y0 < parameters->projection_bounds.y1);
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

	real v0 = fabsf(((((projection->world_to_view.left.k * point->y) + (projection->world_to_view.forward.k * point->x)) + (projection->world_to_view.up.k * point->z)) + projection->world_to_view.position.z));

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

void render_debug_camera_projection()
{
	if (!debug_camera_projection)
		return;

	render_camera const* rasterizer_camera = c_player_view::get_current()->get_rasterizer_camera();
	render_projection const* rasterizer_projection = c_player_view::get_current()->get_rasterizer_projection();

	real verical_fov_half_tan = tanf(rasterizer_camera->vertical_field_of_view / 2);
	short width = rasterizer_camera->render_title_safe_pixel_bounds.x1 - rasterizer_camera->render_title_safe_pixel_bounds.x0;
	short height = rasterizer_camera->render_title_safe_pixel_bounds.y1 - rasterizer_camera->render_title_safe_pixel_bounds.y0;

	real_point3d points[3][3]{};
	for (long i = 0; i < 3; i++)
	{
		for (long j = 0; j < 3; j++)
		{
			real point_x = real((width * ((j - 1) * verical_fov_half_tan)) / height);
			real point_y = real(verical_fov_half_tan * (i - 1));

			set_real_point3d(&points[j][i], point_x, point_y, -1.0f);
			matrix4x3_transform_point(&rasterizer_projection->view_to_world, &points[j][i], &points[j][i]);
		}
	}

	for (long i = 0; i < 3; i++)
	{
		for (long j = 0; j < 3; j++)
		{
			render_debug_line(true, &points[j][0], &points[i][2], global_real_argb_red);
			render_debug_line(true, &points[0][j], &points[2][i], global_real_argb_red);
		}
	}
}

