#include "interface/gui_animation.hpp"

//.text:00B17830 ; public: __cdecl s_animation_transform::s_animation_transform()

gui_real_rectangle2d* gui_real_rectangle2d::apply_projection_transform(real depth, rectangle2d const* window_bounds)
{
	return INVOKE_CLASS_MEMBER(0x00B17910, gui_real_rectangle2d, apply_projection_transform, depth, window_bounds);
}

real_point2d* gui_real_rectangle2d::get_centroid(real_point2d* centroid) const
{
	return INVOKE_CLASS_MEMBER(0x00B17A50, gui_real_rectangle2d, get_centroid, centroid);
}

real gui_real_rectangle2d::get_height() const
{
	return (real)DECLFUNC(0x00B17A90, double, __thiscall, gui_real_rectangle2d const*)(this);
}

real gui_real_rectangle2d::get_width() const
{
	return (real)DECLFUNC(0x00B17AE0, double, __thiscall, gui_real_rectangle2d const*)(this);
}

gui_real_rectangle2d* gui_real_rectangle2d::offset(real dx, real dy)
{
	return INVOKE_CLASS_MEMBER(0x00B17B30, gui_real_rectangle2d, offset, dx, dy);
}

gui_real_rectangle2d* gui_real_rectangle2d::pin(real_rectangle2d const* pin_bounds)
{
	return INVOKE_CLASS_MEMBER(0x00B17BB0, gui_real_rectangle2d, pin, pin_bounds);
}

gui_real_rectangle2d* gui_real_rectangle2d::rotate_about_local_point(real_point2d const* local_point, real angle_radians)
{
	//return INVOKE_CLASS_MEMBER(0x00B17C10, gui_real_rectangle2d, rotate_about_local_point, local_point, angle_radians);
	return DECLFUNC(0x00B17C10, gui_real_rectangle2d*, __thiscall, gui_real_rectangle2d*, real_point2d const*, real)(this, local_point, angle_radians);
}

gui_real_rectangle2d* gui_real_rectangle2d::rotate_about_local_point(real_point2d const* local_point, real sine_angle, real cosine_angle)
{
	//return INVOKE_CLASS_MEMBER(0x00B17DC0, gui_real_rectangle2d, rotate_about_local_point, local_point, sine_angle, cosine_angle);
	return DECLFUNC(0x00B17DC0, gui_real_rectangle2d*, __thiscall, gui_real_rectangle2d*, real_point2d const*, real, real)(this, local_point, sine_angle, cosine_angle);
}

gui_real_rectangle2d* gui_real_rectangle2d::scale_about_local_point(real_point2d const* local_point, real_vector2d const* scale)
{
	return INVOKE_CLASS_MEMBER(0x00B17F40, gui_real_rectangle2d, scale_about_local_point, local_point, scale);
}

void gui_real_rectangle2d::scale_direct(real_vector2d const* scale)
{
	vertex[0].x *= scale->i;
	vertex[0].y *= scale->j;
	vertex[1].x *= scale->i;
	vertex[1].y *= scale->j;
	vertex[2].x *= scale->i;
	vertex[2].y *= scale->j;
	vertex[3].x *= scale->i;
	vertex[3].y *= scale->j;
}

void gui_real_rectangle2d::set(real_rectangle2d const* source)
{
	//INVOKE_CLASS_MEMBER(0x00B18080, gui_real_rectangle2d, set, source);
	DECLFUNC(0x00B18080, void, __thiscall, gui_real_rectangle2d*, real_rectangle2d const*)(this, source);

	//ASSERT(source != NULL);
	//vertex[0].n[0] = source->n[0];
	//vertex[0].n[1] = source->n[2];
	//vertex[1].n[0] = source->n[0];
	//vertex[1].n[1] = source->n[3];
	//vertex[2].n[0] = source->n[1];
	//vertex[2].n[1] = source->n[2];
	//vertex[3].n[0] = source->n[1];
	//vertex[3].n[1] = source->n[3];
}

void gui_real_rectangle2d::set(rectangle2d const* source)
{
	//INVOKE_CLASS_MEMBER(0x00B180C0, gui_real_rectangle2d, set, source);
	DECLFUNC(0x00B180C0, void, __thiscall, gui_real_rectangle2d*, rectangle2d const*)(this, source);

	//ASSERT(source != NULL);
	//vertex[0].n[0] = (real)source->n[1];
	//vertex[0].n[1] = (real)source->n[0];
	//vertex[1].n[0] = (real)source->n[1];
	//vertex[1].n[1] = (real)source->n[2];
	//vertex[2].n[0] = (real)source->n[3];
	//vertex[2].n[1] = (real)source->n[0];
	//vertex[3].n[0] = (real)source->n[3];
	//vertex[3].n[1] = (real)source->n[2];
}

void gui_real_rectangle2d::transform_real_point3d(real_point3d* point, rectangle2d const* window_bounds)
{
	INVOKE(0x00B18150, gui_real_rectangle2d::transform_real_point3d, point, window_bounds);
}

