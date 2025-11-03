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
static_assert(sizeof(c_render_method_shader_decal) == 0x40);
static_assert(sizeof(c_render_method_shader_decal) == sizeof(c_render_method));

