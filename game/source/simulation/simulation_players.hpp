#pragma once

#include "cseries/cseries.hpp"
#include "game/player_control.hpp"
#include "game/players.hpp"

enum e_simulation_player_update_type
{
	_simulation_player_update_type_left_game = 0,
	_simulation_player_update_type_swap,
	_simulation_player_update_type_remove,
	_simulation_player_update_type_added,
	_simulation_player_update_type_configuration,

	k_simulation_player_update_type_count
};

struct simulation_player_update
{
	int32 player_index;
	s_player_identifier player_identifier;
	c_enum<e_simulation_player_update_type, int32, _simulation_player_update_type_left_game, k_simulation_player_update_type_count> update_type;
	s_machine_identifier machine_identifier;
	bool player_left_game;
	bool player_joined_in_progress;
	int16 user_index;
	int16 controller_index;

	byte __pad22[0x2];

	s_player_configuration configuration;
	int32 swap_player_index;
	s_player_identifier swap_player_identifier;

	byte __pad1654[0x4];
};
COMPILE_ASSERT(sizeof(simulation_player_update) == 0x1658);

// generated based on `e_simulation_player_update_type`
struct s_player_collection_player
{
	s_player_identifier identifier;
	bool left_game;
	int32 left_game_time;
	s_machine_identifier machine_identifier;

	// Removed from Halo Online, these still exist in `simulation_player_update`
	//int32 user_index;
	//int32 controller_index;

	s_player_configuration configuration_data;
};
COMPILE_ASSERT(sizeof(s_player_collection_player) == 0x1640);

struct s_player_collection
{
	uns32 player_valid_mask;
	uns32 __unknown4;
	s_player_collection_player collection_players[16];
};
COMPILE_ASSERT(sizeof(s_player_collection) == 0x16408);

class c_simulation_world;
class c_simulation_player
{
public:
	int32 m_player_index;
	int32 m_player_datum_index;
	int32 m_player_type;
	s_player_identifier m_player_identifier;
	s_machine_identifier m_player_machine_identifier;
	c_simulation_world* m_world;
	bool m_pending_deletion;
	bool m_active;
	int32 m_current_action_time;
	player_action m_current_action;
};
COMPILE_ASSERT(sizeof(c_simulation_player) == 0xB0);

extern void __cdecl simulation_player_collection_apply_update(s_player_collection* collection, const simulation_player_update* player_update);
extern void __cdecl simulation_player_collection_build(s_player_collection* collection);
extern void __cdecl simulation_player_collection_clear(s_player_collection* collection);
extern uns32 __cdecl simulation_player_collection_get_in_game_mask(const s_player_collection* collection);
extern void __cdecl simulation_player_collection_verify(const s_player_collection* collection);
extern bool __cdecl simulation_player_update_apply_added(const simulation_player_update* player_update);
extern bool __cdecl simulation_player_update_apply_configuration(const simulation_player_update* player_update);
extern bool __cdecl simulation_player_update_apply_left_game(const simulation_player_update* player_update);
extern bool __cdecl simulation_player_update_apply_remove(const simulation_player_update* player_update);
extern bool __cdecl simulation_player_update_apply_swap(const simulation_player_update* player_update);
extern void __cdecl simulation_player_update_generate_add(simulation_player_update* player_update, s_player_collection* players, int32 player_index, const s_player_identifier* player_identifier, const s_machine_identifier* machine_identifier, const s_player_configuration* player_data, bool joined_in_progress);
extern void __cdecl simulation_player_update_generate_configuration(simulation_player_update* player_update, s_player_collection* players, int32 player_index, const s_player_identifier* player_identifier, const s_player_configuration* player_data);
extern void __cdecl simulation_player_update_generate_left_game(simulation_player_update* player_update, s_player_collection* players, int32 player_index, const s_player_identifier* player_identifier);
extern void __cdecl simulation_player_update_generate_remove(simulation_player_update* player_update, s_player_collection* players, int32 player_index);
extern void __cdecl simulation_player_update_generate_swap(simulation_player_update* player_update, s_player_collection* players, int32 player_index, int32 swap_player_index);
extern bool __cdecl simulation_players_apply_update(const simulation_player_update* player_update);

