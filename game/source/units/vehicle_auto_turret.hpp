#pragma once

#include "cseries/cseries.hpp"
#include "objects/damage_owner.hpp"

struct c_vehicle_auto_turret
{
	long m_target_index;
	long m_target_time;
	long m_range_time;
	long m_lost_time;
	s_damage_owner m_damage_owner;
	bool m_damage_owner_valid;
	long m_volume_index;
	real m_min_range;
	real m_alt_range;
	real m_alt_time;
};
static_assert(sizeof(c_vehicle_auto_turret) == 0x30);

