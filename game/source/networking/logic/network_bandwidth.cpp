#include "networking/logic/network_bandwidth.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/network_time.hpp"
#include "networking/session/network_observer.hpp"

REFERENCE_DECLARE(0x019E8D28, s_network_bandwidth_globals, network_bandwidth_globals);

HOOK_DECLARE(0x00455890, network_bandwidth_set_online_network_environment);

int32 __cdecl network_bandwidth_compute_average(int32 sample_count, const int32* samples)
{
	return INVOKE(0x00455550, network_bandwidth_compute_average, sample_count, samples);
}

void __cdecl network_bandwidth_dispose()
{
	INVOKE(0x004556D0, network_bandwidth_dispose);

	//network_bandwidth_globals.initialized = false;
}

int32 __cdecl network_bandwidth_get_estimated_bandwidth_bps(bool* available)
{
	return INVOKE(0x004556E0, network_bandwidth_get_estimated_bandwidth_bps, available);

	//ASSERT(network_bandwidth_globals.initialized);
	//*available = network_bandwidth_globals.estimated_bandwidth_bps_available;
	//return network_bandwidth_globals.estimated_bandwidth_bps;
}

int32 __cdecl network_bandwidth_get_estimated_desired_machine_capacity_of_bandwidth(int32 a1)
{
	return INVOKE(0x00455700, network_bandwidth_get_estimated_desired_machine_capacity_of_bandwidth, a1);
}

int32 __cdecl network_bandwidth_get_estimated_host_capacity_machines()
{
	return INVOKE(0x00455740, network_bandwidth_get_estimated_host_capacity_machines);

	//ASSERT(network_bandwidth_globals.initialized);
	//return network_bandwidth_globals.estimated_host_capacity_machines;
}

int32 __cdecl network_bandwidth_get_estimated_maximum_machine_capacity_of_bandwidth(int32 a1)
{
	return INVOKE(0x00455750, network_bandwidth_get_estimated_maximum_machine_capacity_of_bandwidth, a1);
}

bool __cdecl network_bandwidth_get_online_network_environment()
{
	return INVOKE(0x00455790, network_bandwidth_get_online_network_environment);

	//return network_bandwidth_globals.online_network_environment;
}

bool __cdecl network_bandwidth_initialize(c_network_observer* observer, const s_bandwidth_configuration* configuration)
{
	return INVOKE(0x004557A0, network_bandwidth_initialize, observer, configuration);

	//ASSERT(observer);
	//ASSERT(configuration);
	//network_bandwidth_globals.observer = observer;
	//network_bandwidth_globals.configuration = configuration;
	//network_bandwidth_globals.online_network_environment = false;
	//network_bandwidth_globals.estimated_bandwidth_bps_available = configuration;
	//network_bandwidth_globals.estimated_bandwidth_bps = NONE;
	//network_bandwidth_globals.estimated_host_capacity_machines = k_network_maximum_machines_per_session;
	//network_bandwidth_globals.data = &observer->quality_statistics_get()->network_statistics;
	//network_bandwidth_globals.initialized = true;
}

void __cdecl network_bandwidth_notify_live_service_qos_measurement(const s_transport_qos_result* qos_result)
{
	INVOKE(0x004557F0, network_bandwidth_notify_live_service_qos_measurement, qos_result);
}

void __cdecl network_bandwidth_set_online_network_environment(bool online_environment)
{
	//INVOKE(0x00455890, network_bandwidth_set_online_network_environment, online_environment);

	//ASSERT(network_bandwidth_globals.observer);
	//network_bandwidth_globals.observer->set_online_network_environment(online_environment);
	//network_bandwidth_globals.online_network_environment = online_environment;
	//network_bandwidth_update_estimate();
}

void __cdecl network_bandwidth_tracking_begin()
{
	INVOKE(0x004558B0, network_bandwidth_tracking_begin);

	//if (network_bandwidth_globals.initialized)
	//{
	//	event(_event_message, "networking:logic:bandwidth: begin bandwidth tracking");
	//	network_bandwidth_globals.observer->reset_bandwidth();
	//	network_bandwidth_globals.tracking = true;
	//	network_bandwidth_globals.tracking_start_time = (int32)network_time_get();
	//}
}

void __cdecl network_bandwidth_tracking_end()
{
	INVOKE(0x00455900, network_bandwidth_tracking_end);
}

void __cdecl network_bandwidth_update()
{
	INVOKE(0x00455A80, network_bandwidth_update);
}

void __cdecl network_bandwidth_update_estimate()
{
	INVOKE(0x00455B40, network_bandwidth_update_estimate);
}

