#include "math/matrix_math.hpp"

#include "cseries/cseries.hpp"

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

