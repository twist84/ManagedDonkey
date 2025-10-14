#include "scenario/scenario_soft_ceilings.hpp"

#include "memory/module.hpp"
#include "render/render_debug.hpp"
#include "scenario/scenario.hpp"
#include "structures/structure_physics_definitions.hpp"

bool soft_ceilings_disable = false;

c_static_stack<s_soft_ceiling_debug_cache, 64> g_soft_ceiling_debug_cache;

void __cdecl scenario_soft_ceiling_enable(string_id name, bool enable)
{
	INVOKE(0x00674640, scenario_soft_ceiling_enable, name, enable);
}

bool __cdecl scenario_soft_ceiling_is_active(string_id name)
{
	return INVOKE(0x006746C0, scenario_soft_ceiling_is_active, name);
}

void __cdecl scenario_soft_ceilings_dispose()
{
	INVOKE(0x00674960, scenario_soft_ceilings_dispose);
}

void __cdecl scenario_soft_ceilings_dispose_from_old_map()
{
	INVOKE(0x00674970, scenario_soft_ceilings_dispose_from_old_map);
}

void __cdecl scenario_soft_ceilings_initialize()
{
	INVOKE(0x00674C50, scenario_soft_ceilings_initialize);
}

void __cdecl scenario_soft_ceilings_initialize_for_new_map()
{
	INVOKE(0x00674CB0, scenario_soft_ceilings_initialize_for_new_map);
}

bool __cdecl scenario_soft_ceilings_should_render_soft_ceiling(string_id name, bool biped, bool vehicle, bool huge_vehicle, bool camera)
{
	struct scenario* scenario = global_scenario_get();
	if (!scenario)
		return true;

	bool result = false;
	for (s_scenario_soft_ceiling& soft_ceiling : scenario->soft_ceilings)
	{
		if (soft_ceiling.name.get_value() != name)
			continue;

		if (biped && !soft_ceiling.flags.test(_scenario_soft_ceiling_flag_ignore_bipeds_bit))
			result = true;

		if (vehicle && !soft_ceiling.flags.test(_scenario_soft_ceiling_flag_ignore_vehicles_bit))
			result = true;

		if (huge_vehicle && !soft_ceiling.flags.test(_scenario_soft_ceiling_flag_ignore_huge_vehicles_bit))
			result = true;

		if (camera && !soft_ceiling.flags.test(_scenario_soft_ceiling_flag_ignore_camera_bit))
			result = true;
	}

	return result;
}

bool __cdecl scenario_soft_ceilings_triangle_plane_get(real_plane3d* plane, const s_structure_soft_ceiling_triangle* triangle)
{
	real_vector3d vector0{};
	real_vector3d vector1{};
	vector_from_points3d(&triangle->vertex0, &triangle->vertex1, &vector0);
	vector_from_points3d(&triangle->vertex0, &triangle->vertex2, &vector1);
	cross_product3d(&vector0, &vector1, &plane->n);

	if (normalize3d(&plane->n) == 0.0f)
		return false;

	plane->d = dot_product3d(&plane->n, &triangle->plane.n);

	return true;
}

bool __cdecl scenario_soft_ceilings_plane_get(real_plane3d* plane, int32 structure_design_index, int32 soft_ceiling_index, int32 triangle_index)
{
	s_structure_design* structure_design = global_structure_design_get(structure_design_index);
	if (!structure_design)
		return false;

	s_structure_soft_ceiling& soft_ceiling = structure_design->physics.soft_ceilings[soft_ceiling_index];
	s_structure_soft_ceiling_triangle& triangle = soft_ceiling.soft_ceiling_triangles[triangle_index];

	return scenario_soft_ceilings_triangle_plane_get(plane, &triangle);
}

void __cdecl scenario_soft_ceilings_render_debug(
	const real_point3d* point,
	bool render_soft_ceilings,
	bool render_soft_kill,
	bool render_slip_surfaces)
{
	if (render_soft_ceilings || render_soft_kill || render_slip_surfaces)
	{
		for (int32 structure_bsp_index = global_structure_bsp_first_active_index_get();
			structure_bsp_index != NONE;
			structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
		{
			s_structure_design* structure_design = global_structure_design_get(structure_bsp_index);
			if (!structure_design)
				continue;

			int32 soft_ceiling_index = 0;
			for (s_structure_soft_ceiling& soft_ceiling : structure_design->physics.soft_ceilings)
			{
				int32 triangle_index = 0;
				for (s_structure_soft_ceiling_triangle& triangle : soft_ceiling.soft_ceiling_triangles)
				{
					switch (soft_ceiling.type.get())
					{
					case _soft_ceiling_type_soft_kill:
					{
						if (!render_soft_kill)
							break;

						render_debug_triangle_outline(true, &triangle.vertex0, &triangle.vertex1, &triangle.vertex2, global_real_argb_cyan);

						real_argb_color soft_kill_color = *global_real_argb_yellow;
						soft_kill_color.alpha = 0.15f;
						render_debug_triangle(true, &triangle.vertex0, &triangle.vertex1, &triangle.vertex2, &soft_kill_color);
					}
					break;
					case _soft_ceiling_type_slip_surface:
					{
						if (!render_slip_surfaces)
							break;

						render_debug_triangle_outline(true, &triangle.vertex0, &triangle.vertex1, &triangle.vertex2, global_real_argb_black);

						real_argb_color slip_surfaces_color = *global_real_argb_red;
						slip_surfaces_color.alpha = 0.15f;
						render_debug_triangle(true, &triangle.vertex0, &triangle.vertex1, &triangle.vertex2, &slip_surfaces_color);
					}
					break;
					default:
					{
						real_plane3d plane{};
						if (!render_soft_ceilings || structure_design->physics.importer_version < 1 || !scenario_soft_ceilings_plane_get(&plane, structure_bsp_index, soft_ceiling_index, triangle_index))
							break;

						real32 distance_to_point = plane3d_distance_to_point(&plane, point);

						real_argb_color soft_ceilings_color = *global_real_argb_pink;
						if (distance_to_point >= 0.0f)
							soft_ceilings_color = *global_real_argb_cyan;

						if (scenario_soft_ceiling_is_active(soft_ceiling.name.get_value()))
						{
							soft_ceilings_color = *global_real_argb_red;
							if (distance_to_point >= 0.0f)
								soft_ceilings_color = *global_real_argb_blue;
						}

						soft_ceilings_color.alpha = 0.15f;
						render_debug_triangle_outline(true, &triangle.vertex0, &triangle.vertex1, &triangle.vertex2, &soft_ceilings_color);
						render_debug_triangle(true, &triangle.vertex0, &triangle.vertex1, &triangle.vertex2, &soft_ceilings_color);
					}
					break;
					}

					triangle_index++;
				}

				soft_ceiling_index++;
			}
		}
	}
}

__declspec(naked) void soft_ceilings_disable_inline0()
{
	// original instructions
	//     cmp dword ptr[esp + 0xB4], 0
	//     jle loc_67603E

	ASM_ADDR(0x00675E51, addr_675E51);
	ASM_ADDR(0x0067603E, addr_67603E);

	__asm
	{
		// execute our instructions
		cmp soft_ceilings_disable, 0
		jnz loc_67603E

		// execute the original instructions
		cmp dword ptr[esp + 0xB4], 0
		jle loc_67603E

		// jump out to after our hook
		jmp addr_675E51

	loc_67603E:
		jmp addr_67603E
	}
}
HOOK_DECLARE(0x00675E43, soft_ceilings_disable_inline0);

__declspec(naked) void soft_ceilings_disable_inline1()
{
	// original instructions
	//		cmp[esp + 0x28], eax
	//		jz      loc_6755B4
	//		movss   xmm1, dword ptr[esp + 0x50]
	//		xorps   xmm0, xmm0
	//		comiss  xmm0, xmm1
	//		jbe     loc_6755B4

	ASM_ADDR(0x00675518, addr_675518);
	ASM_ADDR(0x006755B4, addr_6755B4);

	__asm
	{
		// execute our instructions
		cmp soft_ceilings_disable, 0
		jz      loc_6755B4

		// execute the original instructions
		cmp[esp + 0x28], eax
		jz      loc_6755B4

		movss   xmm1, dword ptr[esp + 0x50]
		xorps   xmm0, xmm0
		comiss  xmm0, xmm1
		jbe     loc_6755B4

		// jump out to after our hook
		jmp addr_675518

	loc_6755B4:
		jmp addr_6755B4
	}
}
HOOK_DECLARE(0x006754FC, soft_ceilings_disable_inline1);

