#pragma once

#include "math/real_math.hpp"

extern void __cdecl matrix4x3_rotation_from_vectors(real_matrix4x3* matrix, vector3d const* forward, vector3d const* up);
extern void __cdecl matrix4x3_from_point_and_vectors(real_matrix4x3* matrix, real_point3d const* point, vector3d const* forward, vector3d const* up);
extern vector3d* __cdecl matrix4x3_transform_vector(real_matrix4x3 const* matrix, vector3d const* in_vector, vector3d* out_vector);
extern real_point3d* __cdecl matrix4x3_transform_point(real_matrix4x3 const* matrix, real_point3d const* in_point, real_point3d* out_point);
extern void __cdecl matrix4x3_transform_points(real_matrix4x3 const* matrix, long total_point_count, real_point3d const* const points, real_point3d* const out_points);
extern void __cdecl matrix4x3_multiply(real_matrix4x3 const* in_matrix0, real_matrix4x3 const* in_matrix1, real_matrix4x3* out_matrix);
extern void __cdecl matrix4x3_inverse(real_matrix4x3 const* matrix, real_matrix4x3* out_matrix);

