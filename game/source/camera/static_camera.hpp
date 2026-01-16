#pragma once

#include "camera/camera.hpp"

class c_static_camera :
	public c_camera
{
public:
	void constructor(int32 user_index)
	{
		INVOKE_CLASS_MEMBER(0x0072F170, c_static_camera, constructor, user_index);
	}
	
//protected:
	real_point3d m_position;
	real_vector3d m_forward;
	real32 m_field_of_view;

	real32 __unknown2C;
	uns32 __unknown30;

	byte unused[0x18];
};
COMPILE_ASSERT(sizeof(c_static_camera) == 0x4C);

class c_scripted_camera :
	public c_camera
{
public:
	void constructor()
	{
		INVOKE_CLASS_MEMBER(0x0072BEB0, c_scripted_camera, constructor);
	}

//protected:
	byte unused[0x3C];
};
COMPILE_ASSERT(sizeof(c_scripted_camera) == 0x4C);

