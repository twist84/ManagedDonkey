#include "rasterizer/rasterizer_shader_definitions.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"

c_rasterizer_pixel_shader const* c_rasterizer_pixel_shader::get(long definition_index)
{
	return static_cast<c_rasterizer_pixel_shader*>(tag_get('pixl', definition_index));
}

c_rasterizer_pixel_shader* c_rasterizer_pixel_shader::get_modifiable(long definition_index)
{
	return static_cast<c_rasterizer_pixel_shader*>(tag_get('pixl', definition_index));
}

c_rasterizer_compiled_pixel_shader const* c_rasterizer_pixel_shader::get_compiled_shader(e_entry_point entry_point, long shader_index) const
{
	if (entry_point < entry_points.count())
	{
		s_compiled_shader_reference& reference = entry_points[entry_point];

		if (VALID_INDEX(shader_index, reference.count))
		{
			return &compiled_shader[shader_index + reference.start_index];
		}
	}

	return NULL;
}

IDirect3DPixelShader9* c_rasterizer_pixel_shader::get_d3d_shader(e_entry_point entry_point, long shader_index) const
{
	if (c_rasterizer_compiled_pixel_shader const* compiled_shader = get_compiled_shader(entry_point, shader_index))
	{
		return compiled_shader->get_d3d_shader();
	}

	generate_event(_event_level_error, ": pixel shader entry point (%d) doesn't exist - shader probably failed to compile", entry_point);
	return NULL;
}

IDirect3DPixelShader9* c_rasterizer_compiled_pixel_shader::get_d3d_shader() const
{
	return runtime_shader;
}

