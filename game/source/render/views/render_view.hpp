#pragma once

#include "render/render_cameras.hpp"
#include "render/render_patchy_fog.hpp"

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

// 0165E108
struct c_reflection_view :
	public c_view
{
};
static_assert(sizeof(c_reflection_view) == sizeof(c_view));

// 0165E11C
struct c_first_person_view :
	public c_view
{
	byte __data294[0x4];
};
static_assert(sizeof(c_first_person_view) == sizeof(c_view) + 0x4);

// 0165E130
struct c_player_view :
	public c_world_view
{
	static c_player_view*& x_current_player_view;
	static c_player_view(&x_global_player_views)[4];

	byte __data298[0x20];
	c_patchy_fog m_patchy_fog;
	byte __data60C[0x28];
	byte __data634[0x34];
	byte __data668[0x40];
	c_first_person_view m_first_person_view;
	c_ui_view m_ui_view;
	c_lights_view m_lights_view;
	c_lightmap_shadows_view m_lightmap_shadows_view;
	c_reflection_view m_reflection_view;
	c_occlusion_view m_occlusion_view;
	short_rectangle2d __rectangle2690;
	long __unknown2698_user_index;
	long __unknown269C_player_count;
	long __unknown26A0;
	long __unknown26A4_user_index;
	long __unknown26A8_user_index;
	byte __data26AC[0xC];
};
static_assert(sizeof(c_player_view) == sizeof(c_world_view) + 0x2420);

