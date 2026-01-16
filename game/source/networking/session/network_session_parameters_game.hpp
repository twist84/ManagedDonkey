#pragma once

#include "cseries/cseries.hpp"
#include "interface/user_interface_session.hpp"
#include "networking/session/network_session_parameters_generic.hpp"

struct s_network_session_parameter_countdown_timer
{
	int32 delayed_reason;
	int32 countdown_timer;
};
COMPILE_ASSERT(sizeof(s_network_session_parameter_countdown_timer) == 0x8);

class c_network_session_parameter_countdown_timer :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_network_session_parameter_countdown_timer>
{
public:
	int32 get_countdown_timer();
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_countdown_timer) == 0x40);

struct s_network_session_privacy_mode
{
	int32 privacy_mode;
	int32 closed_mode;
	int32 maximum_player_count;
	bool is_closed_by_user;
};
COMPILE_ASSERT(sizeof(s_network_session_privacy_mode) == 0x10);

// $TODO map this
struct s_network_session_parameter_initial_participants
{
	byte __data[0x16530];
};
COMPILE_ASSERT(sizeof(s_network_session_parameter_initial_participants) == 0x16530);

class c_network_session_parameter_initial_participants :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_network_session_parameter_initial_participants>
{
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_initial_participants) == 0x2CA90);

struct s_network_session_parameter_game_start_status
{
	e_session_game_start_status game_start_status;
	e_session_game_start_error game_start_error;
	uns16 player_error_mask;
	int16 map_load_progress;
};
COMPILE_ASSERT(sizeof(s_network_session_parameter_game_start_status) == 0xC);

class c_network_session_parameter_game_start_status :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_network_session_parameter_game_start_status>
{
public:
	bool set(const s_network_session_parameter_game_start_status* game_status);
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_game_start_status) == 0x48);

struct s_network_session_parameter_leader_properties
{
	c_static_string<64> override_hopper_directory;
};
COMPILE_ASSERT(sizeof(s_network_session_parameter_leader_properties) == 0x40);

