#pragma once

#include "math/real_math.hpp"

extern void __cdecl matrix4x3_rotation_from_vectors(real_matrix4x3* matrix, vector3d const* forward, vector3d const* up);
extern void __cdecl matrix4x3_from_point_and_vectors(real_matrix4x3* matrix, real_point3d const* point, vector3d const* forward, vector3d const* up);
extern vector3d* __cdecl matrix4x3_transform_vector(real_matrix4x3 const* matrix, vector3d const* in_vector, vector3d* out_vector);
extern void __cdecl matrix4x3_multiply(real_matrix4x3 const* in_matrix0, real_matrix4x3 const* in_matrix1, real_matrix4x3* out_matrix);

