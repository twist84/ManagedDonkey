#pragma once

typedef float real;
static_assert(sizeof(real) == 0x4);

typedef float real_fraction;
static_assert(sizeof(real_fraction) == 0x4);

struct real_bounds
{
	real lower;
	real upper;
};
static_assert(sizeof(real_bounds) == 0x8);

struct real_point2d
{
	real x;
	real y;
};
static_assert(sizeof(real_point2d) == 0x8);

struct real_point3d
{
	real x;
	real y;
	real z;
};
static_assert(sizeof(real_point3d) == 0xC);

struct real_vector2d
{
	real i;
	real j;
};
static_assert(sizeof(real_vector2d) == 0x8);

struct real_vector3d
{
	real i;
	real j;
	real k;
};
static_assert(sizeof(real_vector3d) == 0xC);

struct real_quaternion
{
	real i;
	real j;
	real k;
	real w;
};
static_assert(sizeof(real_quaternion) == 0x10);

struct real_plane2d
{
	real_vector2d normal;
	real distance;
};
static_assert(sizeof(real_plane2d) == 0xC);

struct real_plane3d
{
	real_vector3d normal;
	real distance;
};
static_assert(sizeof(real_plane3d) == 0x10);

struct real_rgb_color
{
	real red;
	real green;
	real blue;
};
static_assert(sizeof(real_rgb_color) == 0xC);

struct real_argb_color
{
	real alpha;
	real red;
	real green;
	real blue;
};
static_assert(sizeof(real_argb_color) == 0x10);

struct real_hsv_color
{
	real hue;
	real saturation;
	real value;
};
static_assert(sizeof(real_hsv_color) == 0xC);

struct real_ahsv_color
{
	real alpha;
	real hue;
	real saturation;
	real value;
};
static_assert(sizeof(real_ahsv_color) == 0x10);

typedef float angle;
static_assert(sizeof(angle) == 0x4);

struct angle_bounds
{
	angle lower;
	angle upper;
};
static_assert(sizeof(angle_bounds) == 0x8);

struct real_euler_angles2d
{
	angle yaw;
	angle pitch;
};
static_assert(sizeof(real_euler_angles2d) == 0x8);

struct real_euler_angles3d
{
	angle yaw;
	angle pitch;
	angle roll;
};
static_assert(sizeof(real_euler_angles3d) == 0xC);

struct real_matrix3x3
{
	real_vector3d forward;
	real_vector3d left;
	real_vector3d up;
};
static_assert(sizeof(real_matrix3x3) == 0x24);

struct real_matrix4x3
{
	real scale;
	real_vector3d forward;
	real_vector3d left;
	real_vector3d up;
	real_vector3d position;
};
static_assert(sizeof(real_matrix4x3) == 0x34);