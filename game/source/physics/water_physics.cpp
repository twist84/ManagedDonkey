#include "physics/water_physics.hpp"

#include "render/old_render_debug.hpp"
#include "render/render_debug.hpp"
#include "scenario/scenario.hpp"
#include "structures/structure_physics_definitions.hpp"

bool water_physics_debug = false;

REFERENCE_DECLARE(0x018B53FC, real, k_full_scale_effect_velocity);
REFERENCE_DECLARE(0x018B5400, real, k_minimum_water_effect_period);
REFERENCE_DECLARE(0x018B5404, real, k_maximum_water_scale_velocity);
REFERENCE_DECLARE(0x018B5408, real, k_maximum_surface_distance);
REFERENCE_DECLARE(0x018B540C, real, k_initial_density_decay_time);
REFERENCE_DECLARE(0x018B5410, real, k_density_decay_time);
REFERENCE_DECLARE(0x018B5414, real, water_physics_velocity_minimum);
REFERENCE_DECLARE(0x018B5418, real, water_physics_velocity_maximum);

bool __cdecl water_physics_test_vector(real_point3d const* point, real_vector3d const* vector, s_water_physics_test_vector_result* result)
{
	return INVOKE(0x005F8570, water_physics_test_vector, point, vector, result);
}

void __cdecl water_physics_render_debug(real_point3d const* point, real_vector3d const* vector)
{
	if (debug_structure_water)
	{
		c_render_debug_line_drawer debug_line_drawer_green{};
		c_render_debug_line_drawer debug_line_drawer_red{};

		debug_line_drawer_green.set_color(global_real_argb_green);
		debug_line_drawer_red.set_color(global_real_argb_red);

		for (long structure_bsp_index = global_structure_bsp_first_active_index_get();
			structure_bsp_index != NONE;
			structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
		{
			s_structure_design* structure_design = global_structure_design_get(structure_bsp_index);
			if (!structure_design)
				continue;

			for (s_structure_physics_water_instance& water_instance : structure_design->physics.water_instances)
			{
				real_rectangle3d enclosed_rect = *global_null_rectangle3d;

				for (s_structure_physics_water_instance_debug_triangle& water_debug_triangle : water_instance.water_debug_triangles)
				{
					for (long i = 0; i < 3; i++)
					{
						real_point3d* point0 = &water_debug_triangle.points[i];
						real_point3d* point1 = &water_debug_triangle.points[(i + 1) % 3];
						debug_line_drawer_green.add_line_3d_unclipped(point0, point1);
						real_rectangle3d_enclose_point(&enclosed_rect, point0);
					}
				}

				if (water_instance.water_debug_triangles.count > 0)
				{
					real_point3d enclosed_rect_center{};
					rectangle3d_center(&enclosed_rect, &enclosed_rect_center);
					real_point3d point{};
					point_from_line3d(&enclosed_rect_center, &water_instance.direction, 1.0f, &point);
					debug_line_drawer_green.add_line_3d_unclipped(&enclosed_rect_center, &point);
					render_debug_sphere(true, &enclosed_rect_center, 0.1f, global_real_argb_red);
				}
			}
		}
	}

	if (water_physics_debug)
	{
		s_water_physics_test_vector_result result{};
		real_vector3d scaled_vector{};
		scale_vector3d(vector, 100.0f, &scaled_vector);
		if (water_physics_test_vector(point, &scaled_vector, &result))
		{
			s_structure_design* structure_design = global_structure_design_get(result.structure_bsp_index);
			if (structure_design)
			{
				s_structure_physics_water_instance& water_instance = structure_design->physics.water_instances[result.water_instance_index];

				c_render_debug_line_drawer debug_line_drawer{};
				debug_line_drawer.set_color(global_real_argb_green);
				render_debug_point(true, &result.__point0, 0.1f, global_real_argb_red);
				render_debug_vector(true, &result.__point0, &result.__vectorC, 0.1f, global_real_argb_blue);

				for (s_structure_physics_water_instance_debug_triangle& water_debug_triangle : water_instance.water_debug_triangles)
				{
					for (long i = 0; i < 3; i++)
					{
						real_point3d* point0 = &water_debug_triangle.points[i];
						real_point3d* point1 = &water_debug_triangle.points[(i + 1) % 3];
						debug_line_drawer.add_line_3d_unclipped(point0, point1);
					}
				}
			}
		}
	}
}

