#pragma once

#include "cseries/cseries.hpp"
#include "game/player_control.hpp"
#include "game/players.hpp"

struct c_simulation_world;
struct c_simulation_player
{
	int m_player_index;
	int m_player_datum_index;
	int m_player_type;
	s_player_identifier m_player_identifier;
	s_machine_identifier m_player_machine_identifier;
	c_simulation_world* m_world;
	bool m_pending_deletion;
	bool m_active;
	char __data2A[2];
	int __unknown2C;
	player_action m_action;
};
static_assert(sizeof(c_simulation_player) == 0xB0);