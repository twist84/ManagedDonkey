#include "render/render_debug_structure.hpp"

#include "physics/physics_constants.hpp"
#include "render/render_debug.hpp"
#include "render/views/render_view.hpp"
#include "scenario/scenario.hpp"
#include "scenario/scenario_soft_ceilings.hpp"
#include "structures/structure_bsp_definitions.hpp"
#include "visibility/visibility_collection.hpp"
#include "physics/collision_references.hpp"

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
			structure_bsp_index != NONE;
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

	if (debug_structure_slip_surfaces)
	{
		c_render_debug_line_drawer debug_line_drawer_green{};
		c_render_debug_line_drawer debug_line_drawer_red{};

		debug_line_drawer_green.set_color(global_real_argb_green);
		debug_line_drawer_red.set_color(global_real_argb_red);

		for (long structure_bsp_index = global_structure_bsp_first_active_index_get();
			structure_bsp_index != NONE;
			structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
		{
			c_collision_bsp_reference bsp_reference(global_structure_bsp_get(structure_bsp_index));
			for (long edge_index = 0; edge_index < bsp_reference.get_edge_count(); edge_index++)
			{
				c_collision_edge_reference edge_reference(bsp_reference, edge_index);
				c_collision_vertex_reference start_vertex_reference(bsp_reference, edge_reference.get_vertex_index(0));
				c_collision_vertex_reference end_vertex_reference(bsp_reference, edge_reference.get_vertex_index(1));

				bool surface_reference_has_slip_bit_enabled = false;
				bool slip_surface_does_not_exceed_maximum_k = true;

				for (long i = 0; i < NUMBEROF(collision_edge::surface_indices); i++)
				{
					if (edge_reference.get_surface_index(i) != NONE)
					{
						c_collision_surface_reference surface_reference(bsp_reference, edge_reference.get_surface_index(i));
						if (TEST_BIT(surface_reference.get_flags(), _surface_flag_slip_bit))
						{
							surface_reference_has_slip_bit_enabled = true;

							plane3d plane{};
							surface_reference.get_plane(&plane);
							if (plane.normal.k > global_slip_surface_maximum_k_get() - 0.001f)
								slip_surface_does_not_exceed_maximum_k = false;
						}
					}

				}

				if (surface_reference_has_slip_bit_enabled)
				{
					real_point3d* end_vertex_position = end_vertex_reference.get_position();
					real_point3d* start_vertex_position = start_vertex_reference.get_position();

					if (slip_surface_does_not_exceed_maximum_k)
						debug_line_drawer_green.add_line_3d_unclipped(end_vertex_position, start_vertex_position);
					else
						debug_line_drawer_red.add_line_3d_unclipped(end_vertex_position, start_vertex_position);
				}
			}

			//ASSERT(c_visible_items::get_instances_starting_index() == 0);
			//for (long i = 0; i < c_visible_items::m_items.instances.get_count(); i++)
			//{
			//	s_visible_instances& instance = c_visible_items::m_items.instances[i];
			//}
		}
	}
}

void __cdecl render_debug_structure_draw()
{
	// #TODO: implement this?
}

