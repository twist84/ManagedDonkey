#pragma once

#include "cseries/cseries.hpp"

const int32 k_maximum_number_of_objectives = 10;

enum e_objective_state
{
	_objective_state_none = 0,
	_objective_state_unavailable,
	_objective_state_show,
	_objective_state_finish,

	k_objective_state_count
};

struct s_current_objective_state
{
	c_static_array<c_enum<e_objective_state, int8, _objective_state_none, k_objective_state_count>, k_maximum_number_of_objectives> primary_objective;
	c_static_array<c_enum<e_objective_state, int8, _objective_state_none, k_objective_state_count>, k_maximum_number_of_objectives> secondary_objective;
};
COMPILE_ASSERT(sizeof(s_current_objective_state) == 0x14);

