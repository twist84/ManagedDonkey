#pragma once

#include "math/real_math.hpp"

extern real_matrix3x3* __cdecl matrix3x3_rotation_from_quaternion(real_matrix3x3* matrix, const real_quaternion* quaternion);
extern void __cdecl matrix4x3_from_point_and_quaternion(real_matrix4x3* matrix, const real_point3d* point, const real_quaternion* quaternion);
extern void __cdecl matrix4x3_from_point_and_vectors(real_matrix4x3* matrix, const real_point3d* point, const real_vector3d* forward, const real_vector3d* up);
extern void __cdecl matrix4x3_inverse(const real_matrix4x3* matrix, real_matrix4x3* result);
extern void __cdecl matrix4x3_multiply(const real_matrix4x3* a, const real_matrix4x3* b, real_matrix4x3* result);
extern void __cdecl matrix4x3_rotation_from_vectors(real_matrix4x3* matrix, const real_vector3d* forward, const real_vector3d* up);
extern void __cdecl matrix4x3_rotation_to_angles(real_matrix4x3* matrix, real_euler_angles3d* angles);
extern real_vector3d* __cdecl matrix4x3_transform_normal(const real_matrix4x3* matrix, const real_vector3d* normal, real_vector3d* result);
extern real_plane3d* __cdecl matrix4x3_transform_plane(const real_matrix4x3* matrix, const real_plane3d* plane, real_plane3d* result);
extern real_point3d* __cdecl matrix4x3_transform_point(const real_matrix4x3* matrix, const real_point3d* point, real_point3d* result);
extern void __cdecl matrix4x3_transform_points(const real_matrix4x3* matrix, int32 point_count, const real_point3d* const points, real_point3d* const results);
extern real_vector3d* __cdecl matrix4x3_transform_vector(const real_matrix4x3* matrix, const real_vector3d* vector, real_vector3d* result);

