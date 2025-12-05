#pragma once

#include "cseries/cseries.hpp"
#include "cseries/location.hpp"

class c_particle_system :
	public s_datum_header
{
public:
	static void __cdecl submit_all(int32 user_index, e_effect_pass effect_pass);

	real32 m_age;

	// 1: ever_frame_advanced
	// 2: die_when_orphaned
	// 3: should_live
	// 4: should_pulse
	// 5: should_frame_advance
	// 6: should_render
	uns16 m_flags;

	uns16 m_particle_system_index;
	int32 m_definition_index;
	uns8 m_event_index;
	uns8 m_priority;
	uns16 m_event_counter;
	int32 m_effect_index;
	int32 m_lightprobe_index;
	int32 m_breakable_surface_event_index;
	s_location location;
	uns16 __flags22;
	real32 m_effect_scale_a;
	real32 m_effect_scale_b;
	uns32 m_random_seed;
	real32 m_real_random_seed[4];
	int32 m_first_location_index;
	int32 m_next_location_index;
	int32 __unknown48;
	int32 __unknown4C;
	int32 m_object_index;
	uns8 m_first_person_user_mask[2];
	int16 m_particle_count;
};
static_assert(sizeof(c_particle_system) == 0x58);

