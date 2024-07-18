#include "networking/logic/network_bandwidth.hpp"

REFERENCE_DECLARE(0x019E8D28, s_network_bandwidth_globals, network_bandwidth_globals);

long __cdecl network_bandwidth_compute_average(long sample_count, long const* samples)
{
    return INVOKE(0x00455550, network_bandwidth_compute_average, sample_count, samples);
}

void __cdecl network_bandwidth_dispose()
{
    INVOKE(0x004556D0, network_bandwidth_dispose);
}

long __cdecl network_bandwidth_get_estimated_bandwidth_bps(bool* available)
{
    return INVOKE(0x004556E0, network_bandwidth_get_estimated_bandwidth_bps, available);
}

//.text:00455700 ; 

long __cdecl network_bandwidth_get_estimated_host_capacity_machines()
{
    return INVOKE(0x00455740, network_bandwidth_get_estimated_host_capacity_machines);
}

//.text:00455750 ; 

bool __cdecl network_bandwidth_get_online_network_environment()
{
    return INVOKE(0x00455790, network_bandwidth_get_online_network_environment);
}

bool __cdecl network_bandwidth_initialize(c_network_observer* observer, s_bandwidth_configuration const* bandwidth_configuration)
{
    return INVOKE(0x004557A0, network_bandwidth_initialize, observer, bandwidth_configuration);
}

void __cdecl network_bandwidth_notify_live_service_qos_measurement(s_transport_qos_result const* qos_result)
{
    INVOKE(0x004557F0, network_bandwidth_notify_live_service_qos_measurement, qos_result);
}

void __cdecl network_bandwidth_set_online_network_environment(bool online_network_environment)
{
    INVOKE(0x00455890, network_bandwidth_set_online_network_environment, online_network_environment);
}

void __cdecl network_bandwidth_tracking_begin()
{
    INVOKE(0x004558B0, network_bandwidth_tracking_begin);
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

