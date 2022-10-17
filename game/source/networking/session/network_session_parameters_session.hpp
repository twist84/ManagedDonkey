#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_parameters_generic.hpp"

enum e_network_session_mode
{
	_network_session_mode_none = 0,
	_network_session_mode_idle,
	_network_session_mode_setup,
	_network_session_mode_in_game,
	_network_session_mode_end_game,
	_network_session_mode_post_game,
	_network_session_mode_matchmaking_start,
	_network_session_mode_matchmaking_searching,
	_network_session_mode_matchmaking_gathering,
	_network_session_mode_matchmaking_slave,
	_network_session_mode_matchmaking_disbanding,
	_network_session_mode_matchmaking_arbitrating,
	_network_session_mode_matchmaking_choosing_game,

	k_network_session_mode_count
};

struct s_network_session_parameter_session_mode
{
	long session_mode_sequence;
	c_enum<e_network_session_mode, long, k_network_session_mode_count> session_mode;
	dword session_mode_timestamp;
};
static_assert(sizeof(s_network_session_parameter_session_mode) == 0xC);

class c_network_session_parameter_session_mode :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_parameter_session_mode>
{
};
static_assert(sizeof(c_network_session_parameter_session_mode) == 0x48);

struct s_network_session_parameter_session_size
{
	long maximum_peer_count;
	long maximum_player_count;
};
static_assert(sizeof(s_network_session_parameter_session_size) == 0x8);

class c_network_session_parameter_session_size
	: public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_parameter_session_size>
{
};
static_assert(sizeof(c_network_session_parameter_session_size) == 0x40);