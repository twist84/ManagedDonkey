#include "render/views/render_view.hpp"

REFERENCE_DECLARE(0x01913430, long, c_view::g_view_stack_top);
REFERENCE_DECLARE_ARRAY(0x050DEDF4, c_view*, c_view::g_view_stack, 4);

REFERENCE_DECLARE(0x050DEDF0, c_player_view*, c_player_view::x_current_player_view);
REFERENCE_DECLARE_ARRAY(0x050DEE10, c_player_view, c_player_view::x_global_player_views, 4);

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

