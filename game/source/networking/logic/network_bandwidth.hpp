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
	byte __data[0xC0];
};
static_assert(sizeof(s_network_quality_statistics) == 0xC0);

