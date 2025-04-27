#pragma once

#include "cseries/cseries.hpp"

struct c_character_physics_component
{
public:
	enum e_mode
	{
		_mode_none = 0,
		_mode_ground,
		_mode_flying,
		_mode_dead,
		_mode_posture,  // sentinel
		_mode_climbing, // sentinel
		_mode_melee,

		k_mode_count,

		_mode_first = _mode_none,
		_mode_last = _mode_melee,
	};

	e_mode get_mode() const;

protected:
	short __unknown0;
	c_enum<e_mode, uint8, _mode_first, k_mode_count> m_mode;
	uint8 m_collision_damage_immunity_duration;
	long m_object_index;
	uint8 __data8[0x78];
};
static_assert(sizeof(c_character_physics_component) == 0x80); // $TODO: find the exact size

