#pragma once

union int16_bounds
{
	struct
	{
		int16 lower;
		int16 upper;
	};
	int16 n[2];
};
static_assert(sizeof(int16_bounds) == 0x4);

union int32_bounds
{
	struct
	{
		int32 lower;
		int32 upper;
	};
	int32 n[2];
};
static_assert(sizeof(int32_bounds) == 0x8);

union int64_bounds
{
	struct
	{
		int64 lower;
		int64 upper;
	};
	int64 n[2];
};
static_assert(sizeof(int64_bounds) == 0x10);

union point2d
{
	struct
	{
		int16 x;
		int16 y;
	};
	int16 n[2];
};
static_assert(sizeof(point2d) == 0x4);

union point3d
{
	struct
	{
		int16 x;
		int16 y;
		int16 z;
	};
	int16 n[3];
};
static_assert(sizeof(point3d) == 0x6);

union long_point2d
{
	struct
	{
		int32 x;
		int32 y;
	};
	int32 n[2];
};
static_assert(sizeof(long_point2d) == 0x8);

union long_point3d
{
	struct
	{
		int32 x;
		int32 y;
		int32 z;
	};
	int32 n[3];
};
static_assert(sizeof(long_point3d) == 0xC);

union rectangle2d
{
	struct
	{
		int16 y0;
		int16 x0;
		int16 y1;
		int16 x1;
	};
	int16 n[4];
};
static_assert(sizeof(rectangle2d) == 0x8);

union long_rectangle2d
{
	struct
	{
		int32 y0;
		int32 x0;
		int32 y1;
		int32 x1;
	};
	int32 n[4];
};
static_assert(sizeof(long_rectangle2d) == 0x10);

union rgb_color
{
	struct
	{
		uint8 : 8;
		uint8 red;
		uint8 green;
		uint8 blue;
	};
	uint8 n[4];
	uint32 value;
};
static_assert(sizeof(rgb_color) == 0x4);

union argb_color
{
	struct
	{
		uint8 alpha;
		uint8 red;
		uint8 green;
		uint8 blue;
	};
	uint8 n[4];
	uint32 value;
};
static_assert(sizeof(argb_color) == 0x4);

extern bool __cdecl bit_vector_and(int32 count, uint32 const* v0, uint32 const* v1, uint32* result);
extern int32 __cdecl bit_vector_count_bits(uint32 const* vector, int32 count);
extern int32 __cdecl bit_vector_highest_bit_set(uint32 const* vector, int32 count);
extern int32 __cdecl bit_vector_lowest_bit_set(uint32 const* vector, int32 count);
extern void __cdecl bit_vector_not(int32 count, uint32 const* vector, uint32* result);
extern void __cdecl bit_vector_or(int32 count, uint32 const* v0, uint32 const* v1, uint32* result);

extern int32 highest_bit_set(uint32 mask);
extern int32 lowest_bit_set(uint32 mask);
extern rectangle2d* set_rectangle2d(rectangle2d* rect, int16 y0, int16 x0, int16 y1, int16 x1);
extern point2d* set_point2d(point2d* point, int16 x, int16 y);

template<typename t_type>
t_type int_min(t_type const& val0, t_type const& val1)
{
	if (val0 <= val1)
		return val0;

	return val1;
}

template<typename t_type>
t_type int_max(t_type const& val0, t_type const& val1)
{
	if (val0 <= val1)
		return val1;

	return val0;
}

template<typename t_type, typename t_other_type>
t_type int_ceiling(t_type const& val0, t_other_type const& val1)
{
	return int_min(val0, static_cast<t_type>(val1));
}

template<typename t_type, typename t_other_type>
t_type int_floor(t_type const& val0, t_other_type const& val1)
{
	return int_max(val0, static_cast<t_type>(val1));
}

template<typename t_type, typename t_other_type0, typename t_other_type1>
t_type int_pin(t_type const& val0, t_other_type0 const& val1, t_other_type1 const& val2)
{
	return int_ceiling(int_floor(val0, val1), val2);
}

