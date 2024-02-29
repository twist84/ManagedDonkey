#include "math/real_math.hpp"

#include "cseries/cseries.hpp"

#include <math.h>

REFERENCE_DECLARE(0x0189CF00, real_point3d const* const, global_origin3d);
REFERENCE_DECLARE(0x0189CF04, vector3d const* const, global_forward3d);
REFERENCE_DECLARE(0x0189CF28, vector3d const* const, global_up3d);
REFERENCE_DECLARE(0x0189CF5C, real_rectangle3d const* const, global_null_rectangle3d);

// networking/network_configuration.cpp
real __cdecl interpolate_linear(real start_value, real end_value, real interpolation_factor)
{
	return start_value + ((end_value - start_value) * interpolation_factor);
}

real __cdecl angle_between_vectors3d(vector3d const* a, vector3d const* b)
{
	return INVOKE(0x004EEC40, angle_between_vectors3d, a, b);
}

vector3d* __cdecl generate_up_vector3d(vector3d const* forward, vector3d* up)
{
	return INVOKE(0x004F2310, generate_up_vector3d, forward, up);
}

void __cdecl real_math_dispose()
{
	INVOKE(0x004FB7D0, real_math_dispose);

	//periodic_functions_dispose();
}

void __cdecl real_math_initialize()
{
	INVOKE(0x004FB7F0, real_math_initialize);

	//real_math_reset_precision();
	//periodic_functions_initialize();
}

//.text:004FB820

void __cdecl real_math_reset_precision()
{
	INVOKE(0x004FB850, real_math_reset_precision);
}

//.text:004FB880
//.text:004FB890
//.text:004FB920

euler_angles2d* __cdecl euler_angles2d_from_vector3d(euler_angles2d* facing, vector3d const* forward)
{
	return INVOKE(0x004F13E0, euler_angles2d_from_vector3d, facing, forward);
}

void __cdecl matrix4x3_from_point_and_quaternion(real_matrix4x3* matrix, real_point3d const* point, real_quaternion const* quaternion)
{
	INVOKE(0x005B1F30, matrix4x3_from_point_and_quaternion, matrix, point, quaternion);
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
	if (fabsf(result - 0.0f) < k_real_epsilon)
		result = 0.0f;
	else
		scale_vector3d(vector, 1.0f / result, vector);

	return result;
}

vector2d* __cdecl rotate_vector2d(vector2d const* vector, real a2, real a3, vector2d* out_vector)
{
	out_vector->i = (a3 * vector->i) - (a2 * vector->j);
	out_vector->j = (a2 * vector->i) + (a3 * vector->j);

	return out_vector;
}

vector3d* __cdecl negate_vector3d(vector3d const* in_vector, vector3d* out_vector)
{
	out_vector->i = -in_vector->i;
	out_vector->j = -in_vector->j;
	out_vector->k = -in_vector->k;

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
	if (fabsf((plane->normal.n[projection] - 0.0f)) >= k_real_epsilon)
		v7 = ((plane->distance - (plane->normal.n[v5] * point->n[0])) - (plane->normal.n[v6] * point->n[1])) / plane->normal.n[projection];

	ASSERT(projection >= _x && projection <= _z);
	out_point->n[v5] = point->n[0];
	out_point->n[v6] = point->n[1];
	out_point->n[projection] = v7;

	return out_point;
}

real __cdecl plane3d_distance_to_point(plane3d const* plane, real_point3d const* point)
{
	return ((((point->x * plane->normal.i) + (point->y * plane->normal.j)) + (point->z * plane->normal.k)) - plane->distance);
}

long __cdecl rectangle3d_build_vertices(real_rectangle3d const* bounds, long maximum_vertex_count, real_point3d* const vertices)
{
	ASSERT(bounds);
	ASSERT(maximum_vertex_count >= k_vertices_per_cube_count);
	ASSERT(vertices);

	vertices[0].x = bounds->x.lower;
	vertices[0].y = bounds->y.lower;
	vertices[0].z = bounds->z.lower;
	vertices[1].x = bounds->x.upper;
	vertices[1].y = bounds->y.lower;
	vertices[1].z = bounds->z.lower;
	vertices[2].x = bounds->x.lower;
	vertices[2].y = bounds->y.upper;
	vertices[2].z = bounds->z.lower;
	vertices[3].x = bounds->x.upper;
	vertices[3].y = bounds->y.upper;
	vertices[3].z = bounds->z.lower;
	vertices[4].x = bounds->x.lower;
	vertices[4].y = bounds->y.lower;
	vertices[4].z = bounds->z.upper;
	vertices[5].x = bounds->x.upper;
	vertices[5].y = bounds->y.lower;
	vertices[5].z = bounds->z.upper;
	vertices[6].x = bounds->x.lower;
	vertices[6].y = bounds->y.upper;
	vertices[6].z = bounds->z.upper;
	vertices[7].x = bounds->x.upper;
	vertices[7].y = bounds->y.upper;
	vertices[7].z = bounds->z.upper;

	return k_vertices_per_cube_count;
}

long __cdecl rectangle3d_build_faces(real_rectangle3d const* bounds, long maximum_face_count, real_point3d(* const faces)[4])
{
	ASSERT(bounds);
	ASSERT(maximum_face_count >= k_faces_per_cube_count);
	ASSERT(faces);

	long face_vertex_indices[k_faces_per_cube_count][4]
	{
		{ 0, 2, 3, 1 },
		{ 0, 1, 5, 4 },
		{ 1, 3, 7, 5 },
		{ 2, 6, 7, 3 },
		{ 0, 4, 6, 2 },
		{ 4, 5, 7, 6 },
	};

	real_point3d vertices[k_vertices_per_cube_count]{};
	rectangle3d_build_vertices(bounds, k_vertices_per_cube_count, vertices);

	for (long face_index = 0; face_index < k_faces_per_cube_count; face_index++)
	{
		for (long vertex_index = 0; vertex_index < k_vertices_per_cube_count; vertex_index++)
		{
			ASSERT((face_vertex_indices[face_index][vertex_index] >= 0) && (face_vertex_indices[face_index][vertex_index] < k_vertices_per_cube_count));

			*faces[face_vertex_indices[face_index][vertex_index]] = vertices[face_vertex_indices[face_index][vertex_index]];
		}
	}

	return k_faces_per_cube_count;
}

long __cdecl rectangle3d_build_edges(real_rectangle3d const* bounds, long maximum_edge_count, real_point3d(* const edges)[2])
{
	ASSERT(bounds);
	ASSERT(maximum_edge_count >= k_edges_per_cube_count);
	ASSERT(edges);

	long line_vertex_indices[k_edges_per_cube_count][2]
	{
		{ 0, 2 },
		{ 2, 3 },
		{ 3, 1 },
		{ 1, 0 },
		{ 0, 4 },
		{ 1, 5 },
		{ 2, 6 },
		{ 3, 7 },
		{ 4, 5 },
		{ 5, 7 },
		{ 7, 6 },
		{ 6, 4 },
	};

	real_point3d vertices[k_vertices_per_cube_count]{};
	rectangle3d_build_vertices(bounds, k_vertices_per_cube_count, vertices);

	for (long edge_index = 0; edge_index < k_edges_per_cube_count; edge_index++)
	{
		ASSERT((line_vertex_indices[edge_index][0] >= 0) && (line_vertex_indices[edge_index][0] < k_vertices_per_cube_count));
		ASSERT((line_vertex_indices[edge_index][1] >= 0) && (line_vertex_indices[edge_index][1] < k_vertices_per_cube_count));

		edges[edge_index][0] = vertices[line_vertex_indices[edge_index][0]];
		edges[edge_index][1] = vertices[line_vertex_indices[edge_index][1]];
	}

	return k_edges_per_cube_count;
}

real_rectangle2d* __cdecl set_real_rectangle2d(real_rectangle2d* rect, real x_lower, real x_upper, real y_lower, real y_upper)
{
	rect->x.lower = x_lower;
	rect->x.upper = x_upper;
	rect->y.lower = y_lower;
	rect->y.upper = y_upper;

	return rect;
}

real_rectangle3d* __cdecl set_real_rectangle3d(real_rectangle3d* rect, real x_lower, real x_upper, real y_lower, real y_upper, real z_lower, real z_upper)
{
	rect->x.lower = x_lower;
	rect->x.upper = x_upper;
	rect->y.lower = y_lower;
	rect->y.upper = y_upper;
	rect->z.lower = z_lower;
	rect->z.upper = z_upper;

	return rect;
}

real_rectangle3d* __cdecl real_rectangle3d_enclose_point(real_rectangle3d* enclosed_rect, real_point3d const* point)
{
	//return INVOKE(0x004FBD80, real_rectangle3d_enclose_point, enclosed_rect, point);

	if (enclosed_rect->x.lower > point->x)
		enclosed_rect->x.lower = point->x;

	if (enclosed_rect->x.upper < point->x)
		enclosed_rect->x.upper = point->x;

	if (enclosed_rect->y.lower > point->y)
		enclosed_rect->y.lower = point->y;

	if (enclosed_rect->y.upper < point->y)
		enclosed_rect->y.upper = point->y;

	if (enclosed_rect->z.lower > point->z)
		enclosed_rect->z.lower = point->z;

	if (enclosed_rect->z.upper < point->z)
		enclosed_rect->z.upper = point->z;

	return enclosed_rect;
}

real_rectangle3d* __cdecl real_rectangle3d_enclose_points(real_rectangle3d* enclosed_rect, long point_count, real_point3d const* points)
{
	//return INVOKE(0x004FBE40, real_rectangle3d_enclose_points, enclosed_rect, rect);

	//ASSERT(valid_polygon3d(point_count, points));

	for (long i = 0; i < point_count; i++)
		real_rectangle3d_enclose_point(enclosed_rect, &points[i]);

	return enclosed_rect;
}

real_rectangle3d* __cdecl real_rectangle3d_enclose_rectangle(real_rectangle3d* enclosed_rect, real_rectangle3d const* rect)
{
	//return INVOKE(0x004FBF30, real_rectangle3d_enclose_rectangle, enclosed_rect, rect);

	real x_lower = enclosed_rect->x.lower;
	if (x_lower > rect->x.lower)
		x_lower = rect->x.lower;
	enclosed_rect->x.lower = x_lower;

	real x_upper = rect->x.upper;
	if (enclosed_rect->x.upper > x_upper)
		x_upper = enclosed_rect->x.upper;
	enclosed_rect->x.upper = x_upper;

	real y_lower = rect->y.lower;
	if (enclosed_rect->y.lower <= y_lower)
		y_lower = enclosed_rect->y.lower;
	enclosed_rect->y.lower = y_lower;

	real y_upper = rect->y.upper;
	if (enclosed_rect->y.upper > y_upper)
		y_upper = enclosed_rect->y.upper;
	enclosed_rect->y.upper = y_upper;

	real z_lower = rect->z.lower;
	if (enclosed_rect->z.lower <= z_lower)
		z_lower = enclosed_rect->z.lower;
	enclosed_rect->z.lower = z_lower;

	real z_upper = rect->z.upper;
	if (enclosed_rect->z.upper > z_upper)
		z_upper = enclosed_rect->z.upper;
	enclosed_rect->z.upper = z_upper;

	return enclosed_rect;
}

vector4d* __cdecl set_real_vector4d(vector4d* result, real i, real j, real k, real w)
{
	result->i = i;
	result->j = j;
	result->k = k;
	result->w = w;

	return result;
}

