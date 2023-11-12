#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

enum e_entry_point
{
	_entry_point_default = 0,
	_entry_point_albedo,
	_entry_point_static_default,
	_entry_point_static_per_pixel,
	_entry_point_static_per_vertex,
	_entry_point_static_sh,
	_entry_point_static_prt_ambient,
	_entry_point_static_prt_linear,
	_entry_point_static_prt_quadratic,
	_entry_point_dynamic_light,
	_entry_point_shadow_generate,
	_entry_point_shadow_apply,
	_entry_point_active_camo,
	_entry_point_lightmap_debug_mode,
	_entry_point_vertex_color_lighting,
	_entry_point_water_tessellation,
	_entry_point_water_shading,
	_entry_point_dynamic_light_cinematic,
	_entry_point_z_only,
	_entry_point_sfx_distort,

	k_entry_point_count
};

__interface IDirect3DVertexShader9;
__interface IDirect3DPixelShader9;

struct s_compiled_shader_reference
{
	char start_index;
	char count;
};
static_assert(sizeof(s_compiled_shader_reference) == 0x2);

struct s_vertex_entry_point
{
	c_typed_tag_block<s_compiled_shader_reference> vertex_types;
};
static_assert(sizeof(s_vertex_entry_point) == sizeof(s_tag_block));

enum e_rasterizer_constant_table_type
{
	_rasterizer_constant_table_type_vertex = 0,
	_rasterizer_constant_table_type_pixel,

	// MCC
	//_rasterizer_constant_table_type_compute,

	k_rasterizer_constant_table_type_count
};

struct s_global_rasterizer_constant_table
{
	s_tag_block constants;
	c_enum<e_rasterizer_constant_table_type, char, _rasterizer_constant_table_type_vertex, k_rasterizer_constant_table_type_count> type;

	// pad
	byte qersaui[0x3];
};
static_assert(sizeof(s_global_rasterizer_constant_table) == 0x10);

struct c_rasterizer_compiled_shader
{
	// ..:xenon compiled shader
	s_tag_data xenon_compiled_shader;

	// ..:dx9 compiled shader
	s_tag_data dx9_compiled_shader;

	// Xenon constant table
	s_global_rasterizer_constant_table xenon_rasterizer_constant_table;

	// DX9 constant table
	s_global_rasterizer_constant_table dx9_rasterizer_constant_table;

	// ..:gprs
	long gprs;
};
static_assert(sizeof(c_rasterizer_compiled_shader) == 0x4C);

struct c_rasterizer_compiled_vertex_shader :
	public c_rasterizer_compiled_shader // compiled shader splut
{
	IDirect3DVertexShader9* runtime_shader;
};
static_assert(sizeof(c_rasterizer_compiled_vertex_shader) == 0x50);

struct c_rasterizer_compiled_pixel_shader :
	public c_rasterizer_compiled_shader // compiled shader splut
{
	IDirect3DPixelShader9* runtime_shader;
};
static_assert(sizeof(c_rasterizer_compiled_pixel_shader) == 0x50);

struct c_rasterizer_vertex_shader
{
	c_flags<e_entry_point, dword, k_entry_point_count> entry_point_flags;
	c_typed_tag_block<s_vertex_entry_point> entry_points;
	long version;
	c_typed_tag_block<c_rasterizer_compiled_vertex_shader> compiled_shader;
};
static_assert(sizeof(c_rasterizer_vertex_shader) == 0x20);

struct c_rasterizer_pixel_shader
{
	c_flags<e_entry_point, dword, k_entry_point_count> entry_point_flags;
	c_typed_tag_block<s_compiled_shader_reference> entry_points;
	long version;
	c_typed_tag_block<c_rasterizer_compiled_pixel_shader> compiled_shader;
};
static_assert(sizeof(c_rasterizer_pixel_shader) == 0x20);

