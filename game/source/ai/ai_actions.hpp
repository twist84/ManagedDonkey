#pragma once

#include "cseries/cseries.hpp"

struct c_ai_point3d
{
	real_point3d m_point;
	short m_reference_frame;
	short m_bsp_index;
};
static_assert(sizeof(c_ai_point3d) == 0x10);

struct c_ai_direction
{
	long m_type;

	union
	{
		long m_pref_index;
		long m_object_index;
		real_point3d m_point;
		real_vector3d m_vector;
		c_ai_point3d m_ai_point;
	};
};
static_assert(sizeof(c_ai_direction) == 0x14);

struct c_ai_action
{
	long m_ai_action;
	bool m_point_valid;
	uint8 m_pad0;
	short m_ticks;

	union
	{
		long m_data;
		long m_animation_name;
		long m_posture_name;
		struct
		{
			short m_corner_type;
			bool m_open;
		} m_corner;
	};

	c_ai_point3d m_point;
	c_ai_direction m_alignment;
};
static_assert(sizeof(c_ai_action) == 0x30);
