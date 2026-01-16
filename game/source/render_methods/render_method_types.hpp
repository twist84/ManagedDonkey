#pragma once

#include "render_methods/render_method_definitions.hpp"

class c_render_method_shader_decal :
	public c_render_method
{
public:
	enum
	{
		k_tag_type = SHADER_DECAL_TAG,
	};
};
COMPILE_ASSERT(sizeof(c_render_method_shader_decal) == 0x40);
COMPILE_ASSERT(sizeof(c_render_method_shader_decal) == sizeof(c_render_method));

