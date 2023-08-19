#include "render/render_debug_structure.hpp"

#include "render/views/render_view.hpp"
#include "scenario/scenario_soft_ceilings.hpp"

bool debug_structure_soft_ceilings = false;
bool debug_structure_soft_kill = false;
bool debug_structure_slip_surfaces = false;

void __cdecl render_debug_structure()
{
	render_camera const* rasterizer_camera = c_player_view::get_global_player_view(0)->get_rasterizer_camera();

    scenario_soft_ceilings_render_debug(
		&rasterizer_camera->position,
		debug_structure_soft_ceilings,
		debug_structure_soft_kill,
		debug_structure_slip_surfaces);
}

