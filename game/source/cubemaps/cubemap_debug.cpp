#include "cubemaps/cubemap_debug.hpp"

#include "cseries/cseries.hpp"
#include "game/player_mapping.hpp"
#include "objects/objects.hpp"
#include "render/render_debug.hpp"
#include "render/render_objects_static_lighting.hpp"
#include "scenario/scenario_cubemap_definitions.hpp"

bool c_cubemap_debug::g_render = false;

void c_cubemap_debug::render()
{
	if (g_render)
	{
		render_user_cubemap_samples(0);
	}
}

void c_cubemap_debug::render_user_cubemap_samples(int32 user_index)
{
	if (g_render && user_index != 4)
	{
		int32 object_index = player_mapping_get_unit_by_output_user(user_index);
		render_object_cubemap_samples(object_index);
	}
}

void __cdecl c_cubemap_debug::render_object_cubemap_samples(int32 object_index)
{
	if (g_render && object_index != NONE)
	{
		const render_lighting* object_lighting = object_get_cached_render_lighting(object_index);
		if (object_lighting)
		{
			s_structure_cluster_cubemap_info* cubemap_info0 = structure_cluster_try_and_get_cubemap_info(object_lighting->cubemap_state.m_current.cluster_reference, object_lighting->cubemap_state.m_current.cluster_cubemap_index);
			s_structure_cluster_cubemap_info* cubemap_info1 = structure_cluster_try_and_get_cubemap_info(object_lighting->cubemap_state.m_last.cluster_reference, object_lighting->cubemap_state.m_last.cluster_cubemap_index);

			real_argb_color color = { 1.0f, 1.0f, 1.0f, 0.2f };
			if (cubemap_info0)
			{
				render_debug_sphere(false, &cubemap_info0->cubemap_position, 10.0f, &color);
			}

			if (cubemap_info1)
			{
				color.green = 0.0f;
				render_debug_sphere(false, &cubemap_info1->cubemap_position, 10.0f, &color);
			}

			if (object_datum* object = object_get(object_index))
			{
				color.green = object_lighting->cubemap_state.m_blend_factor;

				if (cubemap_info0 && color.green > 0.0f)
				{
					render_debug_line(false, &cubemap_info0->cubemap_position, &object->object.bounding_sphere_center, &color);
				}

				if (cubemap_info1 && color.green < 1.0f)
				{
					render_debug_line(false, &object->object.bounding_sphere_center, &cubemap_info1->cubemap_position, &color);
				}
			}
		}
	}
}

