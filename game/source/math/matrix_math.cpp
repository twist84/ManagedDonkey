#include "math/matrix_math.hpp"

#include "cseries/cseries.hpp"

#include <math.h>

void __cdecl matrix4x3_rotation_from_vectors(real_matrix4x3* matrix, vector3d const* forward, vector3d const* up)
{
	matrix->scale = 1.0f;
	matrix->forward = *forward;
	cross_product3d(up, forward, &matrix->left);
	matrix->up = *up;
	set_real_point3d(&matrix->position, 0.0f, 0.0f, 0.0f);
}

void __cdecl matrix4x3_from_point_and_vectors(real_matrix4x3* matrix, real_point3d const* point, vector3d const* forward, vector3d const* up)
{
	ASSERT(point);

	matrix4x3_rotation_from_vectors(matrix, forward, up);
	matrix->position = *point;
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

	out_vector->n[0] = ((forward * matrix->forward.n[0]) + (left * matrix->left.n[0])) + (up * matrix->up.n[0]);
	out_vector->n[1] = ((forward * matrix->forward.n[1]) + (left * matrix->left.n[1])) + (up * matrix->up.n[1]);
	out_vector->n[2] = ((forward * matrix->forward.n[2]) + (left * matrix->left.n[2])) + (up * matrix->up.n[2]);

	return out_vector;
}

real_point3d* __cdecl matrix4x3_transform_point(real_matrix4x3 const* matrix, real_point3d const* in_point, real_point3d* out_point)
{
	real forward = in_point->n[0] * matrix->scale;
	real left = in_point->n[1] * matrix->scale;
	real up = in_point->n[2] * matrix->scale;

	out_point->n[0] = (((matrix->left.n[0] * left) + (matrix->forward.n[0] * forward)) + (matrix->up.n[0] * up)) + matrix->position.n[0];
	out_point->n[1] = (((matrix->left.n[1] * left) + (matrix->forward.n[1] * forward)) + (matrix->up.n[1] * up)) + matrix->position.n[1];
	out_point->n[2] = (((matrix->left.n[2] * left) + (matrix->forward.n[2] * forward)) + (matrix->up.n[2] * up)) + matrix->position.n[2];

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

	out_matrix->forward.n[0] = ((in_matrix0->forward.n[0] * in_matrix1->forward.n[0]) + (in_matrix0->left.n[0] * in_matrix1->forward.n[1])) + (in_matrix0->up.n[0] * in_matrix1->forward.n[2]);
	out_matrix->forward.n[1] = ((in_matrix0->forward.n[1] * in_matrix1->forward.n[0]) + (in_matrix0->left.n[1] * in_matrix1->forward.n[1])) + (in_matrix0->up.n[1] * in_matrix1->forward.n[2]);
	out_matrix->forward.n[2] = ((in_matrix0->forward.n[2] * in_matrix1->forward.n[0]) + (in_matrix0->left.n[2] * in_matrix1->forward.n[1])) + (in_matrix0->up.n[2] * in_matrix1->forward.n[2]);

	out_matrix->left.n[0] = ((in_matrix0->forward.n[0] * in_matrix1->left.n[0]) + (in_matrix0->left.n[0] * in_matrix1->left.n[1])) + (in_matrix0->up.n[0] * in_matrix1->left.n[2]);
	out_matrix->left.n[1] = ((in_matrix0->forward.n[1] * in_matrix1->left.n[0]) + (in_matrix0->left.n[1] * in_matrix1->left.n[1])) + (in_matrix0->up.n[1] * in_matrix1->left.n[2]);
	out_matrix->left.n[2] = ((in_matrix0->forward.n[2] * in_matrix1->left.n[0]) + (in_matrix0->left.n[2] * in_matrix1->left.n[1])) + (in_matrix0->up.n[2] * in_matrix1->left.n[2]);

	out_matrix->up.n[0] = ((in_matrix0->forward.n[0] * in_matrix1->up.n[0]) + (in_matrix0->left.n[0] * in_matrix1->up.n[1])) + (in_matrix0->up.n[0] * in_matrix1->up.n[2]);
	out_matrix->up.n[1] = ((in_matrix0->forward.n[1] * in_matrix1->up.n[0]) + (in_matrix0->left.n[1] * in_matrix1->up.n[1])) + (in_matrix0->up.n[1] * in_matrix1->up.n[2]);
	out_matrix->up.n[2] = ((in_matrix0->forward.n[2] * in_matrix1->up.n[0]) + (in_matrix0->left.n[2] * in_matrix1->up.n[1])) + (in_matrix0->up.n[2] * in_matrix1->up.n[2]);

	out_matrix->position.n[0] = in_matrix0->position.n[0] + (in_matrix0->scale * (((in_matrix0->forward.n[0] * in_matrix1->position.n[0]) + (in_matrix0->left.n[0] * in_matrix1->position.n[1])) + (in_matrix0->up.n[0] * in_matrix1->position.n[2])));
	out_matrix->position.n[1] = in_matrix0->position.n[1] + (in_matrix0->scale * (((in_matrix0->forward.n[1] * in_matrix1->position.n[0]) + (in_matrix0->left.n[1] * in_matrix1->position.n[1])) + (in_matrix0->up.n[1] * in_matrix1->position.n[2])));
	out_matrix->position.n[2] = in_matrix0->position.n[2] + (in_matrix0->scale * (((in_matrix0->forward.n[2] * in_matrix1->position.n[0]) + (in_matrix0->left.n[2] * in_matrix1->position.n[1])) + (in_matrix0->up.n[2] * in_matrix1->position.n[2])));

	out_matrix->scale = in_matrix0->scale * in_matrix1->scale;
}

void __cdecl matrix4x3_inverse(real_matrix4x3 const* matrix, real_matrix4x3* out_matrix)
{
	real negative_x = -matrix->position.x;
	real negative_y = -matrix->position.y;
	real negative_z = -matrix->position.z;

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

	out_matrix->forward.i = matrix->forward.i;
	out_matrix->left.j = matrix->left.j;
	out_matrix->up.k = matrix->up.k;

	out_matrix->left.i = matrix->forward.j;
	out_matrix->forward.j = matrix->left.i;
	out_matrix->up.i = matrix->forward.k;

	out_matrix->forward.k = matrix->up.i;
	out_matrix->up.j = matrix->left.k;
	out_matrix->left.k = matrix->up.j;

	out_matrix->position.x = ((negative_x * out_matrix->forward.i) + (negative_y * out_matrix->left.i)) + (negative_z * out_matrix->up.i);
	out_matrix->position.y = ((negative_x * out_matrix->forward.j) + (negative_y * out_matrix->left.j)) + (negative_z * out_matrix->up.j);
	out_matrix->position.z = ((negative_x * out_matrix->forward.k) + (negative_y * out_matrix->left.k)) + (negative_z * out_matrix->up.k);
}

vector3d* __cdecl matrix4x3_transform_normal(real_matrix4x3 const* matrix, vector3d const* vector, vector3d* out_vector)
{
	out_vector->i = ((vector->i * matrix->forward.i) + (vector->j * matrix->left.i)) + (vector->k * matrix->up.i);
	out_vector->j = ((vector->i * matrix->forward.j) + (vector->j * matrix->left.j)) + (vector->k * matrix->up.j);
	out_vector->k = ((vector->i * matrix->forward.k) + (vector->j * matrix->left.k)) + (vector->k * matrix->up.k);

	return out_vector;
}

plane3d* __cdecl matrix4x3_transform_plane(real_matrix4x3 const* matrix, plane3d const* plane, plane3d* out_plane)
{
	matrix4x3_transform_normal(matrix, &plane->n, &out_plane->n);
	out_plane->d = (matrix->scale * plane->d) + dot_product3d((vector3d*)&matrix->position, &out_plane->n);

	return out_plane;
}

