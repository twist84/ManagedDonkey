#include "render/render_debug_structure.hpp"

#include "physics/physics_constants.hpp"
#include "render/render_debug.hpp"
#include "render/old_render_debug.hpp"
#include "render/views/render_view.hpp"
#include "scenario/scenario.hpp"
#include "scenario/scenario_soft_ceilings.hpp"
#include "structures/structure_bsp_definitions.hpp"
#include "visibility/visibility_collection.hpp"
#include "physics/collision_references.hpp"
#include "physics/water_physics.hpp"

#include <math.h>

real32 k_log_complex_geometry_edge_length_cutoff = log10f(0.2f);
real32 k_log_complex_geometry_edge_length_max = log10f(0.2f);
real32 k_log_complex_geometry_edge_length_min = log10f(0.05f);

void __cdecl render_debug_structure()
{
	const bool lost_camera = render_debug_lost_camera();
	const render_camera* render_camera = c_player_view::get_current()->get_rasterizer_camera();
	s_cluster_reference camera_cluster_reference{};
	c_player_view::get_current()->get_starting_cluster(&camera_cluster_reference);

	int32 debug_structure_bsp_index = global_structure_bsp_first_active_index_get();
	if (global_structure_bsp_is_active(camera_cluster_reference.bsp_index))
	{
		debug_structure_bsp_index = camera_cluster_reference.bsp_index;
	}

	if (global_structure_bsp_is_active(debug_structure_cluster_structure_bsp_index))
	{
		const structure_bsp* structure = global_structure_bsp_get(debug_structure_cluster_structure_bsp_index);
		if (VALID_INDEX(debug_structure_cluster_cluster_index, structure->clusters.count))
		{
			const structure_cluster* cluster = TAG_BLOCK_GET_ELEMENT(&structure->clusters, debug_structure_cluster_cluster_index, const structure_cluster);
			int32 sky_index = scenario_zone_set_structure_bsp_cluster_visible_sky_index_get(
				global_scenario_get(),
				scenario_zone_set_index_get(),
				debug_structure_cluster_structure_bsp_index,
				debug_structure_cluster_cluster_index);
			
			c_static_string<128> string;
			string.print("bsp %d,%d sky %d",
				debug_structure_cluster_structure_bsp_index,
				debug_structure_cluster_structure_bsp_index,
				sky_index);

			real_point3d center{};
			rectangle3d_center(&cluster->bounds, &center);

			const real_argb_color* color = global_real_argb_red;
			render_debug_box_outline(true, &cluster->bounds, color);

			render_debug_string_at_point(&center, string.get_string(), color);

			for (int32 portal_index_index = 0; portal_index_index < cluster->portal_indices.count; portal_index_index++)
			{
				int32 portal_index = *TAG_BLOCK_GET_ELEMENT(&cluster->portal_indices, portal_index_index, int16);
				cluster_portal* portal = TAG_BLOCK_GET_ELEMENT(&structure->cluster_portals, portal_index, cluster_portal);
				for (int32 vertex_index = 0; vertex_index < portal->vertices.count; vertex_index++)
				{
					const real_point3d* vertex = TAG_BLOCK_GET_ELEMENT(&portal->vertices, vertex_index, const real_point3d);
					int32 alt_vertex_index = (vertex_index + 1) % portal->vertices.count;
					const real_point3d* alt_vertex = TAG_BLOCK_GET_ELEMENT(&portal->vertices, alt_vertex_index, const real_point3d);
					render_debug_line(true, vertex, alt_vertex, global_real_argb_white);
				}
			}
		}
	}

	if (debug_structure_markers)
	{
		for (int32 debug_structure_bsp_index = global_structure_bsp_first_active_index_get();
			debug_structure_bsp_index != NONE;
			debug_structure_bsp_index = global_structure_bsp_next_active_index_get(debug_structure_bsp_index))
		{
			structure_bsp* structure = global_structure_bsp_get(debug_structure_bsp_index);
			for (structure_marker& marker : structure->markers)
			{
				real_matrix4x3 matrix{};
				matrix4x3_from_point_and_quaternion(&matrix, &marker.position, &marker.rotation);
				render_debug_matrix(true, &matrix, 0.05f);
				render_debug_string_at_point(&matrix.position, marker.name.get_string(), global_real_argb_white);
			}
		}
	}

	if (debug_structure_surface_references)
	{

	}

	scenario_soft_ceilings_render_debug(
		&render_camera->position,
		debug_structure_soft_ceilings,
		debug_structure_soft_kill,
		debug_structure_slip_surfaces);

	if (debug_structure_cluster_skies)
	{

	}

	if (debug_structure_invisible)
	{

	}

	water_physics_render_debug(&render_camera->position, &render_camera->forward);

	if (debug_structure || debug_structure_automatic && render_debug_lost_camera())
	{
		real_rectangle3d world_bounds = *global_null_rectangle3d;

		c_render_debug_line_drawer debug_line_drawer{};
		debug_line_drawer.set_color(global_real_argb_green);

		for (int32 structure_bsp_index = global_structure_bsp_first_active_index_get();
			structure_bsp_index != NONE;
			structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
		{
			if (debug_structure_unique_colors)
			{
				real_argb_color color{};
				render_debug_unique_color(structure_bsp_index, &color);
				debug_line_drawer.set_color(&color);
			}

			if (debug_plane_index != NONE && debug_structure)
			{
				c_collision_bsp_reference bsp_reference(global_structure_bsp_get(structure_bsp_index));
				int32 edge_count = bsp_reference.get_edge_count();
				for (int32 edge_index = 0; edge_index < edge_count; edge_index++)
				{
					c_collision_edge_reference edge_reference(bsp_reference, edge_index);
					c_collision_vertex_reference start_vertex_reference(bsp_reference, edge_reference.get_vertex_index(0));
					c_collision_vertex_reference end_vertex_reference(bsp_reference, edge_reference.get_vertex_index(1));

					for (int32 i = 0; i < NUMBEROF(collision_edge::surface_indices); i++)
					{
						if (edge_reference.get_surface_index(i) != NONE)
						{
							c_collision_surface_reference surface_reference(bsp_reference, edge_reference.get_surface_index(i));
							if (surface_reference.get_plane_index() == debug_plane_index)
							{
								debug_line_drawer.add_line_3d_unclipped(start_vertex_reference.get_position(), end_vertex_reference.get_position());
								break;
							}
						}
					}
				}
			}
			else
			{
				c_collision_bsp_reference bsp_reference(global_structure_bsp_get(structure_bsp_index));
				int32 edge_count = bsp_reference.get_edge_count();
				for (int32 edge_index = 0; edge_index < edge_count; edge_index++)
				{
					c_collision_edge_reference edge_reference(bsp_reference, edge_index);
					c_collision_vertex_reference start_vertex_reference(bsp_reference, edge_reference.get_vertex_index(0));
					c_collision_vertex_reference end_vertex_reference(bsp_reference, edge_reference.get_vertex_index(1));

					if (debug_structure_complexity)
					{
						real32 log_edge_length = 0.5f * log10f(distance_squared3d(start_vertex_reference.get_position(), end_vertex_reference.get_position()));
						if (log_edge_length < k_log_complex_geometry_edge_length_cutoff)
						{
							real32 green_fraction = (fminf(fmaxf(log_edge_length, k_log_complex_geometry_edge_length_min), k_log_complex_geometry_edge_length_max) - k_log_complex_geometry_edge_length_min) / (k_log_complex_geometry_edge_length_max - k_log_complex_geometry_edge_length_min);
							real32 red_fraction = 1.0f - green_fraction;

							real_argb_color color{};
							set_real_argb_color(&color, 1.0f, red_fraction, green_fraction, 0.0f);
							debug_line_drawer.set_color(&color);
						}
					}

					debug_line_drawer.add_line_3d_unclipped(start_vertex_reference.get_position(), end_vertex_reference.get_position());
				}
			}

			real_rectangle3d_enclose_rectangle(&world_bounds, &global_structure_bsp_get(0)->world_bounds);
		}

		if (world_bounds.x1 > world_bounds.x0)
		{
			render_debug_box_outline(true, &world_bounds, global_real_argb_red);
		}
	}

	if (debug_structure_slip_surfaces)
	{
		c_render_debug_line_drawer debug_line_drawer_green{};
		c_render_debug_line_drawer debug_line_drawer_red{};

		debug_line_drawer_green.set_color(global_real_argb_green);
		debug_line_drawer_red.set_color(global_real_argb_red);

		for (int32 structure_bsp_index = global_structure_bsp_first_active_index_get();
			structure_bsp_index != NONE;
			structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
		{
			c_collision_bsp_reference bsp_reference(global_structure_bsp_get(structure_bsp_index));
			for (int32 edge_index = 0; edge_index < bsp_reference.get_edge_count(); edge_index++)
			{
				c_collision_edge_reference edge_reference(bsp_reference, edge_index);
				c_collision_vertex_reference start_vertex_reference(bsp_reference, edge_reference.get_vertex_index(0));
				c_collision_vertex_reference end_vertex_reference(bsp_reference, edge_reference.get_vertex_index(1));

				bool surface_reference_has_slip_bit_enabled = false;
				bool slip_surface_does_not_exceed_maximum_k = true;

				for (int32 i = 0; i < NUMBEROF(collision_edge::surface_indices); i++)
				{
					if (edge_reference.get_surface_index(i) != NONE)
					{
						c_collision_surface_reference surface_reference(bsp_reference, edge_reference.get_surface_index(i));
						if (TEST_BIT(surface_reference.get_flags(), _collision_surface_slip_bit))
						{
							surface_reference_has_slip_bit_enabled = true;

							real_plane3d plane{};
							surface_reference.get_plane(&plane);
							if (plane.n.k > global_slip_surface_maximum_k_get() - k_test_real_epsilon)
							{
								slip_surface_does_not_exceed_maximum_k = false;
							}
						}
					}

				}

				if (surface_reference_has_slip_bit_enabled)
				{
					real_point3d* end_vertex_position = end_vertex_reference.get_position();
					real_point3d* start_vertex_position = start_vertex_reference.get_position();

					if (slip_surface_does_not_exceed_maximum_k)
					{
						debug_line_drawer_green.add_line_3d_unclipped(end_vertex_position, start_vertex_position);
					}
					else
					{
						debug_line_drawer_red.add_line_3d_unclipped(end_vertex_position, start_vertex_position);
					}
				}
			}

			//ASSERT(c_visible_items::get_instances_starting_index() == 0);
			//for (int32 i = 0; i < c_visible_items::m_items.instances.get_count(); i++)
			//{
			//	s_visible_instances& instance = c_visible_items::m_items.instances[i];
			//}
		}
	}

	if (debug_structure_seam_edges)
	{

	}

	if (debug_instanced_geometry)
	{
		if (debug_instanced_geometry_bounding_spheres)
		{
		}

		if (debug_instanced_geometry_names)
		{
		}

		if (debug_instanced_geometry_vertex_counts)
		{
		}

		if (debug_instanced_geometry_collision_geometry)
		{
		}
	}

	if (debug_zone_set_critical_portals)
	{

	}
}

void __cdecl render_debug_structure_draw()
{
	// $IMPLEMENT
}

