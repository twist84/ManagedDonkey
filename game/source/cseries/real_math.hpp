#pragma once

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

union real_vector2d
{
	struct
	{
		real i;
		real j;
	};
	real n[2];
};
static_assert(sizeof(real_vector2d) == 0x8);

union real_vector3d
{
	struct
	{
		real i;
		real j;
		real k;
	};
	real n[3];
};
static_assert(sizeof(real_vector3d) == 0xC);

union real_quaternion
{
	struct
	{
		real i;
		real j;
		real k;
		real w;
	};
	real n[4];
};
static_assert(sizeof(real_quaternion) == 0x10);

union real_plane2d
{
	struct
	{
		real_vector2d normal;
		real distance;
	};
	real n[3];
};
static_assert(sizeof(real_plane2d) == 0xC);

union real_plane3d
{
	struct
	{
		real_vector3d normal;
		real distance;
	};
	real n[4];
};
static_assert(sizeof(real_plane3d) == 0x10);

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

union real_euler_angles2d
{
	struct
	{
		angle yaw;
		angle pitch;
	};
	angle n[2];
};
static_assert(sizeof(real_euler_angles2d) == 0x8);

union real_euler_angles3d
{
	struct
	{
		angle yaw;
		angle pitch;
		angle roll;
	};
	angle n[3];
};
static_assert(sizeof(real_euler_angles3d) == 0xC);

union real_matrix3x3
{
	struct
	{
		real_vector3d forward;
		real_vector3d left;
		real_vector3d up;
	};
	real n[9];
};
static_assert(sizeof(real_matrix3x3) == 0x24);

union real_matrix4x3
{
	struct
	{
		real scale;
		real_vector3d forward;
		real_vector3d left;
		real_vector3d up;
		real_vector3d position;
	};
	real n[13];
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

