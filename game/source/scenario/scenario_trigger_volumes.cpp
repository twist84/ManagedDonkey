#include "scenario/scenario_trigger_volumes.hpp"

bool __cdecl trigger_volume_get_center(scenario_trigger_volume const* volume, union real_point3d* center)
{
	return INVOKE(0x005FAC20, trigger_volume_get_center, volume, center);
}

bool __cdecl trigger_volume_get_matrix(scenario_trigger_volume const* volume, real_matrix4x3* matrix)
{
	return INVOKE(0x005FACC0, trigger_volume_get_matrix, volume, matrix);
}

real __cdecl trigger_volume_get_radius(scenario_trigger_volume const* volume)
{
	return INVOKE(0x005FAF10, trigger_volume_get_radius, volume);
}

bool __cdecl trigger_volume_build_faces(scenario_trigger_volume const* volume, real_point3d(&faces)[k_faces_per_cube_count][4])
{
	real_matrix4x3 matrix{};
	if (trigger_volume_get_matrix(volume, &matrix))
	{
		real_rectangle3d rectangle{};
		rectangle.x.lower = 0.0f;
		rectangle.x.upper = volume->extents.x;
		rectangle.y.lower = 0.0f;
		rectangle.y.upper = volume->extents.y;
		rectangle.z.lower = 0.0f;
		rectangle.z.upper = volume->extents.z;

		rectangle3d_build_faces(&rectangle, k_faces_per_cube_count, faces);
		for (long i = 0; i < k_faces_per_cube_count; ++i)
			matrix4x3_transform_points(&matrix, 4, faces[i], faces[i]);

		return true;
	}

	return false;
}
