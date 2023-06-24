#pragma once

#include "cseries/cseries.hpp"
#include "game/game_options.hpp"
#include "networking/session/network_session_parameters_chunked.hpp"

struct s_network_session_parameter_game_variant
{
	bool valid;
	c_game_variant game_variant;
};
static_assert(sizeof(s_network_session_parameter_game_variant) == 0x268);

struct c_network_session_parameter_game_variant :
	public c_network_session_parameter_chunked<s_network_session_parameter_game_variant, 6144>
{
	c_game_variant const* get() const;
	bool request_change(c_game_variant const* game_variant);
	bool set(c_game_variant const* game_variant);
};
static_assert(sizeof(c_network_session_parameter_game_variant) == 0x3570);

