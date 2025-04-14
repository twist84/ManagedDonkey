#pragma once

#include "math/real_math.hpp"

extern real_matrix3x3* __cdecl matrix3x3_rotation_from_quaternion(real_matrix3x3* matrix, real_quaternion const* quaternion);
extern void __cdecl matrix4x3_from_point_and_quaternion(real_matrix4x3* matrix, real_point3d const* point, real_quaternion const* quaternion);
extern void __cdecl matrix4x3_from_point_and_vectors(real_matrix4x3* matrix, real_point3d const* point, real_vector3d const* forward, real_vector3d const* up);
extern void __cdecl matrix4x3_inverse(real_matrix4x3 const* matrix, real_matrix4x3* result);
extern void __cdecl matrix4x3_multiply(real_matrix4x3 const* a, real_matrix4x3 const* b, real_matrix4x3* result);
extern void __cdecl matrix4x3_rotation_from_vectors(real_matrix4x3* matrix, real_vector3d const* forward, real_vector3d const* up);
extern void __cdecl matrix4x3_rotation_to_angles(real_matrix4x3* matrix, real_euler_angles3d* angles);
extern real_vector3d* __cdecl matrix4x3_transform_normal(real_matrix4x3 const* matrix, real_vector3d const* normal, real_vector3d* result);
extern real_plane3d* __cdecl matrix4x3_transform_plane(real_matrix4x3 const* matrix, real_plane3d const* plane, real_plane3d* result);
extern real_point3d* __cdecl matrix4x3_transform_point(real_matrix4x3 const* matrix, real_point3d const* point, real_point3d* result);
extern void __cdecl matrix4x3_transform_points(real_matrix4x3 const* matrix, long point_count, real_point3d const* const points, real_point3d* const results);
extern real_vector3d* __cdecl matrix4x3_transform_vector(real_matrix4x3 const* matrix, real_vector3d const* vector, real_vector3d* result);

