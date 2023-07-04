#include "game/player_mapping.hpp"

#include "memory/thread_local.hpp"

s_player_mapping_globals* player_mapping_globals_get()
{
	if (!get_tls())
		return nullptr;

	TLS_DATA_GET_VALUE_REFERENCE(player_mapping_globals);
	return player_mapping_globals;
}

long player_index_from_user_index(long user_index)
{
	if (user_index == -1)
		return -1;

	return player_mapping_globals_get()->output_user_player_mapping[user_index];

	//ASSERT(user_index < 4);
	//s_player_mapping_globals* player_mapping_globals = player_mapping_globals_get();
	//datum_index input_user_index = player_mapping_globals->input_user_player_mapping[user_index];
	//if (input_user_index != -1)
	//{
	//	ASSERT(input_user_index < 16);
	//	ASSERT(player_mapping_globals->player_input_user_mapping[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)] == user_index);
	//}
	//return input_user_index;
}

bool players_user_is_active(long user_index)
{
	if (user_index == -1)
		return false;

	return player_mapping_globals_get()->output_user_player_mapping[user_index] != -1;
}

long player_mapping_get_player_count()
{
	return player_mapping_globals_get()->active_output_user_count;
}

long player_mapping_first_active_output_user()
{
	long index = 0;
	for (long* active_output_user = player_mapping_globals_get()->output_user_player_mapping; *active_output_user == -1; ++active_output_user)
	{
		if (++index >= 4)
			return -1;
	}

	return index;
}

long player_mapping_get_unit_by_output_user(long output_user_index)
{
	ASSERT(output_user_index != -1 && (output_user_index >= 0 && output_user_index < 4));

	return player_mapping_globals_get()->output_user_unit_mapping[output_user_index];
}

long player_mapping_output_user_active_count()
{
	return player_mapping_globals_get()->active_output_user_count;
}

