#pragma once

#include "cseries/cseries.hpp"

struct c_aim_target_base
{
	enum e_aim_target_type
	{
		_aim_target_type_none = 0,
		_aim_target_type_object,
		_aim_target_type_laser_designation,

		k_aim_target_type_count
	};
	c_enum<e_aim_target_type, uns8, _aim_target_type_none, k_aim_target_type_count> m_target_type;
	int32 m_model_index;
};

struct c_aim_target_object :
	c_aim_target_base
{
	union
	{
		int32 m_object_index;
		int32 m_laser_designation;
	};
};
static_assert(sizeof(c_aim_target_object) == 0xC);

struct c_aim_target_predicted :
	c_aim_target_base
{
	union
	{
		int32 m_entity_index;
		int32 m_laser_designation;
	};
};
static_assert(sizeof(c_aim_target_predicted) == 0xC);

struct c_aim_target_simulation_entity :
	c_aim_target_base
{
	bool m_data_valid;
	union
	{
		int32 m_aim_target;
		int32 m_laser_designation;
	};
	int32 m_gamestate_index;
};
static_assert(sizeof(c_aim_target_simulation_entity) == 0x14);

