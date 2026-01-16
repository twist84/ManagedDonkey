#pragma once

#include "cseries/cseries.hpp"

class c_ai_point3d
{
public:
	int16 bsp_index() const
	{
		return m_bsp_index;
	}

	const real_point3d* point() const
	{
		return &m_point;
	}

	int16 reference_frame() const
	{
		return m_reference_frame;
	}

	void set(const real_point3d* point)
	{
		set(point, NONE);
	}

	void set(const real_point3d* point, int16 reference_frame)
	{
		set(point, reference_frame, NONE);
	}

	void set(const real_point3d* point, int16 reference_frame, int16 bsp_index)
	{
		m_point = *point;
		m_reference_frame = reference_frame;
		m_bsp_index = bsp_index;
	}

private:
	real_point3d m_point;
	int16 m_reference_frame;
	int16 m_bsp_index;
};
COMPILE_ASSERT(sizeof(c_ai_point3d) == 0x10);

class c_ai_direction
{
public:
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
COMPILE_ASSERT(sizeof(c_ai_direction) == 0x14);

class c_ai_action
{
public:
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
COMPILE_ASSERT(sizeof(c_ai_action) == 0x30);
