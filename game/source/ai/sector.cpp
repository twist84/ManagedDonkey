#include "ai/sector.hpp"

#include "ai/ai_hint_definitions.hpp"
#include "ai/sector_definitions.hpp"
#include "render/render_debug.hpp"

//.text:01445320 ; void __cdecl get_sector_flags_normal(int32, real_vector3d*)
//.text:014453E0 ; void __cdecl get_sector_normal(sector*, real_vector3d*, bool)
//.text:01445420 ; bool __cdecl pathfinding_data_generate(int16)
//.text:01445430 ; bool __cdecl pathfinding_data_generate_all(void)

const pathfinding_data* __cdecl pathfinding_data_get(int16 structure_bsp_index)
{
	return INVOKE(0x01445440, pathfinding_data_get, structure_bsp_index);
}

//.text:014454A0 ; bool __cdecl pathfinding_data_valid(int32, int16)
//.text:01445510 ; sector* __cdecl pathfinding_get_sector(const pathfinding_data*, int16)

sector* __cdecl pathfinding_get_sector(c_sector_ref ref)
{
	return INVOKE(0x01445530, pathfinding_get_sector, ref);
}

//.text:01445590 ; uns16 __cdecl runtime_ccw_link(const pathfinding_data*, int32, int32)
//.text:014455C0 ; uns16 __cdecl runtime_cw_link(const pathfinding_data*, int32, int32)
//.text:01445680 ; void __cdecl runtime_link_iterator_new(runtime_link_iterator*, const pathfinding_data*, int32)
//.text:014456C0 ; void __cdecl runtime_link_iterator_new(runtime_link_iterator*, const pathfinding_data*, int32, int32)
//.text:01445700 ; sector_link* __cdecl runtime_link_iterator_next(runtime_link_iterator*)
//.text:01445790 ; bool __cdecl sector_broken(int16, pathfinding_data*, sector*)
//.text:01445840 ; bool __cdecl sector_broken(c_sector_ref)

void c_sector_ref::clear()
{
	m_bsp_index = NONE;
	m_sector_index = NONE;
}

int32 c_sector_ref::get_index() const
{
	return (m_bsp_index << 16) | m_sector_index;
}

const pathfinding_data* c_sector_ref::pathfinding_data()
{
	return pathfinding_data_get(m_bsp_index);
}

void c_sector_ref::set(int16 bsp_index, uns16 sector_index)
{
	//INVOKE_CLASS_MEMBER(0x0145FC30, c_sector_ref, set, bsp_index, sector_index);

	if (sector_index == NONE)
	{
		m_bsp_index = NONE;
		m_sector_index = NONE;
	}
	else
	{
		m_bsp_index = bsp_index;
		m_sector_index = sector_index;
	}
}

bool c_sector_ref::valid() const
{
	return INVOKE_CLASS_MEMBER(0x014CBED0, c_sector_ref, valid);
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

