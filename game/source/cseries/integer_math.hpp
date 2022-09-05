#pragma once

union point2d
{
	struct
	{
		short x;
		short y;
	};
	short n[2];
};
static_assert(sizeof(point2d) == 0x4);

union long_point3d
{
	struct
	{
		long x;
		long y;
		long z;
	};
	long n[3];
};
static_assert(sizeof(long_point3d) == 0xC);

union rectangle2d
{
	struct
	{
		short top;
		short left;
		short bottom;
		short right;
	};
	short n[4];
};
static_assert(sizeof(rectangle2d) == 0x8);

typedef unsigned long rgb_color;
static_assert(sizeof(rgb_color) == 0x4);

typedef unsigned long argb_color;
static_assert(sizeof(argb_color) == 0x4);

union short_bounds
{
	struct
	{
		short lower;
		short upper;
	};
	short n[2];
};
static_assert(sizeof(short_bounds) == 0x4);