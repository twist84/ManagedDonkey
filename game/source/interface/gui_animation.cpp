#include "interface/gui_animation.hpp"

//.text:00B17830 ; public: __cdecl s_animation_transform::s_animation_transform()

gui_real_rectangle2d* gui_real_rectangle2d::apply_projection_transform(real32 depth, const rectangle2d* window_bounds)
{
	return INVOKE_CLASS_MEMBER(0x00B17910, gui_real_rectangle2d, apply_projection_transform, depth, window_bounds);
}

real_point2d* gui_real_rectangle2d::get_centroid(real_point2d* centroid) const
{
	return INVOKE_CLASS_MEMBER(0x00B17A50, gui_real_rectangle2d, get_centroid, centroid);
}

real32 gui_real_rectangle2d::get_height() const
{
	return (real32)DECLFUNC(0x00B17A90, real64, __thiscall, const gui_real_rectangle2d*)(this);
}

real32 gui_real_rectangle2d::get_width() const
{
	return (real32)DECLFUNC(0x00B17AE0, real64, __thiscall, const gui_real_rectangle2d*)(this);
}

gui_real_rectangle2d* gui_real_rectangle2d::offset(real32 dx, real32 dy)
{
	return INVOKE_CLASS_MEMBER(0x00B17B30, gui_real_rectangle2d, offset, dx, dy);
}

gui_real_rectangle2d* gui_real_rectangle2d::pin(const real_rectangle2d* pin_bounds)
{
	return INVOKE_CLASS_MEMBER(0x00B17BB0, gui_real_rectangle2d, pin, pin_bounds);
}

gui_real_rectangle2d* gui_real_rectangle2d::rotate_about_local_point(const real_point2d* local_point, real32 angle_radians)
{
	//return INVOKE_CLASS_MEMBER(0x00B17C10, gui_real_rectangle2d, rotate_about_local_point, local_point, angle_radians);
	return DECLFUNC(0x00B17C10, gui_real_rectangle2d*, __thiscall, gui_real_rectangle2d*, const real_point2d*, real32)(this, local_point, angle_radians);
}

gui_real_rectangle2d* gui_real_rectangle2d::rotate_about_local_point(const real_point2d* local_point, real32 sine_angle, real32 cosine_angle)
{
	//return INVOKE_CLASS_MEMBER(0x00B17DC0, gui_real_rectangle2d, rotate_about_local_point, local_point, sine_angle, cosine_angle);
	return DECLFUNC(0x00B17DC0, gui_real_rectangle2d*, __thiscall, gui_real_rectangle2d*, const real_point2d*, real32, real32)(this, local_point, sine_angle, cosine_angle);
}

gui_real_rectangle2d* gui_real_rectangle2d::scale_about_local_point(const real_point2d* local_point, const real_vector2d* scale)
{
	return INVOKE_CLASS_MEMBER(0x00B17F40, gui_real_rectangle2d, scale_about_local_point, local_point, scale);
}

void gui_real_rectangle2d::scale_direct(const real_vector2d* scale)
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

void gui_real_rectangle2d::set(const real_rectangle2d* source)
{
	//INVOKE_CLASS_MEMBER(0x00B18080, gui_real_rectangle2d, set, source);
	DECLFUNC(0x00B18080, void, __thiscall, gui_real_rectangle2d*, const real_rectangle2d*)(this, source);

	//ASSERT(source != nullptr);
	//vertex[0].n[0] = source->n[0];
	//vertex[0].n[1] = source->n[2];
	//vertex[1].n[0] = source->n[0];
	//vertex[1].n[1] = source->n[3];
	//vertex[2].n[0] = source->n[1];
	//vertex[2].n[1] = source->n[2];
	//vertex[3].n[0] = source->n[1];
	//vertex[3].n[1] = source->n[3];
}

void gui_real_rectangle2d::set(const rectangle2d* source)
{
	//INVOKE_CLASS_MEMBER(0x00B180C0, gui_real_rectangle2d, set, source);
	DECLFUNC(0x00B180C0, void, __thiscall, gui_real_rectangle2d*, const rectangle2d*)(this, source);

	//ASSERT(source != nullptr);
	//vertex[0].n[0] = (real32)source->n[1];
	//vertex[0].n[1] = (real32)source->n[0];
	//vertex[1].n[0] = (real32)source->n[1];
	//vertex[1].n[1] = (real32)source->n[2];
	//vertex[2].n[0] = (real32)source->n[3];
	//vertex[2].n[1] = (real32)source->n[0];
	//vertex[3].n[0] = (real32)source->n[3];
	//vertex[3].n[1] = (real32)source->n[2];
}

void gui_real_rectangle2d::transform_real_point3d(real_point3d* point, const rectangle2d* window_bounds)
{
	INVOKE(0x00B18150, gui_real_rectangle2d::transform_real_point3d, point, window_bounds);
}

