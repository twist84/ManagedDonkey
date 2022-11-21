#pragma once

#include "render/render_cameras.hpp"
// 0165DB98
struct c_view
{
public:
	virtual void __cdecl render();
	virtual long __cdecl render_setup();
	virtual long __cdecl compute_visibility();
	virtual long __cdecl render_submit_visibility();

	static long& g_view_stack_top;
	static c_view*(&g_view_stack)[4];

	render_camera m_rasterizer_camera;
	render_projection m_rasterizer_projection;
	render_camera m_render_camera;
	render_projection m_render_projection;
};
static_assert(sizeof(c_view) == 0x294);

// 0165DBAC
struct c_ui_view :
	public c_view
{
	byte __data294[0x8];
};
static_assert(sizeof(c_ui_view) == sizeof(c_view) + 0x8);

// 0165DBBC
struct c_fullscreen_view :
	public c_view
{
};
static_assert(sizeof(c_fullscreen_view) == sizeof(c_view));

// 01692BDC
struct c_world_view :
	public c_view
{
	s_location m_location;
};
static_assert(sizeof(c_world_view) == sizeof(c_view) + 0x4);

// 0165E0C4
struct c_lights_view :
	public c_world_view
{
	byte __data298[0xFD8];
};
static_assert(sizeof(c_lights_view) == sizeof(c_world_view) + 0xFD8);

// 0165E0D8
struct c_lightmap_shadows_view :
	public c_world_view
{
	byte __data298[0x40];
};
static_assert(sizeof(c_lightmap_shadows_view) == sizeof(c_world_view) + 0x40);

// 0165E0EC
struct c_occlusion_view :
	public c_lightmap_shadows_view
{
};
static_assert(sizeof(c_occlusion_view) == sizeof(c_lightmap_shadows_view));

