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

namespace DirectX
{
	struct XMMATRIX;
}

enum
{
	//k_viewproj_xform = ?,
	//k_view_xform = ?,
	//k_screen_xform = ?,
	//k_clip_plane = ?,
	//k_vs_always_true = ?,
	//k_viewport_scale = ?,
	//k_viewport_scale_pad = ?,
	//k_viewport_offset = ?,
	//k_viewport_offset_pad = ?,
	//k_viewproj_xform_x = ?,
	//k_viewproj_xform_y = ?,
	//k_viewproj_xform_z = ?,
	//k_viewproj_xform_w = ?,
	//k_camera_forward = ?,
	//k_camera_left = ?,
	//k_camera_up = ?,
	//k_camera_position = ?,
	//k_screen_xform_x = ?,
	//k_screen_xform_y = ?,
	//k_vs_exposure = ?,
	//k_vs_alt_exposure = ?,
	//k_vs_atmosphere_constant_0 = ?,
	//k_vs_atmosphere_constant_1 = ?,
	//k_vs_atmosphere_constant_2 = ?,
	//k_vs_atmosphere_constant_3 = ?,
	//k_vs_atmosphere_constant_4 = ?,
	//k_vs_atmosphere_constant_5 = ?,
	//k_vs_atmosphere_constant_extra = ?,
	//k_vs_lighting_constant_0 = ?,
	//k_vs_lighting_constant_1 = ?,
	//k_vs_lighting_constant_2 = ?,
	//k_vs_lighting_constant_3 = ?,
	//k_vs_lighting_constant_4 = ?,
	//k_vs_lighting_constant_5 = ?,
	//k_vs_lighting_constant_6 = ?,
	//k_vs_lighting_constant_7 = ?,
	//k_vs_lighting_constant_8 = ?,
	//k_vs_lighting_constant_9 = ?,
	//k_vs_dynamic_light_clip_planes = ?,
	//k_vs_shadow_projection = ?,
	//k_register_camera_position_ps = ?,
	//k_ps_exposure = ?,
	//k_ps_alt_exposure = ?,
	//k_ps_lighting_constant_0 = ?,
	//k_ps_lighting_constant_1 = ?,
	//k_ps_lighting_constant_2 = ?,
	//k_ps_lighting_constant_3 = ?,
	//k_ps_lighting_constant_4 = ?,
	//k_ps_lighting_constant_5 = ?,
	//k_ps_lighting_constant_6 = ?,
	//k_ps_lighting_constant_7 = ?,
	//k_ps_lighting_constant_8 = ?,
	//k_ps_lighting_constant_9 = ?,
	//k_ps_dynamic_light_gel_xform = ?,
	//k_ps_texture_size = ?,
	//k_ps_texture_size_pad = ?,
	//k_ps_dynamic_environment_blend = ?,
	//k_ps_render_debug_mode = ?,
	//k_shader_pc_specular_enabled = ?,
	//k_shader_pc_specular_enabled_pad = ?,
	//k_shader_pc_albedo_lighting = ?,
	//k_shader_pc_albedo_lighting_pad = ?,
	k_ps_ldr_gamma2 = 14,
	//k_ps_hdr_gamma2 = ?,
	k_ps_actually_calc_albedo = 12,
	//k_ps_lightmap_compress_constant_using_dxt = ?,
	//k_ps_lightmap_compress_constant_0 = ?,
	//k_ps_lightmap_compress_constant_1 = ?,
	//k_ps_lightmap_compress_constant_2 = ?,
	//k_register_simple_light_count = ?,
	//k_register_simple_light_count_pad = ?,
	//k_register_simple_light_start = ?,
	//k_vs_sampler_atmosphere_neta_table = ?,
	//k_vs_sampler_weather_occlusion = ?,
	//k_sampler_lightprobe_texture_array = ?,
	//k_sampler_dominant_light_intensity_map = ?,
	//k_sampler_scene_ldr_texture = ?,
	//k_sampler_albedo_texture = ?,
	//k_sampler_normal_texture = ?,
	//k_sampler_depth_buffer = ?,
};

class c_rasterizer_index_buffer
{
public:
	enum e_primitive_type
	{
		_primitive_type_point_list = 0,

		_primitive_type_line_list,
		_primitive_type_list_strip,

		_primitive_type_triangle_list,
		_primitive_type_triangle_fan,
		_primitive_type_triangle_strip,

		_primitive_type_quad_list,

		k_number_of_primitive_types
	};

	//D3DPRIMITIVETYPE g_primitive_types[k_number_of_primitive_types]
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
	int32 resolution_width;
	int32 resolution_height;
	int32 back_buffer_width;
	int32 back_buffer_height;
	int32 resolution_offset_x;
	int32 resolution_offset_y;
	real32 resolution_scale_x;
	real32 resolution_scale_y;
	int32 k_shadow_resolution;
	int32 window_width24;
	int32 window_height28;
	int32 width2C;
	int32 height30;
	int32 resolution_width34;
	int32 resolution_height38;
	void* window_handle;
	bool is_d3d9ex;
};
COMPILE_ASSERT(sizeof(s_rasterizer_render_globals) == 0x44);

class c_rasterizer
{
public:
	enum
	{
		k_number_of_color_surfaces = 4,
	};

	enum e_splitscreen_res
	{
		_res_default = 0,
		_res_half_width,
		_res_half_size,
		_res_8_x_3_half_height,

		k_number_of_splitscreen_res_resolutions
	};

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

		_stencil_mode_tron_write,
		_stencil_mode_tron_read,
		_stencil_mode_ambient_object_clear_shadow_bit,
		_stencil_mode_glass_decals_write,
		_stencil_mode_glass_decals_read,
		_stencil_mode_object_rendering,

		_stencil_mode_unknown17, // _stencil_mode_tron_read_pc
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
		_cull_mode_off = 1,
		_cull_mode_cw = 2,
		_cull_mode_ccw = 3,
		_cull_mode_force_dword = 0x7FFFFFFF,
	};

	enum e_fill_mode
	{
		_fill_mode_point = 1,
		_fill_mode_wireframe = 2,
		_fill_mode_solid = 3,
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
		_surface_depth_stencil_read_only,
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
		_surface_aux_chud_overlay,
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
		_surface_aux_depth_of_field_high_rez,
		_surface_aux_depth_of_field_high_rez2,
		_surface_distortion,
		_surface_depth_camera_depth,
		_surface_depth_camera,
		_surface_aux_water_interaction_height,
		_surface_aux_water_interaction_slope,
		_surface_chud_turbulence,
		_surface_cortana_effect_mua_haha,
		_surface_depth_camera_texture,
		_surface_weather_occlusion,
		_surface_depth_visr,

		//_surface_dynamic_albedo,
		//_surface_dynamic_accumulation,
		//_surface_dynamic_normal,
		//_surface_dynamic_depth_stencil,
		//_surface_dynamic_depth_stencil_read_only,

		k_number_of_surfaces,
		k_surface_aux_exposure_count = 8,
	};

	enum e_sampler_address_mode
	{
		_sampler_address_wrap = 0,
		_sampler_address_clamp,
		_sampler_address_mirror,
		_sampler_address_border,
		_sampler_address_mirror_once,
		_sampler_address_mirror_once_border,

		k_number_of_sampler_address_modes
	};

	enum e_sampler_filter_mode
	{
		_sampler_filter_trilinear = 0,
		_sampler_filter_point,
		_sampler_filter_bilinear,
		_sampler_filter_anisotropic_1,
		_sampler_filter_anisotropic_2,
		_sampler_filter_anisotropic_3,
		_sampler_filter_anisotropic_4,
		_sampler_filter_lightprobe_texture_array,
		_sampler_filter_texture_array_quadlinear,
		_sampler_filter_texture_array_quadanisotropic_2,

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
		_z_buffer_mode_debug_geometry,
		_z_buffer_mode_unknown8,

		k_number_of_z_buffer_modes
	};

	enum e_render_mode
	{
		_render_mode_default = 0,
		_render_mode_7e3,
		_render_mode_gamma2,
		_render_mode_xRGB,

		k_number_of_render_modes,
	};

	enum
	{
		_color_write_enable_none = 0,
		_color_write_enable_red = FLAG(0),
		_color_write_enable_green = FLAG(1),
		_color_write_enable_blue = FLAG(2),
		_color_write_enable_alpha = FLAG(3),
		_color_write_enable_color = _color_write_enable_red | _color_write_enable_green | _color_write_enable_blue,
		_color_write_enable_all = _color_write_enable_color | _color_write_enable_alpha,
	};

	enum e_platform
	{
		_platform_xenon = 0,
		_platform_dx9,

		k_number_of_platforms
	};

	struct s_stream_source
	{
		IDirect3DVertexBuffer9* data;
		unsigned int offset;
		unsigned int stride;
	};
	COMPILE_ASSERT(sizeof(s_stream_source) == 0xC);

	static void __cdecl begin(rectangle2d viewport, rectangle2d scissor_rect);
	static bool __cdecl cleanup_before_device_reset();
	static void __cdecl cleanup_d3d_states(bool block_until_idle);
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
	static real32 __cdecl get_aspect_ratio();
	static e_surface __cdecl get_depth_stencil_surface();
	static void __cdecl get_display_pixel_bounds(rectangle2d* display_pixel_bounds);
	static void __cdecl get_display_title_safe_pixel_bounds(rectangle2d* resolution);
	static void __cdecl get_fullscreen_render_pixel_bounds(rectangle2d* resolution);
	static void __cdecl get_fullscreen_render_title_safe_pixel_bounds(rectangle2d* resolution);
	static bool __cdecl get_is_widescreen();
	static void __cdecl initialize();
	static bool __cdecl initialize_after_device_creation_or_reset();
	static void __cdecl initialize_for_new_map();
	static void __cdecl initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
	static void __cdecl notify_game_tick_begin();
	static void __cdecl restore_last_scissor_rect();
	static void __cdecl restore_last_viewport();
	static void __cdecl shell_dispose();
	static void __cdecl shell_initialize(bool window_exists, bool windowed);
	static void __cdecl window_reset();
	static void __cdecl set_render_resolution(int32 width, int32 height, bool fullscreen);
	static bool __cdecl test_cooperative_level();
	static bool __cdecl reset_device();
	static bool __cdecl begin_frame();
	static void __cdecl begin_high_quality_blend();
	//static void __cdecl clearf(uns32, const real_vector4d*, real32, uns32);
	static bool __cdecl end_frame();
	static void __cdecl end_high_quality_blend();
	static bool __cdecl get_is_tiling_bracket_active();
	static bool __cdecl get_is_tiling_enabled();
	static e_surface __cdecl get_read_only_depth_stencil_surface(e_surface depth_stencil);
	static e_platform __cdecl get_runtime_platform();
	static bool __cdecl initialize_device(bool window_exists, bool windowed);
	static void __cdecl rasterizer_device_acquire_thread();
	static void __cdecl rasterizer_device_release_thread();
	static bool __cdecl rasterizer_thread_owns_device();
	static e_gpr_allocation __cdecl set_gprs_allocation(e_gpr_allocation type);
	static void __cdecl clear_sampler_textures(uns32 samplers_flag);
	static e_cull_mode __cdecl get_cull_mode(void);
	static void __cdecl set_alpha_blend_mode(e_alpha_blend_mode);
	static void __cdecl set_alpha_blend_mode_custom_device_no_cache(IDirect3DDevice9Ex*, e_alpha_blend_mode);
	static void __cdecl set_color_write_enable(int32 target_index, int32 enable);
	static bool __cdecl surface_valid(e_surface surface);
	static bool __cdecl set_compiled_pixel_shader(const c_rasterizer_compiled_pixel_shader*, e_entry_point);
	static bool __cdecl set_compiled_vertex_shader(const c_rasterizer_compiled_vertex_shader*, e_vertex_type, e_transfer_vector_vertex_types, e_entry_point);
	static void __cdecl set_cull_mode(e_cull_mode);
	static bool __cdecl set_explicit_shaders(int32, e_vertex_type, e_transfer_vector_vertex_types, e_entry_point);
	static void __cdecl set_fill_mode(e_fill_mode);
	static void __cdecl set_indices(IDirect3DIndexBuffer9*);
	static bool __cdecl set_pixel_shader(const c_rasterizer_pixel_shader*, e_entry_point);
	static void __cdecl set_sampler_texture_direct(int32 sampler_index, e_surface surface);
	static void __cdecl set_sampler_address_mode(int32 sampler_index, e_sampler_address_mode u, e_sampler_address_mode v, e_sampler_address_mode w);
	static void __cdecl set_sampler_filter_mode(int32 sampler_index, e_sampler_filter_mode sampler_filter_mode);
	static void __cdecl set_sampler_texture(int32 sampler_index, c_rasterizer_texture_ref sampler_texture);
	static void __cdecl set_scissor_rect(const rectangle2d* scissor_rect);
	static void __cdecl set_separate_alpha_blend_mode(e_separate_alpha_blend_mode mode);
	static void __cdecl set_stencil_mode(e_stencil_mode stencil_mode);
	static void __cdecl set_stencil_mode_with_value(e_stencil_mode stencil_mode, uns8 value);
	static void __cdecl set_stencil_write_mask(uns8 mask);
	static bool __cdecl set_vertex_declaration(IDirect3DVertexDeclaration9*);
	static bool __cdecl set_vertex_shader(const c_rasterizer_vertex_shader* vertex_shader, e_vertex_type base_vertex_type, e_transfer_vector_vertex_types transfer_vertex_type, e_entry_point entry_point);
	static void __cdecl set_z_buffer_mode(e_z_buffer_mode mode);
	static bool __cdecl get_is_using_floating_point_depth_buffer();

	static void __cdecl setup_occlusion_state();
	static void __cdecl setup_render_target_globals_with_exposure(real32 view_exposure, real32 illum_scale, real32 HDR_target_stops, bool alpha_blend);
	static void __cdecl setup_render_target_globals_with_exposure_for_texture_camera_only(real32 view_exposure, real32 illum_scale, real32 HDR_target_stops, bool alpha_blend);
	static void __cdecl setup_targets_albedo(bool clear_stencil, bool is_clear);
	static void __cdecl setup_targets_distortion(const rectangle2d* pixel_bounds, bool depth_test);
	static void __cdecl setup_targets_simple();
	static void __cdecl setup_targets_static_lighting(real32 view_exposure, real32 illum_scale, bool render_to_HDR_target, real32 HDR_target_stops, bool clear, bool copy_albedo_pc, bool a7);
	static void __cdecl setup_targets_static_lighting_alpha_blend(bool render_to_HDR_target, bool alpha_blend);

	static void __cdecl draw_indexed_primitive(const c_rasterizer_index_buffer* indices, int32 base_vertex_index, int32 num_vertices, int32 min_index, int32 triangle_count);
	static void __cdecl draw_vertices(c_rasterizer_index_buffer::e_primitive_type primitive_type, int32 start_vertex, int32 primitive_count);
	static void __cdecl draw_primitive(c_rasterizer_index_buffer::e_primitive_type primitive_type, int32 start_vertex, int32 primitive_count);
	static void __cdecl draw_primitive_up(c_rasterizer_index_buffer::e_primitive_type primitive_type, uns32 primitive_count, const void* stream_data, uns32 stride);

	static void __cdecl set_current_splitscreen_res(e_splitscreen_res res);

	static e_surface __cdecl get_render_target(int32 surface_index);
	static real32 __cdecl get_render_target_alpha_multiplier(int32 surface_index);

	static e_surface get_display_surface();

	static int32 __cdecl get_surface_height(e_surface surface);
	static c_rasterizer_texture_ref __cdecl get_surface_texture(c_rasterizer::e_surface surface);
	static int32 __cdecl get_surface_width(e_surface surface);

	static void __cdecl resolve_surface(e_surface surface, int32 source_render_target, const rectangle2d* source_rectangle, int16 dest_left, int16 dest_top);
	static void __cdecl set_depth_stencil_surface(e_surface depth_stencil);
	static void __cdecl set_possibly_stale_surface_as_texture(int32 sampler_index, e_surface surface);
	static void __cdecl set_render_target(int32 surface_index, e_surface surface, int32 force_is_srgb);
	static void __cdecl set_surface_as_texture(int32 sampler_index, e_surface surface);
	static void __cdecl set_using_albedo_sampler(bool value);
	static void __cdecl set_viewport(const rectangle2d& viewport, real32 min_z, real32 max_z);
	static void __cdecl stretch_rect(e_surface source_surface, e_surface dest_surface);
	static void __cdecl wait_for_gpu_idle();

	static void __cdecl draw_debug_line2d(const real_point3d& point1, const real_point3d& point2, uns32 color0, uns32 color1);
	static void __cdecl draw_debug_line(const real_point3d& point1, const real_point3d& point2, uns32 color0, uns32 color1);
	static void __cdecl draw_debug_line_list2d_explicit(const rasterizer_vertex_debug* points, int32 primitive_count);
	static void __cdecl draw_debug_line_list_explicit(const rasterizer_vertex_debug* points, int32 line_count);
	static void __cdecl draw_debug_linestrip2d(const point2d* points, int32 point_count, uns32 color);
	static void __cdecl draw_debug_polygon2d(const rasterizer_vertex_debug* polygon2d, int32 primitive_count, c_rasterizer_index_buffer::e_primitive_type primitive_type);
	static void __cdecl draw_debug_polygon(const rasterizer_vertex_debug* vertices, int32 primitive_count, c_rasterizer_index_buffer::e_primitive_type primitive_type);
	static void __cdecl draw_full_window_displacement(const rectangle2d* window_pixel_bounds, const DirectX::XMMATRIX& view_projection, const DirectX::XMMATRIX& previous_view_projection, const DirectX::XMMATRIX& projection, const DirectX::XMMATRIX& screen_to_world, bool use_motion_blur);
	static void __cdecl draw_fullscreen_quad(int width, int height);
	static void __cdecl draw_fullscreen_quad_with_texture_xform(int width, int height, const real_rectangle2d* bounds);
	static void __cdecl draw_screen_quad_with_texture_transform(int target_width, int target_height, const real_rectangle2d* dest_texcoords, const real_rectangle2d* source_texcoords);
	static void __cdecl draw_textured_screen_quad(real32 x0, real32 y0, real32 x1, real32 y1);
	static void __cdecl draw_textured_screen_quad(const rasterizer_vertex_screen* vertices, bool strip);
	static void __cdecl draw_textured_screen_triangle_list(const rasterizer_vertex_screen* textured_screen_triangle_list, int32 primitive_count);
	static void __cdecl draw_textured_transparent_polygon(const rasterizer_vertex_transparent* textured_transparent_polygon, int32 polygon_count, e_alpha_blend_mode alpha_blend_mode);
	static void __cdecl draw_textured_transparent_quad(const rasterizer_vertex_transparent* textured_transparent_quad, e_alpha_blend_mode alpha_blend_mode);
	static void __cdecl draw_worldspace_polygon(const real_point3d* worldspace_polygon, int32 polygon_count);
	static void __cdecl draw_worldspace_polygon(const rasterizer_vertex_world* worldspace_polygon, int32 polygon_count);

	static void __cdecl set_pixel_shader_constant(int32 constant_index, int32 count, const real_vector4d* constants);
	static void __cdecl set_pixel_shader_constant_bool(int32 constant_index, int32 count, const int* constants);
	static void __cdecl set_pixel_shader_constant_int(int32 constant_index, int32 vector_count, const int* constants);
	static void __cdecl set_pixel_shader_constant_single(int32 constant_index, real32 value);
	static void __cdecl set_vertex_shader_constant(int32 constant_index, int32 vector_count, const real_vector4d* constants);
	static void __cdecl set_vertex_shader_constant_bool(int32 constant_index, int32 count, const int* constants);
	static void __cdecl set_vertex_shader_constant_int(int32 start_register, int32 vector4i_count, const int* constant_data);
	static void __cdecl set_vertex_shader_constant_owned(int32 constant_index, int32 count, const real_vector4d* constants);

	static bool& g_d3d_device_is_lost;
	static bool& g_d3d_device_reset;
	static _D3DRENDERSTATETYPE(&x_last_render_state_types)[4];

	static e_render_mode& g_LDR_render_mode;
	static e_render_mode& g_HDR_render_mode;
	static int32& g_triliner_threshold;
	static int32& g_present_immediate_threshold;
	static c_rasterizer_texture_ref& m_albedo_texture;
	static c_rasterizer_texture_ref& m_normal_texture;

	static uns32& g_render_thread;

	static s_rasterizer_render_globals& render_globals;
	static bool& g_using_albedo_sampler;

	static uns32& g_render_thread_begin_scene;

	static IDirect3D9Ex*& g_direct3d;
	static IDirect3DDevice9Ex*& g_device;

	static bool(&byte_50DADE0)[3];
	static IDirect3DQuery9* (&dword_50DADE4)[3];

	static e_alpha_blend_mode& g_current_alpha_blend_mode;
	static e_separate_alpha_blend_mode& g_current_separate_alpha_blend_mode;
	static e_z_buffer_mode& g_current_z_buffer_mode;
	static bool& g_current_z_buffer_floating_point;
	static uns8& g_current_stencil_value;
	static bool& initialized;
	static bool& m_use_floating_point_z_buffer;
	static e_stencil_mode& g_current_stencil_mode;
	static bool& g_lock_clip_plane;
	static int32(&x_last_render_state_value)[4];
	static e_cull_mode& g_current_cull_mode;
	static IDirect3DVertexShader9*& g_current_vertex_shader;
	static IDirect3DPixelShader9*& g_current_pixel_shader;
	static IDirect3DVertexDeclaration9*& g_current_vertex_declaration;
	static IDirect3DIndexBuffer9*& g_current_index_buffer;
	static s_stream_source(&x_last_stream_source)[16];

	static e_surface& g_depth_stencil_surface;
	static e_surface(&g_color_surfaces)[k_number_of_color_surfaces];

	static bool& g_tiling_force_4x_msaa;
	static e_splitscreen_res& g_current_splitscreen_res;

	static rectangle2d& g_last_viewport;
	static rectangle2d& g_last_scissor_rect;

	static uns32& g_max_vs_gprs;
	static uns32& g_max_ps_gprs;
	static int32& g_adapter;

	static int32& dip_cur;
	static int32& dip_last;
	static int32& dip_max;

	static void __cdecl clear(uns32 clear_channels, uns32 clear_color, real32 clear_z, uns8 clear_stencil);
	static void __cdecl sub_A21440();
	static void __cdecl end();
	static bool __cdecl end_albedo(const rectangle2d* bounds);
};

struct s_global_bitmaps;
struct s_texture_references_block;
class c_rasterizer_globals
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
		c_typed_tag_reference<VERTEX_SHADER_TAG, INVALID_TAG> vertex_shader;
		c_typed_tag_reference<PIXEL_SHADER_TAG, INVALID_TAG> pixel_shader;

		void update_reference_names();
	};
	COMPILE_ASSERT(sizeof(s_explicit_shader) == 0x20);

	const s_tag_reference* get_default_texture_ref(int32 index);
	const s_explicit_shader* get_explicit_shaders(int32 index);
	const s_tag_reference* get_explicit_pixel_shader_ref(int32 index);
	const s_tag_reference* get_explicit_vertex_shader_ref(int32 index);
	uns32 __cdecl get_max_vs_gprs() const;
	uns32 __cdecl get_max_ps_gprs() const;
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
	c_typed_tag_block<s_global_bitmaps> m_default_textures_refs;

	// material textures
	//	0: ............... SH Glossy CT CC0236
	//	1: ............... SH Glossy CT C78D78
	//	2: ............... SH Glossy CT DD0236
	//	3: ............... TBD
	//	4: ............... TBD
	//	5: ............... TBD
	c_typed_tag_block<s_texture_references_block> m_material_textures_refs;

	// default shader vertex shader
	//	Renderer uses this shader when a vertex shader is invalid
	c_typed_tag_reference<VERTEX_SHADER_TAG, INVALID_TAG> m_default_vertex_shader_ref;

	// default pixel shader
	//	Renderer uses this shader when a pixel shader is invalid
	c_typed_tag_reference<PIXEL_SHADER_TAG, INVALID_TAG> m_default_pixel_shader_ref;

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
	c_typed_tag_block<s_explicit_shader> m_explicit_shader_refs;

	// atmosphere lookup tables
	//	0: ............... 1D Neta Table
	//	1: ............... TBD
	//	2: ............... TBD
	//	3: ............... TBD
	//	4: ............... TBD
	//	5: ............... TBD
	c_typed_tag_block<s_texture_references_block> m_atmosphere_textures;

	// runtime
	uns32 m_max_vs_gprs;
	uns32 m_max_ps_gprs;

	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> m_active_camo_distort_texture;
	c_typed_tag_reference<PERFORMANCE_THROTTLES_TAG, INVALID_TAG> m_default_performance_throttles;
	c_typed_tag_reference<SHIELD_IMPACT_TAG, INVALID_TAG> m_shield_impact_parameters;
	c_typed_tag_reference<VISION_MODE_TAG, INVALID_TAG> m_vision_mode_parameters;

	// Motion blur parameters
	//	max blur:............  max amount to blur, as a percentage of the screen
	//	blur scale:..........  scales blur for a given velocity
	//	center falloff:....... reduces motion blur around the screen center (larger values give smaller reduction regions...)
	//	expected dt:.......... expected time per tick, in seconds (used to correct motion blur in fast/slow frames)

	uns32 m_motion_blur_taps;
	real32 m_motion_blur_max_x;
	real32 m_motion_blur_max_y;
	real32 m_motion_blur_scale_x;
	real32 m_motion_blur_scale_y;
	real32 m_motion_blur_center_falloff;
	real32 m_motion_blur_expected_dt;
};
COMPILE_ASSERT(sizeof(c_rasterizer_globals) == 0xB4);

struct s_global_bitmaps
{
	int32 flags;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> ref;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_global_bitmaps) == 0x14);

struct s_texture_references_block
{
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> ref;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_texture_references_block) == sizeof(s_tag_reference));

struct rasterizer_dynamic_screen_geometry_parameters
{
	real_vector2d* offset;
	bool map_anchor_screen[3];
	c_rasterizer_texture_ref hardware_formats[3];
	bool map_wrapped[3];
	real_point2d* map_offset[3];
	real_vector2d map_scale[3];
	real_vector2d map_texture_scale[3];
	real_rgb_color* map_tint[3][2];
	real_argb_color plasma_fade;
	bool doing_plasma_effect;
	real32* map_fade[3];
	int16 map0_to_1_blend_function;
	int16 map1_to_2_blend_function;
	int16 framebuffer_blend_function;
	bool point_sampled;
	bool using_shader_system;
	int32 explicit_override_index;
	c_rasterizer::e_surface instead_of_texture;
};
COMPILE_ASSERT(sizeof(rasterizer_dynamic_screen_geometry_parameters) == 0x9C);

extern void rasterizer_reset_device();

extern bool __cdecl rasterizer_initialized();
extern void __cdecl rasterizer_window_initialize();

extern void __cdecl draw_tesselated_quad();
extern void __cdecl rasterizer_psuedo_dynamic_screen_quad_draw(const rasterizer_dynamic_screen_geometry_parameters* parameters, rasterizer_vertex_screen* vertices);
extern void __cdecl rasterizer_quad_screenspace(point2d const(&points)[4], uns32 color, const s_tag_reference* reference, int16 bitmap_index, bool point_sampled);
extern void __cdecl rasterizer_quad_screenspace_explicit(point2d const(&points)[4], uns32 color, c_rasterizer_texture_ref texture_ref, bool point_sampled, c_rasterizer::e_alpha_blend_mode blend_mode);
extern bool __cdecl rasterizer_set_explicit_debug_shader(c_rasterizer_globals::e_explicit_shader shader_type);
extern void __cdecl set_motion_blur_parameters();

extern bool rasterizer_dump_display_to_bmp(const char* file_name);

