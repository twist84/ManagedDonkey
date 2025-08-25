#include "simulation/simulation_players.hpp"

#include "game/game_time.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"

// cseries, const char* const g_zero_buffer;
char const g_zero_buffer[256]{};

//HOOK_DECLARE(0x004A7BC0, simulation_player_collection_apply_update);
//HOOK_DECLARE(0x004A7D70, simulation_player_collection_build);
//HOOK_DECLARE(0x004A7E50, simulation_player_collection_clear);
//HOOK_DECLARE(0x004A7E90, simulation_player_collection_get_in_game_mask);
//HOOK_DECLARE(0x004A7ED0, simulation_player_collection_verify);
//HOOK_DECLARE(0x004A7EE0, simulation_player_update_apply_added);
//HOOK_DECLARE(0x004A8160, simulation_player_update_apply_configuration);
//HOOK_DECLARE(0x004A81D0, simulation_player_update_apply_left_game);
//HOOK_DECLARE(0x004A8240, simulation_player_update_apply_remove);
//HOOK_DECLARE(0x004A82B0, simulation_player_update_apply_swap);
//HOOK_DECLARE(0x004A8390, simulation_player_update_generate_add);
//HOOK_DECLARE(0x004A8400, simulation_player_update_generate_configuration);
//HOOK_DECLARE(0x004A8450, simulation_player_update_generate_left_game);
//HOOK_DECLARE(0x004A8480, simulation_player_update_generate_remove);
//HOOK_DECLARE(0x004A84C0, simulation_player_update_generate_swap);
//HOOK_DECLARE(0x004A8560, simulation_players_apply_update);

void __cdecl simulation_player_collection_apply_update(s_player_collection* collection, const simulation_player_update* player_update)
{
	INVOKE(0x004A7BC0, simulation_player_collection_apply_update, collection, player_update);

	//ASSERT(collection);
	//ASSERT(player_update);
	//ASSERT(player_update->player_index >= 0 && player_update->player_index < k_maximum_players);
	//
	//e_simulation_player_update_type type = player_update->update_type;
	//event(_event_verbose, "networking:simulation:players: player-collection test-applying type %d to player %d at time %d",
	//	type,
	//	player_update->player_index,
	//	game_time_get());
	//
	//simulation_player_collection_verify(collection);
	//
	//s_player_collection_player* collection_player = &collection->collection_players[player_update->player_index];
	//
	//switch (type)
	//{
	//case _simulation_player_update_type_left_game:
	//{
	//	ASSERT(TEST_BIT(collection->player_valid_mask, player_update->player_index));
	//	ASSERT(!collection_player->left_game);
	//	ASSERT(collection_player->left_game_time == NONE);
	//	ASSERT(csmemcmp(&player_update->player_identifier, &collection_player->identifier, sizeof(player_update->player_identifier)) == 0);
	//
	//	collection_player->left_game = true;
	//	collection_player->left_game_time = game_time_get();
	//	collection_player->machine_identifier = {};
	//}
	//break;
	//case _simulation_player_update_type_swap:
	//{
	//	ASSERT(player_update->swap_player_index >= 0 && player_update->swap_player_index < k_maximum_players);
	//	ASSERT(TEST_BIT(collection->player_valid_mask, player_update->swap_player_index));
	//
	//	s_player_collection_player* swap_player = &collection->collection_players[player_update->swap_player_index];
	//
	//	bool player1_valid = TEST_BIT(collection->player_valid_mask, player_update->player_index);
	//	bool player2_valid = TEST_BIT(collection->player_valid_mask, player_update->swap_player_index);
	//
	//	bool player1_left_game = swap_player->left_game;
	//	uns32 player1_left_game_time = swap_player->left_game_time;
	//
	//	bool player2_left_game = swap_player->left_game;
	//	uns32 player2_left_game_time = swap_player->left_game_time;
	//
	//	ASSERT(player1_valid || player2_valid);
	//	ASSERT(player2_valid == player2_left_game);
	//
	//	ASSERT(csmemcmp(&player_update->player_identifier, &collection_player->identifier, sizeof(player_update->player_identifier)) == 0);
	//	if (player1_valid)
	//	{
	//		ASSERT(player1_left_game);
	//	}
	//	else
	//	{
	//		ASSERT(csmemcmp(&player_update->player_identifier, g_zero_buffer, sizeof(player_update->player_identifier)) == 0);
	//	}
	//
	//	ASSERT(csmemcmp(&player_update->swap_player_identifier, &swap_player->identifier, sizeof(player_update->swap_player_identifier)) == 0);
	//	if (player2_valid)
	//	{
	//		ASSERT(player2_left_game);
	//	}
	//	else
	//	{
	//		ASSERT(csmemcmp(&player_update->swap_player_identifier, g_zero_buffer, sizeof(player_update->swap_player_identifier)) == 0);
	//	}
	//
	//	SET_BIT(collection->player_valid_mask, player_update->player_index, player2_valid != 0);
	//	swap_player->identifier = player_update->swap_player_identifier;
	//	swap_player->left_game = player2_left_game;
	//	swap_player->left_game_time = player2_left_game_time;
	//
	//	SET_BIT(collection->player_valid_mask, player_update->swap_player_index, player1_valid != 0);
	//	swap_player->identifier = player_update->player_identifier;
	//	swap_player->left_game = player1_left_game;
	//	swap_player->left_game_time = player1_left_game_time;
	//}
	//break;
	//case _simulation_player_update_type_remove:
	//{
	//	ASSERT(TEST_BIT(collection->player_valid_mask, player_update->player_index));
	//	ASSERT(collection_player->left_game);
	//	ASSERT(collection_player->left_game_time >= 0);
	//	ASSERT(csmemcmp(&player_update->player_identifier, &collection_player->identifier, sizeof(player_update->player_identifier)) == 0);
	//
	//	SET_BIT(collection->player_valid_mask, player_update->player_index, false);
	//	collection_player->identifier = {};
	//	collection_player->left_game = false;
	//	collection_player->left_game_time = NONE;
	//}
	//break;
	//case _simulation_player_update_type_added:
	//{
	//	if (TEST_BIT(collection->player_valid_mask, player_update->player_index))
	//	{
	//		ASSERT(collection_player->left_game);
	//		ASSERT(collection_player->left_game_time >= 0);
	//		ASSERT(csmemcmp(&player_update->player_identifier, &collection_player->identifier, sizeof(player_update->player_identifier)) == 0);
	//		ASSERT(!player_update->player_left_game);
	//
	//		collection_player->left_game = false;
	//		collection_player->left_game_time = NONE;
	//	}
	//	else
	//	{
	//		ASSERT(!collection_player->left_game);
	//		ASSERT(collection_player->left_game_time == NONE);
	//		ASSERT(csmemcmp(&collection_player->identifier, g_zero_buffer, sizeof(collection_player->identifier)) == 0);
	//		ASSERT(csmemcmp(&player_update->player_identifier, g_zero_buffer, sizeof(player_update->player_identifier)) != 0);
	//
	//		SET_BIT(collection->player_valid_mask, player_update->player_index, true);
	//		collection_player->identifier = player_update->player_identifier;
	//		if (player_update->player_left_game)
	//		{
	//			collection_player->left_game = true;
	//			collection_player->left_game_time = game_time_get();
	//		}
	//	}
	//	collection_player->configuration = player_update->configuration;
	//}
	//break;
	//case _simulation_player_update_type_configuration:
	//{
	//	ASSERT(TEST_BIT(collection->player_valid_mask, player_update->player_index));
	//	ASSERT(!collection_player->left_game);
	//	ASSERT(collection_player->left_game_time == NONE);
	//	ASSERT(csmemcmp(&player_update->player_identifier, &collection_player->identifier, sizeof(player_update->player_identifier)) == 0);
	//
	//	csmemcpy(&collection_player->configuration, &player_update->configuration, sizeof(collection_player->configuration));
	//}
	//break;
	//default:
	//	VASSERT("unreachable");
	//	break;
	//}
	//
	//simulation_player_collection_verify(collection);
}

void __cdecl simulation_player_collection_build(s_player_collection* collection)
{
	INVOKE(0x004A7D70, simulation_player_collection_build, collection);

	//ASSERT(collection);
	//ASSERT(collection->player_valid_mask == 0);
	//
	//c_data_iterator<player_datum> player_iterator;
	//player_iterator.begin(*player_data);
	//while (player_iterator.next())
	//{
	//	int32 player_absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(player_iterator.get_index());
	//	player_datum* player = player_iterator.get_datum();
	//
	//	s_player_collection_player* collection_player = &collection->collection_players[player_absolute_index];
	//	ASSERT(player_absolute_index >= 0 && player_absolute_index < k_maximum_players);
	//	ASSERT(!TEST_BIT(collection->player_valid_mask, player_absolute_index));
	//
	//	SET_BIT(collection->player_valid_mask, player_absolute_index, true);
	//	collection_player->identifier = player->player_identifier;
	//
	//	if (TEST_BIT(player->flags, _player_left_game_bit))
	//	{
	//		collection_player->left_game = true;
	//		collection_player->left_game_time = player->left_game_time;
	//		collection_player->machine_identifier = {};
	//		//collection_player->controller_index = k_no_controller;
	//		//collection_player->user_index = NONE;
	//	}
	//	else
	//	{
	//		collection_player->left_game = false;
	//		collection_player->left_game_time = NONE;
	//		collection_player->machine_identifier = player->machine_identifier;
	//		//collection_player->controller_index = player->machine_controller_index;
	//		//collection_player->user_index = player->machine_user_index;
	//		csmemcpy(&collection_player->configuration, &player->desired_configuration, sizeof(collection_player->configuration));
	//	}
	//}
}

void __cdecl simulation_player_collection_clear(s_player_collection* collection)
{
	INVOKE(0x004A7E50, simulation_player_collection_clear, collection);

	//ASSERT(collection);
	//
	//csmemset(collection, 0, sizeof(s_player_collection));
	//for (int32 player_index = 0; player_index < k_maximum_players; player_index++)
	//{
	//	s_player_collection_player* collection_player = &collection->collection_players[player_index];
	//	collection_player->left_game = false;
	//	collection_player->left_game_time = NONE;
	//	//collection_player->controller_index = k_no_controller;
	//	//collection_player->user_index = NONE;
	//}
}

uns32 simulation_player_collection_get_in_game_mask(const s_player_collection* collection)
{
	return INVOKE(0x004A7E90, simulation_player_collection_get_in_game_mask, collection);

	//ASSERT(collection);
	//
	//uns32 in_game_mask = 0;
	//for (int32 player_index = 0; player_index < k_maximum_players; player_index++)
	//{
	//	if (TEST_BIT(collection->player_valid_mask, player_index))
	//		SET_BIT(in_game_mask, player_index, !collection->collection_players[player_index].left_game);
	//}
	//return in_game_mask;
}

void __cdecl simulation_player_collection_verify(const s_player_collection* collection)
{
	INVOKE(0x004A7ED0, simulation_player_collection_verify, collection);

	//ASSERT(collection);
	//
	//for (int32 player_index = 0; player_index < k_maximum_players; player_index++)
	//{
	//	const s_player_collection_player* collection_player = &collection->collection_players[player_index];
	//	if (TEST_BIT(collection->player_valid_mask, player_index))
	//	{
	//		ASSERT(csmemcmp(&collection_player->identifier, g_zero_buffer, sizeof(collection_player->identifier)) != 0);
	//
	//		if (collection_player->left_game)
	//		{
	//			ASSERT(collection_player->left_game_time >= 0);
	//			ASSERT(csmemcmp(&collection_player->machine_identifier, g_zero_buffer, sizeof(collection_player->machine_identifier)) == 0);
	//			//ASSERT(collection_player->user_index == NONE);
	//			//ASSERT(collection_player->controller_index == k_no_controller);
	//		}
	//		else
	//		{
	//			ASSERT(collection_player->left_game_time == NONE);
	//			//ASSERT(collection_player->user_index >= 0 && collection_player->user_index < k_number_of_users);
	//			//ASSERT(collection_player->controller_index >= 0 && collection_player->controller_index < k_number_of_controllers);
	//		}
	//
	//		//for (int32 test_player_index = player_index + 1; test_player_index < k_maximum_players; test_player_index++)
	//		//{
	//		//	if (TEST_BIT(collection->player_valid_mask, test_player_index))
	//		//	{
	//		//		const s_player_collection_player* test_player = &collection->collection_players[test_player_index];
	//		//		ASSERT(csmemcmp(&collection_player->identifier, &test_player->identifier, sizeof(collection_player->identifier)) != 0);
	//		//
	//		//		if (collection_player->left_game &&
	//		//			!test_player->left_game &&
	//		//			!csmemcmp(&collection_player->machine_identifier, &test_player->machine_identifier, sizeof(collection_player->machine_identifier)))
	//		//		{
	//		//			ASSERT(collection_player->user_index != test_player->user_index);
	//		//			ASSERT(collection_player->controller_index != test_player->controller_index);
	//		//		}
	//		//	}
	//		//}
	//
	//		continue;
	//	}
	//
	//	ASSERT(csmemcmp(&collection_player->identifier, g_zero_buffer, sizeof(collection_player->identifier)) == 0);
	//	ASSERT(!collection_player->left_game);
	//	ASSERT(collection_player->left_game_time == NONE);
	//	ASSERT(csmemcmp(&collection_player->machine_identifier, g_zero_buffer, sizeof(collection_player->machine_identifier)) == 0);
	//	//ASSERT(collection_player->user_index == NONE);
	//	//ASSERT(collection_player->controller_index == k_no_controller);
	//}
}

bool __cdecl simulation_player_update_apply_added(const simulation_player_update* player_update)
{
	return INVOKE(0x004A7EE0, simulation_player_update_apply_added, player_update);

	//ASSERT(player_update);
	//ASSERT(player_update->update_type == _simulation_player_update_type_added);
	//
	//player_datum* player = &player_data[player_update->player_index];
	//if (!player)
	//{
	//	event(_event_error, "networking:simulation:players:apply-configuration: simulation asked us to update unknown player [#%d]",
	//		player_update->player_index);
	//
	//	return false;
	//}
	//
	//if (csmemcmp(&player->player_identifier, &player_update->player_identifier, sizeof(player->player_identifier)))
	//{
	//	event(_event_error, "networking:simulation:players:apply-configuration: identifier mismatch updating player [#%d]",
	//		player_update->player_index);
	//
	//	return false;
	//}
	//
	//int32 player_index = player_data->get_index(player_update->player_index);
	//player_set_configuration(player_index, &player_update->configuration);
	//
	//event(_event_message, "networking:simulation:players:apply-configuration: player [#%d] configuration updated",
	//	player_update->player_index);
	//
	//return true;
}

bool __cdecl simulation_player_update_apply_configuration(const simulation_player_update* player_update)
{
	return INVOKE(0x004A8160, simulation_player_update_apply_configuration, player_update);

	//ASSERT(player_update);
	//ASSERT(player_update->update_type == _simulation_player_update_type_configuration);
	// $IMPLEMENT
}

bool __cdecl simulation_player_update_apply_left_game(const simulation_player_update* player_update)
{
	return INVOKE(0x004A81D0, simulation_player_update_apply_left_game, player_update);

	//ASSERT(player_update);
	//ASSERT(player_update->update_type == _simulation_player_update_type_left_game);
	// $IMPLEMENT
}

bool __cdecl simulation_player_update_apply_remove(const simulation_player_update* player_update)
{
	return INVOKE(0x004A8240, simulation_player_update_apply_remove, player_update);

	//ASSERT(player_update);
	//ASSERT(player_update->update_type == _simulation_player_update_type_remove);
	// $IMPLEMENT
}

bool __cdecl simulation_player_update_apply_swap(const simulation_player_update* player_update)
{
	return INVOKE(0x004A82B0, simulation_player_update_apply_swap, player_update);

	//ASSERT(player_update);
	//ASSERT(player_update->update_type == _simulation_player_update_type_swap);
	// $IMPLEMENT
}

//void __cdecl simulation_player_update_generate_add(simulation_player_update* player_update, s_player_collection* players, int32 player_index, const s_player_identifier* player_identifier, const s_machine_identifier* machine_identifier, int32 machine_user_index, e_controller_index machine_controller_index, const s_player_configuration* player_data, bool joined_in_progress)
void __cdecl simulation_player_update_generate_add(simulation_player_update* player_update, s_player_collection* players, int32 player_index, const s_player_identifier* player_identifier, const s_machine_identifier* machine_identifier, const s_player_configuration* player_data, bool joined_in_progress)
{
	INVOKE(0x004A8390, simulation_player_update_generate_add, player_update, players, player_index, player_identifier, machine_identifier, player_data, joined_in_progress);

	//ASSERT(player_update);
	//ASSERT(players);
	//ASSERT(player_index >= 0 && player_index < k_maximum_players);
	//ASSERT(player_identifier);
	//ASSERT(machine_identifier);
	////ASSERT(machine_user_index >= 0 && machine_user_index < k_number_of_users);
	////ASSERT(machine_controller_index >= 0 && machine_controller_index < k_number_of_controllers);
	//ASSERT(player_data);
	//
	//if (TEST_BIT(players->player_valid_mask, player_index))
	//{
	//	s_player_collection_player* collection_player = &players->collection_players[player_index];
	//	ASSERT(collection_player->left_game);
	//	ASSERT(csmemcmp(&collection_player->identifier, player_identifier, sizeof(collection_player->identifier)) == 0);
	//}
	//
	//player_update->update_type = _simulation_player_update_type_added;
	//player_update->player_index = player_index;
	//player_update->player_identifier = *player_identifier;
	//player_update->player_left_game = false;
	//player_update->player_joined_in_progress = joined_in_progress;
	//player_update->machine_identifier = *machine_identifier;
	//csmemcpy(&player_update->configuration, player_data, sizeof(player_update->configuration));
	//
	//simulation_player_collection_apply_update(players, player_update);
}

void __cdecl simulation_player_update_generate_configuration(simulation_player_update* player_update, s_player_collection* players, int32 player_index, const s_player_identifier* player_identifier, const s_player_configuration* player_data)
{
	INVOKE(0x004A8400, simulation_player_update_generate_configuration, player_update, players, player_index, player_identifier, player_data);

	//s_player_collection_player* collection_player = &players->collection_players[player_index];
	//
	//ASSERT(player_update);
	//ASSERT(players);
	//ASSERT(player_index >= 0 && player_index < k_maximum_players);
	//ASSERT(player_identifier);
	//ASSERT(player_data);
	//
	//player_update->update_type = _simulation_player_update_type_configuration;
	//
	//ASSERT(TEST_BIT(players->player_valid_mask, player_index));
	//ASSERT(!collection_player->left_game);
	//ASSERT(csmemcmp(&collection_player->identifier, player_identifier, sizeof(collection_player->identifier)) == 0);
	//
	//player_update->player_index = player_index;
	//player_update->player_identifier = *player_identifier;
	//csmemcpy(&player_update->configuration, player_data, sizeof(player_update->configuration));
	//
	//simulation_player_collection_apply_update(players, player_update);
}

void __cdecl simulation_player_update_generate_left_game(simulation_player_update* player_update, s_player_collection* players, int32 player_index, const s_player_identifier* player_identifier)
{
	INVOKE(0x004A8450, simulation_player_update_generate_left_game, player_update, players, player_index, player_identifier);

	//ASSERT(player_update);
	//ASSERT(players);
	//ASSERT(player_index >= 0 && player_index < k_maximum_players);
	//
	//player_update->update_type = _simulation_player_update_type_left_game;
	//player_update->player_index = player_index;
	//player_update->player_identifier = *player_identifier;
	//
	//simulation_player_collection_apply_update(players, player_update);
}

void __cdecl simulation_player_update_generate_remove(simulation_player_update* player_update, s_player_collection* players, int32 player_index)
{
	INVOKE(0x004A8480, simulation_player_update_generate_remove, player_update, players, player_index);

	//s_player_collection_player* collection_player = &players->collection_players[player_index];
	//
	//ASSERT(player_update);
	//ASSERT(players);
	//ASSERT(player_index >= 0 && player_index < k_maximum_players);
	//
	//player_update->update_type = _simulation_player_update_type_remove;
	//
	//ASSERT(TEST_BIT(players->player_valid_mask, player_index));
	//ASSERT(collection_player->left_game);
	//
	//player_update->player_index = player_index;
	//simulation_player_collection_apply_update(players, player_update);
}

void __cdecl simulation_player_update_generate_swap(simulation_player_update* player_update, s_player_collection* players, int32 player_index, int32 swap_player_index)
{
	INVOKE(0x004A84C0, simulation_player_update_generate_swap, player_update, players, player_index, swap_player_index);

	//ASSERT(player_update);
	//ASSERT(players);
	//ASSERT(player_index >= 0 && player_index < k_maximum_players);
	//ASSERT(swap_player_index >= 0 && swap_player_index < k_maximum_players);
	//
	//player_update->update_type = _simulation_player_update_type_swap;
	//
	//player_update->player_index = player_index;
	//if (TEST_BIT(players->player_valid_mask, player_index))
	//{
	//	s_player_collection_player* collection_player = &players->collection_players[player_index];
	//	ASSERT(collection_player->left_game);
	//	player_update->player_identifier = players->collection_players[player_index].identifier;
	//}
	//else
	//{
	//	player_update->player_identifier = {};
	//}
	//
	//player_update->swap_player_index = swap_player_index;
	//if (TEST_BIT(players->player_valid_mask, swap_player_index))
	//{
	//	s_player_collection_player* swap_player = &players->collection_players[swap_player_index];
	//	ASSERT(swap_player->left_game);
	//	player_update->player_identifier = players->collection_players[player_index].identifier;
	//}
	//else
	//{
	//	player_update->swap_player_identifier = {};
	//}
	//
	//simulation_player_collection_apply_update(players, player_update);
}

bool __cdecl simulation_players_apply_update(const simulation_player_update* player_update)
{
	return INVOKE(0x004A8560, simulation_players_apply_update, player_update);

	//ASSERT(player_update);
	//
	//e_simulation_player_update_type type = player_update->update_type;
	//event(_event_verbose, "networking:simulation:players: for-real applying type %d to player %d at time %d",
	//	type,
	//	player_update->player_index,
	//	game_time_get());
	//
	//switch (type)
	//{
	//case _simulation_player_update_type_left_game:
	//	return simulation_player_update_apply_left_game(player_update);
	//case _simulation_player_update_type_swap:
	//	return simulation_player_update_apply_swap(player_update);
	//case _simulation_player_update_type_remove:
	//	return simulation_player_update_apply_remove(player_update);
	//case _simulation_player_update_type_added:
	//	return simulation_player_update_apply_added(player_update);
	//case _simulation_player_update_type_configuration:
	//	return simulation_player_update_apply_configuration(player_update);
	//}
	//
	//return false;
}

