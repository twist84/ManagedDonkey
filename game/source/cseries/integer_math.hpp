#pragma once

struct point2d
{
	short x;
	short y;
};
static_assert(sizeof(point2d) == 0x4);

struct rectangle2d
{
	short top;
	short left;
	short bottom;
	short right;
};
static_assert(sizeof(rectangle2d) == 0x8);

typedef unsigned long rgb_color;
static_assert(sizeof(rgb_color) == 0x4);

typedef unsigned long argb_color;
static_assert(sizeof(argb_color) == 0x4);

struct short_bounds
{
	short lower;
	short upper;
};
static_assert(sizeof(short_bounds) == 0x4);