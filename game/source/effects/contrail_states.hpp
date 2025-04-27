#pragma once

#include "cseries/cseries.hpp"

struct c_contrail_system;
struct c_contrail_location;
struct c_contrail;
struct c_contrail_profile;
struct c_contrail_states
{
	c_contrail_system const* m_contrail_system;
	c_contrail_location const* m_contrail_location;
	c_contrail const* m_contrail;
	c_contrail_profile const* m_contrail_profile;
	real32 m_state_values[16];
	uint32 m_valid_states;
	uint32 m_used_states;
};
static_assert(sizeof(c_contrail_states) == 0x58);

