#pragma once

#define PI 3.14159265359
#define TWO_PI PI * 2
#define RAD 180 / PI
#define DEG PI / 180

typedef float real;
static_assert(sizeof(real) == sizeof(float));

typedef float real_fraction;
static_assert(sizeof(real_fraction) == sizeof(real));

enum
{
	_x = 0,
	_y,
	_z,
	_w,

	k_2d_count = 2,
	k_3d_count = 3,
	k_4d_count = 4,

	k_faces_per_cube_count = 6,
	k_vertices_per_cube_count = 8,
};

union real_bounds
{
	struct
	{
		real lower;
		real upper;
	};
	real n[2];
};
static_assert(sizeof(real_bounds) == sizeof(real) * 2);

union real_point2d
{
	struct
	{
		real x;
		real y;
	};
	real n[k_2d_count];
};
static_assert(sizeof(real_point2d) == sizeof(real) * k_2d_count);

union real_point3d
{
	struct
	{
		real x;
		real y;
		real z;
	};
	real n[k_3d_count];
};
static_assert(sizeof(real_point3d) == sizeof(real) * k_3d_count);

union real_point4d
{
	struct
	{
		real x;
		real y;
		real z;
		real w;
	};
	real n[k_4d_count];
};
static_assert(sizeof(real_point4d) == sizeof(real) * k_4d_count);

union vector2d
{
	struct
	{
		real i;
		real j;
	};
	real n[k_2d_count];
};
static_assert(sizeof(vector2d) == sizeof(real) * k_2d_count);

union vector3d
{
	struct
	{
		real i;
		real j;
		real k;
	};
	real n[k_3d_count];
};
static_assert(sizeof(vector3d) == sizeof(real) * k_3d_count);

union vector4d
{
	struct
	{
		real i;
		real j;
		real k;
		real w;
	};
	real n[k_4d_count];
};
static_assert(sizeof(vector4d) == sizeof(real) * k_4d_count);

struct real32_quaternion
{
	real i;
	real j;
	real k;
	real w;
};
static_assert(sizeof(real32_quaternion) == sizeof(real) * 4);

struct plane2d
{
	vector2d normal;
	real distance;
};
static_assert(sizeof(plane2d) == 0xC);

struct plane3d
{
	vector3d normal;
	real distance;
};
static_assert(sizeof(plane3d) == 0x10);

union real_rgb_color
{
	struct
	{
		real red;
		real green;
		real blue;
	};
	real n[3];
};
static_assert(sizeof(real_rgb_color) == sizeof(real) * 3);

union real_argb_color
{
	struct
	{
		real alpha;
		real_rgb_color color;
	};
	real n[4];
};
static_assert(sizeof(real_argb_color) == sizeof(real) * 4);

union real_hsv_color
{
	struct
	{
		real hue;
		real saturation;
		real value;
	};
	real n[3];
};
static_assert(sizeof(real_hsv_color) == sizeof(real) * 3);

union real_ahsv_color
{
	struct
	{
		real alpha;
		real_rgb_color color;
	};
	real n[4];
};
static_assert(sizeof(real_ahsv_color) == sizeof(real) * 4);

typedef float angle;
static_assert(sizeof(angle) == sizeof(float));

union angle_bounds
{
	struct
	{
		angle lower;
		angle upper;
	};
	angle n[2];
};
static_assert(sizeof(angle_bounds) == sizeof(angle) * 2);

union euler_angles2d
{
	struct
	{
		angle yaw;
		angle pitch;
	};
	angle n[k_2d_count];
};
static_assert(sizeof(euler_angles2d) == sizeof(angle) * k_2d_count);

union euler_angles3d
{
	struct
	{
		angle yaw;
		angle pitch;
		angle roll;
	};
	angle n[k_3d_count];
};
static_assert(sizeof(euler_angles3d) == sizeof(angle) * k_3d_count);

struct matrix3x3
{
	vector3d forward;
	vector3d left;
	vector3d up;
};
static_assert(sizeof(matrix3x3) == sizeof(vector3d) * 3);

struct real_matrix4x3
{
	real scale;
	matrix3x3 matrix;
	real_point3d center;
};
static_assert(sizeof(real_matrix4x3) == 0x34);

union real_rectangle2d
{
	struct
	{
		real_bounds x;
		real_bounds y;
	};
	real_bounds n[k_2d_count];
};
static_assert(sizeof(real_rectangle2d) == sizeof(real_bounds) * k_2d_count);

union real_rectangle3d
{
	struct
	{
		real_bounds x;
		real_bounds y;
		real_bounds z;
	};
	real_bounds n[k_3d_count];
};
static_assert(sizeof(real_rectangle3d) == sizeof(real_bounds) * k_3d_count);

struct real_quaternion
{
	real i;
	real j;
	real k;
	real w;
};
static_assert(sizeof(real_quaternion) == sizeof(real) * 4);

extern real __cdecl angle_between_vectors3d(vector3d const* a, vector3d const* b);
extern vector3d* __cdecl generate_up_vector3d(vector3d const* forward, vector3d* up);
extern euler_angles2d* __cdecl euler_angles2d_from_vector3d(euler_angles2d* facing, vector3d const* forward);
extern vector3d* __cdecl cross_product3d(vector3d const* a, vector3d const* b, vector3d* out);
extern real __cdecl dot_product3d(vector3d const* a, vector3d const* b);
extern vector3d* __cdecl vector_from_points3d(real_point3d const* a, real_point3d const* b, vector3d* out);
extern real __cdecl magnitude_squared3d(vector3d const* a);
extern real __cdecl distance_squared3d(real_point3d const* a, real_point3d const* b);
extern real __cdecl distance3d(real_point3d const* a, real_point3d const* b);
extern real __cdecl square_root(real value);
extern real __cdecl magnitude3d(vector3d const* vector);
extern vector3d* __cdecl perpendicular3d(vector3d const* vector, vector3d* out_vector);
extern real __cdecl normalize3d(vector3d* vector);
extern vector2d* __cdecl rotate_vector2d(vector2d const* vector, real a2, real a3, vector2d* out_vector);
extern vector3d* __cdecl scale_vector3d(vector3d const* in_vector, real scale, vector3d* out_vector);
extern real_point2d* __cdecl set_real_point2d(real_point2d* point, real x, real y);
extern real_point3d* __cdecl set_real_point3d(real_point3d* point, real x, real y, real z);
extern real_point2d* __cdecl point_from_line2d(real_point2d const* point, vector2d const* vector, real scale, real_point2d* out_point);
extern vector2d* __cdecl vector_from_points2d(real_point2d const* point0, real_point2d const* point1, vector2d* out_vector);
extern real_point3d* __cdecl project_point2d(real_point2d const* point, plane3d const* plane, short projection, bool a4, real_point3d* out_point);
