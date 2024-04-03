#pragma once

#include "cseries/cseries.hpp"
#include "interface/user_interface_session.hpp"
#include "networking/session/network_session_parameters_generic.hpp"

struct s_network_session_parameter_countdown_timer
{
	long delayed_reason;
	long countdown_timer;
};
static_assert(sizeof(s_network_session_parameter_countdown_timer) == 0x8);

struct c_network_session_parameter_countdown_timer :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_parameter_countdown_timer>
{
public:
	long get_countdown_timer();
};
static_assert(sizeof(c_network_session_parameter_countdown_timer) == 0x40);

struct s_network_session_privacy_mode
{
	long privacy_mode;
	long closed_mode;
	long maximum_player_count;
	bool is_closed_by_user;
};
static_assert(sizeof(s_network_session_privacy_mode) == 0x10);

// #TODO: map this
struct s_network_session_parameter_initial_participants
{
	byte __data[0x16530];
};
static_assert(sizeof(s_network_session_parameter_initial_participants) == 0x16530);

struct c_network_session_parameter_initial_participants :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_parameter_initial_participants>
{
};
static_assert(sizeof(c_network_session_parameter_initial_participants) == 0x2CA90);

struct s_network_session_parameter_game_start_status
{
	c_enum<e_session_game_start_status, long, _session_game_start_status_none, k_session_game_start_status_count> game_start_status;
	long game_start_error;
	word player_error_mask;
	short map_load_progress;
};
static_assert(sizeof(s_network_session_parameter_game_start_status) == 0xC);

struct c_network_session_parameter_game_start_status :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_parameter_game_start_status>
{
};
static_assert(sizeof(c_network_session_parameter_game_start_status) == 0x48);

struct s_network_session_parameter_leader_properties
{
	c_static_string<64> override_hopper_directory;
};
static_assert(sizeof(s_network_session_parameter_leader_properties) == 0x40);