#pragma once

#include "cseries/cseries.hpp"
#include "objects/damage_owner.hpp"

struct c_vehicle_auto_turret
{
	long m_damage_owner_object_index;
	long __time4;
	long __time8;
	long __timeC;
	s_damage_owner m_damage_owner;
	bool m_damage_owner_set;
	long m_trigger_volume;
	real __unknown24;
	real __unknown28;
	real __unknown2C;
};
static_assert(sizeof(c_vehicle_auto_turret) == 0x30);

