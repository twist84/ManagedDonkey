#include "math/real_math.hpp"

#include "cseries/cseries.hpp"

real __cdecl angle_between_vectors3d(vector3d const* a, vector3d const* b)
{
	return INVOKE(0x004EEC40, angle_between_vectors3d, a, b);
}

vector3d* __cdecl generate_up_vector3d(vector3d const* forward, vector3d* up)
{
	return INVOKE(0x004F2310, generate_up_vector3d, forward, up);
}

euler_angles2d* __cdecl euler_angles2d_from_vector3d(euler_angles2d* facing, vector3d const* forward)
{
	return INVOKE(0x004F13E0, euler_angles2d_from_vector3d, facing, forward);
}

vector3d* __cdecl cross_product3d(vector3d const* a, vector3d const* b, vector3d* out)
{
	out->n[0] = real(a->n[1] * b->n[2]) - real(a->n[2] * b->n[1]);
	out->n[1] = real(a->n[2] * b->n[0]) - real(a->n[0] * b->n[2]);
	out->n[2] = real(a->n[0] * b->n[1]) - real(a->n[1] * b->n[0]);

	return out;
}

real __cdecl dot_product3d(vector3d const* a, vector3d const* b)
{
	return ((a->n[0] * b->n[0]) + (a->n[1] * b->n[1])) + (a->n[2] * b->n[2]);
}

vector3d* __cdecl vector_from_points3d(real_point3d const* a, real_point3d const* b, vector3d* out)
{
	out->n[0] = b->n[0] - a->n[0];
	out->n[1] = b->n[1] - a->n[1];
	out->n[2] = b->n[2] - a->n[2];

	return out;
}

real __cdecl magnitude_squared3d(vector3d const* a)
{
	return real(a->n[0] * a->n[0]) + real(a->n[1] * a->n[1]) + real(a->n[2] * a->n[2]);
}

real __cdecl distance_squared3d(real_point3d const* a, real_point3d const* b)
{
	vector3d temp{};
	return magnitude_squared3d(vector_from_points3d(a, b, &temp));
}

real __cdecl distance3d(real_point3d const* a, real_point3d const* b)
{
	return square_root(distance_squared3d(a, b));
}

real __cdecl square_root(real value)
{
	return sqrtf(value); // sqrt
}

real __cdecl magnitude3d(vector3d const* vector)
{
	return square_root(magnitude_squared3d(vector));
}

vector3d* __cdecl perpendicular3d(vector3d const* vector, vector3d* out_vector)
{
	real i = fabsf(vector->i);
	real j = fabsf(vector->j);
	real k = fabsf(vector->k);
	if (i > j || i > k)
	{
		if (j > k)
		{
			out_vector->i = vector->j;
			out_vector->j = -vector->i;
			out_vector->k = 0.0f;
		}
		else
		{
			out_vector->i = -vector->k;
			out_vector->j = 0.0f;
			out_vector->k = vector->i;
		}
	}
	else
	{
		out_vector->i = 0.0f;
		out_vector->j = vector->k;
		out_vector->k = -vector->j;
	}

	return out_vector;
}

real __cdecl normalize3d(vector3d* vector)
{
	real result = magnitude3d(vector);
	if (fabsf(result - 0.0f) < 0.000099999997f)
		result = 0.0f;
	else
		scale_vector3d(vector, 1.0f / result, vector);

	return result;
}

vector2d* __cdecl rotate_vector2d(vector2d const* vector, real a2, real a3, vector2d* out_vector)
{
	out_vector->i = (a2 * vector->i) + (a3 * vector->i);
	out_vector->j = (a3 * vector->j) + (a2 * vector->j);

	return out_vector;
}

vector3d* __cdecl scale_vector3d(vector3d const* in_vector, real scale, vector3d* out_vector)
{
	out_vector->i = scale * in_vector->i;
	out_vector->j = scale * in_vector->j;
	out_vector->k = scale * in_vector->k;

	return out_vector;
}

real_point2d* __cdecl set_real_point2d(real_point2d* point, real x, real y)
{
	point->x = x;
	point->y = y;

	return point;
}

real_point3d* __cdecl set_real_point3d(real_point3d* point, real x, real y, real z)
{
	point->x = x;
	point->y = y;
	point->z = z;

	return point;
}

