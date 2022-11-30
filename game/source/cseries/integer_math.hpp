#pragma once

union int16_point2d
{
	struct
	{
		short x;
		short y;
	};
	short n[2];
};
static_assert(sizeof(int16_point2d) == 0x4);

union int16_point3d
{
	struct
	{
		short x;
		short y;
		short z;
	};
	short n[3];
};
static_assert(sizeof(int16_point3d) == 0x6);

union point2d
{
	struct
	{
		long x;
		long y;
	};
	long n[2];
};
static_assert(sizeof(point2d) == 0x8);

union int32_point3d
{
	struct
	{
		long x;
		long y;
		long z;
	};
	long n[3];
};
static_assert(sizeof(int32_point3d) == 0xC);

union short_rectangle2d
{
	struct
	{
		short y0;
		short x0;
		short y1;
		short x1;
	};
	short n[4];
};
static_assert(sizeof(short_rectangle2d) == 0x8);

union rectangle2d
{
	struct
	{
		long y0;
		long x0;
		long y1;
		long x1;
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
	unsigned long value;
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
	unsigned long value;
};
static_assert(sizeof(argb_color) == 0x4);

