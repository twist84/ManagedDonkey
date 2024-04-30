#include "physics/collision_bsp.hpp"

#include "physics/collision_references.hpp"
#include "render/render_debug.hpp"

void render_debug_collision_edge(c_collision_bsp_reference bsp_reference, long edge_index, real_matrix4x3 const* matrix, real_argb_color const* color)
{
	c_collision_edge_reference edge_reference(bsp_reference, edge_index);

	real_point3d* start_vertex_position = c_collision_vertex_reference(bsp_reference, edge_reference.get_vertex_index(0)).get_position();
	real_point3d* end_vertex_position = c_collision_vertex_reference(bsp_reference, edge_reference.get_vertex_index(1)).get_position();

	if (matrix)
	{
		real_point3d start_vertex_position_{};
		real_point3d end_vertex_position_{};

		start_vertex_position = matrix4x3_transform_point(matrix, start_vertex_position, &start_vertex_position_);
		end_vertex_position = matrix4x3_transform_point(matrix, end_vertex_position, &end_vertex_position_);
	}

	render_debug_line(true, start_vertex_position, end_vertex_position, color);
}

