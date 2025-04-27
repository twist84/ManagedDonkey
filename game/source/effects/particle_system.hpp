#pragma once

#include "cseries/cseries.hpp"

struct c_particle_system :
	s_datum_header
{
public:
	static void __cdecl submit_all(long user_index, e_effect_pass effect_pass);

	real32 m_age;

	// 1: ever_frame_advanced
	// 2: die_when_orphaned
	// 3: should_live
	// 4: should_pulse
	// 5: should_frame_advance
	// 6: should_render
	uint16 m_flags;

	uint16 m_particle_system_index;
	long m_definition_index;
	uint8 m_event_index;
	uint8 m_priority;
	uint16 m_event_counter;
	long m_effect_index;
	long m_lightprobe_index;
	long m_breakable_surface_event_index;
	s_location location;
	uint16 __flags22;
	real32 m_effect_scale_a;
	real32 m_effect_scale_b;
	uint32 m_random_seed;
	real32 m_real_random_seed[4];
	long m_first_location_index;
	long m_next_location_index;
	long __unknown48;
	long __unknown4C;
	long m_object_index;
	uint8 m_first_person_user_mask[2];
	short m_particle_count;
};
static_assert(sizeof(c_particle_system) == 0x58);

