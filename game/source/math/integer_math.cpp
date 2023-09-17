#include "math/integer_math.hpp"

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

