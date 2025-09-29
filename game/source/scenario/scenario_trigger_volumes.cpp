#include "scenario/scenario_trigger_volumes.hpp"

//.text:005FA190 ; public: c_trigger_volume_query::c_trigger_volume_query(const scenario_trigger_volume*)
//.text:005FA1D0 ; public: real32 c_trigger_volume_query::distance_to_face(int32, const real_point3d*) const
//.text:005FA210 ; public: real32 c_trigger_volume_query::distance_to_face_local(int32, const real_point3d*) const
//.text:005FA380 ; public: void c_trigger_volume_query::get_local_bounds(real_rectangle3d*) const
//.text:005FA450 ; 
//.text:005FA4E0 ; bool __cdecl scenario_trigger_volume_get_matrix(int32, real_matrix4x3*)
//.text:005FA500 ; int32 __cdecl scenario_trigger_volume_return_objects(int32)
//.text:005FA520 ; int32 __cdecl scenario_trigger_volume_return_objects_by_type(int32, int32)

bool __cdecl scenario_trigger_volume_test_object(int32 trigger_volume_index, int32 object_index)
{
	return INVOKE(0x005FA680, scenario_trigger_volume_test_object, trigger_volume_index, object_index);
}

//.text:005FA6E0 ; bool __cdecl scenario_trigger_volume_test_players(int32)
//.text:005FA780 ; bool __cdecl scenario_trigger_volume_test_players_all(int32)
//.text:005FA820 ; bool __cdecl scenario_trigger_volume_test_point(int32, const real_point3d*)
//.text:005FAB10 ; public: real_point3d* c_trigger_volume_query::transform_to_trigger_space(const real_point3d*, real_point3d*) const
//.text:005FAB30 ; public: real_vector3d* c_trigger_volume_query::transform_to_trigger_space(const real_vector3d*, real_vector3d*) const

bool __cdecl trigger_volume_build_faces(const scenario_trigger_volume* volume, real_point3d(&face_vertices)[k_faces_per_cube_count][4])
{
	//return INVOKE(0x005FAB50, trigger_volume_build_faces, volume, face_vertices);

	real_matrix4x3 transform{};
	if (!trigger_volume_get_matrix(volume, &transform))
		return false;

	real_rectangle3d bounds{};
	bounds.x0 = 0.0f;
	bounds.x1 = volume->extents.i;
	bounds.y0 = 0.0f;
	bounds.y1 = volume->extents.j;
	bounds.z0 = 0.0f;
	bounds.z1 = volume->extents.k;

	rectangle3d_build_faces(&bounds, k_faces_per_cube_count, face_vertices);
	for (int32 face_index = 0; face_index < k_faces_per_cube_count; face_index++)
		matrix4x3_transform_points(&transform, 4, face_vertices[face_index], face_vertices[face_index]);

	return true;
}

//.text:005FABF0 ; int32 __cdecl trigger_volume_face_index_to_axis(int32)
//.text:005FAC00 ; e_trigger_volume_side __cdecl trigger_volume_face_index_to_side(int32)

bool __cdecl trigger_volume_get_center(const scenario_trigger_volume* volume, real_point3d* out_center_point)
{
	return INVOKE(0x005FAC20, trigger_volume_get_center, volume, out_center_point);
}

//.text:005FACB0 ; real32 __cdecl trigger_volume_get_height(const scenario_trigger_volume*)
//.text:005FADE0 ; void __cdecl trigger_volume_get_position(const scenario_trigger_volume*, real_point3d*)

bool __cdecl trigger_volume_get_matrix(const scenario_trigger_volume* volume, real_matrix4x3* matrix)
{
	return INVOKE(0x005FACC0, trigger_volume_get_matrix, volume, matrix);
}

real32 __cdecl trigger_volume_get_radius(const scenario_trigger_volume* volume)
{
	return INVOKE(0x005FAF10, trigger_volume_get_radius, volume);
}

//.text:005FAF20 ; s_sector_point_template<real_point3d>* __cdecl trigger_volume_get_sector_point(const scenario_trigger_volume*, int32)
//.text:005FAF40 ; real32 __cdecl trigger_volume_get_sink(const scenario_trigger_volume*)
//.text:005FAFE0 ; const s_trigger_volume_triangle* __cdecl trigger_volume_get_triangle(const scenario_trigger_volume*, int32)
//.text:005FB000 ; void __cdecl trigger_volume_set_height(scenario_trigger_volume*, real32)
//.text:005FB020 ; void __cdecl trigger_volume_set_position(scenario_trigger_volume*, const real_point3d*)
//.text:005FB0C0 ; void __cdecl trigger_volume_set_sink(scenario_trigger_volume*, real32)

