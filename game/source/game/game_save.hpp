#pragma once

#include "cseries/cseries.hpp"

enum e_game_save_type
{
	_game_save_type_cancel = 0,
	_game_save_type_safe,
	_game_save_type_no_timeout,
	_game_save_type_immediate,
	_game_save_type_cinematic_skip,

	k_game_save_type_count
};

struct s_game_save_globals
{
	// e_game_save_type
	long game_save_type;

	// timeout?
	long __unknown4;

	// timeout?
	long __unknown8;

	// attempt count?
	long __unknownC;

	long time_of_last_game_save;

	long unsafe_object_index;
};
static_assert(sizeof(s_game_save_globals) == 0x18);

