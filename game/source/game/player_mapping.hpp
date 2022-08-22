#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_player_mapping_globals
{
	short active_input_user_count;
	short active_input_controller_count;
	long input_user_player_mapping[4];
	datum_index input_user_unit_mapping[4];
	long input_controller_player_mapping[4];
	long player_input_controller_mapping[16];
	datum_index player_input_user_mapping[16];
	short active_output_user_count;
	short : 16;
	long output_user_player_mapping[4];
	long output_user_unit_mapping[4];
	byte player_output_user_mapping[16];
};
static_assert(sizeof(s_player_mapping_globals) == 0xE8);

extern long player_index_from_user_index(long user_index);
extern bool players_user_is_active(long user_index);
extern long player_mapping_get_player_count();