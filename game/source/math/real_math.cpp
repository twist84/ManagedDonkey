#include "math/real_math.hpp"

#include "cseries/cseries.hpp"

#include <math.h>

REFERENCE_DECLARE(0x0189CED8, const real_point2d* const, global_origin2d);
REFERENCE_DECLARE(0x0189CEDC, const real_point2d* const, global_x_axis2d);
REFERENCE_DECLARE(0x0189CEE0, const real_point2d* const, global_y_axis2d);
REFERENCE_DECLARE(0x0189CEE4, const real_point2d* const, global_negative_x_axis2d);
REFERENCE_DECLARE(0x0189CEE8, const real_point2d* const, global_negative_y_axis2d);
REFERENCE_DECLARE(0x0189CEEC, const real_vector2d* const, global_zero_vector2d);
REFERENCE_DECLARE(0x0189CEF0, const real_vector2d* const, global_forward2d);
REFERENCE_DECLARE(0x0189CEF4, const real_vector2d* const, global_left2d);
REFERENCE_DECLARE(0x0189CEF8, const real_vector2d* const, global_backward2d);
REFERENCE_DECLARE(0x0189CEFC, const real_vector2d* const, global_right2d);
REFERENCE_DECLARE(0x0189CF00, const real_point3d* const, global_origin3d);
REFERENCE_DECLARE(0x0189CF04, const real_vector3d* const, global_x_axis3d);
REFERENCE_DECLARE(0x0189CF08, const real_vector3d* const, global_y_axis3d);
REFERENCE_DECLARE(0x0189CF0C, const real_vector3d* const, global_z_axis3d);
REFERENCE_DECLARE(0x0189CF10, const real_vector3d* const, global_negative_x_axis3d);
REFERENCE_DECLARE(0x0189CF14, const real_vector3d* const, global_negative_y_axis3d);
REFERENCE_DECLARE(0x0189CF18, const real_vector3d* const, global_negative_z_axis3d);
REFERENCE_DECLARE(0x0189CF1C, const real_vector3d* const, global_zero_vector3d);
REFERENCE_DECLARE(0x0189CF20, const real_vector3d* const, global_forward3d);
REFERENCE_DECLARE(0x0189CF24, const real_vector3d* const, global_left3d);
REFERENCE_DECLARE(0x0189CF28, const real_vector3d* const, global_up3d);
REFERENCE_DECLARE(0x0189CF2C, const real_vector3d* const, global_backward3d);
REFERENCE_DECLARE(0x0189CF30, const real_vector3d* const, global_right3d);
REFERENCE_DECLARE(0x0189CF34, const real_vector3d* const, global_down3d);
REFERENCE_DECLARE(0x0189CF38, const real_euler_angles2d* const, global_zero_angles2d);
REFERENCE_DECLARE(0x0189CF3C, const real_euler_angles3d* const, global_zero_angles3d);
REFERENCE_DECLARE(0x0189CF40, const real_vector4d* const, global_zero_vector4d);
REFERENCE_DECLARE(0x0189CF44, const real_quaternion* const, global_identity_quaternion);
REFERENCE_DECLARE(0x0189CF48, const real_matrix3x3* const, global_identity3x3);
REFERENCE_DECLARE(0x0189CF4C, const real_matrix4x3* const, global_identity4x3);
REFERENCE_DECLARE(0x0189CF50, const real_matrix4x3* const, global_negative_identity4x3);
REFERENCE_DECLARE(0x0189CF54, const real_orientation* const, global_identity_orientation);
REFERENCE_DECLARE(0x0189CF58, const real_rectangle2d* const, global_null_rectangle2d);
REFERENCE_DECLARE(0x0189CF5C, const real_rectangle3d* const, global_null_rectangle3d);
REFERENCE_DECLARE(0x0189CF60, const real_rectangle2d* const, global_zero_rectangle2d);
REFERENCE_DECLARE(0x0189CF64, const real_rectangle3d* const, global_zero_rectangle3d);

// 0164F660
int16 const global_projection3d_mappings[3][2][3] =
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
int16 const global_projection3d_inverse_mappings[3][2][3] =
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

real32 __cdecl sine(real32 angle)
{
	return (real32)sin(angle);
}

real32 __cdecl cosine(real32 angle)
{
	return (real32)cos(angle);
}

real32 __cdecl arctangent(real32 y, real32 x)
{
	return (real32)atan2(y, x);
}

real32 __cdecl arccosine(real32 x)
{
	return (real32)acos(x);
}

real32 __cdecl square_root(real32 x)
{
	return (real32)sqrt(x);
}

real32 __cdecl reciprocal_square_root(real32 x)
{
	return 1.0f / square_root(x);
}

real32 __cdecl real_max(real32 a, real32 b)
{
	return (a > b) ? a : b;
}

real32 __cdecl real_min(real32 a, real32 b)
{
	return (a < b) ? a : b;
}

real32 __cdecl real_pin(real32 value, real32 min_value, real32 max_value)
{
	return real_min(real_max(value, min_value), max_value);
}

void __cdecl interpolate_scalar(real32* current, real32 desired, real32 maximum_speed)
{
	*current += real_pin(desired - *current, -maximum_speed, maximum_speed);
}

bool __cdecl valid_real_vector2d(const real_vector2d* v)
{
	return valid_real(v->i)
		&& valid_real(v->j);
}

bool __cdecl valid_real_vector4d(const real_vector4d* v)
{
	return valid_real(v->i)
		&& valid_real(v->j)
		&& valid_real(v->k)
		&& valid_real(v->l);
}

bool __cdecl valid_real_sine_cosine(real32 sine, real32 cosine)
{
	return valid_realcmp(((sine * sine) + (cosine * cosine)), 1.0f);
}

bool __cdecl valid_real_quaternion(const real_quaternion* quaternion)
{
	return valid_realcmp((magnitude_squared3d(&quaternion->v) + (quaternion->w * quaternion->w)), 1.0f);
}

bool __cdecl valid_real_normal2d(const real_vector2d* normal)
{
	return valid_realcmp(magnitude_squared2d(normal), 1.0f);
}

bool __cdecl valid_real_plane2d(const real_plane2d* plane)
{
	return valid_real_normal2d(&plane->n)
		&& valid_real(plane->d);
}

bool __cdecl valid_real_plane3d(const real_plane3d* plane)
{
	return valid_real_normal3d(&plane->n)
		&& valid_real(plane->d);
}

bool __cdecl valid_real_vector3d_right_handed_axes3(const real_vector3d* forward, const real_vector3d* left, const real_vector3d* up)
{
	return valid_real_vector3d_axes3(forward, left, up)
		&& triple_product3d(forward, left, up) > 0.0f;
}

bool __cdecl valid_real_matrix4x3(const real_matrix4x3* matrix)
{
	return valid_real(matrix->scale)
		&& valid_real_vector3d_axes3(&matrix->forward, &matrix->left, &matrix->up)
		&& valid_real_point3d(&matrix->position);
}

bool __cdecl valid_real_euler_angles3d(const real_euler_angles3d* angles)
{
	return valid_real(angles->yaw)
		&& valid_real(angles->pitch)
		&& valid_real(angles->roll)
		&& IN_RANGE_INCLUSIVE(angles->yaw, 0.0f, k_full_circle)
		&& IN_RANGE_INCLUSIVE(angles->pitch, -k_quarter_circle, k_quarter_circle)
		&& IN_RANGE_INCLUSIVE(angles->roll, -k_half_circle, k_half_circle);
}

// networking/network_configuration.cpp
real32 __cdecl interpolate_linear(real32 start_value, real32 end_value, real32 interpolation_factor)
{
	return start_value + ((end_value - start_value) * interpolation_factor);
}

bool __cdecl valid_real(const real32& value)
{
	//return INVOKE(0x004AB2A0, valid_real, value);

	return ((uns32)value & 0x7F800000) != 0x7F800000;
}

bool __cdecl valid_real_euler_angles2d(const real_euler_angles2d* angles)
{
	//return INVOKE(0x004AB2C0, valid_real_euler_angles2d, angles);

	return valid_real(angles->yaw)
		&& valid_real(angles->pitch)
		&& IN_RANGE_INCLUSIVE(angles->yaw, 0.0f, k_full_circle)
		&& IN_RANGE_INCLUSIVE(angles->pitch, -k_quarter_circle, k_quarter_circle);
}

bool __cdecl valid_real_normal3d(const real_vector3d* normal)
{
	//return INVOKE(0x004AB330, valid_real_normal3d, normal);

	return valid_realcmp(magnitude_squared3d(normal), 1.0f);
}

bool __cdecl valid_real_point3d(const real_point3d* point)
{
	//return INVOKE(0x004AB3A0, valid_real_point3d, point);

	return valid_real(point->x)
		&& valid_real(point->y)
		&& valid_real(point->z);
}

bool __cdecl valid_real_point2d(const real_point2d* point)
{
	//return INVOKE(0x004AB3E0, valid_real_point2d, point);

	return valid_real(point->x)
		&& valid_real(point->y);
}

bool __cdecl valid_real_vector3d(const real_vector3d* vector)
{
	//return INVOKE(0x004AB410, valid_real_vector3d, vector);

	return valid_real(vector->i)
		&& valid_real(vector->j)
		&& valid_real(vector->k);
}

bool __cdecl valid_real_vector3d_axes2(const real_vector3d* forward, const real_vector3d* up)
{
	//return INVOKE(0x004AB450, valid_real_vector3d_axes2, forward, up);

	return valid_real_normal3d(forward)
		&& valid_real_normal3d(up)
		&& valid_realcmp(dot_product3d(forward, up), 0.0f);
}

bool __cdecl valid_realcmp(real32 a, real32 b)
{
	//return INVOKE(0x004AB570, valid_realcmp, a, b);

	real32 diff = a - b;
	return valid_real(diff)
		&& fabs(diff) < k_test_real_epsilon;
}

real32 __cdecl angle_between_vectors3d(const real_vector3d* a, const real_vector3d* b)
{
	return INVOKE(0x004EEC40, angle_between_vectors3d, a, b);
}

real_euler_angles2d* __cdecl euler_angles2d_from_vector3d(real_euler_angles2d* facing, const real_vector3d* forward)
{
	return INVOKE(0x004F13E0, euler_angles2d_from_vector3d, facing, forward);
}

real_vector3d* __cdecl generate_up_vector3d(const real_vector3d* forward, real_vector3d* up)
{
	return INVOKE(0x004F2310, generate_up_vector3d, forward, up);
}

real_point3d* __cdecl project_point2d(const real_point2d* p2d, const real_plane3d* plane, int16 projection, bool sign, real_point3d* p3d)
{
	//return INVOKE(0x004F9830, project_point2d, p2d, plane, projection, sign, p3d);

	int16 v5 = global_projection3d_mappings[projection][sign][_x];
	int16 v6 = global_projection3d_mappings[projection][sign][_y];

	real32 v7 = 0.0f;
	if (fabsf((plane->n.n[projection] - 0.0f)) >= k_real_epsilon)
		v7 = ((plane->d - (plane->n.n[v5] * p2d->n[0])) - (plane->n.n[v6] * p2d->n[1])) / plane->n.n[projection];

	ASSERT(projection >= _x && projection <= _z);
	p3d->n[v5] = p2d->n[0];
	p3d->n[v6] = p2d->n[1];
	p3d->n[projection] = v7;

	return p3d;
}

//.text:004F98F0 ; real_point2d* __cdecl project_point3d(const real_point3d*, int16, bool, real_point2d*)
//.text:004F9930 ; void __cdecl project_polygon2d(int32, const real_point2d* const, const real_plane3d*, int32, bool, real_point3d* const)
//.text:004F9A20 ; void __cdecl project_polygon3d(int32, const real_point3d* const, int32, bool, real_point2d* const)
//.text:004F9A80 ; real_vector2d* __cdecl project_vector3d(const real_vector3d*, int16, bool, real_vector2d*)

void __cdecl quaternion_transform_point(const real_quaternion* q, const real_point3d* p, real_point3d* result)
{
	INVOKE(0x004FAAF0, quaternion_transform_point, q, p, result);
}

void __cdecl real_math_dispose()
{
	INVOKE(0x004FB7D0, real_math_dispose);

	//periodic_functions_dispose();
}

//.text:004FB7E0 ; void __cdecl real_math_enable_floating_point_exceptions(bool)

void __cdecl real_math_initialize()
{
	INVOKE(0x004FB7F0, real_math_initialize);

	//real_math_reset_precision();
	//periodic_functions_initialize();
}

//.text:004FB820 ; void __cdecl real_math_recover_fpu_exceptions()

void __cdecl real_math_reset_precision()
{
	INVOKE(0x004FB850, real_math_reset_precision);
}

//.text:004FB880
//.text:004FB890
//.text:004FB920
//.text:004FB960 ; real32 __cdecl real_rectangle2d_area(const real_rectangle2d*)

void __cdecl real_rectangle2d_clamp_bounds(real_rectangle2d* clampee, const real_rectangle2d* clamper)
{
	INVOKE(0x004FB990, real_rectangle2d_clamp_bounds, clampee, clamper);
}

bool __cdecl real_rectangle2d_compute_intersection(const real_rectangle2d* a, const real_rectangle2d* b, real_rectangle2d* result)
{
	return INVOKE(0x004FB9E0, real_rectangle2d_compute_intersection, a, b, result);
}

//.text:004FBAA0 ; real_rectangle2d* __cdecl real_rectangle2d_enclose_point(real_rectangle2d*, const real_point2d*)
//.text:004FBB30 ; real_rectangle2d* __cdecl real_rectangle2d_enclose_points(real_rectangle2d*, int32, const real_point2d* const)
//.text:004FBBE0 ; real_rectangle2d* __cdecl real_rectangle2d_enclose_rectangle(real_rectangle2d*, const real_rectangle2d*)
//.text:004FBC70 ; bool __cdecl real_rectangle3d_compute_intersection(const real_rectangle3d*, const real_rectangle3d*, real_rectangle3d*)

real_rectangle3d* __cdecl real_rectangle3d_enclose_point(real_rectangle3d* bounds, const real_point3d* point)
{
	return INVOKE(0x004FBD80, real_rectangle3d_enclose_point, bounds, point);

	if (bounds->x0 > point->x)
	{
		bounds->x0 = point->x;
	}

	if (bounds->x1 < point->x)
	{
		bounds->x1 = point->x;
	}

	if (bounds->y0 > point->y)
	{
		bounds->y0 = point->y;
	}

	if (bounds->y1 < point->y)
	{
		bounds->y1 = point->y;
	}

	if (bounds->z0 > point->z)
	{
		bounds->z0 = point->z;
	}

	if (bounds->z1 < point->z)
	{
		bounds->z1 = point->z;
	}

	return bounds;
}

real_rectangle3d* __cdecl real_rectangle3d_enclose_points(real_rectangle3d* bounds, int32 point_count, const real_point3d* points)
{
	return INVOKE(0x004FBE40, real_rectangle3d_enclose_points, bounds, point_count, points);

	//ASSERT(valid_polygon3d(point_count, points));

	for (int32 i = 0; i < point_count; i++)
	{
		real_rectangle3d_enclose_point(bounds, &points[i]);
	}

	return bounds;
}

real_rectangle3d* __cdecl real_rectangle3d_enclose_rectangle(real_rectangle3d* bounds, const real_rectangle3d* rectangle)
{
	return INVOKE(0x004FBF30, real_rectangle3d_enclose_rectangle, bounds, rectangle);

	real32 x_lower = bounds->x0;
	if (x_lower > rectangle->x0)
	{
		x_lower = rectangle->x0;
	}
	bounds->x0 = x_lower;

	real32 x_upper = rectangle->x1;
	if (bounds->x1 > x_upper)
	{
		x_upper = bounds->x1;
	}
	bounds->x1 = x_upper;

	real32 y_lower = rectangle->y0;
	if (bounds->y0 <= y_lower)
	{
		y_lower = bounds->y0;
	}
	bounds->y0 = y_lower;

	real32 y_upper = rectangle->y1;
	if (bounds->y1 > y_upper)
	{
		y_upper = bounds->y1;
	}
	bounds->y1 = y_upper;

	real32 z_lower = rectangle->z0;
	if (bounds->z0 <= z_lower)
	{
		z_lower = bounds->z0;
	}
	bounds->z0 = z_lower;

	real32 z_upper = rectangle->z1;
	if (bounds->z1 > z_upper)
	{
		z_upper = bounds->z1;
	}
	bounds->z1 = z_upper;

	return bounds;
}

//.text:004FBFF0 ; real32 __cdecl real_rectangle3d_volume(const real_rectangle3d*)
//.text:004FC030 ; void __cdecl real_vector3d_build_axes_from_forward(const real_vector3d*, real_vector3d*, real_vector3d*)
//.text:004FC060 ; void __cdecl real_vector3d_build_axes_from_forward_and_left(const real_vector3d*, const real_vector3d*, real_vector3d*, real_vector3d*, real_vector3d*)
//.text:004FC610 ; void __cdecl real_vector3d_build_axes_from_vectors(const real_vector3d*, const real_vector3d*, real_vector3d*, real_vector3d*, real_vector3d*)
//.text:004FCA60 ; void __cdecl real_vector3d_build_axes_from_vectors_using_up(const real_vector3d*, const real_vector3d*, real_vector3d*, real_vector3d*, real_vector3d*)
//.text:004FCEB0 ; 

int32 __cdecl rectangle3d_build_edges(const real_rectangle3d* bounds, int32 maximum_edge_count, real_point3d(* const edges)[2])
{
	return INVOKE(0x004FCF10, rectangle3d_build_edges, bounds, maximum_edge_count, edges);

	ASSERT(bounds);
	ASSERT(maximum_edge_count >= k_edges_per_cube_count);
	ASSERT(edges);

	int32 line_vertex_indices[k_edges_per_cube_count][2]
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

	for (int32 edge_index = 0; edge_index < k_edges_per_cube_count; edge_index++)
	{
		ASSERT((line_vertex_indices[edge_index][0] >= 0) && (line_vertex_indices[edge_index][0] < k_vertices_per_cube_count));
		ASSERT((line_vertex_indices[edge_index][1] >= 0) && (line_vertex_indices[edge_index][1] < k_vertices_per_cube_count));

		edges[edge_index][0] = vertices[line_vertex_indices[edge_index][0]];
		edges[edge_index][1] = vertices[line_vertex_indices[edge_index][1]];
	}

	return k_edges_per_cube_count;
}

int32 __cdecl rectangle3d_build_faces(const real_rectangle3d* bounds, int32 maximum_face_count, real_point3d(* const faces)[4])
{
	return INVOKE(0x004FD030, rectangle3d_build_faces, bounds, maximum_face_count, faces);

	ASSERT(bounds);
	ASSERT(maximum_face_count >= k_faces_per_cube_count);
	ASSERT(faces);

	int32 face_vertex_indices[k_faces_per_cube_count][4]
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

	for (int32 face_index = 0; face_index < k_faces_per_cube_count; face_index++)
	{
		for (int32 vertex_index = 0; vertex_index < k_vertices_per_cube_count; vertex_index++)
		{
			ASSERT((face_vertex_indices[face_index][vertex_index] >= 0) && (face_vertex_indices[face_index][vertex_index] < k_vertices_per_cube_count));

			*faces[face_vertex_indices[face_index][vertex_index]] = vertices[face_vertex_indices[face_index][vertex_index]];
		}
	}

	return k_faces_per_cube_count;
}

int32 __cdecl rectangle3d_build_vertices(const real_rectangle3d* bounds, int32 maximum_vertex_count, real_point3d* const vertices)
{
	return INVOKE(0x004FD1A0, rectangle3d_build_vertices, bounds, maximum_vertex_count, vertices);

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

//.text:004FD240 ; void __cdecl rectangle3d_compute_nearest_point(const real_rectangle3d*, const real_point3d*, real_point3d*)
//.text:004FD2D0 ; real_vector3d* __cdecl reflect_vector3d(const real_vector3d*, const real_vector3d*, real_vector3d*)
//.text:004FD350 ; 
//.text:004FD560 ; real_vector3d* __cdecl rotate_vector_about_axis(real_vector3d*, const real_vector3d*, real32, real32)
//.text:004FD660 ; 
//.text:004FD6F0 ; void __cdecl scalars_interpolate(real32, real32, real32, real32*)
//.text:004FD720 ; 
//.text:004FD780 ; 
//.text:004FD7B0 ; 
//.text:004FD7D0 ; 
//.text:004FD800 ; 
//.text:004FD830 ; 
//.text:004FD850 ; 
//.text:004FD880 ; real32 __cdecl signed_angle_between_normals3d(const real_vector3d*, const real_vector3d*, const real_vector3d*)
//.text:004FD9C0 ; 
//.text:004FDB20 ; real32 __cdecl signed_angle_between_vectors2d(const real_vector2d*, const real_vector2d*)
//.text:004FDBF0 ; 
//.text:004FDC10 ; 
//.text:004FDC30 ; bool __cdecl sphere_intersects_cone3d(const real_point3d*, real32, const real_point3d*, const real_vector3d*, real32, real32, real32)
//.text:004FDCF0 ; bool __cdecl sphere_intersects_rectangle3d(const real_point3d*, real32, const real_rectangle3d*)
//.text:004FDDA0 ; bool __cdecl sphere_intersects_sector3d(const real_point3d*, real32, const real_point3d*, const real_vector3d*, real32, real32, real32)
//.text:004FDEE0 ; bool __cdecl sphere_intersects_triangle3d(const real_point3d*, real32, const real_point3d*, const real_point3d*, const real_point3d*)
//.text:004FE4D0 ;
//.text:004FE710 ; 
//.text:004FE740 ; 

real_vector3d* __cdecl vector3d_from_angle(real_vector3d* vector, real32 angle)
{
	return INVOKE(0x004FF020, vector3d_from_angle, vector, angle);

	//vector->i = cosine(angle);
	//vector->j = sine(angle);
	//vector->k = 0.0f;
	//return vector;
}

real_vector3d* __cdecl vector3d_from_euler_angles2d(real_vector3d* vector, const real_euler_angles2d* angles)
{
	return INVOKE(0x004FF060, vector3d_from_euler_angles2d, vector, angles);

	//vector->i = cosine(angles->yaw) * cosine(angles->pitch);
	//vector->j = sine(angles->yaw) * cosine(angles->pitch);
	//vector->k = sine(angles->pitch);
	//return vector;
}

//.text:004FF0D0 ; 
//.text:004FF100 ;
//.text:004FF170 ; bool __cdecl vector_intersects_pill2d(const real_point2d*, const real_vector2d*, const real_point2d*, const real_vector2d*, real32)
//.text:004FF5B0 ; bool __cdecl vector_intersects_pill3d(const real_point3d*, const real_vector3d*, const real_point3d*, const real_vector3d*, real32)
//.text:004FFD80 ; bool __cdecl vector_intersects_rectangle2d(const real_point2d*, const real_vector2d*, const real_rectangle2d*)
//.text:004FFEF0 ; 
//.text:00500120 ; 
//.text:00500160 ; 
//.text:005003A0 ; 
//.text:005005F0 ; real32 __cdecl vector_to_line_distance_squared3d(const real_point3d*, const real_vector3d*, const real_point3d*, const real_vector3d*)
//.text:00500FF0 ; void __cdecl vectors3d_from_euler_angles2d(real_vector3d*, real_vector3d*, const real_euler_angles2d*)
//.text:00501040 ; void __cdecl vectors3d_from_euler_angles3d(real_vector3d*, real_vector3d*, const real_euler_angles3d*)
//.text:00501090 ; 
//.text:00501100 ; void __cdecl yaw_vectors(real_vector3d*, const real_vector3d*, real32, real32)
//.text:005011A0 ; 
//.text:005011C0 ; 
//.text:005011E0 ; 
//.text:00501230 ; 
//.text:00501250 ; 
//.text:00501270 ; 
//.text:005012B0 ; 
//.text:00501300 ; 
//.text:00501340 ; 

//.text:00523B20 ; 
//.text:00523B70 ; 

bool __cdecl valid_real_vector3d_axes3(const real_vector3d* forward, const real_vector3d* left, const real_vector3d* up)
{
	//return INVOKE(0x005B3D30, valid_real_vector3d_axes3, forward, left, up);

	return valid_real_normal3d(forward)
		&& valid_real_normal3d(left)
		&& valid_real_normal3d(up)
		&& valid_realcmp(dot_product3d(forward, left), 0.0f)
		&& valid_realcmp(dot_product3d(left, up), 0.0f)
		&& valid_realcmp(dot_product3d(up, forward), 0.0f);
}

bool __cdecl valid_polygon2d(int32 point_count, const real_point2d* const points)
{
	//return INVOKE(0x0078AF90, valid_polygon2d, point_count, points);

	ASSERT(point_count >= 0);
	ASSERT(point_count == 0 || points);

	for (int32 i = 0; i < point_count; i++)
	{
		if (!valid_real_point2d(&points[i]))
			return false;
	}

	return true;
}

//.text:0078B440 ; 
//.text:0078BF90 ; 
//.text:00C35C30 ; 

real_vector3d* __cdecl add_vectors3d(const real_vector3d* a, const real_vector3d* b, real_vector3d* result)
{
	result->i = a->i + b->i;
	result->j = a->j + b->j;
	result->k = a->k + b->k;

	return result;
}

real_vector3d* __cdecl cross_product3d(const real_vector3d* a, const real_vector3d* b, real_vector3d* result)
{
	result->i = (a->j * b->k) - (a->k * b->j);
	result->j = (a->k * b->i) - (a->i * b->k);
	result->k = (a->i * b->j) - (a->j * b->i);

	return result;
}

real32 __cdecl distance_squared2d(const real_point2d* a, const real_point2d* b)
{
	real_vector2d temp{};
	return magnitude_squared2d(vector_from_points2d(a, b, &temp));
}

real32 __cdecl distance_squared3d(const real_point3d* a, const real_point3d* b)
{
	real_vector3d temp{};
	return magnitude_squared3d(vector_from_points3d(a, b, &temp));
}

real32 __cdecl distance2d(const real_point2d* a, const real_point2d* b)
{
	return square_root(distance_squared2d(a, b));
}

real32 __cdecl distance3d(const real_point3d* a, const real_point3d* b)
{
	return square_root(distance_squared3d(a, b));
}

real32 __cdecl dot_product2d(const real_vector2d* a, const real_vector2d* b)
{
	return (a->i * b->i) + (a->j * b->j);
}

real32 __cdecl dot_product3d(const real_vector3d* a, const real_vector3d* b)
{
	return ((a->i * b->i) + (a->j * b->j)) + (a->k * b->k);
}

real32 __cdecl dot_product4d(const real_vector4d* a, const real_vector4d* b)
{
	return (((a->i * b->i) + (a->j * b->j)) + (a->k * b->k)) + (a->l * b->l);
}

real32 __cdecl magnitude_squared2d(const real_vector2d* v)
{
	return (v->i * v->i) + (v->j * v->j);
}

real32 __cdecl magnitude_squared3d(const real_vector3d* v)
{
	return ((v->i * v->i) + (v->j * v->j)) + (v->k * v->k);
}

real32 __cdecl magnitude2d(const real_vector2d* v)
{
	return square_root(magnitude_squared2d(v));
}

real32 __cdecl magnitude3d(const real_vector3d* v)
{
	return square_root(magnitude_squared3d(v));
}

real_vector3d* __cdecl negate_vector3d(const real_vector3d* a, real_vector3d* result)
{
	result->i = -a->i;
	result->j = -a->j;
	result->k = -a->k;

	return result;
}

real32 __cdecl normalize2d(real_vector2d* v)
{
	real32 magnitude = magnitude2d(v);
	if (fabsf(magnitude - 0.0f) < k_real_epsilon)
	{
		magnitude = 0.0f;
	}
	else
	{
		scale_vector2d(v, 1.0f / magnitude, v);
	}

	return magnitude;
}

real32 __cdecl normalize3d(real_vector3d* v)
{
	real32 magnitude = magnitude3d(v);
	if (fabsf(magnitude - 0.0f) < k_real_epsilon)
	{
		magnitude = 0.0f;
	}
	else
	{
		scale_vector3d(v, 1.0f / magnitude, v);
	}

	return magnitude;
}

real_rectangle2d* __cdecl offset_real_rectangle2d(real_rectangle2d* rectangle, real32 dx, real32 dy)
{
	rectangle->x0 += dx;
	rectangle->x1 += dx;
	rectangle->y0 += dy;
	rectangle->y1 += dy;

	return rectangle;
}

real_vector2d* __cdecl perpendicular2d(const real_vector2d* a, real_vector2d* result)
{
	result->i = -a->j;
	result->j = a->i;
	return result;
}

real_vector3d* __cdecl perpendicular3d(const real_vector3d* a, real_vector3d* result)
{
	real32 i = fabsf(a->i);
	real32 j = fabsf(a->j);
	real32 k = fabsf(a->k);
	if (i > j || i > k)
	{
		if (j > k)
		{
			result->i = a->j;
			result->j = -a->i;
			result->k = 0.0f;
		}
		else
		{
			result->i = -a->k;
			result->j = 0.0f;
			result->k = a->i;
		}
	}
	else
	{
		result->i = 0.0f;
		result->j = a->k;
		result->k = -a->j;
	}

	return result;
}

real32 __cdecl plane3d_distance_to_point(const real_plane3d* plane, const real_point3d* point)
{
	return ((((point->x * plane->n.i) + (point->y * plane->n.j)) + (point->z * plane->n.k)) - plane->d);
}

real_point2d* __cdecl point_from_line2d(const real_point2d* p, const real_vector2d* v, real32 t, real_point2d* result)
{
	result->x = p->x * (v->i * t);
	result->y = p->y * (v->j * t);

	return result;
}

real_point3d* __cdecl point_from_line3d(const real_point3d* p, const real_vector3d* v, real32 t, real_point3d* result)
{
	result->x = p->x + (v->i * t);
	result->y = p->y + (v->j * t);
	result->z = p->z + (v->k * t);

	return result;
}

bool __cdecl point_in_sphere(const real_point3d* point, const real_point3d* center, real32 radius)
{
	return radius * radius > distance_squared3d(point, center);
}

bool __cdecl point_intersects_rectangle2d(const real_point2d* p, const real_rectangle2d* bounds)
{
	return p->x >= bounds->x0 && bounds->x1 >= p->x && p->y >= bounds->y0 && bounds->y1 >= p->y;
}

real_vector2d* __cdecl rotate_vector2d(const real_vector2d* v, real32 sine, real32 cosine, real_vector2d* result)
{
	result->i = (cosine * v->i) - (sine * v->j);
	result->j = (sine * v->i) + (cosine * v->j);

	return result;
}

real_vector2d* __cdecl scale_vector2d(const real_vector2d* a, real32 c, real_vector2d* result)
{
	result->i = c * a->i;
	result->j = c * a->j;

	return result;
}

real_vector3d* __cdecl scale_vector3d(const real_vector3d* a, real32 c, real_vector3d* result)
{
	result->i = c * a->i;
	result->j = c * a->j;
	result->k = c * a->k;

	return result;
}

real_point2d* __cdecl set_real_point2d(real_point2d* p, real32 x, real32 y)
{
	p->x = x;
	p->y = y;

	return p;
}

real_point3d* __cdecl set_real_point3d(real_point3d* p, real32 x, real32 y, real32 z)
{
	p->x = x;
	p->y = y;
	p->z = z;

	return p;
}

real_rectangle2d* __cdecl set_real_rectangle2d(real_rectangle2d* bounds, real32 x0, real32 x1, real32 y0, real32 y1)
{
	bounds->x0 = x0;
	bounds->x1 = x1;
	bounds->y0 = y0;
	bounds->y1 = y1;

	return bounds;
}

real_rectangle3d* __cdecl set_real_rectangle3d(real_rectangle3d* bounds, real32 x0, real32 x1, real32 y0, real32 y1, real32 z0, real32 z1)
{
	bounds->x0 = x0;
	bounds->x1 = x1;
	bounds->y0 = y0;
	bounds->y1 = y1;
	bounds->z0 = z0;
	bounds->z1 = z1;

	return bounds;
}

real_vector2d* __cdecl set_real_vector2d(real_vector2d* vector, real32 i, real32 j)
{
	vector->i = i;
	vector->j = j;

	return vector;
}

real_vector3d* __cdecl set_real_vector3d(real_vector3d* vector, real32 i, real32 j, real32 k)
{
	vector->i = i;
	vector->j = j;
	vector->k = k;

	return vector;
}

real_vector4d* __cdecl set_real_vector4d(real_vector4d* result, real32 i, real32 j, real32 k, real32 l)
{
	result->i = i;
	result->j = j;
	result->k = k;
	result->l = l;

	return result;
}

real32 __cdecl triple_product3d(const real_vector3d* a, const real_vector3d* b, const real_vector3d* n)
{
	real_vector3d product3d;
	return dot_product3d(cross_product3d(a, b, &product3d), n);
}

real_vector2d* __cdecl vector_from_points2d(const real_point2d* a, const real_point2d* b, real_vector2d* result)
{
	result->i = b->x - a->x;
	result->j = b->y - a->y;

	return result;
}

real_vector3d* __cdecl vector_from_points3d(const real_point3d* a, const real_point3d* b, real_vector3d* result)
{
	result->i = b->x - a->x;
	result->j = b->y - a->y;
	result->k = b->z - a->z;

	return result;
}

real_vector4d* __cdecl real_rgb_color_to_real_vector4d(real_rgb_color* color, real32 alpha, real_vector4d* result)
{
	return set_real_vector4d(result, color->red, color->green, color->blue, alpha);
}

