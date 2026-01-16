#pragma once

#include "cseries/cseries.hpp"

class c_aim_target_base
{
public:
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

class c_aim_target_object :
	public c_aim_target_base
{
public:
	union
	{
		int32 m_object_index;
		int32 m_laser_designation;
	};
};
COMPILE_ASSERT(sizeof(c_aim_target_object) == 0xC);

class c_aim_target_predicted :
	public c_aim_target_base
{
public:
	union
	{
		int32 m_entity_index;
		int32 m_laser_designation;
	};
};
COMPILE_ASSERT(sizeof(c_aim_target_predicted) == 0xC);

class c_aim_target_simulation_entity :
	public c_aim_target_base
{
public:
	bool m_data_valid;
	union
	{
		int32 m_aim_target;
		int32 m_laser_designation;
	};
	int32 m_gamestate_index;
};
COMPILE_ASSERT(sizeof(c_aim_target_simulation_entity) == 0x14);

