#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_target.hpp"
#include "units/units.hpp"

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

struct s_player_predicted_aim_assist
{
	c_aim_target_predicted predicted_aim_assist;
	real primary_autoaim_level;
	real secondary_autoaim_level;
	vector3d lead_vector;
	byte_flags flags;
};
static_assert(sizeof(s_player_predicted_aim_assist) == 0x24);

struct s_player_motion
{
	dword control_context;
	byte control_context_identifier;
	euler_angles2d aiming_angles;
	real_point2d throttle;
	word_flags motion_flags;
	s_unit_weapon_set weapon_set;
	word zoom_level;

	// 0: primary, 1: secondary
	bool primary_predicted_fire[2];

	// 0: primary, 1: secondary
	bool secondary_predicted_fire[2];

	bool motion_sensor_velocity_threshold_exceeded;
	bool __unknown25;
	bool predicted_aim_assist_exists;
	s_player_predicted_aim_assist aim_assist;
};
static_assert(sizeof(s_player_motion) == 0x4C);

struct c_bitstream;
extern bool __cdecl player_prediction_compare_identical(s_player_prediction const* player_prediction_a, s_player_prediction const* player_prediction_b);
extern bool __cdecl player_prediction_decode(c_bitstream* packet, s_player_prediction* player_prediction, bool replicated);
extern void __cdecl player_prediction_encode(c_bitstream* packet, s_player_prediction const* player_prediction, bool replicated);
extern long __cdecl player_prediction_minimum_required_bits();
extern bool __cdecl player_prediction_valid(s_player_prediction const* player_prediction);

