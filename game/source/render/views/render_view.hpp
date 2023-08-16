#pragma once

#include "render/render_cameras.hpp"
#include "render/render_patchy_fog.hpp"
#include "rasterizer/rasterizer_text.hpp"

struct s_observer_result;

// 0165DB98
struct c_view
{
protected:

	// HACK: so we don't have to manually construct the class
	struct
	{
		void(__thiscall* render)(c_view*);
		long(__thiscall* render_setup)(c_view*);
		long(__thiscall* compute_visibility)(c_view*);
		long(__thiscall* render_submit_visibility)(c_view*);
	}*__vftable;

public:
	c_view() :
		__vftable(reinterpret_cast<decltype(__vftable)>(0x0165DB98)),
		m_rasterizer_camera(),
		m_rasterizer_projection(),
		m_render_camera(),
		m_render_projection()
	{
	}

	void __cdecl render() { __vftable->render(this); }
	long __cdecl render_setup() { return __vftable->render_setup(this); }
	long __cdecl compute_visibility() { return __vftable->compute_visibility(this); }
	long __cdecl render_submit_visibility() { return __vftable->render_submit_visibility(this); }

	static void __cdecl abort_current_view_stack();
	static void __cdecl begin(c_view* view);
	static void __cdecl end();
	static long __cdecl get_current_stack_level();
	static c_view* __cdecl top();
	
	render_camera const* __cdecl get_render_camera() const;
	render_camera* __cdecl get_render_camera_modifiable();

	render_camera const* __cdecl get_rasterizer_camera() const;
	render_camera* __cdecl get_rasterizer_camera_modifiable();

	render_projection const* __cdecl get_rasterizer_projection() const;
	render_projection* __cdecl get_rasterizer_projection_modifiable();

	render_projection const* __cdecl get_render_projection() const;
	render_projection* __cdecl get_render_projection_modifiable();

protected:
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
public:
	c_ui_view() :
		c_view(),
		__unknown298(),
		m_render_target()
	{
		__vftable = reinterpret_cast<decltype(__vftable)>(0x0165DBAC);
	}

	// if true bypass if statement in `c_gui_widget::render`
	bool __unknown298;

	long m_render_target;
};
static_assert(sizeof(c_ui_view) == sizeof(c_view) + 0x8);

// 0165DBC0
struct c_fullscreen_view :
	public c_view
{
public:
	c_fullscreen_view() :
		c_view()
	{
		__vftable = reinterpret_cast<decltype(__vftable)>(0x0165DBC0);
	}

	void __cdecl setup_camera(s_observer_result const* result);
	void __cdecl render_blank_frame(real_rgb_color const* color);
};
static_assert(sizeof(c_fullscreen_view) == sizeof(c_view));

// 01692BDC
struct c_world_view :
	public c_view
{
protected:
	s_location m_location;
};
static_assert(sizeof(c_world_view) == sizeof(c_view) + 0x4);

// 0165E0C4
struct c_lights_view :
	public c_world_view
{
public:
	static long& g_gel_bitmap_index;
	static real& g_render_light_intensity;
	static dword& g_debug_clip_planes;

protected:
	byte __data298[0xD54];
	byte __dataFEC[0x280];
	real m_light_intensity_scale;
};
static_assert(sizeof(c_lights_view) == sizeof(c_world_view) + 0xFD8);

// 0165E0D8
struct c_lightmap_shadows_view :
	public c_world_view
{
protected:
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
protected:

	// c_first_person_camera::update
	// c_first_person_view::override_projection
	// hs_external_globals[242]
	static real& m_fov_scale;

	// c_first_person_view::override_projection
	static real& m_z_far_scale; // from reach, missing `g_enable_first_person_squish && ` in if statement, always `1.0f`

	byte __data294[0x4];
};
static_assert(sizeof(c_first_person_view) == sizeof(c_view) + 0x4);

// 0165E130
struct c_player_view :
	public c_world_view
{
	static c_player_view*& x_current_player_view;
	static c_player_view(&x_global_player_views)[4];

	static c_player_view* __cdecl get_global_player_view()
	{
		return x_current_player_view;
	}

	static c_player_view* __cdecl get_global_player_view(long view_index)
	{
		return &x_global_player_views[view_index];
	}

	static void __cdecl set_global_player_view(c_player_view* view)
	{
		// asserts

		x_current_player_view = view;
	}

	static void __cdecl get_player_render_camera_orientation(real_matrix4x3* camera);

protected:
	// c_camera_fx_values?
	byte __data298[0x20];

	c_patchy_fog m_patchy_fog;

	// struct?
	real_point3d m_position;
	vector3d m_forward;
	vector3d m_up;
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
	long m_window_index;
	long m_output_user_index;
	long m_splitscreen_res;

	long __unknown26B0;

	// __unknown26B4 = player_window_index == iterator.m_window_count - 1
	bool __unknown26B4;
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
protected:
	c_rasterizer_texture_ref m_accumulation_texture_ref;
	c_rasterizer_texture_ref m_albedo_texture_ref;
	c_rasterizer_texture_ref m_normal_texture_ref;
	c_rasterizer_texture_ref __unknown26C4_texture_ref;
	c_rasterizer_texture_ref __unknown26C8_texture_ref;
	long __unknown26CC;
	long m_depth_stencil_surface_index;
	dword_flags __unknown26D4_flags;
	long __unknown26D8;
	long __unknown26DC;
	long __unknown26E0;
	long __unknown26E4;
};
static_assert(sizeof(c_texture_camera_view) == sizeof(c_player_view) + 0x30);

struct s_render_fullscreen_text_context_colors
{
	real_rgb_color blank_frame;
	real_rgb_color text_color;
	real_rgb_color text_shadow_color;
};

extern real(&pregame_frame_scales)[9];
extern s_render_fullscreen_text_context_colors(&pregame_frame_colors)[9];

extern void __cdecl render_debug_frame_render();

