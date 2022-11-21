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
	byte __data298[0xD54];
	byte __dataFEC[0x280];
	real m_light_intensity_scale;
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

	// c_camera_fx_values?
	byte __data298[0x20];

	c_patchy_fog m_patchy_fog;

	// struct?
	real_point3d m_position;
	real_vector3d m_forward;
	real_vector3d m_up;
	long __unknown630;
	real_matrix4x3 __matrix634;
	real m_projection_matrix[4][4];

	c_first_person_view m_first_person_view;
	c_ui_view m_ui_view;
	c_lights_view m_lights_view;
	c_lightmap_shadows_view m_lightmap_shadows_view;
	c_reflection_view m_reflection_view;
	c_occlusion_view m_occlusion_view;

	short_rectangle2d __rectangle2690;

	long m_player_index;
	long m_window_count;
	long m_window_arrangement;
	long m_user_index;
	long m_output_user_index;
	long m_splitscreen_res;

	long __unknown26B0;
	long __unknown26B4;
};
static_assert(sizeof(c_player_view) == sizeof(c_world_view) + 0x2420);

struct c_hud_camera_view :
	public c_player_view
{
};
static_assert(sizeof(c_hud_camera_view) == sizeof(c_player_view));

struct c_texture_camera_view :
	public c_player_view
{
	long __unknown26B8;
	long __unknown26BC;
	long __unknown26C0;
	long __unknown26C4;
	long __unknown26C8;
	long __unknown26CC;
	long __unknown26D0;
	dword_flags __unknown26D4;
	long __unknown26D8;
	long __unknown26DC;
	long __unknown26E0;
	long __unknown26E4;
};
static_assert(sizeof(c_texture_camera_view) == sizeof(c_player_view) + 0x30);

