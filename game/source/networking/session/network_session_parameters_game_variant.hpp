#pragma once

#include "cseries/cseries.hpp"
#include "game/game_options.hpp"
#include "networking/session/network_session_parameters_chunked.hpp"

struct s_network_session_parameter_game_variant
{
	bool valid;
	c_game_variant game_variant;
};
COMPILE_ASSERT(sizeof(s_network_session_parameter_game_variant) == 0x268);

class c_network_session_parameter_game_variant :
	public c_network_session_parameter_chunked<s_network_session_parameter_game_variant, 6144>
{
	const c_game_variant* get() const;
	bool request_change(const c_game_variant* game_variant);
	bool set(const c_game_variant* game_variant);
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_game_variant) == 0x3570);

