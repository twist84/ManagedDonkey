#pragma once

#include "cseries/cseries.hpp"

struct s_network_bandwidth_persistent_data
{
	long qos_sample_count;
	long qos_samples[8];

	long bandwidth_measurement_count;
	long bandwidth_measurement_successful_bps[8];
	long bandwidth_measurement_unsafe_bps[8];

	long bandwidth_dispute_count;
};
static_assert(sizeof(s_network_bandwidth_persistent_data) == 0x6C);

struct s_network_quality_session_statistics
{
	qword client_badness_history[2];
	qword host_badness_history[2];
};
static_assert(sizeof(s_network_quality_session_statistics) == 0x20);

struct s_network_quality_statistics
{
	s_network_quality_session_statistics session[2];
	qword connectivity_history[2];
	s_network_bandwidth_persistent_data bandwidth_data;
};
static_assert(sizeof(s_network_quality_statistics) == 0xC0);

struct c_network_observer;
struct s_bandwidth_configuration;
struct s_network_bandwidth_globals
{
	bool initialized;
	c_network_observer* observer;
	s_bandwidth_configuration* configuration;
	s_network_bandwidth_persistent_data* data;
	bool online_environment;
	bool tracking_bandwidth;
	bool in_game;
	dword tracking_start_timestamp;
	bool estimate_based_on_measurement;
	long estimated_bps;
	long estimated_max_machine_count;
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

