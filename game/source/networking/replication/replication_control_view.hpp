#pragma once

#include "cseries/cseries.hpp"
#include "networking/replication/replication_scheduler.hpp"
#include "replication/replication_encoding.hpp"
#include "units/units.hpp"

int32 const k_network_maximum_control_streams = 32;

struct c_simulation_view_telemetry_provider;
struct c_replication_control_view :
	c_replication_scheduler_client
{
	bool m_initialized;
	c_simulation_view_telemetry_provider* m_telemetry_provider;
	uint32 m_motion_available_send;
	uint32 m_motion_available_receive;
	s_player_motion m_motion[k_network_maximum_control_streams];
	uint32 m_motion_timestamp[k_network_maximum_control_streams];
	uint32 m_prediction_available_send;
	uint32 m_prediction_available_receive;
	s_player_prediction m_prediction[k_network_maximum_control_streams];
};
static_assert(sizeof(c_replication_control_view) == 0x1320);

