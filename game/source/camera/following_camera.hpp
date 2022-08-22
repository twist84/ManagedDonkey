#pragma once

#include "camera/camera.hpp"

#pragma pack(push, 2)
struct c_following_camera : public c_camera
{
	dword __unknown10;
	dword __unknown14;
	real __unknown18;
	real_vector3d __vector1C;

	datum_index m_target_object_index;

	word __unknown2C;
	dword __unknown2E;
	word __unknown32;

	byte unused[0x18];
};
static_assert(sizeof(c_following_camera) == 0x4C);
#pragma pack(pop)