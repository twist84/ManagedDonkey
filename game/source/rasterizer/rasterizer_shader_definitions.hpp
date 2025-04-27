#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

enum e_vertex_type
{
	_vertex_type_world = 0,
	_vertex_type_rigid,
	_vertex_type_skinned,
	_vertex_type_particle_model,
	_vertex_type_flat_world,
	_vertex_type_flat_rigid,
	_vertex_type_flat_skinned,
	_vertex_type_screen,
	_vertex_type_debug,
	_vertex_type_transparent,
	_vertex_type_particle,
	_vertex_type_contrail,
	_vertex_type_light_volume,
	_vertex_type_simple_chud,
	_vertex_type_fancy_chud,
	_vertex_type_decorator,
	_vertex_type_tiny_position,
	_vertex_type_patchy_fog,
	_vertex_type_water,
	_vertex_type_ripple,
	_vertex_type_implicit,
	_vertex_type_beam,
	_vertex_type_dual_quat,

	k_number_of_vertex_types
};

enum e_lighting_vertex_types
{
	// default
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_type_unknown0, _transfer_vertex_none)
	// _entry_point_static_prt_ambient
	// _entry_point_static_prt_linear
	// _entry_point_static_prt_quadratic
	// _entry_point_active_camo
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_type_unknown0, transfer_vertex_type)
	_lighting_vertex_type_unknown0 = 0,

	// _entry_point_vertex_color_lighting
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_type_unknown1, _transfer_vertex_none)
	_lighting_vertex_type_unknown1,

	// _entry_point_static_per_pixel
	// _entry_point_lightmap_debug_mode
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_type_unknown2, _transfer_vertex_none)
	_lighting_vertex_type_unknown2,

	// _entry_point_static_per_vertex
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_type_unknown3, _transfer_vertex_none)
	_lighting_vertex_type_unknown3,

	k_number_of_lighting_vertex_types
};

enum e_transfer_vector_vertex_types
{
	_transfer_vertex_none = 0,
	_transfer_prt_ambient_only,
	_transfer_prt_linear,
	_transfer_prt_quadratic,

	k_number_of_transfer_vertex_types
};

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

struct s_rasterizer_vertex_shader_entry_point
{
	s_compiled_shader_reference const* get_shader_reference(e_vertex_type vertex_type) const;

	c_typed_tag_block<s_compiled_shader_reference> vertex_types;
};
static_assert(sizeof(s_rasterizer_vertex_shader_entry_point) == sizeof(s_tag_block));

enum e_rasterizer_constant_table_type
{
	_rasterizer_constant_table_type_vertex = 0,
	_rasterizer_constant_table_type_pixel,

	// MCC
	//_rasterizer_constant_table_type_compute,

	k_rasterizer_constant_table_type_count
};

struct c_rasterizer_constant_table_definition
{
	s_tag_block constants;
	c_enum<e_rasterizer_constant_table_type, char, _rasterizer_constant_table_type_vertex, k_rasterizer_constant_table_type_count> type;

	// pad
	uint8 qersaui[0x3];
};
static_assert(sizeof(c_rasterizer_constant_table_definition) == 0x10);

struct c_rasterizer_compiled_shader
{
	// ..:xenon compiled shader
	// ..:dx9 compiled shader
	s_tag_data compiled_shader[2]; // c_rasterizer::e_platform::k_platform_count

	// Xenon constant table
	// DX9 constant table
	c_rasterizer_constant_table_definition rasterizer_constant_table[2]; // c_rasterizer::e_platform::k_platform_count

	// ..:gprs
	long gprs;
};
static_assert(sizeof(c_rasterizer_compiled_shader) == 0x4C);

struct c_rasterizer_compiled_vertex_shader :
	public c_rasterizer_compiled_shader // compiled shader splut
{
	IDirect3DVertexShader9* get_d3d_shader() const;

	IDirect3DVertexShader9* runtime_shader;
};
static_assert(sizeof(c_rasterizer_compiled_vertex_shader) == 0x50);

struct c_rasterizer_compiled_pixel_shader :
	public c_rasterizer_compiled_shader // compiled shader splut
{
	IDirect3DPixelShader9* get_d3d_shader() const;

	IDirect3DPixelShader9* runtime_shader;
};
static_assert(sizeof(c_rasterizer_compiled_pixel_shader) == 0x50);

struct c_rasterizer_vertex_shader
{
	static c_rasterizer_vertex_shader const* get(long definition_index);
	static c_rasterizer_vertex_shader* get_modifiable(long definition_index);

	s_rasterizer_vertex_shader_entry_point const* get_entry_point(long entry_point) const;
	c_rasterizer_compiled_vertex_shader const* get_compiled_shader(long shader_index) const;
	c_rasterizer_compiled_vertex_shader const* get_compiled_shader(e_vertex_type vertex_type, e_entry_point entry_point, long shader_index) const;
	IDirect3DVertexShader9* get_d3d_shader(e_vertex_type vertex_type, e_entry_point entry_point, long shader_index) const;

	c_flags<e_entry_point, uint32, k_entry_point_count> entry_point_flags;
	c_typed_tag_block<s_rasterizer_vertex_shader_entry_point> entry_points;
	long version;
	c_typed_tag_block<c_rasterizer_compiled_vertex_shader> compiled_shader;
};
static_assert(sizeof(c_rasterizer_vertex_shader) == 0x20);

struct c_rasterizer_pixel_shader
{
	static c_rasterizer_pixel_shader const* get(long definition_index);
	static c_rasterizer_pixel_shader* get_modifiable(long definition_index);

	c_rasterizer_compiled_pixel_shader const* get_compiled_shader(e_entry_point entry_point, long shader_index) const;
	IDirect3DPixelShader9* get_d3d_shader(e_entry_point entry_point, long shader_index) const;

	c_flags<e_entry_point, uint32, k_entry_point_count> entry_point_flags;
	c_typed_tag_block<s_compiled_shader_reference> entry_points;
	long version;
	c_typed_tag_block<c_rasterizer_compiled_pixel_shader> compiled_shader;
};
static_assert(sizeof(c_rasterizer_pixel_shader) == 0x20);

