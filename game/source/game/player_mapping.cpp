#include "game/player_mapping.hpp"

#include "game/players.hpp"
#include "memory/thread_local.hpp"

void __cdecl __tls_set_g_player_mapping_globals_allocator(void* address)
{
	INVOKE(0x00589620, __tls_set_g_player_mapping_globals_allocator, address);

	//player_mapping_globals = (s_player_mapping_globals*)address;
}

//.text:005895C0 ; 
//.text:005895E0 ; 
//.text:005895F0 ; 
//.text:00589600 ; 
//.text:00589610 ; 
//.text:00589620 ; void __cdecl __tls_set_g_player_mapping_globals_allocator(void*)
//.text:00589640 ; 
//.text:00589680 ; public: void __cdecl c_player_output_user_iterator::begin_player(int32)
//.text:005896A0 ; public: void __cdecl c_player_output_user_iterator::begin_unit(int32)
//.text:00589700 ; 
//.text:00589730 ; 
//.text:00589760 ; public: int32 __cdecl c_player_output_user_iterator::get_output_user_index()
//.text:00589770 ; public: bool __cdecl c_player_output_user_iterator::next()

void __cdecl player_mapping_attach_output_user(int32 user_index, int32 player_index)
{
	INVOKE(0x005897A0, player_mapping_attach_output_user, user_index, player_index);
}

void __cdecl player_mapping_detach_output_users(int32 player_index)
{
	INVOKE(0x005898D0, player_mapping_detach_output_users, player_index);
}

void __cdecl player_mapping_dispose()
{
	INVOKE(0x005899C0, player_mapping_dispose);

	//if (player_mapping_globals)
	//	player_mapping_globals = NULL;
}

int32 __cdecl player_mapping_first_active_input_user()
{
	//return INVOKE(0x005899F0, player_mapping_first_active_input_user);

	return player_mapping_next_active_input_user(NONE);
}

int32 __cdecl player_mapping_first_active_output_user()
{
	//return INVOKE(0x00589A30, player_mapping_first_active_output_user);

	return player_mapping_next_active_output_user(NONE);
}

int32 __cdecl player_mapping_get_first_output_user(int32 player_index)
{
	//return INVOKE(0x00589A70, player_mapping_get_first_output_user, player_index);

	return player_mapping_get_next_output_user(player_index, NONE);
}

e_controller_index __cdecl player_mapping_get_input_controller(int32 player_index)
{
	return INVOKE(0x00589AD0, player_mapping_get_input_controller, player_index);

	//ASSERT(player_index != NONE); //ASSERT(player_index != k_player_index_none);
	//
	//int32 player_absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index);
	//int32 controller_index = player_mapping_globals->player_input_controller_mapping[player_absolute_index];
	//if (controller_index != NONE)
	//	ASSERT(player_mapping_globals->input_controller_player_mapping[controller_index] == player_index);
	//
	//return static_cast<e_controller_index>(controller_index);
}

int32 __cdecl player_mapping_get_input_user(int32 player_index)
{
	return INVOKE(0x00589B00, player_mapping_get_input_user, player_index);

	//ASSERT(player_index != NONE); //ASSERT(player_index != k_player_index_none);
	//
	//int32 input_user_index = NONE;
	//if (player_index != NONE)
	//{
	//	int32 player_absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index);
	//	input_user_index = player_mapping_globals->player_input_user_mapping[player_absolute_index];
	//	if (input_user_index != NONE)
	//		ASSERT(player_mapping_globals->input_user_player_mapping[input_user_index] == player_index);
	//}
	//return input_user_index;
}

int32 __cdecl player_mapping_get_input_user_by_unit(int32 unit_index)
{
	return INVOKE(0x00589B30, player_mapping_get_input_user_by_unit, unit_index);

	//int32 input_user_index = NONE;
	//if (unit_index != NONE)
	//{
	//	unit_datum* unit = UNIT_GET(unit_index);
	//	if (unit->player_index != NONE)
	//		input_user_index = player_mapping_get_input_user(unit->player_index);
	//}
	//return input_user_index;
}

int32 __cdecl player_mapping_get_next_output_user(int32 player_index, int32 user_index)
{
	return INVOKE(0x00589B90, player_mapping_get_next_output_user, player_index, user_index);
}

int32 __cdecl player_mapping_get_player_by_input_controller(e_controller_index controller_index)
{
	return INVOKE(0x00589C00, player_mapping_get_player_by_input_controller, controller_index);
}

int32 __cdecl player_mapping_get_player_by_input_user(int32 input_user_index)
{
	return INVOKE(0x00589C30, player_mapping_get_player_by_input_user, input_user_index);
}

int32 __cdecl player_mapping_get_player_by_output_user(int32 user_index)
{
	return INVOKE(0x00589C60, player_mapping_get_player_by_output_user, user_index);
}

int32 __cdecl player_mapping_get_unit_by_input_user(int32 input_user_index)
{
	return INVOKE(0x00589C90, player_mapping_get_unit_by_input_user, input_user_index);
}

int32 __cdecl player_mapping_get_unit_by_output_user(int32 user_index)
{
	return INVOKE(0x00589CC0, player_mapping_get_unit_by_output_user, user_index);
}

void __cdecl player_mapping_initialize()
{
	INVOKE(0x00589CF0, player_mapping_initialize);
}

int32 __cdecl player_mapping_input_controller_active_count()
{
	return INVOKE(0x00589D50, player_mapping_input_controller_active_count);
}

bool __cdecl player_mapping_input_controller_is_active(e_controller_index controller_index)
{
	return INVOKE(0x00589D70, player_mapping_input_controller_is_active, controller_index);
}

int32 __cdecl player_mapping_input_user_active_count()
{
	return INVOKE(0x00589DA0, player_mapping_input_user_active_count);
}

bool __cdecl player_mapping_input_user_is_active(int32 input_user_index)
{
	return INVOKE(0x00589DC0, player_mapping_input_user_is_active, input_user_index);
}

void __cdecl player_mapping_input_user_set_unit(int32 input_user_index, int32 unit_index)
{
	INVOKE(0x00589DF0, player_mapping_input_user_set_unit, input_user_index, unit_index);
}

int32 __cdecl player_mapping_next_active_input_user(int32 input_user_index)
{
	return INVOKE(0x00589E60, player_mapping_next_active_input_user, input_user_index);
}

int32 __cdecl player_mapping_next_active_output_user(int32 user_index)
{
	return INVOKE(0x00589EB0, player_mapping_next_active_output_user, user_index);
}

int32 __cdecl player_mapping_output_user_active_count()
{
	return INVOKE(0x00589F00, player_mapping_output_user_active_count);
}

bool __cdecl player_mapping_output_user_is_active(int32 user_index)
{
	return INVOKE(0x00589F20, player_mapping_output_user_is_active, user_index);
}

void __cdecl player_mapping_output_user_set_unit(int32 user_index, int32 unit_index)
{
	INVOKE(0x00589F50, player_mapping_output_user_set_unit, user_index, unit_index);
}

void __cdecl player_mapping_reset()
{
	INVOKE(0x0058A000, player_mapping_reset);

	//event(_event_message, "player_mapping: reset");
	//
	//player_mapping_globals->active_input_user_count = 0;
	//player_mapping_globals->active_input_controller_count = 0;
	//player_mapping_globals->input_user_player_mapping.set_all(NONE);
	//player_mapping_globals->input_user_unit_mapping.set_all(NONE);
	//player_mapping_globals->input_controller_player_mapping.set_all(NONE);
	//player_mapping_globals->player_input_controller_mapping.set_all(NONE);
	//player_mapping_globals->player_input_user_mapping.set_all(NONE);
	//player_mapping_globals->active_output_user_count = 0;
	//player_mapping_globals->output_user_player_mapping.set_all(NONE);
	//player_mapping_globals->output_user_unit_mapping.set_all(NONE);
	//player_mapping_globals->player_output_user_mapping.set_all(NONE);
	players_verify();
}

void __cdecl player_mapping_set_input_controller(int32 player_index, e_controller_index controller_index)
{
	INVOKE(0x0058A140, player_mapping_set_input_controller, player_index, controller_index);
}

void __cdecl player_mapping_set_input_user(int32 player_index, int32 input_user_index)
{
	INVOKE(0x0058A1D0, player_mapping_set_input_user, player_index, input_user_index);
}

void __cdecl player_mapping_set_player_unit(int32 player_index, int32 unit_index)
{
	INVOKE(0x0058A2B0, player_mapping_set_player_unit, player_index, unit_index);
}

