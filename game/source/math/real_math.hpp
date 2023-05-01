#pragma once

#define PI 3.14159265359
#define TWO_PI PI * 2
#define RAD 180 / PI
#define DEG PI / 180

typedef float real;
static_assert(sizeof(real) == 0x4);

typedef float real_fraction;
static_assert(sizeof(real_fraction) == 0x4);

union real_bounds
{
	struct
	{
		real lower;
		real upper;
	};
	real n[2];
};
static_assert(sizeof(real_bounds) == 0x8);

union real_point2d
{
	struct
	{
		real x;
		real y;
	};
	real n[2];
};
static_assert(sizeof(real_point2d) == 0x8);

union real_point3d
{
	struct
	{
		real x;
		real y;
		real z;
	};
	real n[3];
};
static_assert(sizeof(real_point3d) == 0xC);

union vector2d
{
	struct
	{
		real i;
		real j;
	};
	real n[2];
};
static_assert(sizeof(vector2d) == 0x8);

union vector3d
{
	struct
	{
		real i;
		real j;
		real k;
	};
	real n[3];
};
static_assert(sizeof(vector3d) == 0xC);

struct real32_quaternion
{
	real i;
	real j;
	real k;
	real w;
};
static_assert(sizeof(real32_quaternion) == 0x10);

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
static_assert(sizeof(real_rgb_color) == 0xC);

union real_argb_color
{
	struct
	{
		real alpha;
		real red;
		real green;
		real blue;
	};
	real n[4];
};
static_assert(sizeof(real_argb_color) == 0x10);

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
static_assert(sizeof(real_hsv_color) == 0xC);

union real_ahsv_color
{
	struct
	{
		real alpha;
		real hue;
		real saturation;
		real value;
	};
	real n[4];
};
static_assert(sizeof(real_ahsv_color) == 0x10);

typedef float angle;
static_assert(sizeof(angle) == 0x4);

union angle_bounds
{
	struct
	{
		angle lower;
		angle upper;
	};
	angle n[2];
};
static_assert(sizeof(angle_bounds) == 0x8);

union euler_angles2d
{
	struct
	{
		angle yaw;
		angle pitch;
	};
	angle n[2];
};
static_assert(sizeof(euler_angles2d) == 0x8);

union euler_angles3d
{
	struct
	{
		angle yaw;
		angle pitch;
		angle roll;
	};
	angle n[3];
};
static_assert(sizeof(euler_angles3d) == 0xC);

struct matrix3x3
{
	vector3d forward;
	vector3d left;
	vector3d up;
};
static_assert(sizeof(matrix3x3) == 0x24);

struct real_matrix4x3
{
	real scale;
	vector3d forward;
	vector3d left;
	vector3d up;
	vector3d center;
};
static_assert(sizeof(real_matrix4x3) == 0x34);

union real_rectangle2d
{
	struct
	{
		real top;
		real left;
		real bottom;
		real right;
	};
	real n[4];
};
static_assert(sizeof(real_rectangle2d) == 0x10);

union real_rectangle3d
{
	struct
	{
		real_bounds x;
		real_bounds y;
		real_bounds z;
	};
	real_bounds n[3];
};
static_assert(sizeof(real_rectangle3d) == 0x18);

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

