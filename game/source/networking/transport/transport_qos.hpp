#pragma once

#include "cseries/cseries.hpp"

struct s_transport_qos_result
{
	long probes_sent;
	long probes_received;
	long ping_msec_minimum;
	long ping_msec_median;
	long bandwidth_upstream_bps;
	long bandwidth_downstream_bps;
	byte __data18[0x8];
};
static_assert(sizeof(s_transport_qos_result) == 0x20);
