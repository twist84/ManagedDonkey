#pragma once

#include "cseries/cseries.hpp"

struct s_network_bandwidth_persistent_data
{
	int32 qos_sample_count;
	int32 qos_samples[8];

	int32 bandwidth_measurement_count;
	int32 bandwidth_measurement_successful_bps[8];
	int32 bandwidth_measurement_unsafe_bps[8];

	int32 bandwidth_dispute_count;
};
COMPILE_ASSERT(sizeof(s_network_bandwidth_persistent_data) == 0x6C);

struct s_network_quality_session_statistics
{
	uns64 client_badness_history[2];
	uns64 host_badness_history[2];
};
COMPILE_ASSERT(sizeof(s_network_quality_session_statistics) == 0x20);

struct s_network_quality_statistics
{
	s_network_quality_session_statistics session[2];
	uns64 connectivity_history[2];
	s_network_bandwidth_persistent_data bandwidth_data;
};
COMPILE_ASSERT(sizeof(s_network_quality_statistics) == 0xC0);

class c_network_observer;
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
	uns32 tracking_start_timestamp;
	bool estimate_based_on_measurement;
	int32 estimated_bps;
	int32 estimated_max_machine_count;
};
COMPILE_ASSERT(sizeof(s_network_bandwidth_globals) == 0x24);

extern s_network_bandwidth_globals& network_bandwidth_globals;

struct s_transport_qos_result;
extern int32 __cdecl network_bandwidth_compute_average(int32 sample_count, const int32* samples);
extern void __cdecl network_bandwidth_dispose();
extern int32 __cdecl network_bandwidth_get_estimated_bandwidth_bps(bool* available);
extern int32 __cdecl network_bandwidth_get_estimated_desired_machine_capacity_of_bandwidth(int32 a1);
extern int32 __cdecl network_bandwidth_get_estimated_host_capacity_machines();
extern int32 __cdecl network_bandwidth_get_estimated_maximum_machine_capacity_of_bandwidth(int32 a1);
extern bool __cdecl network_bandwidth_get_online_network_environment();
extern bool __cdecl network_bandwidth_initialize(c_network_observer* observer, const s_bandwidth_configuration* configuration);
extern void __cdecl network_bandwidth_notify_live_service_qos_measurement(const s_transport_qos_result* qos_result);
extern void __cdecl network_bandwidth_set_online_network_environment(bool online_network_environment);
extern void __cdecl network_bandwidth_tracking_begin();
extern void __cdecl network_bandwidth_tracking_end();
extern void __cdecl network_bandwidth_update();
extern void __cdecl network_bandwidth_update_estimate();

