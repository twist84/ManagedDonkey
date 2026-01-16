#pragma once

#include "cseries/cseries.hpp"

class c_contrail_system;
class c_contrail_location;
class c_contrail;
class c_contrail_profile;
class c_contrail_states
{
	const c_contrail_system* m_contrail_system;
	const c_contrail_location* m_contrail_location;
	const c_contrail* m_contrail;
	const c_contrail_profile* m_contrail_profile;
	real32 m_state_values[16];
	uns32 m_valid_states;
	uns32 m_used_states;
};
COMPILE_ASSERT(sizeof(c_contrail_states) == 0x58);

