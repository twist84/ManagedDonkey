#include "physics/collisions.hpp"

#include "physics/collision_features.hpp"

bool __cdecl collision_get_features_in_sphere(s_collision_test_flags flags, real_point3d const* point, real radius, real height, real width, long ignore_object_index, long a7, collision_feature_list* features)
{
	return INVOKE(0x006D3C80, collision_get_features_in_sphere, flags, point, radius, height, width, ignore_object_index, a7, features);
}

bool __cdecl collision_test_vector(s_collision_test_flags flags, real_point3d const* point, vector3d const* vector, long first_ignore_object_index, long second_ignore_object_index, collision_result* collision)
{
	return INVOKE(0x006D7160, collision_test_vector, flags, point, vector, first_ignore_object_index, second_ignore_object_index, collision);
}

//.text:006D8BB0 ; bool __cdecl object_is_boarding(long object_index)
//.text:006D8C50 ; bool __cdecl object_test_point(long object_index, s_collision_test_flags flags, real_point3d const* point, long first_ignore_object_index, long second_ignore_object_index)
//.text:006D8CA0 ; bool __cdecl object_test_point_internal(long object_index, bool a2, s_collision_test_flags flags, real_point3d const* point, long first_ignore_object_index, long second_ignore_object_index)
//.text:006D8DE0 ; bool __cdecl object_test_vector(long, s_collision_test_flags, real_point3d const* point, vector3d const* vector, long first_ignore_object_index, long second_ignore_object_index, long third_ignore_object_index, collision_result* collision)

