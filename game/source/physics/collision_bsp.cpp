#include "physics/collision_bsp.hpp"

#include "physics/collision_references.hpp"
#include "render/render_debug.hpp"

void render_debug_collision_surface(c_collision_bsp_reference bsp_reference, int32 surface_index, const real_matrix4x3* matrix, const real_argb_color* color)
{
	c_collision_surface_reference surface_reference(bsp_reference, surface_index);
	int32 first_edge_index = surface_reference.get_first_edge_index();
	int32 edge_index = first_edge_index;

	do
	{
		c_collision_edge_reference edge_reference(bsp_reference, edge_index);

		int32 edge_surface_index1 = edge_reference.get_surface_index(1);
		bool edge_surface_index_is_input = edge_surface_index1 == surface_index;

		render_debug_collision_edge(bsp_reference, edge_index, matrix, color);

		edge_index = edge_reference.get_edge_index(edge_surface_index_is_input);

	} while (edge_index != first_edge_index);
}

void render_debug_collision_edge(c_collision_bsp_reference bsp_reference, int32 edge_index, const real_matrix4x3* matrix, const real_argb_color* color)
{
	c_collision_edge_reference edge_reference(bsp_reference, edge_index);

	real_point3d* point0 = c_collision_vertex_reference(bsp_reference, edge_reference.get_vertex_index(0)).get_position();
	real_point3d* point1 = c_collision_vertex_reference(bsp_reference, edge_reference.get_vertex_index(1)).get_position();

	if (matrix)
	{
		real_point3d transformed_point0{};
		real_point3d transformed_point1{};

		point0 = matrix4x3_transform_point(matrix, point0, &transformed_point0);
		point1 = matrix4x3_transform_point(matrix, point1, &transformed_point1);
	}

	render_debug_line(true, point0, point1, color);
}

