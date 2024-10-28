#pragma once

#include "camera/camera.hpp"

#pragma pack(push, 2)
struct c_following_camera :
	public c_camera
{
	dword __unknown10;
	real __unknown14;
	real __unknown18;
	vector3d __vector1C;

	long m_target_object_index;

	bool __unknown2C;
	bool __unknown2D;
	bool __unknown2E;
	bool __unknown2F;
	bool __unknown30;
	bool __unknown31;
	short m_seat_index;

	byte unused[0x18];

	void constructor(long unit_index)
	{
		INVOKE_CLASS_MEMBER(0x00728630, c_following_camera, constructor, unit_index);
	}
};
static_assert(sizeof(c_following_camera) == 0x4C);
#pragma pack(pop)

