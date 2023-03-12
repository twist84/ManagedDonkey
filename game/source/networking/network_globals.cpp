#include "networking/network_globals.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "networking/logic/network_session_interface.hpp"

void __cdecl network_update()
{
	INVOKE(0x0049E7B0, network_update);
}

void __cdecl network_test_set_map_name(char const* scenario_path)
{
	if (network_squad_session_set_map(-1, -2, scenario_path))
	{
		c_console::write_line("set %s successfully", scenario_path);
	}
	else
	{
		c_console::write_line("unable to set map %s", scenario_path);
	}
}

