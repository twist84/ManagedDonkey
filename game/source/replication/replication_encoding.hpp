#pragma once

#include "cseries/cseries.hpp"

struct s_player_prediction
{
	long predicted_entity_index;
	long predicted_gamestate_index;
	byte control_context_identifier;
	real_point3d position;
	vector3d forward;
	vector3d up;
	vector3d translational_velocity;
	vector3d angular_velocity;
};
static_assert(sizeof(s_player_prediction) == 0x48);

