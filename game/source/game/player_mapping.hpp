#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

long const k_number_of_input_users = 4;

struct s_player_mapping_globals
{
	short active_input_user_count;
	short active_input_controller_count;
	long input_user_player_mapping[k_number_of_input_users];
	long input_user_unit_mapping[k_number_of_input_users]; // datum_index
	long input_controller_player_mapping[k_number_of_input_users];
	long player_input_controller_mapping[16];
	long player_input_user_mapping[16]; // datum_index
	short active_output_user_count;
	short : 16;
	long output_user_player_mapping[k_number_of_input_users];
	long output_user_unit_mapping[k_number_of_input_users];
	byte player_output_user_mapping[16];
};
static_assert(sizeof(s_player_mapping_globals) == 0xE8);

extern long __cdecl player_index_from_user_index(long user_index);
extern bool __cdecl players_user_is_active(long user_index);
extern long __cdecl player_mapping_get_player_count();
extern long __cdecl player_mapping_first_active_output_user();
extern long __cdecl player_mapping_get_unit_by_output_user(long output_user_index);
extern long __cdecl player_mapping_get_unit_by_input_user(long user_index);
extern long __cdecl player_mapping_output_user_active_count();

