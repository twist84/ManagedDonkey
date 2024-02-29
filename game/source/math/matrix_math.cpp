#include "math/matrix_math.hpp"

#include "cseries/cseries.hpp"

#include <math.h>

void __cdecl matrix4x3_rotation_from_vectors(real_matrix4x3* matrix, vector3d const* forward, vector3d const* up)
{
	matrix->scale = 1.0f;
	matrix->matrix.forward = *forward;
	cross_product3d(up, forward, &matrix->matrix.left);
	matrix->matrix.up = *up;
	set_real_point3d(&matrix->center, 0.0f, 0.0f, 0.0f);
}

void __cdecl matrix4x3_from_point_and_vectors(real_matrix4x3* matrix, real_point3d const* point, vector3d const* forward, vector3d const* up)
{
	ASSERT(point);

	matrix4x3_rotation_from_vectors(matrix, forward, up);
	matrix->center = *point;
}

vector3d* __cdecl matrix4x3_transform_vector(real_matrix4x3 const* matrix, vector3d const* in_vector, vector3d* out_vector)
{
	real forward = in_vector->n[0];
	real left = in_vector->n[1];
	real up = in_vector->n[2];

	if (matrix->scale != 1.0)
	{
		forward *= matrix->scale;
		left *= matrix->scale;
		up *= matrix->scale;
	}

	out_vector->n[0] = ((forward * matrix->matrix.forward.n[0]) + (left * matrix->matrix.left.n[0])) + (up * matrix->matrix.up.n[0]);
	out_vector->n[1] = ((forward * matrix->matrix.forward.n[1]) + (left * matrix->matrix.left.n[1])) + (up * matrix->matrix.up.n[1]);
	out_vector->n[2] = ((forward * matrix->matrix.forward.n[2]) + (left * matrix->matrix.left.n[2])) + (up * matrix->matrix.up.n[2]);

	return out_vector;
}

real_point3d* __cdecl matrix4x3_transform_point(real_matrix4x3 const* matrix, real_point3d const* in_point, real_point3d* out_point)
{
	real forward = in_point->n[0] * matrix->scale;
	real left = in_point->n[1] * matrix->scale;
	real up = in_point->n[2] * matrix->scale;

	out_point->n[0] = (((matrix->matrix.left.n[0] * left) + (matrix->matrix.forward.n[0] * forward)) + (matrix->matrix.up.n[0] * up)) + matrix->center.n[0];
	out_point->n[1] = (((matrix->matrix.left.n[1] * left) + (matrix->matrix.forward.n[1] * forward)) + (matrix->matrix.up.n[1] * up)) + matrix->center.n[1];
	out_point->n[2] = (((matrix->matrix.left.n[2] * left) + (matrix->matrix.forward.n[2] * forward)) + (matrix->matrix.up.n[2] * up)) + matrix->center.n[2];

	return out_point;
}

void __cdecl matrix4x3_transform_points(real_matrix4x3 const* matrix, long total_point_count, real_point3d const* const points, real_point3d* const out_points)
{
	for (long i = 0; i < total_point_count; ++i)
		matrix4x3_transform_point(matrix, &points[i], &out_points[i]);
}

void __cdecl matrix4x3_multiply(real_matrix4x3 const* in_matrix0, real_matrix4x3 const* in_matrix1, real_matrix4x3* out_matrix)
{
	if (in_matrix0 == out_matrix)
		csmemcpy(out_matrix, in_matrix0, sizeof(real_matrix4x3));

	if (in_matrix1 == out_matrix)
		csmemcpy(out_matrix, in_matrix1, sizeof(real_matrix4x3));

	out_matrix->matrix.forward.n[0] = ((in_matrix0->matrix.forward.n[0] * in_matrix1->matrix.forward.n[0]) + (in_matrix0->matrix.left.n[0] * in_matrix1->matrix.forward.n[1])) + (in_matrix0->matrix.up.n[0] * in_matrix1->matrix.forward.n[2]);
	out_matrix->matrix.forward.n[1] = ((in_matrix0->matrix.forward.n[1] * in_matrix1->matrix.forward.n[0]) + (in_matrix0->matrix.left.n[1] * in_matrix1->matrix.forward.n[1])) + (in_matrix0->matrix.up.n[1] * in_matrix1->matrix.forward.n[2]);
	out_matrix->matrix.forward.n[2] = ((in_matrix0->matrix.forward.n[2] * in_matrix1->matrix.forward.n[0]) + (in_matrix0->matrix.left.n[2] * in_matrix1->matrix.forward.n[1])) + (in_matrix0->matrix.up.n[2] * in_matrix1->matrix.forward.n[2]);

	out_matrix->matrix.left.n[0] = ((in_matrix0->matrix.forward.n[0] * in_matrix1->matrix.left.n[0]) + (in_matrix0->matrix.left.n[0] * in_matrix1->matrix.left.n[1])) + (in_matrix0->matrix.up.n[0] * in_matrix1->matrix.left.n[2]);
	out_matrix->matrix.left.n[1] = ((in_matrix0->matrix.forward.n[1] * in_matrix1->matrix.left.n[0]) + (in_matrix0->matrix.left.n[1] * in_matrix1->matrix.left.n[1])) + (in_matrix0->matrix.up.n[1] * in_matrix1->matrix.left.n[2]);
	out_matrix->matrix.left.n[2] = ((in_matrix0->matrix.forward.n[2] * in_matrix1->matrix.left.n[0]) + (in_matrix0->matrix.left.n[2] * in_matrix1->matrix.left.n[1])) + (in_matrix0->matrix.up.n[2] * in_matrix1->matrix.left.n[2]);

	out_matrix->matrix.up.n[0] = ((in_matrix0->matrix.forward.n[0] * in_matrix1->matrix.up.n[0]) + (in_matrix0->matrix.left.n[0] * in_matrix1->matrix.up.n[1])) + (in_matrix0->matrix.up.n[0] * in_matrix1->matrix.up.n[2]);
	out_matrix->matrix.up.n[1] = ((in_matrix0->matrix.forward.n[1] * in_matrix1->matrix.up.n[0]) + (in_matrix0->matrix.left.n[1] * in_matrix1->matrix.up.n[1])) + (in_matrix0->matrix.up.n[1] * in_matrix1->matrix.up.n[2]);
	out_matrix->matrix.up.n[2] = ((in_matrix0->matrix.forward.n[2] * in_matrix1->matrix.up.n[0]) + (in_matrix0->matrix.left.n[2] * in_matrix1->matrix.up.n[1])) + (in_matrix0->matrix.up.n[2] * in_matrix1->matrix.up.n[2]);

	out_matrix->center.n[0] = in_matrix0->center.n[0] + (in_matrix0->scale * (((in_matrix0->matrix.forward.n[0] * in_matrix1->center.n[0]) + (in_matrix0->matrix.left.n[0] * in_matrix1->center.n[1])) + (in_matrix0->matrix.up.n[0] * in_matrix1->center.n[2])));
	out_matrix->center.n[1] = in_matrix0->center.n[1] + (in_matrix0->scale * (((in_matrix0->matrix.forward.n[1] * in_matrix1->center.n[0]) + (in_matrix0->matrix.left.n[1] * in_matrix1->center.n[1])) + (in_matrix0->matrix.up.n[1] * in_matrix1->center.n[2])));
	out_matrix->center.n[2] = in_matrix0->center.n[2] + (in_matrix0->scale * (((in_matrix0->matrix.forward.n[2] * in_matrix1->center.n[0]) + (in_matrix0->matrix.left.n[2] * in_matrix1->center.n[1])) + (in_matrix0->matrix.up.n[2] * in_matrix1->center.n[2])));

	out_matrix->scale = in_matrix0->scale * in_matrix1->scale;
}

void __cdecl matrix4x3_inverse(real_matrix4x3 const* matrix, real_matrix4x3* out_matrix)
{
	real negative_x = -matrix->center.x;
	real negative_y = -matrix->center.y;
	real negative_z = -matrix->center.z;

	if (matrix->scale == 1.0f)
	{
		out_matrix->scale = 1.0f;
	}
	else
	{
		out_matrix->scale = 1.0f / (matrix->scale < 0.0f ? fmaxf(matrix->scale, -_real_epsilon) : fmaxf(matrix->scale, _real_epsilon));

		negative_x *= out_matrix->scale;
		negative_y *= out_matrix->scale;
		negative_z *= out_matrix->scale;
	}

	out_matrix->matrix.forward.i = matrix->matrix.forward.i;
	out_matrix->matrix.left.j = matrix->matrix.left.j;
	out_matrix->matrix.up.k = matrix->matrix.up.k;

	out_matrix->matrix.left.i = matrix->matrix.forward.j;
	out_matrix->matrix.forward.j = matrix->matrix.left.i;
	out_matrix->matrix.up.i = matrix->matrix.forward.k;

	out_matrix->matrix.forward.k = matrix->matrix.up.i;
	out_matrix->matrix.up.j = matrix->matrix.left.k;
	out_matrix->matrix.left.k = matrix->matrix.up.j;

	out_matrix->center.x = ((negative_x * out_matrix->matrix.forward.i) + (negative_y * out_matrix->matrix.left.i)) + (negative_z * out_matrix->matrix.up.i);
	out_matrix->center.y = ((negative_x * out_matrix->matrix.forward.j) + (negative_y * out_matrix->matrix.left.j)) + (negative_z * out_matrix->matrix.up.j);
	out_matrix->center.z = ((negative_x * out_matrix->matrix.forward.k) + (negative_y * out_matrix->matrix.left.k)) + (negative_z * out_matrix->matrix.up.k);
}

vector3d* __cdecl matrix4x3_transform_normal(real_matrix4x3 const* matrix, vector3d const* vector, vector3d* out_vector)
{
	out_vector->i = ((vector->i * matrix->matrix.forward.i) + (vector->j * matrix->matrix.left.i)) + (vector->k * matrix->matrix.up.i);
	out_vector->j = ((vector->i * matrix->matrix.forward.j) + (vector->j * matrix->matrix.left.j)) + (vector->k * matrix->matrix.up.j);
	out_vector->k = ((vector->i * matrix->matrix.forward.k) + (vector->j * matrix->matrix.left.k)) + (vector->k * matrix->matrix.up.k);

	return out_vector;
}

plane3d* __cdecl matrix4x3_transform_plane(real_matrix4x3 const* matrix, plane3d const* plane, plane3d* out_plane)
{
	matrix4x3_transform_normal(matrix, &plane->normal, &out_plane->normal);
	out_plane->distance = (matrix->scale * plane->distance) + dot_product3d((vector3d*)&matrix->center, &out_plane->normal);

	return out_plane;
}

