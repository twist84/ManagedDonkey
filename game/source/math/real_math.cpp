#include "math/real_math.hpp"

#include "cseries/cseries.hpp"

#include <math.h>

REFERENCE_DECLARE(0x0189CED8, real_point2d const* const, global_origin2d);
REFERENCE_DECLARE(0x0189CEDC, real_point2d const* const, global_x_axis2d);
REFERENCE_DECLARE(0x0189CEE0, real_point2d const* const, global_y_axis2d);
REFERENCE_DECLARE(0x0189CEE4, real_point2d const* const, global_negative_x_axis2d);
REFERENCE_DECLARE(0x0189CEE8, real_point2d const* const, global_negative_y_axis2d);
REFERENCE_DECLARE(0x0189CEEC, real_vector2d const* const, global_zero_vector2d);
REFERENCE_DECLARE(0x0189CEF0, real_vector2d const* const, global_forward2d);
REFERENCE_DECLARE(0x0189CEF4, real_vector2d const* const, global_left2d);
REFERENCE_DECLARE(0x0189CEF8, real_vector2d const* const, global_backward2d);
REFERENCE_DECLARE(0x0189CEFC, real_vector2d const* const, global_right2d);
REFERENCE_DECLARE(0x0189CF00, real_point3d const* const, global_origin3d);
REFERENCE_DECLARE(0x0189CF04, real_vector3d const* const, global_x_axis3d);
REFERENCE_DECLARE(0x0189CF08, real_vector3d const* const, global_y_axis3d);
REFERENCE_DECLARE(0x0189CF0C, real_vector3d const* const, global_z_axis3d);
REFERENCE_DECLARE(0x0189CF10, real_vector3d const* const, global_negative_x_axis3d);
REFERENCE_DECLARE(0x0189CF14, real_vector3d const* const, global_negative_y_axis3d);
REFERENCE_DECLARE(0x0189CF18, real_vector3d const* const, global_negative_z_axis3d);
REFERENCE_DECLARE(0x0189CF1C, real_vector3d const* const, global_zero_vector3d);
REFERENCE_DECLARE(0x0189CF20, real_vector3d const* const, global_forward3d);
REFERENCE_DECLARE(0x0189CF24, real_vector3d const* const, global_left3d);
REFERENCE_DECLARE(0x0189CF28, real_vector3d const* const, global_up3d);
REFERENCE_DECLARE(0x0189CF2C, real_vector3d const* const, global_backward3d);
REFERENCE_DECLARE(0x0189CF30, real_vector3d const* const, global_right3d);
REFERENCE_DECLARE(0x0189CF34, real_vector3d const* const, global_down3d);
REFERENCE_DECLARE(0x0189CF38, real_euler_angles2d const* const, global_zero_angles2d);
REFERENCE_DECLARE(0x0189CF3C, real_euler_angles3d const* const, global_zero_angles3d);
REFERENCE_DECLARE(0x0189CF40, real_vector4d const* const, global_zero_vector4d);
REFERENCE_DECLARE(0x0189CF44, real_quaternion const* const, global_identity_quaternion);
REFERENCE_DECLARE(0x0189CF48, real_matrix3x3 const* const, global_identity3x3);
REFERENCE_DECLARE(0x0189CF4C, real_matrix4x3 const* const, global_identity4x3);
REFERENCE_DECLARE(0x0189CF50, real_matrix4x3 const* const, global_negative_identity4x3);
REFERENCE_DECLARE(0x0189CF54, real_orientation const* const, global_identity_orientation);
REFERENCE_DECLARE(0x0189CF58, real_rectangle2d const* const, global_null_rectangle2d);
REFERENCE_DECLARE(0x0189CF5C, real_rectangle3d const* const, global_null_rectangle3d);
REFERENCE_DECLARE(0x0189CF60, real_rectangle2d const* const, global_zero_rectangle2d);
REFERENCE_DECLARE(0x0189CF64, real_rectangle3d const* const, global_zero_rectangle3d);

bool __cdecl valid_real_vector2d(real_vector2d const* vector)
{
	return valid_real(vector->i)
		&& valid_real(vector->j);
}

bool __cdecl valid_real_vector4d(real_vector4d const* vector)
{
	return valid_real(vector->i)
		&& valid_real(vector->j)
		&& valid_real(vector->k)
		&& valid_real(vector->l);
}

bool __cdecl valid_real_sine_cosine(real sine, real cosine)
{
	return valid_realcmp(((sine * sine) + (cosine * cosine)), 1.0f);
}

bool __cdecl valid_real_quaternion(real_quaternion const* quaternion)
{
	return valid_realcmp((magnitude_squared3d(&quaternion->v) + (quaternion->w * quaternion->w)), 1.0f);
}

bool __cdecl valid_real_normal2d(real_vector2d const* normal)
{
	return valid_realcmp(magnitude_squared2d(normal), 1.0f);
}

bool __cdecl valid_real_plane2d(real_plane2d const* plane)
{
	return valid_real_normal2d(&plane->n)
		&& valid_real(plane->d);
}

bool __cdecl valid_real_plane3d(real_plane3d const* plane)
{
	return valid_real_normal3d(&plane->n)
		&& valid_real(plane->d);
}

bool __cdecl valid_real_vector3d_right_handed_axes3(real_vector3d const* forward, real_vector3d const* left, real_vector3d const* up)
{
	return valid_real_vector3d_axes3(forward, left, up)
		&& triple_product3d(forward, left, up) > 0.0f;
}

bool __cdecl valid_real_matrix4x3(real_matrix4x3 const* matrix)
{
	return valid_real(matrix->scale)
		&& valid_real_vector3d_axes3(&matrix->forward, &matrix->left, &matrix->up)
		&& valid_real_point3d(&matrix->position);
}

bool __cdecl valid_real_euler_angles3d(real_euler_angles3d const* angles)
{
	return valid_real(angles->yaw)
		&& valid_real(angles->pitch)
		&& valid_real(angles->roll)
		&& IN_RANGE_INCLUSIVE(angles->yaw, 0.0f, k_full_circle)
		&& IN_RANGE_INCLUSIVE(angles->pitch, -k_quarter_circle, k_quarter_circle)
		&& IN_RANGE_INCLUSIVE(angles->roll, -k_half_circle, k_half_circle);
}

// networking/network_configuration.cpp
real __cdecl interpolate_linear(real start_value, real end_value, real interpolation_factor)
{
	return start_value + ((end_value - start_value) * interpolation_factor);
}

bool __cdecl valid_real(real const& value)
{
	//return INVOKE(0x004AB2A0, valid_real, value);

	return ((dword)value & 0x7F800000) != 0x7F800000;
}

bool __cdecl valid_real_euler_angles2d(real_euler_angles2d const* angles)
{
	//return INVOKE(0x004AB2C0, valid_real_euler_angles2d, angles);

	return valid_real(angles->yaw)
		&& valid_real(angles->pitch)
		&& IN_RANGE_INCLUSIVE(angles->yaw, 0.0f, k_full_circle)
		&& IN_RANGE_INCLUSIVE(angles->pitch, -k_quarter_circle, k_quarter_circle);
}

bool __cdecl valid_real_normal3d(real_vector3d const* normal)
{
	//return INVOKE(0x004AB330, valid_real_normal3d, normal);

	return valid_realcmp(magnitude_squared3d(normal), 1.0f);
}

bool __cdecl valid_real_point3d(real_point3d const* point)
{
	//return INVOKE(0x004AB3A0, valid_real_point3d, point);

	return valid_real(point->x)
		&& valid_real(point->y)
		&& valid_real(point->z);
}

bool __cdecl valid_real_point2d(real_point2d const* point)
{
	//return INVOKE(0x004AB3E0, valid_real_point2d, point);

	return valid_real(point->x)
		&& valid_real(point->y);
}

bool __cdecl valid_real_vector3d(real_vector3d const* vector)
{
	//return INVOKE(0x004AB410, valid_real_vector3d, vector);

	return valid_real(vector->i)
		&& valid_real(vector->j)
		&& valid_real(vector->k);
}

bool __cdecl valid_real_vector3d_axes2(real_vector3d const* forward, real_vector3d const* up)
{
	//return INVOKE(0x004AB450, valid_real_vector3d_axes2, forward, up);

	return valid_real_normal3d(forward)
		&& valid_real_normal3d(up)
		&& valid_realcmp(dot_product3d(forward, up), 0.0f);
}

bool __cdecl valid_realcmp(real a, real b)
{
	//return INVOKE(0x004AB570, valid_realcmp, a, b);

	real diff = a - b;
	return valid_real(diff)
		&& fabs(diff) < k_test_real_epsilon;
}

real __cdecl angle_between_vectors3d(real_vector3d const* a, real_vector3d const* b)
{
	return INVOKE(0x004EEC40, angle_between_vectors3d, a, b);
}

real_vector3d* __cdecl generate_up_vector3d(real_vector3d const* forward, real_vector3d* up)
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

real_euler_angles2d* __cdecl euler_angles2d_from_vector3d(real_euler_angles2d* facing, real_vector3d const* forward)
{
	return INVOKE(0x004F13E0, euler_angles2d_from_vector3d, facing, forward);
}

//.text:00523B20 ; 
//.text:00523B70 ; 

bool __cdecl valid_real_vector3d_axes3(real_vector3d const* forward, real_vector3d const* left, real_vector3d const* up)
{
	//return INVOKE(0x005B3D30, valid_real_vector3d_axes3, forward, left, up);

	return valid_real_normal3d(forward)
		&& valid_real_normal3d(left)
		&& valid_real_normal3d(up)
		&& valid_realcmp(dot_product3d(forward, left), 0.0f)
		&& valid_realcmp(dot_product3d(left, up), 0.0f)
		&& valid_realcmp(dot_product3d(up, forward), 0.0f);
}

//.text:005B0330 ; real __cdecl matrix3x3_determinant(real_matrix3x3 const*)
//.text:005B03B0 ; void __cdecl matrix3x3_from_angles(real_matrix3x3*, real, real, real)
//.text:005B04F0 ; 
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
//.text:005B1780 ; 
//.text:005B1AF0 ; 
//.text:005B1C70 ; 
//.text:005B1E10 ; void __cdecl matrix4x3_from_point_and_nonaxial_forward_and_left(real_matrix4x3*, real_point3d const*, real_vector3d const*, real_vector3d const*)
//.text:005B1E70 ; void __cdecl matrix4x3_from_point_and_nonaxial_vectors(real_matrix4x3*, real_point3d const*, real_vector3d const*, real_vector3d const*)
//.text:005B1ED0 ; void __cdecl matrix4x3_from_point_and_nonaxial_vectors_using_up(real_matrix4x3*, real_point3d const*, real_vector3d const*, real_vector3d const*)

void __cdecl matrix4x3_from_point_and_quaternion(real_matrix4x3* matrix, real_point3d const* point, real_quaternion const* quaternion)
{
	INVOKE(0x005B1F30, matrix4x3_from_point_and_quaternion, matrix, point, quaternion);
}

bool __cdecl valid_polygon2d(long point_count, real_point2d const* const points)
{
	//return INVOKE(0x0078AF90, valid_polygon2d, point_count, points);

	ASSERT(point_count >= 0);
	ASSERT(point_count == 0 || points);

	for (long i = 0; i < point_count; i++)
	{
		if (!valid_real_point2d(&points[i]))
			return false;
	}

	return true;
}

//.text:0078B440 ; 
//.text:0078BF90 ; 
//.text:00C35C30 ; 

real_vector3d* __cdecl cross_product3d(real_vector3d const* a, real_vector3d const* b, real_vector3d* out)
{
	out->n[0] = real(a->n[1] * b->n[2]) - real(a->n[2] * b->n[1]);
	out->n[1] = real(a->n[2] * b->n[0]) - real(a->n[0] * b->n[2]);
	out->n[2] = real(a->n[0] * b->n[1]) - real(a->n[1] * b->n[0]);

	return out;
}

real __cdecl dot_product3d(real_vector3d const* a, real_vector3d const* b)
{
	return ((a->n[0] * b->n[0]) + (a->n[1] * b->n[1])) + (a->n[2] * b->n[2]);
}

real __cdecl triple_product3d(real_vector3d const* a, real_vector3d const* b, real_vector3d const* c)
{
	real_vector3d product3d;
	return dot_product3d(cross_product3d(a, b, &product3d), c);
}

real_vector3d* __cdecl vector_from_points3d(real_point3d const* a, real_point3d const* b, real_vector3d* out)
{
	out->n[0] = b->n[0] - a->n[0];
	out->n[1] = b->n[1] - a->n[1];
	out->n[2] = b->n[2] - a->n[2];

	return out;
}

real_point3d* __cdecl point_from_line3d(real_point3d const* in_point, real_vector3d const* in_vector, real scale, real_point3d* out_point)
{
	*out_point = *in_point;
	out_point->n[0] += in_vector->n[0] * scale;
	out_point->n[1] += in_vector->n[1] * scale;
	out_point->n[2] += in_vector->n[2] * scale;

	return out_point;
}

real __cdecl magnitude_squared3d(real_vector3d const* a)
{
	return real(a->n[0] * a->n[0]) + real(a->n[1] * a->n[1]) + real(a->n[2] * a->n[2]);
}

real __cdecl distance_squared3d(real_point3d const* a, real_point3d const* b)
{
	real_vector3d temp{};
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

real __cdecl magnitude3d(real_vector3d const* vector)
{
	return square_root(magnitude_squared3d(vector));
}

real __cdecl arctangent(real y, real x)
{
	return atan2f(y, x); // atan2
}

real_vector3d* __cdecl perpendicular3d(real_vector3d const* vector, real_vector3d* out_vector)
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

real __cdecl normalize3d(real_vector3d* vector)
{
	real magnitude = magnitude3d(vector);
	if (fabsf(magnitude - 0.0f) < k_real_epsilon)
		magnitude = 0.0f;
	else
		scale_vector3d(vector, 1.0f / magnitude, vector);

	return magnitude;
}

real_vector2d* __cdecl rotate_vector2d(real_vector2d const* vector, real a2, real a3, real_vector2d* out_vector)
{
	out_vector->i = (a3 * vector->i) - (a2 * vector->j);
	out_vector->j = (a2 * vector->i) + (a3 * vector->j);

	return out_vector;
}

real __cdecl magnitude_squared2d(real_vector2d const* a)
{
	return real(a->n[0] * a->n[0]) + real(a->n[1] * a->n[1]);
}

real_vector3d* __cdecl negate_vector3d(real_vector3d const* in_vector, real_vector3d* out_vector)
{
	out_vector->i = -in_vector->i;
	out_vector->j = -in_vector->j;
	out_vector->k = -in_vector->k;

	return out_vector;
}

real_vector3d* __cdecl scale_vector3d(real_vector3d const* in_vector, real scale, real_vector3d* out_vector)
{
	out_vector->i = scale * in_vector->i;
	out_vector->j = scale * in_vector->j;
	out_vector->k = scale * in_vector->k;

	return out_vector;
}

real_vector3d* __cdecl add_vectors3d(real_vector3d const* in_vector_a, real_vector3d const* in_vector_b, real_vector3d* out_vector)
{
	out_vector->i = in_vector_a->i + in_vector_b->i;
	out_vector->j = in_vector_a->j + in_vector_b->j;
	out_vector->k = in_vector_a->k + in_vector_b->k;

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

real_point2d* __cdecl point_from_line2d(real_point2d const* point, real_vector2d const* vector, real scale, real_point2d* out_point)
{
	*out_point = *point;
	out_point->x *= (vector->i * scale);
	out_point->y *= (vector->j * scale);

	return out_point;
};

real_vector2d* __cdecl vector_from_points2d(real_point2d const* point0, real_point2d const* point1, real_vector2d* out_vector)
{
	out_vector->i = point1->x - point0->x;
	out_vector->j = point1->y - point0->y;

	return out_vector;
};

// 0164F660
short const global_projection3d_mappings[3][2][3] =
{
	{
		{ _z, _y, _x },
		{ _y, _z, _x }
	},
	{
		{ _x, _z, _y },
		{ _z, _x, _y }
	},
	{
		{ _y, _x, _z },
		{ _x, _y, _z }
	}
};

// 0164F684
short const global_projection3d_inverse_mappings[3][2][3] =
{
	{
		{ _z, _y, _x },
		{ _z, _x, _y }
	},
	{
		{ _x, _z, _y },
		{ _y, _z, _x }
	},
	{
		{ _y, _x, _z },
		{ _x, _y, _z }
	}
};

real_point3d* __cdecl project_point2d(real_point2d const* point, real_plane3d const* plane, short projection, bool projection_sign, real_point3d* out_point)
{
	//return INVOKE(0x004F9830, project_point2d, point, plane, projection, projection_sign, out_point);

	short v5 = global_projection3d_mappings[projection][projection_sign][_x];
	short v6 = global_projection3d_mappings[projection][projection_sign][_y];

	real v7 = 0.0f;
	if (fabsf((plane->n.n[projection] - 0.0f)) >= k_real_epsilon)
		v7 = ((plane->d - (plane->n.n[v5] * point->n[0])) - (plane->n.n[v6] * point->n[1])) / plane->n.n[projection];

	ASSERT(projection >= _x && projection <= _z);
	out_point->n[v5] = point->n[0];
	out_point->n[v6] = point->n[1];
	out_point->n[projection] = v7;

	return out_point;
}

//.text:004F98F0 ; real_point2d* __cdecl project_point3d(real_point3d const*, short, bool, real_point2d*)
//.text:004F9930 ; void __cdecl project_polygon2d(long, real_point2d const* const, real_plane3d const*, long, bool, real_point3d* const)
//.text:004F9A20 ; void __cdecl project_polygon3d(long, real_point3d const* const, long, bool, real_point2d* const)
//.text:004F9A80 ; real_vector2d* __cdecl project_vector3d(real_vector3d const *, short, bool, real_vector2d*)

real __cdecl plane3d_distance_to_point(real_plane3d const* plane, real_point3d const* point)
{
	return ((((point->x * plane->n.i) + (point->y * plane->n.j)) + (point->z * plane->n.k)) - plane->d);
}

long __cdecl rectangle3d_build_vertices(real_rectangle3d const* bounds, long maximum_vertex_count, real_point3d* const vertices)
{
	ASSERT(bounds);
	ASSERT(maximum_vertex_count >= k_vertices_per_cube_count);
	ASSERT(vertices);

	vertices[0].x = bounds->x0;
	vertices[0].y = bounds->y0;
	vertices[0].z = bounds->z0;
	vertices[1].x = bounds->x1;
	vertices[1].y = bounds->y0;
	vertices[1].z = bounds->z0;
	vertices[2].x = bounds->x0;
	vertices[2].y = bounds->y1;
	vertices[2].z = bounds->z0;
	vertices[3].x = bounds->x1;
	vertices[3].y = bounds->y1;
	vertices[3].z = bounds->z0;
	vertices[4].x = bounds->x0;
	vertices[4].y = bounds->y0;
	vertices[4].z = bounds->z1;
	vertices[5].x = bounds->x1;
	vertices[5].y = bounds->y0;
	vertices[5].z = bounds->z1;
	vertices[6].x = bounds->x0;
	vertices[6].y = bounds->y1;
	vertices[6].z = bounds->z1;
	vertices[7].x = bounds->x1;
	vertices[7].y = bounds->y1;
	vertices[7].z = bounds->z1;

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
	rect->x0 = x_lower;
	rect->x1 = x_upper;
	rect->y0 = y_lower;
	rect->y1 = y_upper;

	return rect;
}

real_rectangle3d* __cdecl set_real_rectangle3d(real_rectangle3d* rect, real x_lower, real x_upper, real y_lower, real y_upper, real z_lower, real z_upper)
{
	rect->x0 = x_lower;
	rect->x1 = x_upper;
	rect->y0 = y_lower;
	rect->y1 = y_upper;
	rect->z0 = z_lower;
	rect->z1 = z_upper;

	return rect;
}

void __cdecl quaternion_transform_point(real_quaternion const* in_quaternion, real_point3d const* in_point, real_point3d* point)
{
	INVOKE(0x004FAAF0, quaternion_transform_point, in_quaternion, in_point, point);
}

real_rectangle3d* __cdecl real_rectangle3d_enclose_point(real_rectangle3d* enclosed_rect, real_point3d const* point)
{
	//return INVOKE(0x004FBD80, real_rectangle3d_enclose_point, enclosed_rect, point);

	if (enclosed_rect->x0 > point->x)
		enclosed_rect->x0 = point->x;

	if (enclosed_rect->x1 < point->x)
		enclosed_rect->x1 = point->x;

	if (enclosed_rect->y0 > point->y)
		enclosed_rect->y0 = point->y;

	if (enclosed_rect->y1 < point->y)
		enclosed_rect->y1 = point->y;

	if (enclosed_rect->z0 > point->z)
		enclosed_rect->z0 = point->z;

	if (enclosed_rect->z1 < point->z)
		enclosed_rect->z1 = point->z;

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

	real x_lower = enclosed_rect->x0;
	if (x_lower > rect->x0)
		x_lower = rect->x0;
	enclosed_rect->x0 = x_lower;

	real x_upper = rect->x1;
	if (enclosed_rect->x1 > x_upper)
		x_upper = enclosed_rect->x1;
	enclosed_rect->x1 = x_upper;

	real y_lower = rect->y0;
	if (enclosed_rect->y0 <= y_lower)
		y_lower = enclosed_rect->y0;
	enclosed_rect->y0 = y_lower;

	real y_upper = rect->y1;
	if (enclosed_rect->y1 > y_upper)
		y_upper = enclosed_rect->y1;
	enclosed_rect->y1 = y_upper;

	real z_lower = rect->z0;
	if (enclosed_rect->z0 <= z_lower)
		z_lower = enclosed_rect->z0;
	enclosed_rect->z0 = z_lower;

	real z_upper = rect->z1;
	if (enclosed_rect->z1 > z_upper)
		z_upper = enclosed_rect->z1;
	enclosed_rect->z1 = z_upper;

	return enclosed_rect;
}

real_vector3d* __cdecl vector3d_from_angle(real_vector3d* vector, real angle)
{
	return INVOKE(0x004FF020, vector3d_from_angle, vector, angle);

	//vector->i = cosf(angle);
	//vector->j = sinf(angle);
	//vector->k = 0.0f;
	//return vector;
}

real_vector3d* __cdecl vector3d_from_euler_angles2d(real_vector3d* vector, real_euler_angles2d const* angles)
{
	return INVOKE(0x004FF060, vector3d_from_euler_angles2d, vector, angles);

	//vector->i = cosf(angles->yaw) * cosf(angles->pitch);
	//vector->j = sinf(angles->yaw) * cosf(angles->pitch);
	//vector->k = sinf(angles->pitch);
	//return vector;
}

real_vector2d* __cdecl set_real_vector2d(real_vector2d* vector, real i, real j)
{
	vector->i = i;
	vector->j = j;

	return vector;
}

real_vector3d* __cdecl set_real_vector3d(real_vector3d* vector, real i, real j, real k)
{
	vector->i = i;
	vector->j = j;
	vector->k = k;

	return vector;
}

real_vector4d* __cdecl set_real_vector4d(real_vector4d* result, real i, real j, real k, real l)
{
	result->i = i;
	result->j = j;
	result->k = k;
	result->l = l;

	return result;
}

bool point_intersects_rectangle2d(real_point2d const* point, real_rectangle2d const* rect)
{
	return point->x >= rect->x0 && rect->x1 >= point->x && point->y >= rect->y0 && rect->y1 >= point->y;
}

bool point_in_sphere(real_point3d const* point, real_point3d const* center, real radius)
{
	return radius * radius > distance_squared3d(point, center);
}

