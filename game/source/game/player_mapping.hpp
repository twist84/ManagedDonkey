#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

struct s_player_mapping_globals
{
	short active_input_user_count;
	short active_input_controller_count;
	c_static_array<long, k_number_of_users> input_user_player_mapping;
	c_static_array<long, k_number_of_users> input_user_unit_mapping;
	c_static_array<long, k_number_of_users> input_controller_player_mapping;
	c_static_array<e_controller_index, 16> player_input_controller_mapping;
	c_static_array<long, 16> player_input_user_mapping;
	short active_output_user_count;
	c_static_array<long, k_number_of_users> output_user_player_mapping;
	c_static_array<long, k_number_of_users> output_user_unit_mapping;
	c_static_array<uint8, 16> player_output_user_mapping;
};
static_assert(sizeof(s_player_mapping_globals) == 0xE8);

extern void __cdecl __tls_set_g_player_mapping_globals_allocator(void* address);
extern void __cdecl player_mapping_attach_output_user(long user_index, long player_index);
extern void __cdecl player_mapping_detach_output_users(long player_index);
extern void __cdecl player_mapping_dispose();
extern long __cdecl player_mapping_first_active_input_user();
extern long __cdecl player_mapping_first_active_output_user();
extern long __cdecl player_mapping_get_first_output_user(long player_index);
extern e_controller_index __cdecl player_mapping_get_input_controller(long player_index);
extern long __cdecl player_mapping_get_input_user(long player_index);
extern long __cdecl player_mapping_get_input_user_by_unit(long unit_index);
extern long __cdecl player_mapping_get_next_output_user(long player_index, long user_index);
extern long __cdecl player_mapping_get_player_by_input_controller(e_controller_index controller_index);
extern long __cdecl player_mapping_get_player_by_input_user(long input_user_index);
extern long __cdecl player_mapping_get_player_by_output_user(long user_index);
extern long __cdecl player_mapping_get_unit_by_input_user(long input_user_index);
extern long __cdecl player_mapping_get_unit_by_output_user(long user_index);
extern void __cdecl player_mapping_initialize();
extern long __cdecl player_mapping_input_controller_active_count();
extern bool __cdecl player_mapping_input_controller_is_active(e_controller_index controller_index);
extern long __cdecl player_mapping_input_user_active_count();
extern bool __cdecl player_mapping_input_user_is_active(long input_user_index);
extern void __cdecl player_mapping_input_user_set_unit(long input_user_index, long unit_index);
extern long __cdecl player_mapping_next_active_input_user(long input_user_index);
extern long __cdecl player_mapping_next_active_output_user(long user_index);
extern long __cdecl player_mapping_output_user_active_count();
extern bool __cdecl player_mapping_output_user_is_active(long user_index);
extern void __cdecl player_mapping_output_user_set_unit(long user_index, long unit_index);
extern void __cdecl player_mapping_reset();
extern void __cdecl player_mapping_set_input_controller(long player_index, e_controller_index controller_index);
extern void __cdecl player_mapping_set_input_user(long player_index, long input_user_index);
extern void __cdecl player_mapping_set_player_unit(long player_index, long unit_index);

