#pragma once

#include "cseries/cseries.hpp"

struct c_contrail_system;
struct c_contrail_location;
struct c_contrail;
struct c_contrail_profile;
struct c_contrail_states
{
	c_contrail_system* m_contrail_system;
	c_contrail_location* m_contrail_location;
	c_contrail* m_contrail;
	c_contrail_profile* m_contrail_profile;
	real m_state_values[15];
	real __unknown4C;
	dword_flags m_flags;
	dword m_used_states;
};
static_assert(sizeof(c_contrail_states) == 0x58);

