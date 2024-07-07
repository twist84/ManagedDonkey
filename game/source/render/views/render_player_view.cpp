#include "render/views/render_player_view.hpp"

#include "render/views/render_view.hpp"

REFERENCE_DECLARE(0x019147BC, real, render_debug_depth_render_scale_r);
REFERENCE_DECLARE(0x019147C0, real, render_debug_depth_render_scale_g);
REFERENCE_DECLARE(0x019147C4, real, render_debug_depth_render_scale_b);
REFERENCE_DECLARE(0x050FB3FC, long, render_debug_depth_render);

void __cdecl c_player_view::get_player_render_camera_orientation(real_matrix4x3* camera)
{
	//INVOKE(0x00A39640, get_player_render_camera_orientation, camera);

	ASSERT(camera);
	ASSERT(c_player_view::x_current_player_view, "cannot get the render camera when not in the player view render loop");

	render_camera const* rasterizer_camera = c_player_view::x_current_player_view->get_rasterizer_camera();

	camera->position = rasterizer_camera->position;
	camera->forward = rasterizer_camera->forward;
	camera->up = rasterizer_camera->up;
	cross_product3d(&camera->up, &camera->forward, &camera->left);
	camera->scale = 1.0f;
}

void __cdecl c_player_view::create_frame_textures(long player_index)
{
	DECLFUNC(0x00A38D70, void, __thiscall, c_player_view*, long)(this, player_index);
}

