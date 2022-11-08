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

union short_rectangle2d
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
static_assert(sizeof(short_rectangle2d) == 0x8);

union rectangle2d
{
	struct
	{
		long top;
		long left;
		long bottom;
		long right;
	};
	long n[4];
};
static_assert(sizeof(rectangle2d) == 0x10);

union rgb_color
{
	struct
	{
		unsigned char : 8;
		unsigned char red;
		unsigned char green;
		unsigned char blue;
	};
	unsigned char n[4];
};
static_assert(sizeof(rgb_color) == 0x4);

union argb_color
{
	struct
	{
		unsigned char alpha;
		unsigned char red;
		unsigned char green;
		unsigned char blue;
	};
	unsigned char n[4];
};
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