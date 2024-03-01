#include "render/views/render_view.hpp"

#include "interface/chud/cortana_effect.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/terminal.hpp"
#include "main/main_time.hpp"
#include "memory/module.hpp"
#include "render/render.hpp"
#include "render/render_debug.hpp"

#include <math.h>

REFERENCE_DECLARE(0x01913430, long, c_view::g_view_stack_top);
REFERENCE_DECLARE_ARRAY(0x050DEDF4, c_view*, c_view::g_view_stack, 4);

REFERENCE_DECLARE(0x050DEDF0, c_player_view*, c_player_view::x_current_player_view);
REFERENCE_DECLARE_ARRAY(0x050DEE10, c_player_view, c_player_view::x_global_player_views, 4);

REFERENCE_DECLARE(0x019180B8, long, c_lights_view::g_gel_bitmap_index);
REFERENCE_DECLARE(0x019180BC, real, c_lights_view::g_render_light_intensity);
REFERENCE_DECLARE(0x019180C0, dword, c_lights_view::g_debug_clip_planes);

REFERENCE_DECLARE(0x01913434, real, c_first_person_view::m_fov_scale);
REFERENCE_DECLARE(0x01913470, real, c_first_person_view::m_z_far_scale);

HOOK_DECLARE_CLASS_MEMBER(0x00A28DA0, c_first_person_view, override_projection);
HOOK_DECLARE(0x00A29220, render_debug_frame_render);
HOOK_DECLARE_CALL(0x00A3A0A5, render_debug_window_render);

void __cdecl c_view::abort_current_view_stack()
{
	g_view_stack_top = -1;
}

void __cdecl c_view::begin(c_view* view)
{
	if (g_view_stack_top < 3)
	{
		g_view_stack[++g_view_stack_top] = view;
		view->render_setup();
	}
}

void __cdecl c_view::end()
{
	if (--g_view_stack_top >= 0)
		g_view_stack[g_view_stack_top]->render_setup();
}

long __cdecl c_view::get_current_stack_level()
{
	return g_view_stack_top;
}

c_view* __cdecl c_view::top()
{
	if (g_view_stack_top < 0)
		return 0;
	else
		return g_view_stack[g_view_stack_top];
}

render_camera const* __cdecl c_view::get_render_camera() const
{
	return &m_render_camera;
}

render_camera* __cdecl c_view::get_render_camera_modifiable()
{
	return &m_render_camera;
}

render_camera const* __cdecl c_view::get_rasterizer_camera() const
{
	return &m_rasterizer_camera;
}

render_camera* __cdecl c_view::get_rasterizer_camera_modifiable()
{
	return &m_rasterizer_camera;
}

render_projection const* __cdecl c_view::get_rasterizer_projection() const
{
	return &m_rasterizer_projection;
}

render_projection* __cdecl c_view::get_rasterizer_projection_modifiable()
{
	return &m_rasterizer_projection;
}

render_projection const* __cdecl c_view::get_render_projection() const
{
	return &m_render_projection;
}

render_projection* __cdecl c_view::get_render_projection_modifiable()
{
	return &m_render_projection;
}

void __cdecl c_fullscreen_view::setup_camera(s_observer_result const* result)
{
	render_camera* rasterizer_camera_modifiable = get_rasterizer_camera_modifiable();
	csmemset(rasterizer_camera_modifiable, 0, sizeof(render_camera));

	render_view_compute_fullscreen_bounds(rasterizer_camera_modifiable);
	render_camera_build(rasterizer_camera_modifiable, nullptr);

	real_rectangle2d frustum_bounds;
	render_camera_build_viewport_frustum_bounds(rasterizer_camera_modifiable, &frustum_bounds);

	render_projection* rasterizer_projection_modifiable = get_rasterizer_projection_modifiable();
	render_camera_build_projection(rasterizer_camera_modifiable, &frustum_bounds, rasterizer_projection_modifiable, 0.0f);

	render_camera* render_camera_modifiable = get_render_camera_modifiable();
	render_camera const* rasterizer_camera = get_rasterizer_camera();
	csmemcpy(render_camera_modifiable, rasterizer_camera, sizeof(render_camera));

	render_projection* render_projection_modifiable = get_render_projection_modifiable();
	render_projection const* rasterizer_projection = get_rasterizer_projection();
	csmemcpy(render_projection_modifiable, rasterizer_projection, sizeof(render_projection));
}

void __cdecl c_fullscreen_view::render_blank_frame(real_rgb_color const* color)
{
	DECLFUNC(0x00A291E0, void, __cdecl, real_rgb_color const*)(color);
}

void __thiscall c_first_person_view::override_projection(bool first_person_squish)
{
	render_camera* rasterizer_camera_modifiable = get_rasterizer_camera_modifiable();
	render_projection* render_projection_modifiable = get_render_projection_modifiable();

	*rasterizer_camera_modifiable = *m_camera;

	m_fov_scale = 0.6908f / fmaxf(rasterizer_camera_modifiable->horizontal_field_of_view, _real_epsilon);
	rasterizer_camera_modifiable->vertical_field_of_view *= m_fov_scale;

	long width = rasterizer_camera_modifiable->window_pixel_bounds.x1 - rasterizer_camera_modifiable->window_pixel_bounds.x0;
	long height = rasterizer_camera_modifiable->window_pixel_bounds.y1 - rasterizer_camera_modifiable->window_pixel_bounds.y0;
	real aspect_ratio = (real)width / (real)height;

	// cortana effect fov?
	rasterizer_camera_modifiable->vertical_field_of_view /= fmaxf(sub_ABEA20(), _real_epsilon);

	if (first_person_squish)
	{
		rasterizer_camera_modifiable->z_far *= m_z_far_scale;
		rasterizer_camera_modifiable->z_near *= aspect_ratio > 1.8f ? 2.4f : 3.2f;
	}

	real static_vertical_field_of_view = rasterizer_camera_modifiable->vertical_field_of_view * 0.78500003f;

	real_rectangle2d frustum_bounds{};
	render_camera_build_viewport_frustum_bounds(rasterizer_camera_modifiable, &frustum_bounds);
	render_camera_build_projection(rasterizer_camera_modifiable, &frustum_bounds, render_projection_modifiable, 0.0f);
	render_setup_window(rasterizer_camera_modifiable, render_projection_modifiable);

	if (debug_static_first_person)
		rasterizer_camera_modifiable->vertical_field_of_view = static_vertical_field_of_view;
}

void __cdecl render_debug_frame_render()
{
	//INVOKE(0x00A29220, render_debug_frame_render);

	if (DECLFUNC(0x0042E5D0, bool, __cdecl)())
		return;

	//render_debug_begin(true, true, true);
	terminal_draw();
	main_time_frame_rate_display();
	render_debug_debug_menu();
	//render_debug_end(false, false, false);
}

void __cdecl render_debug_window_render(long user_index)
{
	// asserts

	INVOKE(0x00A29230, render_debug_window_render, user_index);
	//c_rasterizer::restore_last_viewport();
	//c_rasterizer::set_depth_stencil_surface(4);

	render_debug_begin(false, false, false);
	render_debug_structure_draw();
	render_debug_clients(user_index);
	render_debug_end(true, false, false);
}

