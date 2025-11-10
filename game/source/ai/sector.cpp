#include "ai/sector.hpp"

#include "ai/ai_hint_definitions.hpp"
#include "ai/sector_definitions.hpp"
#include "render/render_debug.hpp"

const pathfinding_data* __cdecl pathfinding_data_get(int16 structure_bsp_index)
{
	return INVOKE(0x01445440, pathfinding_data_get, structure_bsp_index);
}

void sector_link_render_debug(int32 link_index, const pathfinding_data* pf_data, const real_argb_color* color, bool a4)
{
	if (VALID_INDEX(link_index, pf_data->sector_links.count))
	{
		sector_link& link = pf_data->sector_links[link_index];
		sector_vertex& vertex1 = pf_data->sector_vertices[link.index];
		sector_vertex& vertex2 = pf_data->sector_vertices[link.index2];
		real_point3d center{};

		add_vectors3d((real_vector3d*)&vertex1.point, (real_vector3d*)&vertex2.point, (real_vector3d*)&center);
		scale_vector3d((real_vector3d*)&center, 0.5f, (real_vector3d*)&center);

		if (!color)
		{
			if (link.hint_index != NONE)
			{
				pathfinding_hint_data& hint = pf_data->hints[link.hint_index];
				switch (hint.type)
				{
				case _hint_type_intersection:
				{
					color = global_real_argb_red;
				}
				break;
				case _hint_type_seam:
				{
					color = global_real_argb_green;
				}
				break;
				case _hint_type_door:
				{
					color = global_real_argb_blue;
				}
				break;
				}
			}

			if (!color && TEST_BIT(link.flags, _sector_link_section_link_bit))
			{
				color = global_real_argb_yellow;
			}

			if (!color)
			{
				color = global_real_argb_grey;
			}
		}

		render_debug_line(true, &vertex1.point, &vertex2.point, color);

		if (a4)
		{
			render_debug_sphere(true, &center, 0.01f, color);
		}
	}
}

