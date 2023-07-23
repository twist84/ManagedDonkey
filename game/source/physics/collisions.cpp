#include "physics/collisions.hpp"

bool __cdecl collision_test_vector(s_collision_test_flags flags, real_point3d const* point, vector3d const* vector, long first_ignore_object_index, long second_ignore_object_index, collision_result* collision)
{
	return INVOKE(0x006D7160, collision_test_vector, flags, point, vector, first_ignore_object_index, second_ignore_object_index, collision);
}

