#include "math/matrix_math.hpp"

#include "cseries/cseries.hpp"

#include <math.h>

//.text:005B0330 ; real __cdecl matrix3x3_determinant(real_matrix3x3 const*)
//.text:005B03B0 ; void __cdecl matrix3x3_from_angles(real_matrix3x3*, real, real, real)
//.text:005B04F0 ; void __cdecl matrix3x3_from_angles_cpp(real_matrix3x3*, real, real, real)
//.text:005B0630 ; real_matrix3x3* __cdecl matrix3x3_from_axis_and_angle(real_matrix3x3*, real_vector3d const*, real, real)
//.text:005B0740 ; real_matrix3x3* __cdecl matrix3x3_from_forward_and_up(real_matrix3x3*, real_vector3d const*, real_vector3d const*)
//.text:005B07C0 ; real_matrix3x3* __cdecl matrix3x3_inverse(real_matrix3x3 const*, real, real_matrix3x3*)
//.text:005B0900 ; real_vector3d* __cdecl matrix3x3_inverse_transform_vector(real_matrix3x3 const*, real_vector3d const*, real_vector3d*)
//.text:005B09B0 ; real_matrix3x3* __cdecl matrix3x3_multiply(real_matrix3x3 const*, real_matrix3x3 const*, real_matrix3x3*)
//.text:005B0BB0 ; bool __cdecl matrix3x3_normalize(real_matrix3x3*)

real_matrix3x3* __cdecl matrix3x3_rotation_from_quaternion(real_matrix3x3* matrix, real_quaternion const* quaternion)
{
	return INVOKE(0x005B0F80, matrix3x3_rotation_from_quaternion, matrix, quaternion);
}

//.text:005B10E0 ; real_quaternion* __cdecl matrix3x3_rotation_to_quaternion(real_matrix3x3 const*, real_quaternion*)
//.text:005B12E0 ; real_vector3d* __cdecl matrix3x3_transform_vector(real_matrix3x3 const*, real_vector3d const*, real_vector3d*)
//.text:005B1390 ; real_matrix3x3* __cdecl matrix3x3_transpose(real_matrix3x3 const*, real_matrix3x3*)
//.text:005B1410 ; void __cdecl matrix4x3_apply_orientation(real_matrix4x3 const restrict*, real_orientation const restrict*, real_matrix4x3 restrict*)
//.text:005B1780 ; void __cdecl matrix4x3_apply_orientation_cpp(real_matrix4x3 const*, real_orientation const*, real_matrix4x3*)
//.text:005B1AF0 ; void __cdecl matrix4x3_from_orientation(real_matrix4x3*, real_orientation const*)
//.text:005B1C70 ; void __cdecl matrix4x3_from_plane(real_matrix4x3*, real_plane3d const*)
//.text:005B1E10 ; void __cdecl matrix4x3_from_point_and_nonaxial_forward_and_left(real_matrix4x3*, real_point3d const*, real_vector3d const*, real_vector3d const*)
//.text:005B1E70 ; void __cdecl matrix4x3_from_point_and_nonaxial_vectors(real_matrix4x3*, real_point3d const*, real_vector3d const*, real_vector3d const*)
//.text:005B1ED0 ; void __cdecl matrix4x3_from_point_and_nonaxial_vectors_using_up(real_matrix4x3*, real_point3d const*, real_vector3d const*, real_vector3d const*)

void __cdecl matrix4x3_from_point_and_quaternion(real_matrix4x3* matrix, real_point3d const* point, real_quaternion const* quaternion)
{
	INVOKE(0x005B1F30, matrix4x3_from_point_and_quaternion, matrix, point, quaternion);
}

void __cdecl matrix4x3_from_point_and_vectors(real_matrix4x3* matrix, real_point3d const* point, real_vector3d const* forward, real_vector3d const* up)
{
	//INVOKE(0x005B20C0, matrix4x3_from_point_and_vectors, matrix, point, forward, up);

	ASSERT(point);

	matrix4x3_rotation_from_vectors(matrix, forward, up);
	matrix->position = *point;
}

//.text:005B2170 ; void __cdecl matrix4x3_identity(real_matrix4x3*)

void __cdecl matrix4x3_inverse(real_matrix4x3 const* matrix, real_matrix4x3* result)
{
	INVOKE(0x005B21E0, matrix4x3_inverse, matrix, result);

	real negative_x = -matrix->position.x;
	real negative_y = -matrix->position.y;
	real negative_z = -matrix->position.z;

	if (matrix->scale == 1.0f)
	{
		result->scale = 1.0f;
	}
	else
	{
		result->scale = 1.0f / (matrix->scale < 0.0f ? fmaxf(matrix->scale, -_real_epsilon) : fmaxf(matrix->scale, _real_epsilon));

		negative_x *= result->scale;
		negative_y *= result->scale;
		negative_z *= result->scale;
	}

	result->forward.i = matrix->forward.i;
	result->left.j = matrix->left.j;
	result->up.k = matrix->up.k;

	result->left.i = matrix->forward.j;
	result->forward.j = matrix->left.i;
	result->up.i = matrix->forward.k;

	result->forward.k = matrix->up.i;
	result->up.j = matrix->left.k;
	result->left.k = matrix->up.j;

	result->position.x = ((negative_x * result->forward.i) + (negative_y * result->left.i)) + (negative_z * result->up.i);
	result->position.y = ((negative_x * result->forward.j) + (negative_y * result->left.j)) + (negative_z * result->up.j);
	result->position.z = ((negative_x * result->forward.k) + (negative_y * result->left.k)) + (negative_z * result->up.k);
}

//.text:005B2320 ; real_vector3d* __cdecl matrix4x3_inverse_transform_normal(real_matrix4x3 const*, real_vector3d const*, real_vector3d*)
//.text:005B23C0 ; real_plane3d* __cdecl matrix4x3_inverse_transform_plane(real_matrix4x3 const*, real_plane3d const*, real_plane3d*)
//.text:005B24D0 ; real_point3d* __cdecl matrix4x3_inverse_transform_point(real_matrix4x3 const*, real_point3d const*, real_point3d*)
//.text:005B25D0 ; void __cdecl matrix4x3_inverse_transform_points(real_matrix4x3 const*, long, real_point3d const* const, real_point3d* const)
//.text:005B2710 ; real_vector3d* __cdecl matrix4x3_inverse_transform_vector(real_matrix4x3 const*, real_vector3d const*, real_vector3d*)

void __cdecl matrix4x3_multiply(real_matrix4x3 const* a, real_matrix4x3 const* b, real_matrix4x3* result)
{
	//INVOKE(0x005B2800, matrix4x3_multiply, a, b, result);

	if (a == result)
		csmemcpy(result, a, sizeof(real_matrix4x3));

	if (b == result)
		csmemcpy(result, b, sizeof(real_matrix4x3));

	result->forward.n[0] = ((a->forward.n[0] * b->forward.n[0]) + (a->left.n[0] * b->forward.n[1])) + (a->up.n[0] * b->forward.n[2]);
	result->forward.n[1] = ((a->forward.n[1] * b->forward.n[0]) + (a->left.n[1] * b->forward.n[1])) + (a->up.n[1] * b->forward.n[2]);
	result->forward.n[2] = ((a->forward.n[2] * b->forward.n[0]) + (a->left.n[2] * b->forward.n[1])) + (a->up.n[2] * b->forward.n[2]);

	result->left.n[0] = ((a->forward.n[0] * b->left.n[0]) + (a->left.n[0] * b->left.n[1])) + (a->up.n[0] * b->left.n[2]);
	result->left.n[1] = ((a->forward.n[1] * b->left.n[0]) + (a->left.n[1] * b->left.n[1])) + (a->up.n[1] * b->left.n[2]);
	result->left.n[2] = ((a->forward.n[2] * b->left.n[0]) + (a->left.n[2] * b->left.n[1])) + (a->up.n[2] * b->left.n[2]);

	result->up.n[0] = ((a->forward.n[0] * b->up.n[0]) + (a->left.n[0] * b->up.n[1])) + (a->up.n[0] * b->up.n[2]);
	result->up.n[1] = ((a->forward.n[1] * b->up.n[0]) + (a->left.n[1] * b->up.n[1])) + (a->up.n[1] * b->up.n[2]);
	result->up.n[2] = ((a->forward.n[2] * b->up.n[0]) + (a->left.n[2] * b->up.n[1])) + (a->up.n[2] * b->up.n[2]);

	result->position.n[0] = a->position.n[0] + (a->scale * (((a->forward.n[0] * b->position.n[0]) + (a->left.n[0] * b->position.n[1])) + (a->up.n[0] * b->position.n[2])));
	result->position.n[1] = a->position.n[1] + (a->scale * (((a->forward.n[1] * b->position.n[0]) + (a->left.n[1] * b->position.n[1])) + (a->up.n[1] * b->position.n[2])));
	result->position.n[2] = a->position.n[2] + (a->scale * (((a->forward.n[2] * b->position.n[0]) + (a->left.n[2] * b->position.n[1])) + (a->up.n[2] * b->position.n[2])));

	result->scale = a->scale * b->scale;
}

//.text:005B2A90 ; void __cdecl matrix4x3_multiply_cpp(real_matrix4x3 const* a, real_matrix4x3 const* b, real_matrix4x3* result)
//.text:005B2D20 ; void __cdecl matrix4x3_rotation_between_vectors(real_matrix4x3*, real_vector3d const*, real_vector3d const*)
//.text:005B3100 ; void __cdecl matrix4x3_rotation_from_angles(real_matrix4x3*, real, real, real)
//.text:005B3270 ; void __cdecl matrix4x3_rotation_from_axis_and_angle(real_matrix4x3*, real_vector3d const*, real, real)
//.text:005B33A0 ; void __cdecl matrix4x3_rotation_from_nonaxial_forward_and_left(real_matrix4x3*, real_vector3d const*, real_vector3d const*)
//.text:005B33E0 ; void __cdecl matrix4x3_rotation_from_nonaxial_vectors(real_matrix4x3*, real_vector3d const*, real_vector3d const*)
//.text:005B3420 ; void __cdecl matrix4x3_rotation_from_nonaxial_vectors_using_up(real_matrix4x3*, real_vector3d const*, real_vector3d const*)
//.text:005B3460 ; void __cdecl matrix4x3_rotation_from_quaternion(real_matrix4x3*, real_quaternion const*)

void __cdecl matrix4x3_rotation_from_vectors(real_matrix4x3* matrix, real_vector3d const* forward, real_vector3d const* up)
{
	//INVOKE(0x005B35D0, matrix4x3_rotation_from_vectors, matrix, forward, up);

	matrix->scale = 1.0f;
	matrix->forward = *forward;
	cross_product3d(up, forward, &matrix->left);
	matrix->up = *up;
	set_real_point3d(&matrix->position, 0.0f, 0.0f, 0.0f);
}

void __cdecl matrix4x3_rotation_to_angles(real_matrix4x3* matrix, real_euler_angles3d* angles)
{
	INVOKE(0x005B3670, matrix4x3_rotation_to_angles, matrix, angles);
}

//.text:005B37D0 ; void __cdecl matrix4x3_rotation_to_quaternion(real_matrix4x3 const*, real_quaternion*)
//.text:005B37E0 ; void __cdecl matrix4x3_scale(real_matrix4x3*, real)
//.text:005B3850 ; void __cdecl matrix4x3_to_orientation(real_matrix4x3 const*, real_orientation*)
//.text:005B3890 ; void __cdecl matrix4x3_to_point_and_vectors(real_matrix4x3 const*, real_point3d*, real_vector3d*, real_vector3d*)

real_vector3d* __cdecl matrix4x3_transform_normal(real_matrix4x3 const* matrix, real_vector3d const* normal, real_vector3d* result)
{
	//return INVOKE(0x005B38D0, matrix4x3_transform_normal, matrix, normal, result);

	result->i = ((normal->i * matrix->forward.i) + (normal->j * matrix->left.i)) + (normal->k * matrix->up.i);
	result->j = ((normal->i * matrix->forward.j) + (normal->j * matrix->left.j)) + (normal->k * matrix->up.j);
	result->k = ((normal->i * matrix->forward.k) + (normal->j * matrix->left.k)) + (normal->k * matrix->up.k);

	return result;
}

real_plane3d* __cdecl matrix4x3_transform_plane(real_matrix4x3 const* matrix, real_plane3d const* plane, real_plane3d* result)
{
	//return INVOKE(0x005B3970, matrix4x3_transform_plane, matrix, plane, result);

	matrix4x3_transform_normal(matrix, &plane->n, &result->n);
	result->d = (matrix->scale * plane->d) + dot_product3d((real_vector3d*)&matrix->position, &result->n);

	return result;
}

real_point3d* __cdecl matrix4x3_transform_point(real_matrix4x3 const* matrix, real_point3d const* point, real_point3d* result)
{
	//return INVOKE(0x005B3A40, matrix4x3_transform_point, matrix, point, result);

	real forward = point->n[0] * matrix->scale;
	real left = point->n[1] * matrix->scale;
	real up = point->n[2] * matrix->scale;

	result->n[0] = (((matrix->left.n[0] * left) + (matrix->forward.n[0] * forward)) + (matrix->up.n[0] * up)) + matrix->position.n[0];
	result->n[1] = (((matrix->left.n[1] * left) + (matrix->forward.n[1] * forward)) + (matrix->up.n[1] * up)) + matrix->position.n[1];
	result->n[2] = (((matrix->left.n[2] * left) + (matrix->forward.n[2] * forward)) + (matrix->up.n[2] * up)) + matrix->position.n[2];

	return result;
}

void __cdecl matrix4x3_transform_points(real_matrix4x3 const* matrix, long point_count, real_point3d const* const points, real_point3d* const results)
{
	//INVOKE(0x005B3B00, matrix4x3_transform_points, matrix, point_count, points, results);

	for (long i = 0; i < point_count; ++i)
		matrix4x3_transform_point(matrix, &points[i], &results[i]);
}

real_vector3d* __cdecl matrix4x3_transform_vector(real_matrix4x3 const* matrix, real_vector3d const* vector, real_vector3d* result)
{
	//return INVOKE(0x005B3BD0, matrix4x3_transform_vector, matrix, vector, result);

	real forward = vector->n[0];
	real left = vector->n[1];
	real up = vector->n[2];

	if (matrix->scale != 1.0)
	{
		forward *= matrix->scale;
		left *= matrix->scale;
		up *= matrix->scale;
	}

	result->n[0] = ((forward * matrix->forward.n[0]) + (left * matrix->left.n[0])) + (up * matrix->up.n[0]);
	result->n[1] = ((forward * matrix->forward.n[1]) + (left * matrix->left.n[1])) + (up * matrix->up.n[1]);
	result->n[2] = ((forward * matrix->forward.n[2]) + (left * matrix->left.n[2])) + (up * matrix->up.n[2]);

	return result;
}

//.text:005B3C90 ; void __cdecl matrix4x3_translation(real_matrix4x3*, real_point3d const*)
//.text:005B3CF0 ; void __cdecl matrix4x3_transpose(real_matrix4x3*)

//.text:005B3F40 ; real_vector3d* __cdecl vector_from_matrices4x3(real_matrix4x3 const*, real_matrix4x3 const*, real_vector3d*)

