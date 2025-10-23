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
		for (int32 structure_bsp_index = global_structure_bsp_first_active_index_get();
			structure_bsp_index != NONE;
			structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
		{
			structure_bsp* structure = global_structure_bsp_get(structure_bsp_index);
			for (int32 marker_index = 0; marker_index < structure->markers.count; marker_index++)
			{
				structure_marker* marker = TAG_BLOCK_GET_ELEMENT(&structure->markers, marker_index, structure_marker);

				real_matrix4x3 matrix{};
				matrix4x3_from_point_and_quaternion(&matrix, &marker->position, &marker->rotation);
				render_debug_matrix(true, &matrix, 0.05f);
				render_debug_string_at_point(&matrix.position, marker->name, global_real_argb_white);
			}
		}
	}

	if (debug_structure_surface_references)
	{
		for (int32 structure_bsp_index = global_structure_bsp_first_active_index_get();
			structure_bsp_index != NONE;
			structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
		{
			const structure_bsp* structure = global_structure_bsp_get(structure_bsp_index);
			for (int32 leaf_index = 0; leaf_index < structure->leaves.count; leaf_index++)
			{
				const structure_leaf* leaf = TAG_BLOCK_GET_ELEMENT(&structure->leaves, leaf_index, const structure_leaf);
				const structure_cluster* cluster = TAG_BLOCK_GET_ELEMENT(&structure->clusters, leaf->cluster_index, const structure_cluster);
				//structure_bsp_cluster_data_available(cluster); // return true
			}
		}
	}

	scenario_soft_ceilings_render_debug(
		&render_camera->position,
		debug_structure_soft_ceilings,
		debug_structure_soft_kill,
		debug_structure_slip_surfaces);

	if (debug_structure_cluster_skies)
	{
		//s_scenario_zone_set* zone_set;
		//const s_game_cluster_bit_vectors* clusters;
		//{
		//	int32 structure_bsp_index;
		//	{
		//		const structure_bsp* structure;
		//		s_scenario_zone_set_pvs* zone_set_pvs;
		//		const int32 zone_set_pvs_index;
		//		int32* cluster_multiple_skies_visible_bit_vector;
		//		s_scenario_zone_set_structure_bsp_pvs* structure_bsp_pvs;
		//		{
		//			int32 cluster_index;
		//			{
		//				s_cluster_reference cluster_reference;
		//				{
		//					const real32 k_world_bounds;
		//					structure_cluster* cluster;
		//					c_static_string<128> string;
		//					bool multiple_skies_visible;
		//					int32 sky_index;
		//					const real_argb_color* color;
		//					real_point3d center;
		//				}
		//			}
		//		}
		//	}
		//}
	}

	if (debug_structure_invisible)
	{
		//c_render_debug_line_drawer debug_open_edge_drawer;
		//c_render_debug_line_drawer debug_edge_drawer;
		//{
		//	int32 structure_bsp_index;
		//	{
		//		const structure_bsp* structure;
		//		const collision_bsp* bsp;
		//		{
		//			int32 edge_index;
		//			{
		//				const collision_edge* edge;
		//				const collision_vertex* v0;
		//				const collision_vertex* v1;
		//				int16 plane_designators[2]{};
		//				bool connected_to_invisible = false;
		//				{
		//					int32 side_index;
		//					{
		//						const collision_surface* surface;
		//					}
		//				}
		//			}
		//		}
		//	}
		//}
	}

	water_physics_render_debug(&render_camera->position, &render_camera->forward);

	if (debug_structure || debug_structure_automatic && lost_camera)
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
				real_argb_color bsp_color{};
				render_debug_unique_color(structure_bsp_index, &bsp_color);
				debug_line_drawer.set_color(&bsp_color);
			}

			const structure_bsp* structure = global_structure_bsp_get(structure_bsp_index);
			c_collision_bsp_reference bsp(structure);
			int32 edge_count = bsp.get_edge_count();

			if (debug_plane_index != NONE && debug_structure)
			{
				for (int32 edge_index = 0; edge_index < edge_count; edge_index++)
				{
					c_collision_edge_reference edge(bsp, edge_index);
					c_collision_vertex_reference v0(bsp, edge.get_vertex_index(0));
					c_collision_vertex_reference v1(bsp, edge.get_vertex_index(1));

					for (int32 i = 0; i < NUMBEROF(collision_edge::surface_indices); i++)
					{
						if (edge.get_surface_index(i) != NONE)
						{
							c_collision_surface_reference surface_reference(bsp, edge.get_surface_index(i));
							if (surface_reference.get_plane_index() == debug_plane_index)
							{
								debug_line_drawer.add_line_3d_unclipped(v0.get_position(), v1.get_position());
								break;
							}
						}
					}
				}
			}
			else
			{
				for (int32 edge_index = 0; edge_index < edge_count; edge_index++)
				{
					c_collision_edge_reference edge(bsp, edge_index);
					c_collision_vertex_reference v0(bsp, edge.get_vertex_index(0));
					c_collision_vertex_reference v1(bsp, edge.get_vertex_index(1));

					if (debug_structure_complexity)
					{
						real32 log_edge_length = 0.5f * log10f(distance_squared3d(v0.get_position(), v1.get_position()));
						if (log_edge_length < k_log_complex_geometry_edge_length_cutoff)
						{
							real32 green_fraction = (fminf(fmaxf(log_edge_length, k_log_complex_geometry_edge_length_min), k_log_complex_geometry_edge_length_max) - k_log_complex_geometry_edge_length_min) / (k_log_complex_geometry_edge_length_max - k_log_complex_geometry_edge_length_min);
							real32 red_fraction = 1.0f - green_fraction;

							real_argb_color color{};
							set_real_argb_color(&color, 1.0f, red_fraction, green_fraction, 0.0f);
							debug_line_drawer.set_color(&color);
						}
					}

					debug_line_drawer.add_line_3d_unclipped(v0.get_position(), v1.get_position());
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
		c_render_debug_line_drawer debug_line_drawer{};
		c_render_debug_line_drawer invalid_debug_line_drawer{};

		debug_line_drawer.set_color(global_real_argb_green);
		invalid_debug_line_drawer.set_color(global_real_argb_red);

		for (int32 structure_bsp_index = global_structure_bsp_first_active_index_get();
			structure_bsp_index != NONE;
			structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
		{
			const structure_bsp* structure = global_structure_bsp_get(structure_bsp_index);
			c_collision_bsp_reference bsp(structure);
			for (int32 edge_index = 0; edge_index < bsp.get_edge_count(); edge_index++)
			{
				c_collision_edge_reference edge(bsp, edge_index);
				c_collision_vertex_reference v0(bsp, edge.get_vertex_index(0));
				c_collision_vertex_reference v1(bsp, edge.get_vertex_index(1));

				bool slip_surface = false;
				bool in_range_slip_surface = true;

				for (int32 side_index = 0; side_index < NUMBEROF(collision_edge::surface_indices); side_index++)
				{
					if (edge.get_surface_index(side_index) != NONE)
					{
						c_collision_surface_reference surface(bsp, edge.get_surface_index(side_index));
						if (TEST_BIT(surface.get_flags(), _collision_surface_slip_bit))
						{
							slip_surface = true;

							real_plane3d plane{};
							surface.get_plane(&plane);
							if (plane.n.k > global_slip_surface_maximum_k_get() - k_test_real_epsilon)
							{
								in_range_slip_surface = false;
							}
						}
					}

				}

				if (slip_surface)
				{
					real_point3d* p0 = v0.get_position();
					real_point3d* p1 = v1.get_position();

					if (in_range_slip_surface)
					{
						debug_line_drawer.add_line_3d_unclipped(p0, p1);
					}
					else
					{
						invalid_debug_line_drawer.add_line_3d_unclipped(p0, p1);
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
		// $IMPLEMENT
	}

	if (debug_instanced_geometry)
	{
		ASSERT(c_visible_items::get_instances_starting_index() == 0);
		uns16 visibility_instance_count = (uns16)c_visible_items::m_items.instances.get_count();
		for (int32 visibility_instance_index = 0; visibility_instance_index < visibility_instance_count; visibility_instance_index++)
		{
			const int32 structure_bsp_index = c_visible_items::m_items.instances[visibility_instance_index].structure_bsp_index;
			int32 bsp_instanced_geometry_index = c_visible_items::m_items.instances[visibility_instance_index].structure_bsp_instance_index;

			const structure_bsp* structure = global_structure_bsp_get(structure_bsp_index);

			const structure_instanced_geometry_instance* instanced_geometry_instance = TAG_BLOCK_GET_ELEMENT(
				&structure->instanced_geometry_instances,
				bsp_instanced_geometry_index,
				const structure_instanced_geometry_instance);

			const structure_instanced_geometry_definition* instanced_geometry_definition = TAG_BLOCK_GET_ELEMENT(
				&structure->resource_interface.get_resources()->instanced_geometries_definitions,
				instanced_geometry_instance->definition_block_index,
				const structure_instanced_geometry_definition);

			if (debug_instanced_geometry_bounding_spheres)
			{
				render_debug_sphere(true, &instanced_geometry_instance->world_bounding_sphere_center, instanced_geometry_instance->world_bounding_sphere_radius, global_real_argb_blue);
			}

			if (debug_instanced_geometry_names)
			{
				c_static_string<128> name_string;
				name_string.print("%s (#%d def #%d cube %d)",
					instanced_geometry_instance->name.get_string(),
					bsp_instanced_geometry_index,
					instanced_geometry_instance->definition_block_index,
					instanced_geometry_instance->cubemap_0_bitmap_index);
				render_debug_string_at_point(&instanced_geometry_instance->world_bounding_sphere_center, name_string.get_string(), global_real_argb_blue);
			}

			// Bungie why add this if you aren't going to populate `vertex_count_string` with a value?
			// disable for now
			if (false && debug_instanced_geometry_vertex_counts)
			{
				c_static_string<16> vertex_count_string;
				real_point3d vertex_count_position = instanced_geometry_instance->world_bounding_sphere_center;
				vertex_count_position.z += instanced_geometry_instance->world_bounding_sphere_radius;
				render_debug_string_at_point(&vertex_count_position, vertex_count_string.get_string(), global_real_argb_blue);
			}

			if (debug_instanced_geometry_collision_geometry)
			{
				c_render_debug_line_drawer edge_drawer;
				edge_drawer.set_color(global_real_argb_blue);

				for (int32 edge_index = 0; edge_index < instanced_geometry_definition->collision_info.edges.count; edge_index++)
				{
					collision_edge* edge = TAG_BLOCK_GET_ELEMENT(&instanced_geometry_definition->collision_info.edges, edge_index, collision_edge);
					collision_vertex* v0 = TAG_BLOCK_GET_ELEMENT(&instanced_geometry_definition->collision_info.vertices, edge->vertex_indices[0], collision_vertex);
					collision_vertex* v1 = TAG_BLOCK_GET_ELEMENT(&instanced_geometry_definition->collision_info.vertices, edge->vertex_indices[1], collision_vertex);

					real_point3d p0{};
					real_point3d p1{};
					matrix4x3_transform_point(&instanced_geometry_instance->world_transform, &v0->point, &p0);
					matrix4x3_transform_point(&instanced_geometry_instance->world_transform, &v1->point, &p1);

					if (debug_structure_complexity)
					{
						real32 log_edge_length = 0.5f * log10f(distance_squared3d(&p0, &p1));
						if (log_edge_length < k_log_complex_geometry_edge_length_cutoff)
						{
							real32 blue_fraction = (fminf(fmaxf(log_edge_length, k_log_complex_geometry_edge_length_min), k_log_complex_geometry_edge_length_max) - k_log_complex_geometry_edge_length_min) / (k_log_complex_geometry_edge_length_max - k_log_complex_geometry_edge_length_min);
							real32 red_fraction = 1.0f - blue_fraction;

							real_argb_color color{};
							set_real_argb_color(&color, 1.0f, red_fraction, 0.0f, blue_fraction);
							edge_drawer.set_color(&color);
							edge_drawer.add_line_3d_unclipped(&p0, &p1);
						}
					}
					else
					{
						edge_drawer.add_line_3d_unclipped(&p0, &p1);
					}
				}
			}
		}
	}

	if (debug_zone_set_critical_portals)
	{

	}
}

void __cdecl render_debug_structure_all_cluster_errors(int32 setting)
{
	//render_debug_structure_internal(_render_debug_structure_mode_all_cluster_errors, setting);
	//debug_structure_automatic = false;
}

void __cdecl render_debug_structure_all_fog_planes(int32 setting)
{
	//render_debug_structure_internal(_render_debug_structure_mode_all_fog_planes, setting);
	//debug_structure_automatic = false;
}

void __cdecl render_debug_structure_cluster_fog(int32 setting)
{
	//render_debug_structure_internal(_render_debug_structure_mode_cluster_fog, setting);
	//debug_structure_automatic = false;
}

void __cdecl render_debug_structure_fog_plane(int32 setting)
{
	//render_debug_structure_internal(_render_debug_structure_mode_fog_plane, setting);
	//debug_structure_automatic = false;
}

void __cdecl render_debug_structure_fog_plane_infinite_extent(int32 setting)
{
	//render_debug_structure_internal(_render_debug_structure_mode_fog_plane_infinite_extent, setting);
	//debug_structure_automatic = false;
}

void __cdecl render_debug_structure_fog_zone(int32 setting)
{
	//render_debug_structure_internal(_render_debug_structure_mode_fog_zone, setting);
	//debug_structure_automatic = false;
}

void __cdecl render_debug_structure_fog_zone_floodfill(int32 setting)
{
	//render_debug_structure_internal(_render_debug_structure_mode_fog_zone_floodfill, setting);
	//debug_structure_automatic = false;
}

void __cdecl render_debug_structure_draw()
{
	// $IMPLEMENT
}

