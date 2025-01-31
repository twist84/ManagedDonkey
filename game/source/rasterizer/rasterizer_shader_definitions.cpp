#include "rasterizer/rasterizer_shader_definitions.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"

s_compiled_shader_reference const* s_rasterizer_vertex_shader_entry_point::get_shader_reference(e_vertex_type vertex_type) const
{
	return &vertex_types[vertex_type];
}

IDirect3DVertexShader9* c_rasterizer_compiled_vertex_shader::get_d3d_shader() const
{
	return runtime_shader;
}

IDirect3DPixelShader9* c_rasterizer_compiled_pixel_shader::get_d3d_shader() const
{
	return runtime_shader;
}

c_rasterizer_vertex_shader const* c_rasterizer_vertex_shader::get(long definition_index)
{
	c_rasterizer_vertex_shader const* result = TAG_GET(VERTEX_SHADER_TAG, c_rasterizer_vertex_shader const, definition_index);
	return result;
}

c_rasterizer_vertex_shader* c_rasterizer_vertex_shader::get_modifiable(long definition_index)
{
	c_rasterizer_vertex_shader* result = TAG_GET(VERTEX_SHADER_TAG, c_rasterizer_vertex_shader, definition_index);
	return result;
}

s_rasterizer_vertex_shader_entry_point const* c_rasterizer_vertex_shader::get_entry_point(long entry_point) const
{
	return &entry_points[entry_point];
}

c_rasterizer_compiled_vertex_shader const* c_rasterizer_vertex_shader::get_compiled_shader(long shader_index) const
{
	return &compiled_shader[shader_index];
}

c_rasterizer_compiled_vertex_shader const* c_rasterizer_vertex_shader::get_compiled_shader(e_vertex_type vertex_type, e_entry_point entry_point, long shader_index) const
{
	if (entry_point >= entry_points.count)
	{
		event(_event_error, "rasterizer: vertex shader entry point (%d) doesn't exist - shader probably failed to compile", entry_point);
		return NULL;
	}

	s_rasterizer_vertex_shader_entry_point const* shader_entry_point = get_entry_point(entry_point);
	if (vertex_type >= shader_entry_point->vertex_types.count)
		return NULL;

	s_compiled_shader_reference const* shader_reference = shader_entry_point->get_shader_reference(vertex_type);
	if (!VALID_INDEX(shader_index, shader_reference->count))
		NULL;

	return get_compiled_shader(shader_reference->start_index + shader_index);
}

IDirect3DVertexShader9* c_rasterizer_vertex_shader::get_d3d_shader(e_vertex_type vertex_type, e_entry_point entry_point, long shader_index) const
{
	if (c_rasterizer_compiled_vertex_shader const* compiled_shader = get_compiled_shader(vertex_type, entry_point, shader_index))
		return compiled_shader->get_d3d_shader();

	return NULL;
}

c_rasterizer_pixel_shader const* c_rasterizer_pixel_shader::get(long definition_index)
{
	c_rasterizer_pixel_shader const* result = TAG_GET(VERTEX_SHADER_TAG, c_rasterizer_pixel_shader const, definition_index);
	return result;
}

c_rasterizer_pixel_shader* c_rasterizer_pixel_shader::get_modifiable(long definition_index)
{
	c_rasterizer_pixel_shader* result = TAG_GET(VERTEX_SHADER_TAG, c_rasterizer_pixel_shader, definition_index);
	return result;
}

c_rasterizer_compiled_pixel_shader const* c_rasterizer_pixel_shader::get_compiled_shader(e_entry_point entry_point, long shader_index) const
{
	if (entry_point >= entry_points.count)
	{
		event(_event_error, ": pixel shader entry point (%d) doesn't exist - shader probably failed to compile", entry_point);
		return NULL;
	}

	s_compiled_shader_reference& reference = entry_points[entry_point];
	if (!VALID_INDEX(shader_index, reference.count))
		return NULL;

	return &compiled_shader[shader_index + reference.start_index];
}

IDirect3DPixelShader9* c_rasterizer_pixel_shader::get_d3d_shader(e_entry_point entry_point, long shader_index) const
{
	if (c_rasterizer_compiled_pixel_shader const* compiled_shader = get_compiled_shader(entry_point, shader_index))
		return compiled_shader->get_d3d_shader();

	return NULL;
}

