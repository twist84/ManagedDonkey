#include "math/integer_math.hpp"

#include "cseries/cseries.hpp"

#include <intrin.h>

//.text:0052DD80 ; unsigned long __cdecl count_bits<long>(long)
//.text:0052DDE0 ; public: __cdecl c_bit_range::c_bit_range(long, long, long)
//.text:0052DE10 ; 

bool __cdecl bit_vector_and(long count, unsigned long const* v0, unsigned long const* v1, unsigned long* result)
{
	return INVOKE(0x0052DF40, bit_vector_and, count, v0, v1, result);
}

//.text:0052DFA0 ; long __cdecl bit_vector_count_bits(unsigned long const*, long)

long __cdecl bit_vector_highest_bit_set(unsigned long const* vector, long count)
{
	return INVOKE(0x0052E1E0, bit_vector_highest_bit_set, vector, count);
}

//.text:0052E280 ; long __cdecl bit_vector_lowest_bit_set(unsigned long const*, long)

void __cdecl bit_vector_not(long count, unsigned long const* vector, unsigned long* result)
{
	INVOKE(0x0052E330, bit_vector_not, count, vector, result);
}

//.text:0052E360 ; 

void __cdecl bit_vector_or(long count, unsigned long const* v0, unsigned long const* v1, unsigned long* result)
{
	INVOKE(0x0052E3A0, bit_vector_or, count, v0, v1, result);
}

//.text:0052E3E0 ; 
//.text:0052E4A0 ; short __cdecl ceiling_log2(unsigned long)

long highest_bit_set(unsigned long mask)
{
	if (mask == 0)
		return NONE;

	dword index;
	_BitScanReverse(&index, mask);

	return static_cast<long>(index);
}

long lowest_bit_set(unsigned long mask)
{
	if (mask == 0)
		return NONE;

	dword index;
	_BitScanForward(&index, mask);

	return static_cast<long>(index);
}

short_rectangle2d* set_rectangle2d(short_rectangle2d* rect, short x0, short y0, short x1, short y1)
{
	rect->x0 = x0;
	rect->y0 = y0;
	rect->x1 = x1;
	rect->y1 = y1;

	return rect;
}

int16_point2d* set_point2d(int16_point2d* point, short x, short y)
{
	point->x = x;
	point->y = y;

	return point;
}

