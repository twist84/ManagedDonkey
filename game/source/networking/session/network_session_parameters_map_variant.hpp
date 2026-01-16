#pragma once

#include "game/game_options.hpp"
#include "networking/session/network_session_parameters_chunked.hpp"

struct s_network_session_parameter_map_variant
{
	bool valid;
	int32 : 32; // padding

	c_map_variant map_variant;
};
static_assert(sizeof(s_network_session_parameter_map_variant) == 0xE098);

class c_network_session_parameter_map_variant :
	public c_network_session_parameter_chunked<s_network_session_parameter_map_variant, 28672>
{
};
static_assert(sizeof(c_network_session_parameter_map_variant) == 0x2A1D0);

