#include "physics/collisions.hpp"

#include "physics/collision_features.hpp"

//.text:006D2D70 ; uns32 __cdecl build_bsp_flags_from_collision_flags(c_flags<e_collision_test_flag, uns32, k_collision_test_flags_count>)
//.text:006D2DE0 ; void __cdecl build_collision_result_from_bsp_result(collision_result*, const c_collision_bsp_test_vector_result*, const real_matrix4x3*)
//.text:006D2EF0 ; 
//.text:006D2FA0 ; 
//.text:006D3020 ; 
//.text:006D30A0 ; 
//.text:006D3110 ; c_global_material_type __cdecl collision_calculate_structure_global_material_type(int32, const real_point3d*, int32, int32, bool)
//.text:006D3190 ; c_global_material_type __cdecl collision_calculate_structure_global_material_type(int32, c_collision_bsp_reference, const real_point3d*, int32, int32, bool)
//.text:006D3370 ; 
//.text:006D3440 ; 
//.text:006D3590 ; bool __cdecl collision_find_good_projectile_point(const real_point3d*, const real_point3d*, real_point3d*, int32, int32)
//.text:006D3760 ; bool __cdecl collision_fix_pill(s_collision_test_flags, const real_point3d*, real32, real32, real32, int32, int32, real_point3d*)
//.text:006D3BC0 ; void __cdecl collision_fix_pill_nudge_collision(s_collision_test_flags, const real_point3d*, const real_vector3d*, int32, int32, collision_plane*)

bool __cdecl collision_get_features_in_sphere(s_collision_test_flags flags, const real_point3d* point, real32 radius, real32 height, real32 width, int32 ignore_object_index, int32 a7, collision_feature_list* features)
{
	return INVOKE(0x006D3C80, collision_get_features_in_sphere, flags, point, radius, height, width, ignore_object_index, a7, features);
}

//.text:006D4040 ; bool __cdecl collision_get_unobstructed_point(s_collision_test_flags, const real_point3d*, real32, int32, int32, real_point3d*, real32*)
//.text:006D4C40 ; 

int16 __cdecl collision_move_pill(s_collision_test_flags flags, const real_point3d* old_position, const real_vector3d* old_velocity, real32 height, real32 radius, int32 first_ignore_object_index, int32 second_ignore_object_index, real_point3d* new_position, real_vector3d* new_velocity, int16 maximum_collision_count, collision_plane* collisions)
{
	return INVOKE(0x006D4C70, collision_move_pill, flags, old_position, old_velocity, height, radius, first_ignore_object_index, second_ignore_object_index, new_position, new_velocity, maximum_collision_count, collisions);
}

int16 __cdecl collision_move_point(const real_point3d* old_position, const real_vector3d* old_velocity, const collision_feature_list* features, real_point3d* new_position, real_vector3d* new_velocity, int16 maximum_collision_count, collision_plane* collisions)
{
	return INVOKE(0x006D4DD0, collision_move_point, old_position, old_velocity, features, new_position, new_velocity, maximum_collision_count, collisions);
}

int16 __cdecl collision_move_sphere(s_collision_test_flags flags, const real_point3d* old_position, const real_vector3d* old_velocity, real32 radius, int32 first_ignore_object_index, int32 second_ignore_object_index, real_point3d* new_position, real_vector3d* new_velocity, int16 maximum_collision_count, collision_plane* collisions)
{
	return INVOKE(0x006D5B00, collision_move_sphere, flags, old_position, old_velocity, radius, first_ignore_object_index, second_ignore_object_index, new_position, new_velocity, maximum_collision_count, collisions);

	//return collision_move_pill(flags, old_position, old_velocity, 0.0f, radius, first_ignore_object_index, second_ignore_object_index, new_position, new_velocity, maximum_collision_count, collisions);
}

//.text:006D5B40 ; bool __cdecl collision_structure_bounds_sort_proc(const void*, const void*, const void*)
//.text:006D5B70 ; const real_vector3d* __cdecl collision_structure_ground_plane_calculate(const real_point3d*, const real_vector3d*, real32, real_vector3d*)
//.text:006D5F60 ; 
//.text:006D6560 ; int32 __cdecl collision_structure_ground_plane_process_surfaces(const collision_bsp_test_sphere_result*, const real_matrix4x3*, c_collision_bsp_reference, const real_point3d*, const real_vector3d*, int32, int32, real_plane3d*)

bool __cdecl collision_test_line(s_collision_test_flags flags, const real_point3d* point0, const real_point3d* point1, int32 first_ignore_object_index, int32 second_ignore_object_index, collision_result* collision)
{
	return INVOKE(0x006D6780, collision_test_line, flags, point0, point1, first_ignore_object_index, second_ignore_object_index, collision);
}

//.text:006D67E0 ; bool __cdecl collision_test_line(s_collision_test_flags, bool, const real_point3d*, const real_point3d*, int32, int32, int32, collision_result*)
//.text:006D6840 ; bool __cdecl collision_test_line_exit(const collision_result*, const real_point3d*, const real_point3d*, collision_result*)
//.text:006D6890 ; bool __cdecl collision_test_pill(s_collision_test_flags, const real_point3d*, const real_vector3d*, real32, int32, int32, collision_result*)

// `collision_test_point_0` is only for `INVOKE` not actual use
bool __cdecl collision_test_point_0(s_collision_test_flags flags, const real_point3d* point, int32 first_ignore_object_index, int32 second_ignore_object_index) { UNREACHABLE(); return false; }
bool __cdecl collision_test_point(s_collision_test_flags flags, const real_point3d* point, int32 first_ignore_object_index, int32 second_ignore_object_index)
{
	return INVOKE(0x006D6AA0, collision_test_point_0, flags, point, first_ignore_object_index, second_ignore_object_index);
}

// `collision_test_point_1` is only for `INVOKE` not actual use
bool __cdecl collision_test_point_1(s_collision_test_flags flags, const real_point3d* point, int32 first_ignore_object_index, int32 second_ignore_object_index, e_collision_result_type* collision_result_type) { UNREACHABLE(); return false; }
bool __cdecl collision_test_point(s_collision_test_flags flags, const real_point3d* point, int32 first_ignore_object_index, int32 second_ignore_object_index, e_collision_result_type* collision_result_type)
{
	return INVOKE(0x006D6AC0, collision_test_point_1, flags, point, first_ignore_object_index, second_ignore_object_index, collision_result_type);
}

//.text:006D6D90 ; bool __cdecl collision_test_rectangle3d(const real_rectangle3d*, const real_point3d*, const real_vector3d*, real32, real32*, real32*)

// `collision_test_sphere_0` is only for `INVOKE` not actual use
bool __cdecl collision_test_sphere_0(int32 structure_bsp_index, s_collision_test_flags flags, const real_point3d* center, real32 radius, int32 first_ignore_object_index, int32 second_ignore_object_index) { UNREACHABLE(); return false; }
bool __cdecl collision_test_sphere(int32 structure_bsp_index, s_collision_test_flags flags, const real_point3d* center, real32 radius, int32 first_ignore_object_index, int32 second_ignore_object_index)
{
	return INVOKE(0x006D6FC0, collision_test_sphere_0, structure_bsp_index, flags, center, radius, first_ignore_object_index, second_ignore_object_index);
}

// `collision_test_sphere_1` is only for `INVOKE` not actual use
bool __cdecl collision_test_sphere_1(s_collision_test_flags flags, const real_point3d* center, real32 radius, int32 first_ignore_object_index, int32 second_ignore_object_index) { UNREACHABLE(); return false; }
bool __cdecl collision_test_sphere(s_collision_test_flags flags, const real_point3d* center, real32 radius, int32 first_ignore_object_index, int32 second_ignore_object_index)
{
	return INVOKE(0x006D7060, collision_test_sphere_1, flags, center, radius, first_ignore_object_index, second_ignore_object_index);
}

//.text:006D7130 ; 

// `collision_test_vector_0` is only for `INVOKE` not actual use
bool __cdecl collision_test_vector_0(s_collision_test_flags flags, const real_point3d* point, const real_vector3d* vector, int32 first_ignore_object_index, int32 second_ignore_object_index, collision_result* collision) { UNREACHABLE(); return false; }
bool __cdecl collision_test_vector(s_collision_test_flags flags, const real_point3d* point, const real_vector3d* vector, int32 first_ignore_object_index, int32 second_ignore_object_index, collision_result* collision)
{
	return INVOKE(0x006D7160, collision_test_vector_0, flags, point, vector, first_ignore_object_index, second_ignore_object_index, collision);
}

// `collision_test_vector_1` is only for `INVOKE` not actual use
bool __cdecl collision_test_vector_1(s_collision_test_flags flags, bool a2, const real_point3d* point, const real_vector3d* vector, int32 first_ignore_object_index, int32 second_ignore_object_index, int32 third_ignore_object_index, collision_result* collision) { UNREACHABLE(); return false; }
bool __cdecl collision_test_vector(s_collision_test_flags flags, bool a2, const real_point3d* point, const real_vector3d* vector, int32 first_ignore_object_index, int32 second_ignore_object_index, int32 third_ignore_object_index, collision_result* collision)
{
	return INVOKE(0x006D7190, collision_test_vector_1, flags, a2, point, vector, first_ignore_object_index, second_ignore_object_index, third_ignore_object_index, collision);
}

//.text:006D7D40 ; bool __cdecl collision_test_vector_exit(const collision_result*, const real_point3d*, const real_vector3d*, collision_result*)
//.text:006D7FF0 ; bool __cdecl get_bsp_from_collision_result(const collision_result*, c_collision_bsp_reference*, const real_matrix4x3**)
//.text:006D80F0 ; void __cdecl initialize_collision_result(collision_result*)
//.text:006D8130 ; void __cdecl instanced_geometry_get_features_in_sphere(const structure_context*, int32, c_flags<e_collision_test_flag, uns32, k_collision_test_flags_count>, const real_point3d*, real32, real32, real32, collision_feature_list*)
//.text:006D82B0 ; 
//.text:006D8330 ; bool __cdecl instanced_geometry_test_point(int32, int32, uns32, const real_point3d*)
//.text:006D8440 ; bool __cdecl instanced_geometry_test_vector(int32, int32, c_flags<e_collision_test_flag, uns32, k_collision_test_flags_count>, const real_point3d*, const real_vector3d*, collision_result*)
//.text:006D8580 ; bool __cdecl instanced_geometry_test_vector_internal(const structure_context*, int32, c_flags<e_collision_test_flag, uns32, k_collision_test_flags_count>, uns32, const real_point3d*, const real_vector3d*, collision_result*)
//.text:006D89E0 ; 
//.text:006D8A30 ; void __cdecl object_get_features_in_sphere(int32, s_collision_test_flags, const real_point3d*, real32, real32, real32, int32, int32, collision_feature_list*)
//.text:006D8BB0 ; bool __cdecl object_is_boarding(int32 object_index)
//.text:006D8C50 ; bool __cdecl object_test_point(int32 object_index, s_collision_test_flags flags, const real_point3d* point, int32 first_ignore_object_index, int32 second_ignore_object_index)
//.text:006D8CA0 ; bool __cdecl object_test_point_internal(int32 object_index, bool a2, s_collision_test_flags flags, const real_point3d* point, int32 first_ignore_object_index, int32 second_ignore_object_index)
//.text:006D8DE0 ; bool __cdecl object_test_vector(int32, s_collision_test_flags, const real_point3d* point, const real_vector3d* vector, int32 first_ignore_object_index, int32 second_ignore_object_index, int32 third_ignore_object_index, collision_result* collision)
//.text:006D9230 ; 
//.text:006D92A0 ; bool __cdecl test_object(int32, const object_header_datum*, const object_datum*, s_collision_test_flags, int32, int32, int32)

