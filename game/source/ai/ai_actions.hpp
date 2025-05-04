#pragma once

#include "cseries/cseries.hpp"

struct c_ai_point3d
{
	real_point3d m_point;
	int16 m_reference_frame;
	int16 m_bsp_index;
};
static_assert(sizeof(c_ai_point3d) == 0x10);

struct c_ai_direction
{
	int32 m_type;

	union
	{
		int32 m_pref_index;
		int32 m_object_index;
		real_point3d m_point;
		real_vector3d m_vector;
		c_ai_point3d m_ai_point;
	};
};
static_assert(sizeof(c_ai_direction) == 0x14);

struct c_ai_action
{
	int32 m_ai_action;
	bool m_point_valid;
	byte m_pad0;
	int16 m_ticks;

	union
	{
		int32 m_data;
		int32 m_animation_name;
		int32 m_posture_name;
		struct
		{
			int16 m_corner_type;
			bool m_open;
		} m_corner;
	};

	c_ai_point3d m_point;
	c_ai_direction m_alignment;
};
static_assert(sizeof(c_ai_action) == 0x30);
