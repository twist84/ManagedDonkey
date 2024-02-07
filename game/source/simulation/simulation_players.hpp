#pragma once

#include "cseries/cseries.hpp"
#include "game/player_control.hpp"
#include "game/players.hpp"

// #TODO: find a home
enum
{
	k_maximum_players = 16
};

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
	long player_index;
	s_player_identifier player_identifier;
	c_enum<e_simulation_player_update_type, long, _simulation_player_update_type_left_game, k_simulation_player_update_type_count> update_type;
	s_machine_identifier machine_identifier;
	bool player_left_game;
	bool player_joined_in_progress;
	short user_index;
	short controller_index;

	byte __pad22[0x2];

	s_player_configuration configuration;
	long swap_player_index;
	s_player_identifier swap_player_identifier;

	byte __pad1654[0x4];
};
static_assert(sizeof(simulation_player_update) == 0x1658);

// generated based on `e_simulation_player_update_type`
struct s_player_collection_player
{
	s_player_identifier identifier;
	bool left_game;
	dword left_game_time;
	s_machine_identifier machine_identifier;

	// Removed from Halo Online, these still exist in `simulation_player_update`
	//long user_index;
	//long controller_index;

	s_player_configuration configuration;
};
static_assert(sizeof(s_player_collection_player) == 0x1640);

struct s_player_collection
{
	c_flags<long, dword, 16> player_valid_mask;
	dword __unknown4;
	s_player_collection_player collection_players[16];
};
static_assert(sizeof(s_player_collection) == 0x16408);

struct c_simulation_world;
struct c_simulation_player
{
	long m_player_index;
	long m_player_datum_index;
	long m_player_type;
	s_player_identifier m_player_identifier;
	s_machine_identifier m_player_machine_identifier;
	c_simulation_world* m_world;
	bool m_pending_deletion;
	bool m_active;
	byte __data2A[0x2];
	long __unknown2C;
	player_action m_action;
};
static_assert(sizeof(c_simulation_player) == 0xB0);

extern void __cdecl simulation_player_collection_apply_update(s_player_collection* collection, simulation_player_update const* player_update);
extern void __cdecl simulation_player_collection_build(s_player_collection* collection);
extern void __cdecl simulation_player_collection_clear(s_player_collection* collection);
extern dword __cdecl simulation_player_collection_get_in_game_mask(s_player_collection const* collection);
extern void __cdecl simulation_player_collection_verify(s_player_collection const* collection);
extern bool __cdecl simulation_player_update_apply_added(simulation_player_update const* player_update);
extern bool __cdecl simulation_player_update_apply_configuration(simulation_player_update const* player_update);
extern bool __cdecl simulation_player_update_apply_left_game(simulation_player_update const* player_update);
extern bool __cdecl simulation_player_update_apply_remove(simulation_player_update const* player_update);
extern bool __cdecl simulation_player_update_apply_swap(simulation_player_update const* player_update);
extern void __cdecl simulation_player_update_generate_add(simulation_player_update* player_update, s_player_collection* players, long player_index, s_player_identifier const* player_identifier, s_machine_identifier const* machine_identifier, s_player_configuration const* player_data, bool joined_in_progress);
extern void __cdecl simulation_player_update_generate_configuration(simulation_player_update* player_update, s_player_collection* players, long player_index, s_player_identifier const* player_identifier, s_player_configuration const* player_data);
extern void __cdecl simulation_player_update_generate_left_game(simulation_player_update* player_update, s_player_collection* players, long player_index, s_player_identifier const* player_identifier);
extern void __cdecl simulation_player_update_generate_remove(simulation_player_update* player_update, s_player_collection* players, long player_index);
extern void __cdecl simulation_player_update_generate_swap(simulation_player_update* player_update, s_player_collection* players, long player_index, long swap_player_index);
extern bool __cdecl simulation_players_apply_update(simulation_player_update const* player_update);

