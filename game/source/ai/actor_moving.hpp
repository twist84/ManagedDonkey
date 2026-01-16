#pragma once

#include "ai/actor_firing_position.hpp"
#include "ai/ai_actions.hpp"
#include "ai/sector.hpp"
#include "cseries/cseries.hpp"

class c_destination_orders
{
public:
	int16 m_destination_type;
	int16 m_flags;

	union
	{
		struct
		{
			c_ai_point3d point;
			c_sector_ref sector_ref;
		} m_raw;

		struct
		{
			firing_position_ref firing_position_index;
		} m_firing_position;

		struct
		{
			int32 prop_index;
		} m_prop;
	};

	int32 m_ignore_target_object_index;
	real32 m_scripted_speed;
	real32 m_scripted_goal_throttle;
	real32 m_destination_tolerance;
	real32 m_raw_path_destination_tolerance;
	real_vector3d m_destination_facing;
	c_ai_action m_destination_action;
};
COMPILE_ASSERT(sizeof(c_destination_orders) == 0x68);

