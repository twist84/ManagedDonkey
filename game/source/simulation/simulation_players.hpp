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

// generated based on `e_simulation_player_update_type`
struct s_player_collection_player
{
	s_player_identifier identifier;
	bool left_game;
	dword left_game_time;
	s_machine_identifier machine_identifier;
	s_player_configuration configuration;
};
static_assert(sizeof(s_player_collection_player) == 0x1640);

struct s_player_collection
{
	dword player_valid_mask;
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

