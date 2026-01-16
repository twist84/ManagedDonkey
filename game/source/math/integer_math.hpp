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
COMPILE_ASSERT(sizeof(int16_bounds) == 0x4);

union int32_bounds
{
	struct
	{
		int32 lower;
		int32 upper;
	};
	int32 n[2];
};
COMPILE_ASSERT(sizeof(int32_bounds) == 0x8);

union int64_bounds
{
	struct
	{
		int64 lower;
		int64 upper;
	};
	int64 n[2];
};
COMPILE_ASSERT(sizeof(int64_bounds) == 0x10);

union point2d
{
	struct
	{
		int16 x;
		int16 y;
	};
	int16 n[2];
};
COMPILE_ASSERT(sizeof(point2d) == 0x4);

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
COMPILE_ASSERT(sizeof(point3d) == 0x6);

union long_point2d
{
	struct
	{
		int32 x;
		int32 y;
	};
	int32 n[2];
};
COMPILE_ASSERT(sizeof(long_point2d) == 0x8);

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
COMPILE_ASSERT(sizeof(long_point3d) == 0xC);

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
COMPILE_ASSERT(sizeof(rectangle2d) == 0x8);

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
COMPILE_ASSERT(sizeof(long_rectangle2d) == 0x10);

union rgb_color
{
	struct
	{
		uns8 : 8;
		uns8 red;
		uns8 green;
		uns8 blue;
	};
	uns8 n[4];
	uns32 value;
};
COMPILE_ASSERT(sizeof(rgb_color) == 0x4);

union argb_color
{
	struct
	{
		uns8 alpha;
		uns8 red;
		uns8 green;
		uns8 blue;
	};
	uns8 n[4];
	uns32 value;
};
COMPILE_ASSERT(sizeof(argb_color) == 0x4);

extern rectangle2d* __cdecl adjust_rectangle2d(const rectangle2d* bounds, const rectangle2d* source, rectangle2d* destination, int16 mode);
extern bool __cdecl bit_vector_and(int32 count, const uns32* v0, const uns32* v1, uns32* result);
extern int32 __cdecl bit_vector_count_bits(const uns32* vector, int32 count);
extern int32 __cdecl bit_vector_highest_bit_set(const uns32* vector, int32 count);
extern int32 __cdecl bit_vector_lowest_bit_set(const uns32* vector, int32 count);
extern void __cdecl bit_vector_not(int32 count, const uns32* vector, uns32* result);
extern void __cdecl bit_vector_not_and(int32 count, const uns32* v0, const uns32* v1, uns32* result);
extern void __cdecl bit_vector_or(int32 count, const uns32* v0, const uns32* v1, uns32* result);
extern void __cdecl bit_vector_set_range_to_true(int32 bit_count, uns32* bit_vector, int32 start_index, int32 set_count);
extern int16 __cdecl ceiling_log2(uns32 k);
extern uns32 __cdecl ceiling_power2(uns32 x);
extern rectangle2d* __cdecl center_rectangle2d(rectangle2d* a, const rectangle2d* b);
extern rectangle2d* __cdecl center_rectangle2d_horizontal(rectangle2d* a, const rectangle2d* b);
extern rectangle2d* __cdecl center_rectangle2d_vertical(rectangle2d* a, const rectangle2d* b);
extern bool __cdecl equal_point2d(const point2d* p1, const point2d* p2);
extern bool __cdecl equal_rectangle2d(const rectangle2d* r1, const rectangle2d* r2);
extern int16 __cdecl floor_log2(uns32 k);
extern rectangle2d* __cdecl inset_rectangle2d(rectangle2d* rectangle, int16 dx, int16 dy);
extern bool __cdecl interior_rectangle2d(rectangle2d* outer, rectangle2d* inner);
extern bool __cdecl intersect_rectangles2d(const rectangle2d* r1, const rectangle2d* r2, rectangle2d* intersection);
extern bool __cdecl is_power_of_2(uns32 k);
extern point2d* __cdecl offset_point2d(point2d* point, int16 dx, int16 dy);
extern rectangle2d* __cdecl offset_rectangle2d(rectangle2d* rectangle, int16 dx, int16 dy);
extern bool __cdecl point2d_in_rectangle2d(const rectangle2d* rectangle, const point2d* point);
extern int16 rectangle2d_height(const rectangle2d* rectangle);
extern rectangle2d* __cdecl rectangle2d_hull_from_rectangles2d(const rectangle2d* r1, const rectangle2d* r2, rectangle2d* hull);
extern int16 rectangle2d_width(const rectangle2d* rectangle);
extern rectangle2d* __cdecl scale_rectangle2d(const rectangle2d* bounds, const rectangle2d* source, rectangle2d* destination, int16 scale);
extern point2d* set_point2d(point2d* point, int16 x, int16 y);
extern rectangle2d* set_rectangle2d(rectangle2d* rectangle, int16 x0, int16 y0, int16 x1, int16 y1);
extern int32 __cdecl short_fixed_to_long(int16 f);

extern int32 highest_bit_set(uns32 mask);
extern int32 lowest_bit_set(uns32 mask);

template<typename t_type>
t_type int_min(const t_type& val0, const t_type& val1)
{
	if (val0 <= val1)
		return val0;

	return val1;
}

template<typename t_type>
t_type int_max(const t_type& val0, const t_type& val1)
{
	if (val0 <= val1)
		return val1;

	return val0;
}

template<typename t_type, typename t_other_type>
t_type int_ceiling(const t_type& val0, const t_other_type& val1)
{
	return int_min(val0, static_cast<t_type>(val1));
}

template<typename t_type, typename t_other_type>
t_type int_floor(const t_type& val0, const t_other_type& val1)
{
	return int_max(val0, static_cast<t_type>(val1));
}

template<typename t_type, typename t_other_type0, typename t_other_type1>
t_type int_pin(const t_type& val0, const t_other_type0& val1, const t_other_type1& val2)
{
	return int_ceiling(int_floor(val0, val1), val2);
}

