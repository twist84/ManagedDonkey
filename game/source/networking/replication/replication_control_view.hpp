#pragma once

#include "cseries/cseries.hpp"
#include "game/game_results.hpp"
#include "networking/replication/replication_scheduler.hpp"
#include "replication/replication_encoding.hpp"
#include "units/units.hpp"

struct c_simulation_view_telemetry_provider;
struct c_replication_control_view : c_replication_scheduler_client
{
	byte __data4[0x4];

	bool m_initialized;
	c_simulation_view_telemetry_provider* m_telemetry_provider;

	dword_flags m_motion_available_send;
	dword_flags __unknown14;
	c_static_array<s_player_motion, 32> m_motion;
	dword __time998[32]; // motion update times?

	dword_flags m_prediction_available_send;
	dword_flags __unknownA1C;
	c_static_array<s_player_prediction, 32> m_prediction;

	byte __data1320[0xA8]; // prediction update times? and other data?

	c_game_results_replicator m_game_results_replicator;
};
static_assert(sizeof(c_replication_control_view) == 0x1C830);

