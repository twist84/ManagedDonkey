#pragma once

#include "cseries/cseries.hpp"
#include "rasterizer/rasterizer_text.hpp"
#include "tag_files/tag_groups.hpp"

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

struct s_global_bitmaps;
struct s_texture_references_block;
struct c_rasterizer_globals
{
public:
	dword __cdecl get_max_vs_gprs() const
	{
		return m_max_vs_gprs;
	}

	dword __cdecl get_max_ps_gprs() const
	{
		return m_max_ps_gprs;
	}

protected:
	struct s_explicit_shader
	{
		s_tag_reference explicit_vertex_shader;
		s_tag_reference explicit_pixel_shader;
	};
	static_assert(sizeof(s_explicit_shader) == 0x20);

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
	c_typed_tag_reference<'vtsh'> default_vertex_shader;

	// default pixel shader
	//	Renderer uses this shader when a pixel shader is invalid
	c_typed_tag_reference<'pixl'> default_pixel_shader;

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
	//	81: .............. displacement with motion blur
	//	83: .............. screenshot memexport
	//	84: .............. bloom: NEW downsample 4x4 gauss with curve LDR
	//	85: ...............bloom_downsample_4x4_gaussian_bloom
	//	86: ...............bloom_downsample_4x4_gaussian_bloom new
	//	87: ...............bloom_curve
	//	88: ...............custom gamma correct
	//	89: ...............pixel copy
	//	90: ...............hud camera
	//	91: ...............hud night vision
	//	92: ...............hud camera mask
	//	93: ...............shield impact procedural uv
	//	94: ...............screenshot night vision
	//	95: .............. bloom: NEW downsample 4x4 gauss with curve LDR
	//	96: .............. decorators edit
	//	97: .............. smirnov test
	//	98: .............. fxaa
	//	99: .............. sniper scope stencil pc
	//	100: ............. ssao
	//	101: ............. ssao_blur
	//	102: ............. lightshafts
	//	103: ............. radial_blur
	//	104: ............. clear_letterbox
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

	c_typed_tag_reference<'bitm'> active_camo_distortion_texture;
	c_typed_tag_reference<'perf'> default_performance_throttles;
	c_typed_tag_reference<'shit'> shield_impact_settings;
	c_typed_tag_reference<'vmdx'> vision_mode_settings;

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
	c_typed_tag_reference<'bitm'> default_bitmaps;
};
static_assert(sizeof(s_global_bitmaps) == 0x14);

struct s_texture_references_block
{
	c_typed_tag_reference<'bitm'> reference;
};
static_assert(sizeof(s_texture_references_block) == sizeof(s_tag_reference));

extern bool rasterizer_dump_display_to_bmp(char const* file_name);

