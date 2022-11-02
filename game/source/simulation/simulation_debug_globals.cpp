#include "simulation/simulation_debug_globals.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x019A0322, bool, g_network_interface_show_latency_and_framerate_metrics_on_chud);
REFERENCE_DECLARE(0x019A0323, bool, g_network_interface_fake_latency_and_framerate_metrics_on_chud);

bool g_network_debug_globals_latency_and_framerate_metrics_enabled = []() -> bool
{
	g_network_interface_show_latency_and_framerate_metrics_on_chud = true;
	g_network_interface_fake_latency_and_framerate_metrics_on_chud = false;

	return true;
}();

