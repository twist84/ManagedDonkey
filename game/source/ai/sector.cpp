#include "ai/sector.hpp"

#include "ai/sector_definitions.hpp"
#include "render/render_debug.hpp"

pathfinding_data const* __cdecl pathfinding_data_get(short structure_bsp_index)
{
	return INVOKE(0x01445440, pathfinding_data_get, structure_bsp_index);
}

void sector_link_render_debug(long link_index, pathfinding_data const* pf_data, real_argb_color const* color, bool a4)
{
	if (VALID_INDEX(link_index, pf_data->links.count()))
	{
		sector_link& link = pf_data->links[link_index];
		sector_vertex& vertex1 = pf_data->vertices[link.vertex1];
		sector_vertex& vertex2 = pf_data->vertices[link.vertex2];
		real_point3d center{};

		add_vectors3d((vector3d*)&vertex1.point, (vector3d*)&vertex2.point, (vector3d*)&vertex1.point);
		scale_vector3d((vector3d*)&vertex1.point, 0.5f, (vector3d*)&center);

		if (!color)
		{
			if (link.hint_index != NONE)
			{
				pathfinding_hint_data& hint = pf_data->pathfinding_hints[link.hint_index];
				e_hint_type hint_type = hint.hint_type;
				switch (hint_type)
				{
				case _hint_type_intersection_link:
					color = global_real_argb_red;
					break;
				case _hint_type_seam_link:
					color = global_real_argb_green;
					break;
				case _hint_type_door_link:
					color = global_real_argb_blue;
					break;
				}
			}

			if (!color && link.link_flags.test(_sector_link_section_link_bit))
				color = global_real_argb_yellow;

			if (!color)
				color = global_real_argb_grey;
		}

		render_debug_line(true, &vertex1.point, &vertex2.point, color);

		if (a4)
			render_debug_sphere(true, &center, 0.01f, color);
	}
}

