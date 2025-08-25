#pragma once

#include "cseries/cseries.hpp"

class c_character_physics_component
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
	int16 m_flags;
	uns8 m_mode;
	uns8 m_collision_damage_imunity_counter;
	int32 m_object_index;
	byte m_character_physics_mode_datum_buffer[0x78];
};
static_assert(sizeof(c_character_physics_component) == 0x80); // $TODO find the exact size

