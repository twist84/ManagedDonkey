#pragma once

#include "cseries/cseries.hpp"

class c_player_output_user_iterator
{
public:
	void begin_player(int32 player_index);
	void begin_unit(int32 unit_index);
	int32 get_user_index();
	bool next();

private:
	int32 m_player_index;
	int32 m_output_user_index;
};

struct s_player_mapping_globals
{
	int16 active_input_user_count;
	int16 active_input_controller_count;
	c_static_array<int32, k_number_of_users> input_user_player_mapping;
	c_static_array<int32, k_number_of_users> input_user_unit_mapping;
	c_static_array<int32, k_number_of_users> input_controller_player_mapping;
	c_static_array<e_controller_index, 16> player_input_controller_mapping;
	c_static_array<int32, 16> player_input_user_mapping;
	int16 active_output_user_count;
	c_static_array<int32, k_number_of_users> output_user_player_mapping;
	c_static_array<int32, k_number_of_users> output_user_unit_mapping;
	c_static_array<uns8, 16> player_output_user_mapping;
};
static_assert(sizeof(s_player_mapping_globals) == 0xE8);

extern void __cdecl __tls_set_g_player_mapping_globals_allocator(void* address);
extern void __cdecl player_mapping_attach_output_user(int32 user_index, int32 player_index);
extern void __cdecl player_mapping_detach_output_users(int32 player_index);
extern void __cdecl player_mapping_dispose();
extern int32 __cdecl player_mapping_first_active_input_user();
extern int32 __cdecl player_mapping_first_active_output_user();
extern int32 __cdecl player_mapping_get_first_output_user(int32 player_index);
extern e_controller_index __cdecl player_mapping_get_input_controller(int32 player_index);
extern int32 __cdecl player_mapping_get_input_user(int32 player_index);
extern int32 __cdecl player_mapping_get_input_user_by_unit(int32 unit_index);
extern int32 __cdecl player_mapping_get_next_output_user(int32 player_index, int32 user_index);
extern int32 __cdecl player_mapping_get_player_by_input_controller(e_controller_index controller_index);
extern int32 __cdecl player_mapping_get_player_by_input_user(int32 input_user_index);
extern int32 __cdecl player_mapping_get_player_by_output_user(int32 user_index);
extern int32 __cdecl player_mapping_get_unit_by_input_user(int32 input_user_index);
extern int32 __cdecl player_mapping_get_unit_by_output_user(int32 user_index);
extern void __cdecl player_mapping_initialize();
extern int32 __cdecl player_mapping_input_controller_active_count();
extern bool __cdecl player_mapping_input_controller_is_active(e_controller_index controller_index);
extern int32 __cdecl player_mapping_input_user_active_count();
extern bool __cdecl player_mapping_input_user_is_active(int32 input_user_index);
extern void __cdecl player_mapping_input_user_set_unit(int32 input_user_index, int32 unit_index);
extern int32 __cdecl player_mapping_next_active_input_user(int32 input_user_index);
extern int32 __cdecl player_mapping_next_active_output_user(int32 user_index);
extern int32 __cdecl player_mapping_output_user_active_count();
extern bool __cdecl player_mapping_output_user_is_active(int32 user_index);
extern void __cdecl player_mapping_output_user_set_unit(int32 user_index, int32 unit_index);
extern void __cdecl player_mapping_reset();
extern void __cdecl player_mapping_set_input_controller(int32 player_index, e_controller_index controller_index);
extern void __cdecl player_mapping_set_input_user(int32 player_index, int32 input_user_index);
extern void __cdecl player_mapping_set_player_unit(int32 player_index, int32 unit_index);

