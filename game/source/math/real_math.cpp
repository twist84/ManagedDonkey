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

real_point2d* __cdecl point_from_line2d(real_point2d const* point, vector2d const* vector, real scale, real_point2d* out_point)
{
	*out_point = *point;
	out_point->x *= (vector->i * scale);
	out_point->y *= (vector->j * scale);

	return out_point;
};

vector2d* __cdecl vector_from_points2d(real_point2d const* point0, real_point2d const* point1, vector2d* out_vector)
{
	out_vector->i = point1->x - point0->x;
	out_vector->j = point1->y - point0->y;

	return out_vector;
};

short const global_projection3d_mappings[18] = { _z, _y, _x, _y, _z, _x, _x, _z, _y, _z, _x, _y, _y, _x, _z, _x, _y, _z };
short const global_projection3d_inverse_mappings[18] = { _z, _y, _x, _z, _x, _y, _x, _z, _y,	_y, _z, _x, _y, _x, _z, _x, _y, _z };

real_point3d* __cdecl project_point2d(real_point2d const* point, plane3d const* plane, short projection, bool a4, real_point3d* out_point)
{
	short v5 = global_projection3d_mappings[2 * projection + 3 * a4];
	short v6 = global_projection3d_mappings[2 * projection + 1 + 3 * a4];

	real v7 = 0.0f;
	if (fabsf((plane->normal.n[projection] - 0.0f)) >= 0.000099999997f)
		v7 = ((plane->distance - (plane->normal.n[v5] * point->n[0])) - (plane->normal.n[v6] * point->n[1])) / plane->normal.n[projection];

	ASSERT(projection >= _x && projection <= _z);
	out_point->n[v5] = point->n[0];
	out_point->n[v6] = point->n[1];
	out_point->n[projection] = v7;

	return out_point;
}

