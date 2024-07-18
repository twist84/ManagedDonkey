#pragma once

#include "cseries/cseries.hpp"

struct s_network_statistics
{
	long qos_sample_count;
	long qos_samples[8];

	long bandwidth_measurement_count;
	long bandwidth_measurement_successful_bps[8];
	long bandwidth_measurement_unsafe_bps[8];

	// bandwidth dispute?
	long __unknown68;
};
static_assert(sizeof(s_network_statistics) == 0x6C);

struct s_network_quality_statistics
{
	byte squad_client_badness_history[16];
	byte squad_host_badness_history[16];
	byte group_client_badness_history[16];
	byte group_host_badness_history[16];
	byte connection_badness_history[16];

	s_network_statistics network_statistics;

	byte __data[0x4];
};
static_assert(sizeof(s_network_quality_statistics) == 0xC0);

struct c_network_observer;
struct s_bandwidth_configuration;
struct s_network_bandwidth_globals
{
	bool initialized;
	c_network_observer* observer;
	s_bandwidth_configuration* configuration;
	s_network_statistics* data;
	bool online_network_environment;
	bool tracking;
	bool congested;
	long tracking_start_time;
	bool estimated_bandwidth_bps_available;
	long estimated_bandwidth_bps;
	long estimated_host_capacity_machines;
};
static_assert(sizeof(s_network_bandwidth_globals) == 0x24);

extern s_network_bandwidth_globals& network_bandwidth_globals;

struct s_transport_qos_result;
extern long __cdecl network_bandwidth_compute_average(long sample_count, long const* samples);
extern void __cdecl network_bandwidth_dispose();
extern long __cdecl network_bandwidth_get_estimated_bandwidth_bps(bool* available);
extern long __cdecl network_bandwidth_get_estimated_desired_machine_capacity_of_bandwidth(long a1);
extern long __cdecl network_bandwidth_get_estimated_host_capacity_machines();
extern long __cdecl network_bandwidth_get_estimated_maximum_machine_capacity_of_bandwidth(long a1);
extern bool __cdecl network_bandwidth_get_online_network_environment();
extern bool __cdecl network_bandwidth_initialize(c_network_observer* observer, s_bandwidth_configuration const* configuration);
extern void __cdecl network_bandwidth_notify_live_service_qos_measurement(s_transport_qos_result const* qos_result);
extern void __cdecl network_bandwidth_set_online_network_environment(bool online_network_environment);
extern void __cdecl network_bandwidth_tracking_begin();
extern void __cdecl network_bandwidth_tracking_end();
extern void __cdecl network_bandwidth_update();
extern void __cdecl network_bandwidth_update_estimate();

