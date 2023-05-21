#include "math/integer_math.hpp"

short_rectangle2d* set_rectangle2d(short_rectangle2d* rect, short x0, short x1, short y0, short y1)
{
	rect->x0 = x0;
	rect->y0 = y0;
	rect->x1 = x1;
	rect->y1 = y1;

	return rect;
}

