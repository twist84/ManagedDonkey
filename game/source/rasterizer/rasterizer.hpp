#pragma once

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "rasterizer/rasterizer_shader_definitions.hpp"
#include "rasterizer/rasterizer_text.hpp"
#include "rasterizer/rasterizer_vertex_definitions.hpp"
#include "tag_files/tag_groups.hpp"

enum _D3DRENDERSTATETYPE;
__interface IDirect3D9Ex;
__interface IDirect3DDevice9Ex;
__interface IDirect3DQuery9;
__interface IDirect3DVertexBuffer9;
__interface IDirect3DIndexBuffer9;
__interface IDirect3DVertexDeclaration9;

struct c_rasterizer_index_buffer
{
	enum e_primitive_type
	{
		_primitive_type_point_list = 0,
		_primitive_type_line_list,
		_primitive_type_list_strip,
		_primitive_type_triangle_list,
		_primitive_type_triangle_fan,
		_primitive_type_triangle_strip,
		_primitive_type_force_dword,

		k_primitive_type_count
	};

	//D3DPRIMITIVETYPE g_primitive_types[k_primitive_type_count]
	//{
	//	D3DPT_POINTLIST,
	//	D3DPT_LINELIST,
	//	D3DPT_LINESTRIP,
	//	D3DPT_TRIANGLELIST,
	//	D3DPT_TRIANGLEFAN,
	//	D3DPT_TRIANGLESTRIP,
	//	D3DPT_FORCE_DWORD
	//};
};

struct s_rasterizer_render_globals
{
	long resolution_width;
	long resolution_height;
	long back_buffer_width;
	long back_buffer_height;
	long resolution_offset_x;
	long resolution_offset_y;
	real resolution_scale_x;
	real resolution_scale_y;
	long occlusion_view_resolution;
	long window_width24;
	long window_height28;
	long width2C;
	long height30;
	long resolution_width34;
	long resolution_height38;
	void* window_handle;
	bool is_d3d9ex;
};
static_assert(sizeof(s_rasterizer_render_globals) == 0x44);

enum e_splitscreen_res;
struct c_rasterizer
{
	enum e_separate_alpha_blend_mode
	{
		_separate_alpha_blend_off = 0,
		_separate_alpha_blend_opaque,
		_separate_alpha_blend_additive,
		_separate_alpha_blend_multiply,
		_separate_alpha_blend_to_constant,

		k_number_of_separate_alpha_blend_modes
	};

	enum e_stencil_mode
	{
		_stencil_mode_off = 0,
		_stencil_mode_old_stipple,
		_stencil_mode_volume_back_planes,
		_stencil_mode_volume_front_planes,
		_stencil_mode_volume_apply,
		_stencil_mode_volume_apply_hi_stencil_only,
		_stencil_mode_volume_apply_and_clear,
		_stencil_mode_volume_clear,
		_stencil_mode_volume_clear_hi_stencil,
		_stencil_mode_decorators,
		_stencil_mode_decals,

		_stencil_mode_unknown11,
		_stencil_mode_unknown12,
		_stencil_mode_unknown13,
		_stencil_mode_unknown14,
		_stencil_mode_unknown15,
		_stencil_mode_unknown16,
		_stencil_mode_unknown17,
		_stencil_mode_unknown18,
		_stencil_mode_unknown19,
		_stencil_mode_unknown20,

		k_number_of_stencil_modes
	};

	enum e_gpr_allocation
	{
		_gpr_default_d3d = 0,
		_gpr_default,
		_gpr_max_to_pixel_shader,
		_gpr_max_to_vertex_shader,
		_gpr_all_to_vertex_shader,
		_gpr_decorator_allocation,
		_gpr_count
	};


	enum e_cull_mode
	{
		_cull_mode_none = 0x1,
		_cull_mode_cw = 0x2,
		_cull_mode_ccw = 0x3,
		_cull_mode_force_dword = 0x7FFFFFFF,
	};

	enum e_fill_mode
	{
		_fill_mode_point = 0x1,
		_fill_mode_wireframe = 0x2,
		_fill_mode_solid = 0x3,
		_fill_mode_force_dword = 0x7FFFFFFF,
	};

	enum e_alpha_blend_mode
	{
		_alpha_blend_opaque = 0,
		_alpha_blend_additive,
		_alpha_blend_multiply,
		_alpha_blend_alpha_blend,
		_alpha_blend_double_multiply,
		_alpha_blend_pre_multiplied_alpha,
		_alpha_blend_maximum,
		_alpha_blend_multiply_add,
		_alpha_blend_add_src_times_dstalpha,
		_alpha_blend_add_src_times_srcalpha,
		_alpha_blend_inv_alpha_blend,
		_alpha_blend_motion_blur_static,
		_alpha_blend_motion_blur_inhibit,

		k_number_of_alpha_blend_modes
	};

	enum e_surface
	{
		_surface_none = 0,
		_surface_disable,
		_surface_display,
		_surface_shadow_1,
		_surface_depth_stencil,
		_surface_depth_stencil_multisample,
		_surface_screenshot_composite_16f,
		_surface_accum_LDR,
		_surface_accum_HDR,
		_surface_albedo,
		_surface_albedo_debug,
		_surface_gbuf,
		_surface_normal,
		_surface_post_LDR,
		_surface_post_HDR,
		_surface_post_half_LDR,
		_surface_post_half_HDR,
		_surface_normal_half,
		_surface_depth_half_fp32,
		_surface_color_half_fp16_0,
		_surface_color_half_fp16_1,
		_surface_fullscreen_blur,
		_surface_shadow_color,
		_surface_depth_fp32,
		_surface_aux_reflection,
		_surface_aux_refraction,
		_surface_hdr_retrieve,
		_surface_ssao_noise,
		_surface_screenshot_composite_8bit,
		_surface_screenshot_display,
		_surface_screenshot_composite_16i,
		_surface_aux_bloom,
		_surface_aux_chud,
		_surface_aux_star,
		_surface_aux_small,
		_surface_aux_tiny,
		_surface_aux_mini,
		_surface_aux_exposure_0,
		_surface_aux_exposure_1,
		_surface_aux_exposure_2,
		_surface_aux_exposure_3,
		_surface_aux_exposure_4,
		_surface_aux_exposure_5,
		_surface_aux_exposure_6,
		_surface_aux_exposure_7,
		_surface_aux_small2,
		_surface_aux_tiny2,
		_surface_aux_mini2,
		_surface_aux_depth_of_field2,
		_surface_aux_depth_of_field,
		_surface_distortion,
		_surface_depth_camera_depth,
		_surface_depth_camera,
		_surface_aux_water_interaction_height,
		_surface_aux_water_interaction_slope,
		_surface_chud_turbulence,
		_surface_cortana_effect_mua_haha,
		_surface_surface_depth_camera_texture,
		_surface_surface_weather_occlusion,
		_surface_surface_depth_visr,

		k_number_of_surfaces
	};

	enum e_sampler_address_mode
	{
		_sampler_address_wrap = 0,
		_sampler_address_clamp,
		_sampler_address_mirror,
		_sampler_address_border,

		k_number_of_sampler_address_modes
	};

	enum e_sampler_filter_mode
	{
		_sampler_filter_mode_trilinear = 0,
		_sampler_filter_mode_point,
		_sampler_filter_mode_bilinear,
		_sampler_filter_mode_unused3,
		_sampler_filter_mode_anisotropic_2x,
		_sampler_filter_mode_unused5,
		_sampler_filter_mode_anisotropic_4x,
		_sampler_filter_mode_lightprobe_texture_array,
		_sampler_filter_mode_texture_array_quadlinear,
		_sampler_filter_mode_texture_array_quadanisotropic_2x,

		k_sampler_filter_modes
	};

	enum e_z_buffer_mode
	{
		_z_buffer_mode_write = 0,
		_z_buffer_mode_read,
		_z_buffer_mode_off,
		_z_buffer_mode_shadow_generate,
		_z_buffer_mode_shadow_generate_dynamic_lights,
		_z_buffer_mode_shadow_apply,
		_z_buffer_mode_decals,
		_z_buffer_mode_unknown7,
		_z_buffer_mode_unknown8,

		k_number_of_z_buffer_modes
	};

	enum e_platform
	{
		_platform_xenon = 0,
		_platform_dx9,

		k_platform_count
	};

	struct s_stream_source
	{
		IDirect3DVertexBuffer9* data;
		unsigned int offset;
		unsigned int stride;
	};
	static_assert(sizeof(s_stream_source) == 0xC);

	static void __cdecl begin(rectangle2d viewport, rectangle2d scissor_rect);
	static bool __cdecl cleanup_before_device_reset();
	static void __cdecl cleanup_d3d_states(bool wait_for_gpu_idle_);
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl dispose_from_old_structure_bsp(dword deactivating_structure_bsp_mask);
	static real __cdecl get_aspect_ratio();
	static void __cdecl get_display_pixel_bounds(rectangle2d* display_pixel_bounds);
	static void __cdecl get_display_title_safe_pixel_bounds(rectangle2d* resolution);
	static void __cdecl get_fullscreen_render_pixel_bounds(rectangle2d* resolution);
	static void __cdecl get_fullscreen_render_title_safe_pixel_bounds(rectangle2d* resolution);
	static void __cdecl initialize();
	static bool __cdecl initialize_after_device_creation_or_reset();
	static void __cdecl initialize_for_new_map();
	static void __cdecl initialize_for_new_structure_bsp(dword activating_structure_bsp_mask);
	static void __cdecl notify_game_tick_begin();
	static void __cdecl restore_last_scissor_rect();
	static void __cdecl restore_last_viewport();
	static void __cdecl shell_dispose();
	static void __cdecl shell_initialize(bool window_exists, bool windowed);
	static void __cdecl window_reset();
	static void __cdecl set_render_resolution(long width, long height, bool fullscreen);
	static bool __cdecl test_cooperative_level();
	static bool __cdecl reset_device();
	static bool __cdecl begin_frame();
	static void __cdecl begin_high_quality_blend();
	//static void __cdecl clearf(dword, real_vector4d const*, real, dword);
	static bool __cdecl end_frame();
	static void __cdecl end_high_quality_blend();
	static bool __cdecl sub_A218C0();
	static bool __cdecl get_is_tiling_enabled();
	static e_platform __cdecl get_runtime_platform();
	static bool __cdecl initialize_device(bool window_exists, bool windowed);
	static void __cdecl rasterizer_device_acquire_thread();
	static void __cdecl rasterizer_device_release_thread();
	static bool __cdecl rasterizer_thread_owns_device();
	static void __cdecl initialize_window();
	static e_gpr_allocation __cdecl set_gprs_allocation(e_gpr_allocation type);
	static void __cdecl clear_sampler_textures(dword);
	static e_cull_mode __cdecl get_cull_mode(void);
	static void __cdecl set_alpha_blend_mode(e_alpha_blend_mode);
	static void __cdecl set_alpha_blend_mode_custom_device_no_cache(IDirect3DDevice9Ex*, e_alpha_blend_mode);
	static void __cdecl set_color_write_enable(long, long);
	static bool __cdecl surface_valid(e_surface surface);
	static bool __cdecl set_compiled_pixel_shader(c_rasterizer_compiled_pixel_shader const*, e_entry_point);
	static bool __cdecl set_compiled_vertex_shader(c_rasterizer_compiled_vertex_shader const*, e_vertex_type, e_transfer_vector_vertex_types, e_entry_point);
	static void __cdecl set_cull_mode(e_cull_mode);
	static bool __cdecl set_explicit_shaders(long, e_vertex_type, e_transfer_vector_vertex_types, e_entry_point);
	static void __cdecl set_fill_mode(e_fill_mode);
	static void __cdecl set_indices(IDirect3DIndexBuffer9*);
	static bool __cdecl set_pixel_shader(c_rasterizer_pixel_shader const*, e_entry_point);
	static void __cdecl set_sampler_texture_direct(long, e_surface);
	static void __cdecl set_sampler_address_mode(long, e_sampler_address_mode);
	static void __cdecl set_sampler_filter_mode(long, e_sampler_filter_mode);
	static void __cdecl set_sampler_texture(long, c_rasterizer_texture_ref);
	static void __cdecl set_scissor_rect(rectangle2d const*);
	static void __cdecl set_separate_alpha_blend_mode(e_separate_alpha_blend_mode);
	static void __cdecl set_stencil_mode(e_stencil_mode);
	static void __cdecl set_stencil_mode_with_value(e_stencil_mode, byte);
	static bool __cdecl set_vertex_declaration(IDirect3DVertexDeclaration9*);
	static bool __cdecl set_vertex_shader(c_rasterizer_vertex_shader const*, e_vertex_type, e_transfer_vector_vertex_types, e_entry_point);
	static void __cdecl set_z_buffer_mode(e_z_buffer_mode);
	static bool __cdecl get_is_using_floating_point_depth_buffer();

	static void __cdecl setup_occlusion_state();
	static void __cdecl setup_render_target_globals_with_exposure(real view_exposure, real illum_scale, real HDR_target_stops, bool alpha_blend);
	static void __cdecl setup_render_target_globals_with_exposure_for_texture_camera_only(real view_exposure, real illum_scale, real HDR_target_stops, bool alpha_blend);
	static void __cdecl setup_targets_albedo(bool clear_stencil, bool is_clear);
	static void __cdecl setup_targets_distortion(rectangle2d* pixel_bounds, bool depth_test);
	static void __cdecl setup_targets_simple();
	static void __cdecl setup_targets_static_lighting(real view_exposure, real illum_scale, bool render_to_HDR_target, real HDR_target_stops, bool clear, bool copy_albedo_pc, bool a7);
	static void __cdecl setup_targets_static_lighting_alpha_blend(bool render_to_HDR_target, bool alpha_blend);

	static void __cdecl draw_indexed_primitive(c_rasterizer_index_buffer const* indices, long base_vertex_index, long num_vertices, long min_index, long triangle_count);
	static void __cdecl draw_primitive(c_rasterizer_index_buffer::e_primitive_type primitive_type, long start_vertex, long primitive_count);
	static void __cdecl draw_primitive_up(c_rasterizer_index_buffer::e_primitive_type primitive_type, dword primitive_count, void const* stream_data, dword stride);
	static void __cdecl draw_vertices(c_rasterizer_index_buffer::e_primitive_type primitive_type, long start_vertex, long primitive_count);

	static void __cdecl set_current_splitscreen_res(e_splitscreen_res res);

	static e_surface __cdecl get_render_target(long surface_index);

	static e_surface sub_A48770();

	static void __cdecl resolve_entire_surface(e_surface surface, long a2, rectangle2d* a3, short a4, short a5);
	static void __cdecl set_depth_stencil_surface(e_surface depth_stencil);
	static void __cdecl set_render_target(long surface_index, e_surface surface, long force_is_srgb);
	static void __cdecl set_using_albedo_sampler(bool value);
	static void __cdecl set_viewport(rectangle2d const& viewport, real min_z, real max_z);
	static void __cdecl wait_for_gpu_idle();

	static void __cdecl draw_debug_line2d(real_point3d const& p0, real_point3d const& p1, dword color0, dword color1);
	static void __cdecl draw_debug_line(real_point3d const& p0, real_point3d const& p1, dword color0, dword color1);
	static void __cdecl draw_debug_line_list2d_explicit(rasterizer_vertex_debug const* vertex_debug, long primitive_count);
	static void __cdecl draw_debug_line_list_explicit(rasterizer_vertex_debug const* vertex_debug, long primitive_count);
	static void __cdecl draw_debug_linestrip2d(point2d const* points, long point_count, dword color);
	static void __cdecl draw_debug_polygon2d(rasterizer_vertex_debug const* polygon2d, long primitive_count, c_rasterizer_index_buffer::e_primitive_type primitive_type);
	static void __cdecl draw_debug_polygon(rasterizer_vertex_debug const* polygon, long primitive_count, c_rasterizer_index_buffer::e_primitive_type primitive_type);
	static void __cdecl draw_fullscreen_quad(int width, int height);
	static void __cdecl draw_fullscreen_quad_with_texture_xform(int width, int height, real_rectangle2d const* bounds);
	static void __cdecl draw_textured_screen_quad(real a1, real a2, real a3, real a4);
	static void __cdecl draw_textured_screen_quad(rasterizer_vertex_screen const* textured_screen_quad, bool a2);
	static void __cdecl draw_textured_screen_triangle_list(rasterizer_vertex_screen const* textured_screen_triangle_list, long primitive_count);
	static void __cdecl draw_textured_transparent_polygon(rasterizer_vertex_transparent const* textured_transparent_polygon, long polygon_count, e_alpha_blend_mode alpha_blend_mode);
	static void __cdecl draw_textured_transparent_quad(rasterizer_vertex_transparent const* textured_transparent_quad, e_alpha_blend_mode alpha_blend_mode);
	static void __cdecl draw_worldspace_polygon(real_point3d const* worldspace_polygon, long polygon_count);
	static void __cdecl draw_worldspace_polygon(rasterizer_vertex_world const* worldspace_polygon, long polygon_count);

	static void __cdecl set_pixel_shader_constant(long constant_index, long count, real_vector4d const* constants);
	static void __cdecl set_pixel_shader_constant_bool(long constant_index, long count, int const* constants);
	static void __cdecl set_pixel_shader_constant_int(long constant_index, long vector_count, int const* constants);
	static void __cdecl set_pixel_shader_constant_single(long constant_index, real value);
	static void __cdecl set_vertex_shader_constant(long constant_index, long vector_count, real_vector4d const* constants);
	static void __cdecl set_vertex_shader_constant_bool(long constant_index, long count, int const* constants);
	static void __cdecl set_vertex_shader_constant_int(long start_register, long vector4i_count, int const* constant_data);
	static void __cdecl set_vertex_shader_constant_owned(long constant_index, long count, real_vector4d const* constants);

	static bool& g_d3d_device_is_lost;
	static bool& g_d3d_device_reset;
	static _D3DRENDERSTATETYPE(&x_last_render_state_types)[4];
	static dword& g_render_thread;

	static s_rasterizer_render_globals& render_globals;

	static dword& g_render_thread_begin_scene;

	static IDirect3D9Ex*& g_direct3d;
	static IDirect3DDevice9Ex*& g_device;

	static bool(&byte_50DADE0)[3];
	static IDirect3DQuery9* (&dword_50DADE4)[3];

	static e_alpha_blend_mode& g_current_alpha_blend_mode;
	static e_separate_alpha_blend_mode& g_current_separate_alpha_blend_mode;
	static e_z_buffer_mode& g_current_z_buffer_mode;
	static bool& g_current_z_buffer_floating_point;
	static byte& g_current_stencil_value;
	static bool& initialized;
	static bool& m_use_floating_point_z_buffer;
	static e_stencil_mode& g_current_stencil_mode;
	static bool& g_lock_clip_plane;
	static long(&x_last_render_state_value)[4];
	static e_cull_mode& g_current_cull_mode;
	static IDirect3DVertexShader9*& g_current_vertex_shader;
	static IDirect3DPixelShader9*& g_current_pixel_shader;
	static IDirect3DVertexDeclaration9*& g_current_vertex_declaration;
	static IDirect3DIndexBuffer9*& g_current_index_buffer;
	static s_stream_source(&x_last_stream_source)[16];

	static e_surface& g_depth_stencil_surface;
	static e_surface(&g_color_surfaces)[4];

	static bool& g_tiling_force_4x_msaa;
	static e_splitscreen_res& g_current_splitscreen_res;

	static rectangle2d& g_last_viewport;
	static rectangle2d& g_last_scissor_rect;

	static dword& g_max_vs_gprs;
	static dword& g_max_ps_gprs;
	static long& g_adapter;

	static void __cdecl clearf(dword flags, dword color, real z, byte stencil);
	static void __cdecl sub_A21440();
	static void __cdecl end();
	static bool __cdecl end_albedo(rectangle2d const* bounds);
};

struct s_global_bitmaps;
struct s_texture_references_block;
struct c_rasterizer_globals
{
public:
	static tag const k_group_tag = RASTERIZER_GLOBALS_TAG;

	enum e_explicit_shader
	{
		_shader_hack_au2_rank_tray = -3,
		_shader_hack_au2_playlist_rating = -2,

		_shader_debug = 0,
		_shader_debug2d,
		_shader_copy_scaled,
		_shader_bloom_spike_vertical,
		_shader_bloom_spike_horizontal,
		_shader_bloom_downsample_4x4_calc_bloom,
		_shader_bloom_downsample_4x4,
		_shader_bloom_rgbe_to_rgb,
		_shader_bloom_update_persistence,
		_shader_bloom_add_downsampled,
		_shader_bloom_add,
		_shader_bloom_blur_kernel_11_horizontal,
		_shader_bloom_blur_kernel_11_vertical,
		_shader_cubemap_blur_phi,
		_shader_cubemap_blur_theta,
		_shader_cubemap_clamp,
		_shader_cubemap_divide,
		_shader_misc_write_depth,
		_shader_postprocess_final_composite,
		_shader_sky_dome_simple,
		_shader_transparent,
		_shader_shield_meter,
		_shader_legacy_meter,
		_shader_overhead_map_geometry,
		_shader_legacy_hud_bitmap,
		_shader_bloom_blend3,
		_shader_particle_update,
		_shader_particle_spawn,
		_shader_screenshot_combine,
		_shader_downsample_2x2,
		_shader_rotate_2d,
		_shader_bspline_resample,
		_shader_bloom_downsample_4x4_bloom_DOF,
		_shader_postprocess_final_composite_DOF,
		_shader_convolve_kernel_5,
		_shader_exposure_downsample,
		_shader_yuv_to_rgb,
		_shader_displacement,
		_shader_screenshot_display,
		_shader_downsample_4x4_block,
		_shader_crop,
		_shader_screenshot_combine_DOF,
		_shader_gamma_correct,
		_shader_contrail_spawn,
		_shader_contrail_update,
		_shader_8x8_dither_pattern,
		_shader_lens_flare_render,
		_shader_decorator_render_default,
		_shader_bloom_downsample_4x4_block_bloom,
		_shader_bloom_downsample_4x4_gaussian,
		_shader_apply_color_matrix,
		_shader_copy,
		_shader_shadow_geometry,
		_shader_shadow_apply,
		_shader_gradient,
		_shader_alpha_test,
		_shader_patchy_fog,
		_shader_light_volume_update,
		_shader_water_ripple,
		_shader_double_gradient,
		_shader_sniper_scope,
		_shader_shield_impact,
		_shader_player_emblem_world,
		_shader_player_emblem_screen,
		_shader_implicit_hill,
		_shader_chud_overlay_blend,
		_shader_bloom_add_alpha1,
		_shader_bloom_downsample_4x4_block_bloom_LDR,
		_shader_ldr_hdr_depth_restore,
		_shader_beam_update,
		_shader_decorator_render_no_wind,
		_shader_decorator_render_static,
		_shader_decorator_render_sun,
		_shader_decorator_render_wavy,
		_shader_postprocess_final_composite_zoom,
		_shader_postprocess_final_composite_debug,
		_shader_shadow_apply_point,
		_shader_shadow_apply_bilinear,
		_shader_shadow_apply_fancy,
		_shader_shadow_apply_faster,
		_shader_shadow_apply_misc,
		_shader_displacement_motion_blur,
		_shader_decorator_render_shaded,
		_shader_screenshot_memexport,
		_shader_bloom_downsample_4x4_gaussian_bloom_LDR,
		_shader_bloom_downsample_4x4_gaussian_bloom,
		_shader_bloom_downsample_4x4_gaussian_bloom_new,
		_shader_bloom_curve,
		_shader_custom_gamma_correct,
		_shader_pixel_copy,
		_shader_decorator_render_edit,
		_shader_hdr_retrieve,
		_shader_smirnov,
		_shader_fxaa,
		_shader_unknown94,
		_shader_sniper_scope_stencil_pc,
		_shader_ssao,
		_shader_ssao_blur,
		_shader_unknown98,
		_shader_lightshafts,
		_shader_radial_blur,
		_shader_unknown101,
		_shader_unknown102,
		_shader_unknown103,
		_shader_unknown104,
		_shader_unknown105,
		_shader_unknown106,
		_shader_unknown107,
		_shader_hud_camera_nightvision,
		_shader_unknown109,

		k_explicit_shader_count
	};

	struct s_explicit_shader
	{
		c_typed_tag_reference<VERTEX_SHADER_TAG, INVALID_TAG> explicit_vertex_shader;
		c_typed_tag_reference<PIXEL_SHADER_TAG, INVALID_TAG> explicit_pixel_shader;

		void update_reference_names();
	};
	static_assert(sizeof(s_explicit_shader) == 0x20);

	dword __cdecl get_max_vs_gprs() const
	{
		return m_max_vs_gprs;
	}

	dword __cdecl get_max_ps_gprs() const
	{
		return m_max_ps_gprs;
	}

	void update_reference_names();

protected:

	// default bitmaps
	//	0:  ............... default 2D texture (white)
	//	1:  ............... default normal map (no bump)
	//	2:  ............... default cube map (error pattern)
	//	3:  ............... color bars
	//	4:  ............... black 2D texture
	//	5:  ............... transparent black 2D texture
	//	6:  ............... gray 2D texture
	//	7:  ............... auto exposure weight
	//	8:  ............... auto exposure weight (4x3)
	//	9:  ............... stencil dither pattern
	//	10: ............... smooth noise warp
	//	11: ............... ripple pattern
	c_typed_tag_block<s_global_bitmaps> default_bitmaps;

	// material textures
	//	0: ............... SH Glossy CT CC0236
	//	1: ............... SH Glossy CT C78D78
	//	2: ............... SH Glossy CT DD0236
	//	3: ............... TBD
	//	4: ............... TBD
	//	5: ............... TBD
	c_typed_tag_block<s_texture_references_block> material_textures;

	// default shader vertex shader
	//	Renderer uses this shader when a vertex shader is invalid
	c_typed_tag_reference<VERTEX_SHADER_TAG, INVALID_TAG> default_vertex_shader;

	// default pixel shader
	//	Renderer uses this shader when a pixel shader is invalid
	c_typed_tag_reference<PIXEL_SHADER_TAG, INVALID_TAG> default_pixel_shader;

	// debug override shader
	//	When this shader is set, it overrides all shaders coming from geometry.
	//	However, if you leave a field as default in this shader, that field will not override the corresponding field in user shaders.
	//	For example, to render everything with a white albedo texture, select a white albedo texture in this shader
	//	and leave everything as defaults.

	// explicit shaders
	//	0: ............... debug
	//	1: ............... debug2d
	//	2: ............... copy w/scale
	//	3: ............... bloom: spike blur (vertical)
	//	4: ............... bloom: spike blur (horizontal)
	//	5: ............... bloom: downsample 4x4 (with bloom curve)
	//	6: ............... bloom: downsample 4x4
	//	7: ............... bloom: copy RGBE -> RGB
	//	8: ............... bloom: update persistence
	//	9: ............... bloom: add downsampled
	//	10: .............. bloom: add
	//	11: .............. bloom: kernel blur (11 horizontal)
	//	12: .............. bloom: kernel blur (11 vertical)
	//	13: .............. cubemap: blur (phi)
	//	14: .............. cubemap: blur (theta)
	//	15: .............. cubemap: clamp
	//	16: .............. cubemap: divide
	//	17: .............. write depth
	//	18: .............. final composite
	//	19: .............. sky dome (simple)
	//	20: .............. transparent
	//	21: .............. (legacy) shield meter
	//	22: .............. (legacy) general meter
	//	23: .............. overhead map geometry
	//	24: .............. (legacy) hud bitmap
	//	25: .............. bloom: blend 3 textures
	//	26: .............. particle update
	//	27: .............. particle spawn
	//	28: .............. screenshot combine
	//	29: .............. downsample 2x2
	//	30: .............. rotate 2d
	//	31: .............. bspline resample
	//	32: .............. bloom; downsample 4x4 (with bloom curve + DOF)
	//	33: .............. final composite (with DOF)
	//	34: .............. convolve 5-tap kernel
	//	35: .............. exposure downsample
	//	36: .............. yuv to rgb
	//	37: .............. displacement
	//	38: .............. screenshot display
	//	39: .............. downsample 4x4 block
	//	40: .............. crop
	//	41: .............. screenshot combine (with DOF)
	//	42: .............. gamma correction
	//	43: .............. contrail spawn
	//	44: .............. contrail update
	//	45: .............. stencil 8x8 dither pattern
	//	46: .............. lens flare render
	//	47: .............. decorator render
	//	48: .............. bloom: NEW downsample 4x4 block (with bloom curve)
	//	49: .............. bloom: NEW downsample 4x4 gaussian
	//	50: .............. apply color matrix
	//	51: .............. copy
	//	52: .............. shadow geometry
	//	53: .............. shadow apply
	//	54: .............. gradient
	//	55: .............. alpha test
	//	56: .............. patchy fog
	//	57: .............. light volume
	//	58: .............. water ripple
	//	59: .............. double gradient
	//	60: .............. sniper scope
	//	61: .............. shield impact
	//	62: .............. player emblem-world
	//	63: .............. player emblem-screen
	//	64: .............. implicit hill
	//	65: .............. chud overlay blend
	//	66: .............. bloom add alpha1
	//	67: .............. bloom: NEW downsample 4x4 block (with bloom curve) LDR
	//	68: .............. restore LDR, HDR, and depth to EDRAM
	//	69: .............. beam
	//	70: .............. decorators no wind
	//	71: .............. decorators static
	//	72: .............. decorators sun only
	//	73: .............. decorators wavy
	//	74: .............. final composite zoom
	//	75: .............. final composite debug
	//	76: .............. 
	//	77: .............. 
	//	78: .............. 
	//	79: .............. 
	//	80: .............. 
	//	81: .............. displacement with motion blur
	//	82: .............. 
	//	83: .............. screenshot memexport
	//	84: .............. bloom: NEW downsample 4x4 gauss with curve LDR
	//	85: .............. bloom_downsample_4x4_gaussian_bloom
	//	86: .............. bloom_downsample_4x4_gaussian_bloom new
	//	87: .............. bloom_curve
	//	88: .............. custom gamma correct
	//	89: .............. pixel copy
	//	90: .............. decorators edit
	//	91: .............. hdr retrieve
	//	92: .............. smirnov test
	//	93: .............. fxaa
	//	94: .............. 
	//	95: .............. sniper scope stencil pc
	//	96: .............. ssao
	//	97: .............. ssao_blur
	//	98: .............. 
	//	99: .............. lightshafts
	//	100: ............. radial_blur
	//	101: ............. 
	//	102: ............. 
	//	103: ............. 
	//	104: ............. 
	//	105: ............. 
	//	106: ............. 
	//	107: ............. 
	//	108: ............. hud night vision
	//	109: ............. 
	c_typed_tag_block<s_explicit_shader> explicit_shaders;

	// atmosphere lookup tables
	//	0: ............... 1D Neta Table
	//	1: ............... TBD
	//	2: ............... TBD
	//	3: ............... TBD
	//	4: ............... TBD
	//	5: ............... TBD
	c_typed_tag_block<s_texture_references_block> atmosphere_lookup_tables;

	// runtime
	dword m_max_vs_gprs;
	dword m_max_ps_gprs;

	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> active_camo_distortion_texture;
	c_typed_tag_reference<PERFORMANCE_THROTTLES_TAG, INVALID_TAG> default_performance_throttles;
	c_typed_tag_reference<SHIELD_IMPACT_TAG, INVALID_TAG> shield_impact_settings;
	c_typed_tag_reference<VISION_MODE_TAG, INVALID_TAG> vision_mode_settings;

	// Motion blur parameters
	//	max blur:............  max amount to blur, as a percentage of the screen
	//	blur scale:..........  scales blur for a given velocity
	//	center falloff:....... reduces motion blur around the screen center (larger values give smaller reduction regions...)
	//	expected dt:.......... expected time per tick, in seconds (used to correct motion blur in fast/slow frames)

	long number_of_taps;
	real max_blur_x;
	real max_blur_y;
	real blur_scale_x;
	real blur_scale_y;
	real center_falloff;
	real expected_dt;

	byte __padB4[4];
};

struct s_global_bitmaps
{
	dword_flags options;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> default_bitmaps;

	void update_reference_names();
};
static_assert(sizeof(s_global_bitmaps) == 0x14);

struct s_texture_references_block
{
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> reference;

	void update_reference_names();
};
static_assert(sizeof(s_texture_references_block) == sizeof(s_tag_reference));

extern void rasterizer_reset_device();

extern void __cdecl draw_tesselated_quad();
extern bool __cdecl rasterizer_initialized();
extern void __cdecl rasterizer_quad_screenspace(point2d const(&points)[4], dword color, s_tag_reference const* reference, short bitmap_index, bool a5);
extern bool __cdecl rasterizer_set_explicit_debug_shader(c_rasterizer_globals::e_explicit_shader explicit_shader);

extern bool rasterizer_dump_display_to_bmp(char const* file_name);

