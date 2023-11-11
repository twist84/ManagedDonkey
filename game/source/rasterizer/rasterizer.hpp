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

struct rasterizer_vertex_debug
{
	real_point3d point;
	argb_color color;
};
static_assert(sizeof(rasterizer_vertex_debug) == 0x10);

struct rasterizer_vertex_screen
{
	real_point2d position;
	real_point2d texcoord;
	argb_color color;
};
static_assert(sizeof(rasterizer_vertex_screen) == 0x14);

struct rasterizer_vertex_transparent
{
	real_point3d position;
	real_point2d texcoord;
	dword color;
};
static_assert(sizeof(rasterizer_vertex_transparent) == 0x18);

struct rasterizer_vertex_world
{
	real_point3d position;
	real_point2d texcoord;
	vector3d normal;
	vector3d tangent;
	vector3d binormal;
};
static_assert(sizeof(rasterizer_vertex_world) == 0x38);

struct c_rasterizer
{
	enum e_separate_alpha_blend_mode
	{
		_separate_alpha_blend_mode_unknown0 = 0,
		_separate_alpha_blend_mode_unknown1,
		_separate_alpha_blend_mode_unknown2,
		_separate_alpha_blend_mode_unknown3,
		_separate_alpha_blend_mode_unknown4,

		k_separate_alpha_blend_mode_count
	};

	enum e_stencil_mode
	{
		_stencil_mode_unknown0 = 0,
		_stencil_mode_unknown1,
		_stencil_mode_unknown2,
		_stencil_mode_unknown3,
		_stencil_mode_unknown4,
		_stencil_mode_unknown5,
		_stencil_mode_unknown6,
		_stencil_mode_unknown7,
		_stencil_mode_unknown8,
		_stencil_mode_unknown9,
		_stencil_mode_unknown10,
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

		k_stencil_mode_count
	};

	enum e_gpr_allocation;

	enum e_cull_mode
	{
		_cull_mode_none = 0x1,
		_cull_mode_clockwise = 0x2,
		_cull_mode_counter_clockwise = 0x3,
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
		_alpha_blend_mode_unknown0 = 0,
		_alpha_blend_mode_unknown1,
		_alpha_blend_mode_unknown2,
		_alpha_blend_mode_unknown3,
		_alpha_blend_mode_unknown4,
		_alpha_blend_mode_unknown5,
		_alpha_blend_mode_unknown6,
		_alpha_blend_mode_unknown7,
		_alpha_blend_mode_unknown8,
		_alpha_blend_mode_unknown9,
		_alpha_blend_mode_unknown10,
		_alpha_blend_mode_unknown11,
		_alpha_blend_mode_unknown12,

		k_alpha_blend_mode_count
	};

	enum e_surface;
	enum e_sampler_address_mode;
	enum e_sampler_filter_mode;

	enum e_z_buffer_mode
	{
		_z_buffer_mode_unknown0 = 0,
		_z_buffer_mode_unknown1, // debug?
		_z_buffer_mode_unknown2, // debug2d?
		_z_buffer_mode_unknown3,
		_z_buffer_mode_unknown4,
		_z_buffer_mode_unknown5,
		_z_buffer_mode_unknown6,
		_z_buffer_mode_unknown7,
		_z_buffer_mode_unknown8,

		k_z_buffer_mode_count
	};

	struct s_stream_source
	{
		IDirect3DVertexBuffer9* data;
		unsigned int offset;
		unsigned int stride;
	};
	static_assert(sizeof(s_stream_source) == 0xC);

	static void __cdecl begin(short_rectangle2d, short_rectangle2d);
	static bool __cdecl cleanup_before_device_reset();
	static void __cdecl cleanup_d3d_states();
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl dispose_from_old_structure_bsp();
	static real __cdecl get_aspect_ratio();
	static void __cdecl get_display_pixel_bounds(short_rectangle2d* display_pixel_bounds);
	static void __cdecl get_display_title_safe_pixel_bounds(short_rectangle2d* resolution);
	static void __cdecl get_fullscreen_render_pixel_bounds(short_rectangle2d* resolution);
	static void __cdecl get_fullscreen_render_title_safe_pixel_bounds(short_rectangle2d* resolution);
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
	static void __cdecl set_fill_mode(e_fill_mode);
	static void __cdecl set_indices(IDirect3DIndexBuffer9*);
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

	static void __cdecl draw_indexed_primitive(c_rasterizer_index_buffer const* indices, long base_vertex_index, long num_vertices, long min_index, long triangle_count);
	static void __cdecl draw_primitive(c_rasterizer_index_buffer::e_primitive_type primitive_type, long start_vertex, long primitive_count);
	static void __cdecl draw_primitive_up(c_rasterizer_index_buffer::e_primitive_type primitive_type, dword primitive_count, void const* stream_data, dword stride);
	static void __cdecl draw_vertices(c_rasterizer_index_buffer::e_primitive_type primitive_type, long start_vertex, long primitive_count);

	static void __cdecl draw_debug_line2d(real_point3d const& p0, real_point3d const& p1, dword color0, dword color1);
	static void __cdecl draw_debug_line(real_point3d const& p0, real_point3d const& p1, dword color0, dword color1);
	static void __cdecl draw_debug_line_list2d_explicit(rasterizer_vertex_debug const* vertex_debug, long primitive_count);
	static void __cdecl draw_debug_line_list_explicit(rasterizer_vertex_debug const* vertex_debug, long primitive_count);
	static void __cdecl draw_debug_linestrip2d(int16_point2d const* points, long point_count, dword color);
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

	static dword& g_max_vs_gprs;
	static dword& g_max_ps_gprs;
};

struct s_global_bitmaps;
struct s_texture_references_block;
struct c_rasterizer_globals
{
public:
	static tag const k_group_tag = RASTERIZER_GLOBALS_TAG;

	enum e_explicit_shader
	{
		_explicit_shader_debug = 0,
		_explicit_shader_debug2d,
	};

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
	struct s_explicit_shader
	{
		c_typed_tag_reference<VERTEX_SHADER_TAG> explicit_vertex_shader;
		c_typed_tag_reference<PIXEL_SHADER_TAG> explicit_pixel_shader;

		void update_reference_names();
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
	c_typed_tag_reference<VERTEX_SHADER_TAG> default_vertex_shader;

	// default pixel shader
	//	Renderer uses this shader when a pixel shader is invalid
	c_typed_tag_reference<PIXEL_SHADER_TAG> default_pixel_shader;

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

	c_typed_tag_reference<BITMAP_TAG> active_camo_distortion_texture;
	c_typed_tag_reference<PERFORMANCE_THROTTLES_TAG> default_performance_throttles;
	c_typed_tag_reference<SHIELD_IMPACT_TAG> shield_impact_settings;
	c_typed_tag_reference<VISION_MODE_TAG> vision_mode_settings;

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
	c_typed_tag_reference<BITMAP_TAG> default_bitmaps;

	void update_reference_names();
};
static_assert(sizeof(s_global_bitmaps) == 0x14);

struct s_texture_references_block
{
	c_typed_tag_reference<BITMAP_TAG> reference;

	void update_reference_names();
};
static_assert(sizeof(s_texture_references_block) == sizeof(s_tag_reference));

extern void __cdecl draw_tesselated_quad();
extern void __cdecl rasterizer_quad_screenspace(int16_point2d const(&points)[4], dword color, s_tag_reference const* reference, short bitmap_index, bool a5);
extern bool __cdecl rasterizer_set_explicit_debug_shader(c_rasterizer_globals::e_explicit_shader explicit_shader);

extern bool rasterizer_dump_display_to_bmp(char const* file_name);

