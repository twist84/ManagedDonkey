#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

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
	int32 gprs;
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
	static c_rasterizer_vertex_shader const* get(int32 definition_index);
	static c_rasterizer_vertex_shader* get_modifiable(int32 definition_index);

	s_rasterizer_vertex_shader_entry_point const* get_entry_point(int32 entry_point) const;
	c_rasterizer_compiled_vertex_shader const* get_compiled_shader(int32 shader_index) const;
	c_rasterizer_compiled_vertex_shader const* get_compiled_shader(e_vertex_type vertex_type, e_entry_point entry_point, int32 shader_index) const;
	IDirect3DVertexShader9* get_d3d_shader(e_vertex_type vertex_type, e_entry_point entry_point, int32 shader_index) const;

	c_flags<e_entry_point, uint32, k_entry_point_count> entry_point_flags;
	c_typed_tag_block<s_rasterizer_vertex_shader_entry_point> entry_points;
	int32 version;
	c_typed_tag_block<c_rasterizer_compiled_vertex_shader> compiled_shader;
};
static_assert(sizeof(c_rasterizer_vertex_shader) == 0x20);

struct c_rasterizer_pixel_shader
{
	static c_rasterizer_pixel_shader const* get(int32 definition_index);
	static c_rasterizer_pixel_shader* get_modifiable(int32 definition_index);

	c_rasterizer_compiled_pixel_shader const* get_compiled_shader(e_entry_point entry_point, int32 shader_index) const;
	IDirect3DPixelShader9* get_d3d_shader(e_entry_point entry_point, int32 shader_index) const;

	c_flags<e_entry_point, uint32, k_entry_point_count> entry_point_flags;
	c_typed_tag_block<s_compiled_shader_reference> entry_points;
	int32 version;
	c_typed_tag_block<c_rasterizer_compiled_pixel_shader> compiled_shader;
};
static_assert(sizeof(c_rasterizer_pixel_shader) == 0x20);

