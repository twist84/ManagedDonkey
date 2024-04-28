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

