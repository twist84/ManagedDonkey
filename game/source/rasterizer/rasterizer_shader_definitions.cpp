#include "rasterizer/rasterizer_shader_definitions.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"

const s_compiled_shader_reference* s_rasterizer_vertex_shader_entry_point::get_shader_reference(e_vertex_type vertex_type) const
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

const c_rasterizer_vertex_shader* c_rasterizer_vertex_shader::get(int32 definition_index)
{
	const c_rasterizer_vertex_shader* result = TAG_GET(VERTEX_SHADER_TAG, c_rasterizer_vertex_shader const, definition_index);
	return result;
}

c_rasterizer_vertex_shader* c_rasterizer_vertex_shader::get_modifiable(int32 definition_index)
{
	c_rasterizer_vertex_shader* result = TAG_GET(VERTEX_SHADER_TAG, c_rasterizer_vertex_shader, definition_index);
	return result;
}

const s_rasterizer_vertex_shader_entry_point* c_rasterizer_vertex_shader::get_entry_point(int32 entry_point) const
{
	return &entry_points[entry_point];
}

const c_rasterizer_compiled_vertex_shader* c_rasterizer_vertex_shader::get_compiled_shader(int32 shader_index) const
{
	return &compiled_shader[shader_index];
}

const c_rasterizer_compiled_vertex_shader* c_rasterizer_vertex_shader::get_compiled_shader(e_vertex_type vertex_type, e_entry_point entry_point, int32 shader_index) const
{
	if (entry_point >= entry_points.count)
	{
		event(_event_error, "rasterizer: vertex shader entry point (%d) doesn't exist - shader probably failed to compile", entry_point);
		return nullptr;
	}

	const s_rasterizer_vertex_shader_entry_point* shader_entry_point = get_entry_point(entry_point);
	if (vertex_type >= shader_entry_point->vertex_types.count)
	{
		return nullptr;
	}

	const s_compiled_shader_reference* shader_reference = shader_entry_point->get_shader_reference(vertex_type);
	if (!VALID_INDEX(shader_index, shader_reference->count))
	{
		nullptr;
	}

	return get_compiled_shader(shader_reference->start_index + shader_index);
}

IDirect3DVertexShader9* c_rasterizer_vertex_shader::get_d3d_shader(e_vertex_type vertex_type, e_entry_point entry_point, int32 shader_index) const
{
	if (const c_rasterizer_compiled_vertex_shader* compiled_shader = get_compiled_shader(vertex_type, entry_point, shader_index))
	{
		return compiled_shader->get_d3d_shader();
	}

	return nullptr;
}

const c_rasterizer_pixel_shader* c_rasterizer_pixel_shader::get(int32 definition_index)
{
	const c_rasterizer_pixel_shader* result = TAG_GET(VERTEX_SHADER_TAG, c_rasterizer_pixel_shader const, definition_index);
	return result;
}

c_rasterizer_pixel_shader* c_rasterizer_pixel_shader::get_modifiable(int32 definition_index)
{
	c_rasterizer_pixel_shader* result = TAG_GET(VERTEX_SHADER_TAG, c_rasterizer_pixel_shader, definition_index);
	return result;
}

const c_rasterizer_compiled_pixel_shader* c_rasterizer_pixel_shader::get_compiled_shader(e_entry_point entry_point, int32 shader_index) const
{
	if (entry_point >= entry_points.count)
	{
		event(_event_error, ": pixel shader entry point (%d) doesn't exist - shader probably failed to compile", entry_point);
		return nullptr;
	}

	s_compiled_shader_reference& reference = entry_points[entry_point];
	if (!VALID_INDEX(shader_index, reference.count))
	{
		return nullptr;
	}

	return &compiled_shader[shader_index + reference.start_index];
}

IDirect3DPixelShader9* c_rasterizer_pixel_shader::get_d3d_shader(e_entry_point entry_point, int32 shader_index) const
{
	if (const c_rasterizer_compiled_pixel_shader* compiled_shader = get_compiled_shader(entry_point, shader_index))
	{
		return compiled_shader->get_d3d_shader();
	}

	return nullptr;
}

