#include "render/render_debug_structure.hpp"

#include "render/render_debug.hpp"
#include "render/views/render_view.hpp"
#include "scenario/scenario.hpp"
#include "scenario/scenario_soft_ceilings.hpp"
#include "structures/structure_bsp_definitions.hpp"

bool debug_structure_markers = false;
bool debug_structure_soft_ceilings = false;
bool debug_structure_soft_kill = false;
bool debug_structure_slip_surfaces = false;

void __cdecl render_debug_structure()
{
	render_camera const* rasterizer_camera = c_player_view::get_global_player_view()->get_rasterizer_camera();

	if (debug_structure_markers)
	{
		for (long structure_bsp_index = global_structure_bsp_first_active_index_get();
			structure_bsp_index != -1;
			structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
		{
			structure_bsp* bsp = global_structure_bsp_get(structure_bsp_index);
			for (structure_marker& marker : bsp->markers)
			{
				real_matrix4x3 matrix{};
				matrix4x3_from_point_and_quaternion(&matrix, &marker.position, &marker.rotation);
				render_debug_matrix(true, &matrix, 0.05f);
				render_debug_string_at_point(&matrix.center, marker.name.get_string(), global_real_argb_white);
			}
		}
	}

    scenario_soft_ceilings_render_debug(
		&rasterizer_camera->position,
		debug_structure_soft_ceilings,
		debug_structure_soft_kill,
		debug_structure_slip_surfaces);
}

void __cdecl render_debug_structure_draw()
{
	// #TODO: implement this?
}

