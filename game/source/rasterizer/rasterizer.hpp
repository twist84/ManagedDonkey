#pragma once

#include "cseries/cseries.hpp"
#include "rasterizer/rasterizer_text.hpp"

#include <d3d9.h>

// force update the backbuffer
extern void __stdcall sub_79BA30(long width, long height);

enum e_platform;
enum e_vertex_type;
enum e_transfer_vector_vertex_types;
enum e_entry_point;

struct c_rasterizer_pixel_shader;
struct c_rasterizer_vertex_shader;
struct c_rasterizer_index_buffer;

struct c_rasterizer
{
	enum e_separate_alpha_blend_mode;
	enum e_stencil_mode;
	enum e_gpr_allocation;
	enum e_cull_mode;
	enum e_alpha_blend_mode;
	enum e_surface;
	enum e_sampler_address_mode;
	enum e_sampler_filter_mode;
	enum e_separate_alpha_blend_mode;
	enum e_z_buffer_mode;

	static void __cdecl begin(short_rectangle2d, short_rectangle2d);
	static bool __cdecl cleanup_before_device_reset();
	static void __cdecl cleanup_d3d_states();
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl dispose_from_old_structure_bsp();
	static real __cdecl get_aspect_ratio();
	static void __cdecl get_display_pixel_bounds(short_rectangle2d* display_pixel_bounds);
	static void __cdecl initialize();
	static bool __cdecl initialize_after_device_creation_or_reset();
	static void __cdecl initialize_for_new_map();
	static void __cdecl restore_last_scissor_rect();
	static void __cdecl restore_last_viewport();
	static void __cdecl shell_dispose();
	static void __cdecl shell_initialize(bool, bool);
	static bool __cdecl begin_frame();
	static void __cdecl begin_high_quality_blend();
	//static void __cdecl clearf(unsigned long, union real_vector4d const*, float, unsigned long);
	static bool __cdecl end_frame();
	static void __cdecl end_high_quality_blend();
	static e_platform __cdecl get_runtime_platform();
	static bool __cdecl initialize_device(bool, bool);
	static void __cdecl rasterizer_device_acquire_thread();
	static void __cdecl rasterizer_device_release_thread();
	static bool __cdecl rasterizer_thread_owns_device();
	static void __cdecl initialize_window();
	static e_gpr_allocation __cdecl set_gprs_allocation(e_gpr_allocation);
	static void __cdecl clear_sampler_textures(dword);
	static e_cull_mode __cdecl get_cull_mode(void);
	static void __cdecl set_alpha_blend_mode(e_alpha_blend_mode);
	static void __cdecl set_color_write_enable(long, long);
	static void __cdecl set_cull_mode(e_cull_mode);
	static bool __cdecl set_explicit_shaders(long, e_vertex_type, e_transfer_vector_vertex_types, e_entry_point);
	static bool __cdecl set_pixel_shader(c_rasterizer_pixel_shader const*, e_entry_point);
	static void __cdecl set_aliased_surface_as_texture(long, e_surface);
	static void __cdecl set_sampler_address_mode(long, e_sampler_address_mode);
	static void __cdecl set_sampler_filter_mode(long, e_sampler_filter_mode);
	static void __cdecl set_sampler_texture(long, c_rasterizer_texture_ref);
	static void __cdecl set_scissor_rect(short_rectangle2d const*);
	static void __cdecl set_separate_alpha_blend_mode(e_separate_alpha_blend_mode);
	static void __cdecl set_stencil_mode(e_stencil_mode);
	static bool __cdecl set_vertex_shader(c_rasterizer_vertex_shader const*, e_vertex_type, e_transfer_vector_vertex_types, e_entry_point);
	static void __cdecl set_z_buffer_mode(e_z_buffer_mode);
	static void __cdecl setup_occlusion_state();
	static void __cdecl setup_render_target_globals_with_exposure(real, real, bool);
	static void __cdecl setup_targets_distortion(short_rectangle2d*);
	static void __cdecl setup_targets_simple();
	static void __cdecl setup_targets_static_lighting_alpha_blend(bool, bool);
	static void __cdecl draw_indexed_primitive(c_rasterizer_index_buffer const*, long, long, long, long);

	static IDirect3DDevice9Ex*& g_device;
	static e_separate_alpha_blend_mode& g_current_separate_alpha_blend_mode;
	static bool& initialized;
	static bool& m_use_floating_point_z_buffer;
	static e_stencil_mode& g_current_stencil_mode;
	static dword& g_max_vs_gprs;
	static dword& g_max_ps_gprs;
};
