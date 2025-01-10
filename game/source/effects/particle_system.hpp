#pragma once

#include "cseries/cseries.hpp"

enum e_effect_pass;

struct c_particle_system :
	s_datum_header
{
public:
	static void __cdecl submit_all(long user_index, e_effect_pass effect_pass);

	real m_age;

	// 1: ever_frame_advanced
	// 2: die_when_orphaned
	// 3: should_live
	// 4: should_pulse
	// 5: should_frame_advance
	// 6: should_render
	word_flags m_flags;

	word m_particle_system_index;
	long m_definition_index;
	byte m_event_index;
	byte m_priority;
	word m_event_counter;
	long m_effect_index;
	long m_lightprobe_index;
	long m_breakable_surface_event_index;
	s_location location;
	word_flags __flags22;
	real m_effect_scale_a;
	real m_effect_scale_b;
	dword m_random_seed;
	real m_real_random_seed[4];
	long m_first_location_index;
	long m_next_location_index;
	long __unknown48;
	long __unknown4C;
	long m_object_index;
	byte m_first_person_user_mask[2];
	short m_particle_count;
};
static_assert(sizeof(c_particle_system) == 0x58);

