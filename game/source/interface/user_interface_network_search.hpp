#pragma once

#include "cseries/cseries.hpp"

struct s_user_interface_network_search_globals
{
	bool network_search_valid;
};

extern s_user_interface_network_search_globals& user_interface_network_search_globals;

extern bool __cdecl user_interface_available_squads_initialize(int32 controller_index, uns32 available_squad_search_flags, char category);

