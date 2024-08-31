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

//.text:00A3A790 ; 
//.text:00A3A7A0 ; public: void __cdecl c_player_view::render_patchy_fog()
//.text:00A3A7F0 ; render_patchy_fog_callback
//.text:00A3A850 ; public: virtual void __cdecl c_player_view::render_setup()
//.text:00A3A8C0 ; protected: void __cdecl c_player_view::render_static_lighting()
//.text:00A3AAA0 ; public: virtual void __cdecl c_player_view::render_submit_visibility()
//.text:00A3AB20 ; 

void __cdecl render_texture_camera_dispose()
{
	INVOKE(0x00A3AB90, render_texture_camera_dispose);
}

void __cdecl render_texture_camera_dispose_from_old_map()
{
	INVOKE(0x00A3ABA0, render_texture_camera_dispose_from_old_map);
}

//.text:00A3ABB0 ; void __cdecl render_texture_camera_enable_dynamic_lights(bool)
//.text:00A3ABD0 ; double __cdecl render_texture_camera_get_aspect_ratio()
//.text:00A3ABF0 ; 
//.text:00A3AC10 ; 
//.text:00A3AC30 ; 
//.text:00A3AC70 ; 
//.text:00A3ACB0 ; 

void __cdecl render_texture_camera_initialize()
{
	INVOKE(0x00A3ACD0, render_texture_camera_initialize);
}

void __cdecl render_texture_camera_initialize_for_new_map()
{
	INVOKE(0x00A3AD30, render_texture_camera_initialize_for_new_map);
}

//.text:00A3AE50 ; void __cdecl render_texture_camera_off()
//.text:00A3AE70 ; void __cdecl render_texture_camera_on()
//.text:00A3AE90 ; void __cdecl render_texture_camera_position_world_offset(real, real, real)
//.text:00A3AEE0 ; void __cdecl render_texture_camera_render_mode(long)
//.text:00A3AF20 ; void __cdecl render_texture_camera_set_aspect_ratio(real)
//.text:00A3AF50 ; void __cdecl render_texture_camera_set_fov(real)
//.text:00A3AFB0 ; void __cdecl render_texture_camera_set_fov_frame_target(real)
//.text:00A3AFF0 ; void __cdecl render_texture_camera_set_object_marker(long, long, real)
//.text:00A3B0F0 ; void __cdecl render_texture_camera_set_position(real, real, real)
//.text:00A3B150 ; void __cdecl render_texture_camera_set_position_and_direction(bool, real_point3d const*, vector3d const*, vector3d const*, real)
//.text:00A3B240 ; 
//.text:00A3B280 ; void __cdecl render_texture_camera_set_resolution(long, long)
//.text:00A3B2E0 ; void __cdecl render_texture_camera_set_target(real, real, real)
//.text:00A3B330 ; void __cdecl render_texture_camera_target_object(long, long)
//.text:00A3B380 ; protected: void __cdecl c_player_view::render_transparents()

