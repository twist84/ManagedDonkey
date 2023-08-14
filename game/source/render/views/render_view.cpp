#include "render/views/render_view.hpp"

#include "interface/terminal.hpp"
#include "hs/hs_runtime.hpp"
#include "main/main_time.hpp"
#include "memory/module.hpp"
#include "objects/objects.hpp"
#include "render/render_debug.hpp"
#include "units/units.hpp"

REFERENCE_DECLARE(0x01913430, long, c_view::g_view_stack_top);
REFERENCE_DECLARE_ARRAY(0x050DEDF4, c_view*, c_view::g_view_stack, 4);

REFERENCE_DECLARE(0x050DEDF0, c_player_view*, c_player_view::x_current_player_view);
REFERENCE_DECLARE_ARRAY(0x050DEE10, c_player_view, c_player_view::x_global_player_views, 4);

REFERENCE_DECLARE(0x019180B8, long, c_lights_view::g_gel_bitmap_index);
REFERENCE_DECLARE(0x019180BC, real, c_lights_view::g_render_light_intensity);
REFERENCE_DECLARE(0x019180C0, dword, c_lights_view::g_debug_clip_planes);

REFERENCE_DECLARE(0x01913434, real, c_first_person_view::m_fov_scale);
REFERENCE_DECLARE(0x01913470, real, c_first_person_view::m_z_far_scale);

REFERENCE_DECLARE_ARRAY(0x018BABE8, real, pregame_frame_scales, 9);
REFERENCE_DECLARE_ARRAY(0x0165DA08, s_render_fullscreen_text_context_colors, pregame_frame_colors, 9);

HOOK_DECLARE(0x00A29220, render_debug_frame_render);

//real pregame_frame_scales[9] =
//{
//	1.0f,
//	1.0f,
//	1.0f,
//	1.0f,
//	1.0f,
//	1.0f,
//	1.0f,
//	1.0f,
//	1.0f
//};
//
//s_render_fullscreen_text_context_colors pregame_frame_colors[9] =
//{
//	{
//		.blank_frame = { 0.0f, 0.0f, 0.0f },
//		.text_color = { 1.0f, 1.0f, 1.0f },
//		.text_shadow_color = { 0.0f, 0.0f, 0.0f }
//	},
//	{
//		.blank_frame = { 0.0f, 0.0f, 0.0f },
//		.text_color = { 1.0f, 1.0f, 1.0f },
//		.text_shadow_color = { 0.0f, 0.0f, 0.0f }
//	},
//	{
//		.blank_frame = { 0.0f, 0.2f, 0.0f },
//		.text_color = { 0.55000001f, 0.44999999f, 0.64999998f },
//		.text_shadow_color = { 0.079999998f, 0.050000001f, 0.012f }
//	},
//	{
//		.blank_frame = { 0.047058824f, 0.06666667f, 0.1254902f },
//		.text_color = { 1.0f, 1.0f, 1.0f },
//		.text_shadow_color = { 0.015686275f, 0.023529412f, 0.039215688f }
//	},
//	{
//		.blank_frame = { 0.0f, 0.0f, 0.0f },
//		.text_color = { 0.55000001f, 0.44999999f, 0.64999998f },
//		.text_shadow_color = { 0.079999998f, 0.050000001f, 0.012f }
//	},
//	{
//		.blank_frame = { 0.0f, 0.0f, 0.0f },
//		.text_color = { 0.55000001f, 0.44999999f, 0.64999998f },
//		.text_shadow_color = { 0.079999998f, 0.050000001f, 0.012f }
//	},
//	{
//		.blank_frame = { 0.28999999f, 0.059999999f, 0.36000001f },
//		.text_color = { 0.55000001f, 0.44999999f, 0.64999998f },
//		.text_shadow_color = { 0.079999998f, 0.050000001f, 0.012f }
//	},
//	{
//		.blank_frame = { 1.0f, 1.0f, 1.0f },
//		.text_color = { 0.0f, 0.0f, 0.0f },
//		.text_shadow_color = { 1.0f, 1.0f, 1.0f }
//	},
//	{
//		.blank_frame = { 0.047058824f, 0.06666667f, 0.1254902f },
//		.text_color = { 1.0f, 1.0f, 1.0f },
//		.text_shadow_color = { 0.015686275f, 0.023529412f, 0.039215688f }
//	}
//};

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

void __cdecl render_debug_frame_render()
{
	//INVOKE(0x00A29220, render_debug_frame_render);

	if (DECLFUNC(0x0042E5D0, bool, __cdecl)())
		return;

	terminal_draw();
	main_time_frame_rate_display();
	render_debug_trigger_volumes();

	// location_messages
	//real_point3d point = { 81.6f, -72.4f, 7.2f };
	//render_debug_point(true, &point, 2.0f, global_real_argb_magenta);

	// the object type render functions don't get called in release, call them here
	c_object_iterator<unit_datum> unit_iterator;
	unit_iterator.begin(UNIT_OBJECTS_MASK, 0);
	while (unit_iterator.next())
	{
		if (unit_iterator.get_index() != NONE)
			unit_render_debug(unit_iterator.get_index());
	}
}

