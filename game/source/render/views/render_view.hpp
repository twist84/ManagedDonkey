#pragma once

#include "camera/observer.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_text.hpp"
#include "render/render_cameras.hpp"
#include "render/render_game_state.hpp"
#include "render/render_patchy_fog.hpp"
#include "visibility/visibility_collection.hpp"

__interface IDirect3DSurface9;
struct s_observer_result;

// 0165DB98
class c_view
{
protected:

	// HACK: so we don't have to manually construct the class
	struct
	{
		void(__thiscall* render)(c_view*);
		int32(__thiscall* render_setup)(c_view*);
		int32(__thiscall* compute_visibility)(c_view*);
		int32(__thiscall* render_submit_visibility)(c_view*);
	}* __vftable;

public:
	c_view() :
		__vftable(reinterpret_cast<decltype(__vftable)>(0x0165DB98)),
		m_rasterizer_camera(),
		m_rasterizer_projection(),
		m_render_camera(),
		m_render_projection()
	{
	}

	void render() { __vftable->render(this); }
	int32 render_setup() { return __vftable->render_setup(this); }
	int32 compute_visibility() { return __vftable->compute_visibility(this); }
	int32 render_submit_visibility() { return __vftable->render_submit_visibility(this); }

	static void __cdecl abort_current_view_stack();
	static void __cdecl begin(c_view* view);
	static void __cdecl sub_A28A40();
	static void __cdecl sub_A28A90();
	static void __cdecl end();
	static int32 __cdecl get_current_stack_level();
	static c_view* __cdecl top();

	const render_camera* get_render_camera() const;
	render_camera* get_render_camera_modifiable();

	const render_camera* get_rasterizer_camera() const;
	render_camera* get_rasterizer_camera_modifiable();

	const render_projection* get_rasterizer_projection() const;
	render_projection* get_rasterizer_projection_modifiable();

	const render_projection* get_render_projection() const;
	render_projection* get_render_projection_modifiable();

protected:
	static int32& g_view_stack_top;
	static c_view*(&g_view_stack)[4];

	render_camera m_rasterizer_camera;
	render_projection m_rasterizer_projection;
	render_camera m_render_camera;
	render_projection m_render_projection;
};
static_assert(sizeof(c_view) == 0x294);

// 0165DBAC
class c_ui_view :
	public c_view
{
public:
	c_ui_view() :
		c_view(),
		m_is_screenshot(),
		m_render_surface()
	{
		__vftable = reinterpret_cast<decltype(__vftable)>(0x0165DBAC);
	}

	void __thiscall render_();

	void setup_camera(const s_observer_result* result, c_rasterizer::e_surface surface);

	bool m_is_screenshot;
	c_rasterizer::e_surface m_render_surface;
};
static_assert(sizeof(c_ui_view) == sizeof(c_view) + 0x8);

// 0165DBC0
class c_fullscreen_view :
	public c_view
{
public:
	c_fullscreen_view() :
		c_view()
	{
		__vftable = reinterpret_cast<decltype(__vftable)>(0x0165DBC0);
	}

	void __thiscall render_();
	static void render_debug_stuff_while_loading();

	void setup_camera(const s_observer_result* observer);
	void render_blank_frame(const real_rgb_color* background_color);
};
static_assert(sizeof(c_fullscreen_view) == sizeof(c_view));

// 01692BDC
class c_world_view :
	public c_view
{
public:
	void get_starting_cluster(s_cluster_reference* starting_cluster);

//protected:
	bool m_using_stored_cluster;
	byte pad;
	s_cluster_reference m_stored_cluster;
};
static_assert(sizeof(c_world_view) == sizeof(c_view) + 0x4);
static_assert(0x0 == OFFSETOF(c_world_view, m_using_stored_cluster) - sizeof(c_view));
static_assert(0x1 == OFFSETOF(c_world_view, pad) - sizeof(c_view));
static_assert(0x2 == OFFSETOF(c_world_view, m_stored_cluster) - sizeof(c_view));

// 0165E0C4
class c_lights_view :
	public c_world_view
{
public:
	void submit_simple_light_draw_list_to_shader() const;
	void build_simple_light_draw_list(int32 a1);
	void clear_simple_light_draw_list(int32 a1);
	void render(int32 user_index, int32 player_index, IDirect3DSurface9* a3, IDirect3DSurface9* a4, IDirect3DSurface9* a5);

	static int32& g_gel_bitmap_index;
	static real32& g_render_light_intensity;
	static uns32& g_debug_clip_planes;

protected:
	struct s_simple_light
	{
		real_point3d m_position;
		real32 m_light_source_size;
		real_vector3d m_inv_direction;
		real32 m_sphere;
		real_rgb_color m_color;
		real32 m_cone_smooth;
		real32 m_distance_scale;
		real32 m_cone_scale;
		real32 m_distance_offset;
		real32 m_cone_offset;
		real32 m_bounding_radius2;
		real32 m_padding[3];
	};
	static_assert(sizeof(s_simple_light) == 0x50);

	IDirect3DSurface9* m_ldr_surface;
	IDirect3DSurface9* m_hdr_surface;
	IDirect3DSurface9* m_depth_surface;
	visibility_projection visibility_projections[6];
	int16 projection_count;
	real32 texture_projection_matrix[4][4];
	bool light_orthogonal;
	real32 light_near_width;
	real32 light_near_height;
	real32 light_near_depth;
	real32 light_far_depth;
	int32 shadow_res_x;
	int32 shadow_res_y;
	int32 m_user_index;
	int32 m_simple_light_count;
	s_simple_light m_local_light;
	s_simple_light m_simple_lights[8];
	real32 m_light_intensity_scale;
};
static_assert(sizeof(c_lights_view) == sizeof(c_world_view) + 0xFD8);

// 0165E0D8
class c_lightmap_shadows_view :
	public c_world_view
{
public:
	s_oriented_bounding_box m_projection_bounds;
};
static_assert(sizeof(c_lightmap_shadows_view) == sizeof(c_world_view) + 0x40);

// 0165E0EC
class c_occlusion_view :
	public c_lightmap_shadows_view
{
public:
};
static_assert(sizeof(c_occlusion_view) == sizeof(c_lightmap_shadows_view));

// 0165E108
class c_reflection_view :
	public c_view
{
public:
};
static_assert(sizeof(c_reflection_view) == sizeof(c_view));

// 0165E11C
class c_first_person_view :
	public c_view
{
public:
	void __thiscall override_projection(bool squish_close_to_camera);
	void __thiscall render_albedo(int32 user_index);

//protected:

	// c_first_person_camera::update
	// c_first_person_view::override_projection
	// hs_external_globals[242]
	static real32& m_fov_scale;

	// c_first_person_view::override_projection
	static real32& m_z_far_scale; // from reach, missing `g_enable_first_person_squish && ` in if statement, always `1.0f`

	render_camera* m_default_rasterizer_camera;
};
static_assert(sizeof(c_first_person_view) == sizeof(c_view) + 0x4);

// 0165E130
class c_player_view :
	public c_world_view
{
public:
	static c_player_view*& x_current_player_view;
	static c_player_view(&x_global_player_views)[4];

	static c_player_view* __cdecl get_current()
	{
		return x_current_player_view;
	}

	static c_player_view* __cdecl get_current(int32 view_index)
	{
		return &x_global_player_views[view_index];
	}

	static void __cdecl set_global_player_view(c_player_view* view)
	{
		// asserts

		x_current_player_view = view;
	}

	int32 get_player_view_user_index()
	{
		return m_camera_user_data.user_index;
	}

protected:
	void animate_water();

public:
	void __thiscall apply_distortions();
	void create_frame_textures(int32 player_index);

//protected:
	void __thiscall generate_distortions();
	static void __cdecl generate_distortions_callback(int32 __formal);

public:
	static void __cdecl get_player_render_camera_orientation(real_matrix4x3* camera);
	void __thiscall queue_patchy_fog();
	void __thiscall render_();
	bool __thiscall render_albedo();
	static void __cdecl render_albedo_decals(bool render_object_decals, bool render_structure_decals);
	void __thiscall render_effects(e_effect_pass effect_pass);
	void __thiscall render_first_person(bool render_only_transparents);
	void __thiscall render_first_person_albedo();
	void __thiscall render_lens_flares();
	void __thiscall render_lightmap_shadows();
	static void __cdecl render_misc_transparents();
	void __thiscall render_static_lighting();
	void __thiscall render_transparents();
	void __thiscall render_water();
	void __thiscall render_weather_occlusion();
	void restore_to_display_surface();
	void setup_camera(int32 player_window_index, int32 player_window_count, int32 player_window_arrangement, int32 user_index, const s_observer_result* observer, bool freeze_render_camera);
	void __thiscall setup_camera_fx_parameters(real32 exposure_boost);
	void __thiscall setup_cinematic_clip_planes();
	void __thiscall submit_attachments();
	void __thiscall submit_distortions();
	void __thiscall submit_occlusion_tests(bool occlusion, bool conditional);

	static void frame_advance();

//protected:
	struct s_camera_user_data
	{
		int32 player_window_index;
		int32 player_window_count;
		int32 player_window_arrangement;
		int32 user_index;
		e_controller_index controller_index;
		c_rasterizer::e_splitscreen_res m_splitscreen_res_index;
		int32 m_splitscreen_resolve_surface;
	};
	static_assert(sizeof(s_camera_user_data) == 0x1C);

	s_render_game_state::s_player_window* m_window_game_state;
	real32 m_render_exposure;
	real32 m_illum_render_scale;
	s_observer_depth_of_field m_observer_depth_of_field;
	c_patchy_fog m_patchy_fog;

	struct
	{
		real_point3d position;
		real_vector3d forward;
		real_vector3d up;
		real32 game_time;
		real_matrix4x3 view_matrix;
		real32 projection_matrix[4][4];
	} m_last_frame_motion_blur_state;

	c_first_person_view m_first_person_view;
	c_ui_view m_ui_view;
	c_lights_view m_lights_view;
	c_lightmap_shadows_view m_lightmap_shadows_view;
	c_reflection_view m_reflection_view;
	c_occlusion_view m_occlusion_view;
	rectangle2d m_final_surface_window_rect;
	s_camera_user_data m_camera_user_data;
	bool m_stall_cpu_to_wait_for_gpu;
};
static_assert(sizeof(c_player_view) == sizeof(c_world_view) + 0x2420);

class c_hud_camera_view :
	public c_player_view
{
public:
	c_hud_camera_view* constructor();

	void render(int32 player_index, const c_player_view* player_view, void* data);
};
static_assert(sizeof(c_hud_camera_view) == sizeof(c_player_view));

class c_texture_camera_view :
	public c_player_view
{
protected:
	c_rasterizer_texture_ref m_render_target_surface;
	c_rasterizer_texture_ref m_albedo_target_surface;
	c_rasterizer_texture_ref m_normal_target_surface;
	c_rasterizer_texture_ref __unknown26C4_texture_ref;
	c_rasterizer_texture_ref __unknown26C8_texture_ref;
	int32 m_cubemap_face;
	c_rasterizer::e_surface m_depth_surface;
	uns32 m_flags;
	int32 __unknown26D8;
	int32 m_user_index;
	void(__cdecl* m_render_callback)(int32);
	int32 m_callback_user_data;
};
static_assert(sizeof(c_texture_camera_view) == sizeof(c_player_view) + 0x30);

extern bool render_debug_pix_events;

extern void __cdecl render_debug_frame_render();
extern void __cdecl render_debug_window_render(int32 user_index);

