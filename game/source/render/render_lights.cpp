#include "render/render_lights.hpp"

REFERENCE_DECLARE(0x05115B3C, bool, render_default_lighting);

real_point3d* __cdecl point_from_line3d(real_point3d const* in_point, vector3d const* in_vector, real scale, real_point3d* out_point)
{
	*out_point = *in_point;
	out_point->n[0] += in_vector->n[0] * scale;
	out_point->n[1] += in_vector->n[1] * scale;
	out_point->n[2] += in_vector->n[2] * scale;

	return out_point;
}

