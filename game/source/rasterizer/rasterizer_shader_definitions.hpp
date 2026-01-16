#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

__interface IDirect3DVertexShader9;
__interface IDirect3DPixelShader9;

struct s_compiled_shader_reference
{
	int8 start_index;
	int8 count;
};
COMPILE_ASSERT(sizeof(s_compiled_shader_reference) == 0x2);

struct s_rasterizer_vertex_shader_entry_point
{
	const s_compiled_shader_reference* get_shader_reference(e_vertex_type vertex_type) const;

	c_typed_tag_block<s_compiled_shader_reference> vertex_types;
};
COMPILE_ASSERT(sizeof(s_rasterizer_vertex_shader_entry_point) == sizeof(s_tag_block));

class c_rasterizer_constant_table_definition
{
public:
	s_tag_block constants;
	c_enum<e_rasterizer_constant_table_type, int8, _rasterizer_constant_table_type_vertex, k_rasterizer_constant_table_type_count> type;

	// pad
	byte qersaui[0x3];
};
COMPILE_ASSERT(sizeof(c_rasterizer_constant_table_definition) == 0x10);

class c_rasterizer_compiled_shader
{
public:
	// ..:xenon compiled shader
	// ..:dx9 compiled shader
	s_tag_data compiled_shader[2]; // c_rasterizer::e_platform::k_number_of_platforms

	// Xenon constant table
	// DX9 constant table
	c_rasterizer_constant_table_definition rasterizer_constant_table[2]; // c_rasterizer::e_platform::k_number_of_platforms

	// ..:gprs
	int32 gprs;
};
COMPILE_ASSERT(sizeof(c_rasterizer_compiled_shader) == 0x4C);

class c_rasterizer_compiled_vertex_shader :
	public c_rasterizer_compiled_shader // compiled shader splut
{
public:
	IDirect3DVertexShader9* get_d3d_shader() const;

	IDirect3DVertexShader9* runtime_shader;
};
COMPILE_ASSERT(sizeof(c_rasterizer_compiled_vertex_shader) == 0x50);

class c_rasterizer_compiled_pixel_shader :
	public c_rasterizer_compiled_shader // compiled shader splut
{
public:
	IDirect3DPixelShader9* get_d3d_shader() const;

	IDirect3DPixelShader9* runtime_shader;
};
COMPILE_ASSERT(sizeof(c_rasterizer_compiled_pixel_shader) == 0x50);

class c_rasterizer_vertex_shader
{
public:
	static const c_rasterizer_vertex_shader* get(int32 definition_index);
	static c_rasterizer_vertex_shader* get_modifiable(int32 definition_index);

	const s_rasterizer_vertex_shader_entry_point* get_entry_point(int32 entry_point) const;
	const c_rasterizer_compiled_vertex_shader* get_compiled_shader(int32 shader_index) const;
	const c_rasterizer_compiled_vertex_shader* get_compiled_shader(e_vertex_type vertex_type, e_entry_point entry_point, int32 shader_index) const;
	IDirect3DVertexShader9* get_d3d_shader(e_vertex_type vertex_type, e_entry_point entry_point, int32 shader_index) const;

	c_flags<e_entry_point, uns32, k_number_of_entry_points> entry_point_flags;
	c_typed_tag_block<s_rasterizer_vertex_shader_entry_point> entry_points;
	int32 version;
	c_typed_tag_block<c_rasterizer_compiled_vertex_shader> compiled_shader;
};
COMPILE_ASSERT(sizeof(c_rasterizer_vertex_shader) == 0x20);

class c_rasterizer_pixel_shader
{
public:
	static const c_rasterizer_pixel_shader* get(int32 definition_index);
	static c_rasterizer_pixel_shader* get_modifiable(int32 definition_index);

	const c_rasterizer_compiled_pixel_shader* get_compiled_shader(e_entry_point entry_point, int32 shader_index) const;
	IDirect3DPixelShader9* get_d3d_shader(e_entry_point entry_point, int32 shader_index) const;

	c_flags<e_entry_point, uns32, k_number_of_entry_points> entry_point_flags;
	c_typed_tag_block<s_compiled_shader_reference> entry_points;
	int32 version;
	c_typed_tag_block<c_rasterizer_compiled_pixel_shader> compiled_shader;
};
COMPILE_ASSERT(sizeof(c_rasterizer_pixel_shader) == 0x20);

