#pragma once

#include "camera/camera.hpp"

struct c_following_camera :
	public c_camera
{
public:
	void constructor(int32 unit_index)
	{
		INVOKE_CLASS_MEMBER(0x00728630, c_following_camera, constructor, unit_index);
	}
	
//protected:
	real_euler_angles2d m_facing_offset;
	real32 m_distance_scale;
	real_vector3d m_acceleration_offset;
	int32 m_last_unit_index;
	bool m_confined;
	bool m_crouched;
	bool m_zoomed;
	bool m_prefer_tight;
	int16 m_zoom_level;
	int16 m_seat_index;

	uint8 unused[0x18];
};
static_assert(sizeof(c_following_camera) == 0x4C);

