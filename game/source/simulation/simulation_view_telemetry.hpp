#pragma once

#include "cseries/cseries.hpp"

struct __declspec(align(4)) s_simulation_view_player_telemetry_data
{
	int32 absolute_player_index;
	int32 controlled_entity_index;
	real_point3d position;
	real_vector3d desired_aiming_vector;
	int8 desired_zoom_level;
};
COMPILE_ASSERT(sizeof(s_simulation_view_player_telemetry_data) == 0x24);

struct __declspec(align(8)) s_simulation_view_telemetry_data
{
	const c_simulation_view_telemetry_provider* provider;
	bool joining;
	uns32 player_acknowledged_mask;
	int32 number_of_players;
	s_simulation_view_player_telemetry_data players[4];
};
COMPILE_ASSERT(sizeof(s_simulation_view_telemetry_data) == 0xA0);

class c_simulation_view;
class c_simulation_view_telemetry_provider
{
public:
	c_simulation_view* m_view;
	s_simulation_view_telemetry_data m_telemetry_data;
};
COMPILE_ASSERT(sizeof(c_simulation_view_telemetry_provider) == 0xA8);

