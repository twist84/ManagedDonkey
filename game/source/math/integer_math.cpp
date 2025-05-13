#include "math/integer_math.hpp"

#include "cseries/cseries.hpp"

#include <intrin.h>

//.text:0052DD80 ; uns32 __cdecl count_bits<int32>(int32)
//.text:0052DDE0 ; public: __cdecl c_bit_range::c_bit_range(int32, int32, int32)

rectangle2d* __cdecl adjust_rectangle2d(rectangle2d const* bounds, rectangle2d const* source, rectangle2d* destination, int16 mode)
{
	return INVOKE(0x0052DE10, adjust_rectangle2d, bounds, source, destination, mode);
}

bool __cdecl bit_vector_and(int32 count, uns32 const* v0, uns32 const* v1, uns32* result)
{
	return INVOKE(0x0052DF40, bit_vector_and, count, v0, v1, result);
}

int32 __cdecl bit_vector_count_bits(uns32 const* vector, int32 count)
{
	return INVOKE(0x0052DFA0, bit_vector_count_bits, vector, count);
}

int32 __cdecl bit_vector_highest_bit_set(uns32 const* vector, int32 count)
{
	return INVOKE(0x0052E1E0, bit_vector_highest_bit_set, vector, count);
}

int32 __cdecl bit_vector_lowest_bit_set(uns32 const* vector, int32 count)
{
	return INVOKE(0x0052E280, bit_vector_lowest_bit_set, vector, count);
}

void __cdecl bit_vector_not(int32 count, uns32 const* vector, uns32* result)
{
	INVOKE(0x0052E330, bit_vector_not, count, vector, result);
}

void __cdecl bit_vector_not_and(int32 count, uns32 const* v0, uns32 const* v1, uns32* result)
{
	INVOKE(0x0052E360, bit_vector_not_and, count, v0, v1, result);
}

void __cdecl bit_vector_or(int32 count, uns32 const* v0, uns32 const* v1, uns32* result)
{
	INVOKE(0x0052E3A0, bit_vector_or, count, v0, v1, result);
}

void __cdecl bit_vector_set_range_to_true(int32 bit_count, uns32* bit_vector, int32 start_index, int32 set_count)
{
	INVOKE(0x0052E3E0, bit_vector_set_range_to_true, bit_count, bit_vector, start_index, set_count);
}

int16 __cdecl ceiling_log2(uns32 k)
{
	return INVOKE(0x0052E4A0, ceiling_log2, k);
}

uns32 __cdecl ceiling_power2(uns32 x)
{
	return INVOKE(0x0052E4D0, ceiling_power2, x);
}

rectangle2d* __cdecl center_rectangle2d(rectangle2d* a, rectangle2d const* b)
{
	return INVOKE(0x0052E4F0, center_rectangle2d, a, b);
}

rectangle2d* __cdecl center_rectangle2d_horizontal(rectangle2d* a, rectangle2d const* b)
{
	return INVOKE(0x0052E570, center_rectangle2d_horizontal, a, b);
}

rectangle2d* __cdecl center_rectangle2d_vertical(rectangle2d* a, rectangle2d const* b)
{
	return INVOKE(0x0052E5C0, center_rectangle2d_vertical, a, b);
}

bool __cdecl equal_point2d(point2d const* p1, point2d const* p2)
{
	//return INVOKE(0x0052E610, equal_point2d, p1, p2);

	return p1->x == p2->x && p1->y == p2->y;
}

bool __cdecl equal_rectangle2d(rectangle2d const* r1, rectangle2d const* r2)
{
	//return INVOKE(0x0052E640, equal_rectangle2d, r1, r2);

	return r1->x0 == r2->x0 && r1->x1 == r2->x1 && r1->y0 == r2->y0 && r1->y1 == r2->y1;
}

int16 __cdecl floor_log2(uns32 k)
{
	return INVOKE(0x0052E680, floor_log2, k);
}

//.text:0052E6A0 ; 
//.text:0052E6D0 ; 
//.text:0052E6E0 ; 
//.text:0052E6F0 ; 
//.text:0052E700 ; 
//.text:0052E710 ; 
//.text:0052E740 ; 

rectangle2d* __cdecl inset_rectangle2d(rectangle2d* rectangle, int16 dx, int16 dy)
{
	//return INVOKE(0x0052E770, inset_rectangle2d, rectangle, dx, dy);

	rectangle->x0 += dx;
	rectangle->x1 -= dx;
	rectangle->y0 += dy;
	rectangle->y1 -= dy;

	return rectangle;
}

bool __cdecl interior_rectangle2d(rectangle2d* outer, rectangle2d* inner)
{
	//return INVOKE(0x0052E7C0, interior_rectangle2d, outer, inner);

	return inner->x0 >= outer->x0 && inner->x1 <= outer->x1 && inner->y0 >= outer->y0 && inner->y1 <= outer->y1;
}

bool __cdecl intersect_rectangles2d(rectangle2d const* r1, rectangle2d const* r2, rectangle2d* intersection)
{
	return INVOKE(0x0052E800, intersect_rectangles2d, r1, r2, intersection);
}

bool __cdecl is_power_of_2(uns32 k)
{
	return INVOKE(0x0052E8B0, is_power_of_2, k);
}

point2d* __cdecl offset_point2d(point2d* point, int16 dx, int16 dy)
{
	//return INVOKE(0x0052E8D0, offset_point2d, point, dx, dy);

	point->x += dx;
	point->y += dy;

	return point;
}

rectangle2d* __cdecl offset_rectangle2d(rectangle2d* rectangle, int16 dx, int16 dy)
{
	//return INVOKE(0x0052E8F0, offset_rectangle2d, rectangle, dx, dy);

	rectangle->x0 += dx;
	rectangle->x1 += dx;
	rectangle->y0 += dy;
	rectangle->y1 += dy;

	return rectangle;
}

bool __cdecl point2d_in_rectangle2d(rectangle2d const* rectangle, point2d const* point)
{
	//return INVOKE(0x0052E910, point2d_in_rectangle2d, rectangle, point);

	return point->x >= rectangle->x0 && point->x < rectangle->x1 && point->y >= rectangle->y0 && point->y < rectangle->y1;
}

int16 rectangle2d_height(rectangle2d const* rectangle)
{
	//return INVOKE(0x0052E950, rectangle2d_height, rectangle);

	return rectangle->y1 - rectangle->y0;
}

rectangle2d* __cdecl rectangle2d_hull_from_rectangles2d(rectangle2d const* r1, rectangle2d const* r2, rectangle2d* hull)
{
	return INVOKE(0x0052E970, rectangle2d_hull_from_rectangles2d, r1, r2, hull);
}

int16 rectangle2d_width(rectangle2d const* rectangle)
{
	//return INVOKE(0x0052E9E0, rectangle2d_width, rectangle);

	return rectangle->x1 - rectangle->x0;
}

rectangle2d* __cdecl scale_rectangle2d(rectangle2d const* bounds, rectangle2d const* source, rectangle2d* destination, int16 scale)
{
	return INVOKE(0x0052EA00, scale_rectangle2d, bounds, source, destination, scale);
}

point2d* set_point2d(point2d* point, int16 x, int16 y)
{
	//return INVOKE(0x0052EAE0, set_point2d, point, x, y);

	point->x = x;
	point->y = y;

	return point;
}

rectangle2d* set_rectangle2d(rectangle2d* rectangle, int16 x0, int16 y0, int16 x1, int16 y1)
{
	//return INVOKE(0x0052EB00, set_rectangle2d, rectangle, x0, y0, x1, y1);

	rectangle->x0 = x0;
	rectangle->y0 = y0;
	rectangle->x1 = x1;
	rectangle->y1 = y1;

	return rectangle;
}

int32 __cdecl short_fixed_to_long(int16 f)
{
	return INVOKE(0x0052EB30, short_fixed_to_long, f);
}

int32 highest_bit_set(uns32 mask)
{
	if (mask == 0)
		return NONE;

	uns32 index;
	_BitScanReverse(&index, mask);

	return static_cast<int32>(index);
}

int32 lowest_bit_set(uns32 mask)
{
	if (mask == 0)
		return NONE;

	uns32 index;
	_BitScanForward(&index, mask);

	return static_cast<int32>(index);
}

