#include "render/views/render_view.hpp"

#include "camera/director.hpp"
#include "cseries/async_xoverlapped.hpp"
#include "game/campaign_metagame.hpp"
#include "game/game_time.hpp"
#include "game/player_mapping.hpp"
#include "hs/hs_runtime.hpp"
#include "interface/c_controller.hpp"
#include "interface/chud/cortana_effect.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/interface_constants.hpp"
#include "interface/terminal.hpp"
#include "interface/user_interface.hpp"
#include "items/weapons.hpp"
#include "main/console.hpp"
#include "main/main_render.hpp"
#include "main/main_time.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "profiler/profiler.hpp"
#include "rasterizer/rasterizer_globals.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/render.hpp"
#include "render/render_debug.hpp"
#include "render/render_objects.hpp"
#include "render/render_visibility_collection.hpp"
#include "render/views/hud_camera_view.hpp"
#include "shell/shell.hpp"

#include <math.h>

REFERENCE_DECLARE(0x01913430, int32, c_view::g_view_stack_top);
REFERENCE_DECLARE_ARRAY(0x050DEDF4, c_view*, c_view::g_view_stack, 4);
REFERENCE_DECLARE(0x050DEDF0, c_player_view*, c_player_view::x_current_player_view);
REFERENCE_DECLARE_ARRAY(0x050DEE10, c_player_view, c_player_view::x_global_player_views, 4);
REFERENCE_DECLARE(0x019180B8, int32, c_lights_view::g_gel_bitmap_index);
REFERENCE_DECLARE(0x019180BC, real32, c_lights_view::g_render_light_intensity);
REFERENCE_DECLARE(0x019180C0, uns32, c_lights_view::g_debug_clip_planes);
REFERENCE_DECLARE(0x0191342C, bool, render_lightmap_shadows_enabled);
REFERENCE_DECLARE(0x01913434, real32, c_first_person_view::m_fov_scale);
REFERENCE_DECLARE(0x01913470, real32, c_first_person_view::m_z_far_scale);

HOOK_DECLARE_CLASS_MEMBER(0x00A28A00, c_first_person_view, compute_visibility);
HOOK_DECLARE_CLASS_MEMBER(0x00A28DA0, c_first_person_view, override_projection);
HOOK_DECLARE_CLASS_MEMBER(0x00A29050, c_fullscreen_view, render_);
HOOK_DECLARE_CLASS_MEMBER(0x00A290A0, c_ui_view, render_);
HOOK_DECLARE(0x00A29220, render_debug_frame_render);
HOOK_DECLARE_CALL(0x00A3A0A5, render_debug_window_render);

bool render_debug_pix_events = false;

void __cdecl c_view::abort_current_view_stack()
{
	//INVOKE(0x00A289C0, c_view::abort_current_view_stack);

	g_view_stack_top = NONE;
}

void __cdecl c_view::begin(c_view* view)
{
	//INVOKE(0x00A289D0, c_view::begin, view);

	if (g_view_stack_top < 3)
	{
		g_view_stack[++g_view_stack_top] = view;
		view->render_setup();
	}
}

void __thiscall c_first_person_view::compute_visibility(int32 user_index)
{
	//INVOKE_CLASS_MEMBER(0x00A28A00, c_first_person_view, compute_visibility, user_index);

	if (user_index != NONE && director_get_perspective(user_index) == _director_perspective_first_person)
	{
		int32 unit_index = player_mapping_get_unit_by_output_user(user_index);
		if (unit_index != NONE)
		{
			render_visibility_add_first_person_object_to_items(user_index, unit_index, true, true);
		}
	}
}

void __cdecl c_view::sub_A28A40()
{
	//INVOKE(0x00A28A40, sub_A28A40);

	int constant_bool = 0;
	real_vector4d constant{};
	set_real_vector4d(&constant, 0.0f, 0.0f, 0.0f, 0.0f);

	c_rasterizer::set_vertex_shader_constant_bool(8, 1, &constant_bool);
	c_rasterizer::set_vertex_shader_constant(250, 1, &constant);
}

void __cdecl c_view::sub_A28A90()
{
	//INVOKE(0x00A28A90, sub_A28A90);

	int constant_bool = 1;
	real_vector4d constant{};

	real32 z_depth_scale = 1.0f / (global_z_far - global_z_near);
	set_real_vector4d(&constant, global_z_far * z_depth_scale, (global_z_near * global_z_far) * z_depth_scale, 0.1f, 1.0f);

	c_rasterizer::set_vertex_shader_constant_bool(8, 1, &constant_bool);
	c_rasterizer::set_vertex_shader_constant(250, 1, &constant);
}

void __cdecl c_view::end()
{
	//INVOKE(0x00A28B10, c_view::end);

	if (--g_view_stack_top >= 0)
	{
		g_view_stack[g_view_stack_top]->render_setup();
	}
}

int32 __cdecl c_view::get_current_stack_level()
{
	//return INVOKE(0x00A28B30, c_view::get_current_stack_level);

	return g_view_stack_top;
}

const render_camera* c_view::get_render_camera() const
{
	return &m_render_camera;
}

render_camera* c_view::get_render_camera_modifiable()
{
	return &m_render_camera;
}

const render_camera* c_view::get_rasterizer_camera() const
{
	return &m_rasterizer_camera;
}

render_camera* c_view::get_rasterizer_camera_modifiable()
{
	return &m_rasterizer_camera;
}

const render_projection* c_view::get_rasterizer_projection() const
{
	return &m_rasterizer_projection;
}

render_projection* c_view::get_rasterizer_projection_modifiable()
{
	return &m_rasterizer_projection;
}

const render_projection* c_view::get_render_projection() const
{
	return &m_render_projection;
}

render_projection* c_view::get_render_projection_modifiable()
{
	return &m_render_projection;
}

c_view* __cdecl c_view::top()
{
	return g_view_stack_top < 0 ? NULL : g_view_stack[g_view_stack_top];
}

void c_lights_view::render(int32 user_index, int32 player_index, IDirect3DSurface9* a3, IDirect3DSurface9* a4, IDirect3DSurface9* a5)
{
	INVOKE_CLASS_MEMBER(0x00A67060, c_lights_view, render, user_index, player_index, a3, a4, a5);
}

void __thiscall c_fullscreen_view::render_()
{
	render_debug_stuff_while_loading();
}

void c_fullscreen_view::render_debug_stuff_while_loading()
{
	rectangle2d screen_bounds{};
	rectangle2d frame_bounds{};
	rectangle2d window_bounds{};
	interface_get_current_display_settings(&screen_bounds, &frame_bounds, &window_bounds, NULL);
	user_interface_render(k_no_controller, NONE, k_number_of_player_windows, &window_bounds, c_rasterizer::get_display_surface(), false);
	terminal_draw(&screen_bounds, &frame_bounds);
	status_line_draw();
	main_time_frame_rate_display();
	render_synchronization_stats();
	game_time_render_debug();
}

void c_fullscreen_view::setup_camera(const s_observer_result* observer)
{
	render_camera* rasterizer_camera_modifiable = get_rasterizer_camera_modifiable();
	csmemset(rasterizer_camera_modifiable, 0, sizeof(render_camera));

	render_view_compute_fullscreen_bounds(rasterizer_camera_modifiable);
	render_camera_build(rasterizer_camera_modifiable, NULL);

	real_rectangle2d frustum_bounds;
	render_camera_build_viewport_frustum_bounds(rasterizer_camera_modifiable, &frustum_bounds);

	render_projection* rasterizer_projection_modifiable = get_rasterizer_projection_modifiable();
	render_camera_build_projection(rasterizer_camera_modifiable, &frustum_bounds, rasterizer_projection_modifiable, 0.0f);

	render_camera* render_camera_modifiable = get_render_camera_modifiable();
	const render_camera* rasterizer_camera = get_rasterizer_camera();
	csmemcpy(render_camera_modifiable, rasterizer_camera, sizeof(render_camera));

	render_projection* render_projection_modifiable = get_render_projection_modifiable();
	const render_projection* rasterizer_projection = get_rasterizer_projection();
	csmemcpy(render_projection_modifiable, rasterizer_projection, sizeof(render_projection));
}

void c_world_view::get_starting_cluster(s_cluster_reference* starting_cluster)
{
	INVOKE_CLASS_MEMBER(0x00A28B50, c_world_view, get_starting_cluster, starting_cluster);
}

void __thiscall c_first_person_view::override_projection(bool squish_close_to_camera)
{
	//INVOKE_CLASS_MEMBER(0x00A28DA0, c_first_person_view, override_projection, first_person_squish);

	render_camera* rasterizer_camera_modifiable = get_rasterizer_camera_modifiable();
	render_projection* render_projection_modifiable = get_render_projection_modifiable();

	*rasterizer_camera_modifiable = *m_default_rasterizer_camera;

	m_fov_scale = 0.6908f / fmaxf(rasterizer_camera_modifiable->field_of_view_scale, _real_epsilon);
	rasterizer_camera_modifiable->vertical_field_of_view *= m_fov_scale;

	int32 width = rectangle2d_width(&rasterizer_camera_modifiable->window_pixel_bounds);
	int32 height = rectangle2d_height(&rasterizer_camera_modifiable->window_pixel_bounds);
	real32 aspect_ratio = (real32)width / (real32)height;

	rasterizer_camera_modifiable->vertical_field_of_view /= fmaxf(cortana_effect_get_fov_scale(), _real_epsilon);

	if (squish_close_to_camera)
	{
		rasterizer_camera_modifiable->z_far *= m_z_far_scale;
		rasterizer_camera_modifiable->z_near *= aspect_ratio > 1.8f ? 2.4f : 3.2f;
	}

	real32 static_vertical_field_of_view = rasterizer_camera_modifiable->vertical_field_of_view * 0.78500003f;

	real_rectangle2d frustum_bounds{};
	render_camera_build_viewport_frustum_bounds(rasterizer_camera_modifiable, &frustum_bounds);
	render_camera_build_projection(rasterizer_camera_modifiable, &frustum_bounds, render_projection_modifiable, 0.0f);
	render_setup_window(rasterizer_camera_modifiable, render_projection_modifiable);

	if (debug_static_first_person)
	{
		rasterizer_camera_modifiable->vertical_field_of_view = static_vertical_field_of_view;
	}
}

void __thiscall c_ui_view::render_()
{
	//INVOKE_CLASS_MEMBER(0x00A290A0, c_ui_view, render);

	rectangle2d window_bounds{};
	interface_get_current_display_settings(NULL, NULL, &window_bounds, NULL);
	user_interface_render(k_no_controller, NONE, k_number_of_player_windows, &window_bounds, m_render_surface, m_is_screenshot);
}

void __thiscall c_first_person_view::render_albedo(int32 user_index)
{
	//INVOKE_CLASS_MEMBER(0x00A290F0, c_first_person_view, render_albedo, user_index);

	constexpr uns32 k_first_person_squished_flags = FLAG(_render_object_mesh_part_cancel_shadows_for_first_person_albedo) | FLAG(_render_object_mesh_part_first_person_squished_bit);
	constexpr uns32 k_first_person_unsquished_flags = FLAG(_render_object_mesh_part_cancel_shadows_for_first_person_albedo) | FLAG(_render_object_mesh_part_first_person_unsquished_bit);

	c_view::sub_A28A90();
	c_object_renderer::render_albedo(k_first_person_squished_flags);
	c_view::sub_A28A40();
	override_projection(false);
	c_object_renderer::render_albedo(k_first_person_unsquished_flags);
}

void c_fullscreen_view::render_blank_frame(const real_rgb_color* background_color)
{
	//DECLFUNC(0x00A291E0, void, __cdecl, const real_rgb_color*)(background_color);

	c_rasterizer::set_render_target(0, c_rasterizer::get_display_surface(), 0xFFFFFFFF);
	c_rasterizer::set_depth_stencil_surface(c_rasterizer::_surface_none);
	c_rasterizer::clear(1, real_rgb_color_to_pixel32(background_color), 1.0f, 1);
}

void __cdecl render_debug_frame_render()
{
	//INVOKE(0x00A29220, render_debug_frame_render);

	if (!sub_42E5D0())
	{
		c_rasterizer_profile_scope _frame_debug(_rasterizer_profile_element_debug, L"frame_debug");

		rectangle2d screen_pixel_bounds{};
		c_rasterizer::get_fullscreen_render_pixel_bounds(&screen_pixel_bounds);

		rectangle2d safe_frame_pixel_bounds{};
		c_rasterizer::get_fullscreen_render_title_safe_pixel_bounds(&safe_frame_pixel_bounds);

		render_debug_begin(true, true, true);

		terminal_draw(&screen_pixel_bounds, &safe_frame_pixel_bounds);
		status_line_draw();
		//cinematic_status_draw();
		render_debug_campaign_metagame();
		main_time_frame_rate_display();
		render_debug_scripting();
		//render_debug_cluster_blend_info();
		profile_render(&screen_pixel_bounds, &safe_frame_pixel_bounds);
		render_synchronization_stats();
		//player_control_debug_render();
		weapons_debug_render();
		render_debug_debug_menu();
		game_time_render_debug();
		overlapped_render();
		controllers_render();
		//font_cache_debug_render();
		//async_display_current_task_status();

		render_debug_end(false, false, false);
	}
}

void __cdecl render_debug_window_render(int32 user_index)
{
	//INVOKE(0x00A29230, render_debug_window_render, user_index);

	c_rasterizer_profile_scope _window_debug(_rasterizer_profile_element_debug, L"window_debug");

	c_rasterizer::restore_last_viewport();
	c_rasterizer::set_depth_stencil_surface(c_rasterizer::_surface_depth_stencil);
	if (c_view::top())
	{
		render_debug_begin(false, false, false);
		render_debug_structure_draw();
		render_debug_clients(user_index);
		render_debug_end(true, false, false);
	}
}

void __thiscall c_first_person_view::render_submit_visibility(int32 user_index, bool submit_transparents)
{
	//INVOKE_CLASS_MEMBER(0x00A29280, c_first_person_view, render_submit_visibility, user_index, submit_transparents);

	c_object_renderer::submit_visibility(FLAG(_submit_visibility_first_person) | (submit_transparents ? FLAG(_submit_visibility_transparents) : 0));
}

void c_ui_view::setup_camera(const s_observer_result* result, c_rasterizer::e_surface surface)
{
	INVOKE_CLASS_MEMBER(0x00A29330, c_ui_view, setup_camera, result, surface);

	//render_camera* rasterizer_camera = get_rasterizer_camera_modifiable();
	//
	//m_render_target = surface;
	//render_view_compute_fullscreen_bounds(rasterizer_camera);
	//render_camera_build(rasterizer_camera, result);
	//c_rasterizer::set_current_splitscreen_res(0);
	//
	//c_rasterizer::begin(rasterizer_camera->render_pixel_bounds, rasterizer_camera->render_pixel_bounds);
	//
	//if (c_rasterizer::get_render_target(0) != m_render_target)
	//{
	//	c_rasterizer::set_render_target(0, m_render_target, NONE);
	//	c_rasterizer::clear(1/*D3DCLEAR_TARGET*/, 0, 0.0f, 0);
	//}
}

c_hud_camera_view* c_hud_camera_view::constructor()
{
	return INVOKE_CLASS_MEMBER(0x0060DC00, c_hud_camera_view, constructor);
}

void c_hud_camera_view::render(int32 player_index, const c_player_view* player_view, void* data)
{
	INVOKE_CLASS_MEMBER(0x00A2D490, c_hud_camera_view, render, player_index, player_view, data);
}

