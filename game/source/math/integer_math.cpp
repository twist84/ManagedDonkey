#include "math/integer_math.hpp"

#include "cseries/cseries.hpp"

#include <intrin.h>

//.text:0052DD80 ; uint32 __cdecl count_bits<int32>(int32)
//.text:0052DDE0 ; public: __cdecl c_bit_range::c_bit_range(int32, int32, int32)
//.text:0052DE10 ; 

bool __cdecl bit_vector_and(int32 count, uint32 const* v0, uint32 const* v1, uint32* result)
{
	return INVOKE(0x0052DF40, bit_vector_and, count, v0, v1, result);
}

int32 __cdecl bit_vector_count_bits(uint32 const* vector, int32 count)
{
	return INVOKE(0x0052DFA0, bit_vector_count_bits, vector, count);
}

int32 __cdecl bit_vector_highest_bit_set(uint32 const* vector, int32 count)
{
	return INVOKE(0x0052E1E0, bit_vector_highest_bit_set, vector, count);
}

int32 __cdecl bit_vector_lowest_bit_set(uint32 const* vector, int32 count)
{
	return INVOKE(0x0052E280, bit_vector_lowest_bit_set, vector, count);
}

void __cdecl bit_vector_not(int32 count, uint32 const* vector, uint32* result)
{
	INVOKE(0x0052E330, bit_vector_not, count, vector, result);
}

//.text:0052E360 ; 

void __cdecl bit_vector_or(int32 count, uint32 const* v0, uint32 const* v1, uint32* result)
{
	INVOKE(0x0052E3A0, bit_vector_or, count, v0, v1, result);
}

//.text:0052E3E0 ; 
//.text:0052E4A0 ; int16 __cdecl ceiling_log2(uint32)

int32 highest_bit_set(uint32 mask)
{
	if (mask == 0)
		return NONE;

	uint32 index;
	_BitScanReverse(&index, mask);

	return static_cast<int32>(index);
}

int32 lowest_bit_set(uint32 mask)
{
	if (mask == 0)
		return NONE;

	uint32 index;
	_BitScanForward(&index, mask);

	return static_cast<int32>(index);
}

rectangle2d* set_rectangle2d(rectangle2d* rect, int16 x0, int16 y0, int16 x1, int16 y1)
{
	rect->x0 = x0;
	rect->y0 = y0;
	rect->x1 = x1;
	rect->y1 = y1;

	return rect;
}

point2d* set_point2d(point2d* point, int16 x, int16 y)
{
	point->x = x;
	point->y = y;

	return point;
}

